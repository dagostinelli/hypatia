// SPDX-License-Identifier: MIT
//
// Compares hypatia with GLM, cglm (single precision only), Eigen and LAPACK.
// Each comparison runs on random inputs and prints the largest difference; the
// edge cases (zero, tiny, huge, NaN, degenerate geometry) are listed separately
// with what each library returns.  Prints markdown.

extern "C" {
#include "hypatia.h"
}
#include "hyp_unalias.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#define HAVE_CGLM 1
#define CGLM_CLIPSPACE_INCLUDE_ALL
#include <cglm/cglm.h>
#endif

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <deque>

extern "C" {
void dgetrf_(int *m, int *n, double *a, int *lda, int *ipiv, int *info);
void dgetri_(int *n, double *a, int *lda, int *ipiv, double *work, int *lwork, int *info);
void dgecon_(char *norm, int *n, double *a, int *lda, double *anorm, double *rcond, double *work, int *iwork, int *info);
double dlange_(char *norm, int *m, int *n, double *a, int *lda, double *work);
}

typedef HYP_FLOAT H;
typedef long double L;
using V2 = glm::vec<2, H>;
using V3 = glm::vec<3, H>;
using V4 = glm::vec<4, H>;
using M2 = glm::mat<2, 2, H>;
using M3 = glm::mat<3, 3, H>;
using M4 = glm::mat<4, 4, H>;
using Q = glm::qua<H>;
using EV3 = Eigen::Matrix<H, 3, 1>;
using EM4 = Eigen::Matrix<H, 4, 4>;
using EQ = Eigen::Quaternion<H>;
using LM4 = Eigen::Matrix<L, 4, 4>;

static const bool SINGLE = sizeof(H) == sizeof(float);
static const double TOL = SINGLE ? 2e-5 : 1e-10;
static const int N = 2000;
static const double PI = 3.14159265358979323846;

/* ---------------------------------------------------------------- report */

struct Row {
	std::string area, test, partner, note;
	long n = 0, over = 0;
	double maxerr = 0;
	std::string worst;
};
static std::deque<Row> rows; /* push_back keeps references to the rows valid */
static Row *cur;

static void begin(const char *area, const char *test, const char *partner, const char *note = "")
{
	rows.push_back(Row());
	cur = &rows.back();
	cur->area = area;
	cur->test = test;
	cur->partner = partner;
	cur->note = note;
}

/* |a - b| relative to max(1, |b|); NaN on one side only is infinite */
static double e1(double a, double b)
{
	if (std::isnan(a) || std::isnan(b))
		return (std::isnan(a) && std::isnan(b)) ? 0 : INFINITY;
	if (std::isinf(a) || std::isinf(b))
		return a == b ? 0 : INFINITY;
	return std::fabs(a - b) / std::max(1.0, std::fabs(b));
}

static double en(const double *a, const double *b, int n)
{
	double m = 0;
	for (int i = 0; i < n; i++)
		m = std::max(m, e1(a[i], b[i]));
	return m;
}

static void check(double err, const std::function<std::string()> &describe)
{
	cur->n++;
	if (!(err <= TOL))
		cur->over++;
	if (!(err <= cur->maxerr) || (cur->n == 1 && err > 0)) {
		if (!(err <= cur->maxerr))
			cur->maxerr = std::isnan(err) ? INFINITY : err;
		cur->worst = describe();
	}
}

static std::string f(double x)
{
	char b[48];
	std::snprintf(b, sizeof b, "%.9g", x);
	return b;
}

static std::string fv(const double *v, int n)
{
	std::string s = "(";
	for (int i = 0; i < n; i++)
		s += (i ? ", " : "") + f(v[i]);
	return s + ")";
}

/* edge cases: what each library gives */
struct Edge {
	std::string what, input, hyp, others;
};
static std::vector<Edge> edges;
static void edge(const std::string &what, const std::string &input, const std::string &hyp, const std::string &others)
{
	edges.push_back({what, input, hyp, others});
}

/* ---------------------------------------------------------------- random */

static uint64_t rng_state = 0x9E3779B97F4A7C15ULL;
static double rnd()
{
	rng_state ^= rng_state << 13;
	rng_state ^= rng_state >> 7;
	rng_state ^= rng_state << 17;
	return (double)(rng_state >> 11) * (1.0 / 9007199254740992.0);
}
static H r(double lo, double hi) { return (H)(lo + (hi - lo) * rnd()); }
static H rangle() { return r(-2 * PI, 2 * PI); }

/* ---------------------------------------------------------------- conversions */

template <int n> static void dbl(const H *in, double *out)
{
	for (int i = 0; i < n; i++)
		out[i] = in[i];
}

static double err_v(const H *a, const H *b, int n)
{
	double x[16], y[16];
	for (int i = 0; i < n; i++) {
		x[i] = a[i];
		y[i] = b[i];
	}
	return en(x, y, n);
}

static std::string sv(const H *v, int n)
{
	double d[16];
	for (int i = 0; i < n; i++)
		d[i] = v[i];
	return fv(d, n);
}

/* hypatia m[] is numbered row by row; GLM is indexed [column][row] */
static M4 g4(const struct matrix4 &h)
{
	M4 g;
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			g[c][r] = h.m[r * 4 + c];
	return g;
}
static M3 g3(const struct matrix3 &h)
{
	M3 g;
	for (int r = 0; r < 3; r++)
		for (int c = 0; c < 3; c++)
			g[c][r] = h.m[r * 3 + c];
	return g;
}
static M2 g2(const struct matrix2 &h)
{
	M2 g;
	for (int r = 0; r < 2; r++)
		for (int c = 0; c < 2; c++)
			g[c][r] = h.m[r * 2 + c];
	return g;
}
static struct matrix4 h4(const M4 &g)
{
	struct matrix4 h;
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			h.m[r * 4 + c] = g[c][r];
	return h;
}
static struct matrix3 h3(const M3 &g)
{
	struct matrix3 h;
	for (int r = 0; r < 3; r++)
		for (int c = 0; c < 3; c++)
			h.m[r * 3 + c] = g[c][r];
	return h;
}
static struct matrix2 h2(const M2 &g)
{
	struct matrix2 h;
	for (int r = 0; r < 2; r++)
		for (int c = 0; c < 2; c++)
			h.m[r * 2 + c] = g[c][r];
	return h;
}
static EM4 e4(const struct matrix4 &h)
{
	EM4 e;
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			e(r, c) = h.m[r * 4 + c];
	return e;
}

/* compare a hypatia matrix with a GLM one, element by element (row, column) */
static double err_m4(const struct matrix4 &h, const M4 &g) { struct matrix4 x = h4(g); return err_v(h.m, x.m, 16); }
static double err_m3(const struct matrix3 &h, const M3 &g) { struct matrix3 x = h3(g); return err_v(h.m, x.m, 9); }
static double err_m2(const struct matrix2 &h, const M2 &g) { struct matrix2 x = h2(g); return err_v(h.m, x.m, 4); }
static double err_e4(const struct matrix4 &h, const EM4 &e)
{
	double m = 0;
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			m = std::max(m, e1(h.m[r * 4 + c], e(r, c)));
	return m;
}

static std::string sm4(const struct matrix4 &h) { return sv(h.m, 16); }
static std::string sm3(const struct matrix3 &h) { return sv(h.m, 9); }
static std::string sm2(const struct matrix2 &h) { return sv(h.m, 4); }

static Q gq(const struct quaternion &h) { return Q(h.w, h.x, h.y, h.z); }
static struct quaternion hq(const Q &g)
{
	struct quaternion h;
	quaternion_setf4(&h, g.x, g.y, g.z, g.w);
	return h;
}
static EQ eq(const struct quaternion &h) { return EQ(h.w, h.x, h.y, h.z); }
static struct quaternion hq(const EQ &e)
{
	struct quaternion h;
	quaternion_setf4(&h, e.x(), e.y(), e.z(), e.w());
	return h;
}

/* quaternion components, exactly */
static double err_q(const struct quaternion &a, const struct quaternion &b) { return err_v(a.q, b.q, 4); }
/* as rotations: q and -q are the same */
static double err_qr(const struct quaternion &a, const struct quaternion &b)
{
	struct quaternion n = b;
	quaternion_negate(&n);
	return std::min(err_q(a, b), err_q(a, n));
}
static std::string sq(const struct quaternion &h) { return "(x " + f(h.x) + ", y " + f(h.y) + ", z " + f(h.z) + ", w " + f(h.w) + ")"; }

static struct vector2 hv(const V2 &g) { struct vector2 h; vector2_setf2(&h, g.x, g.y); return h; }
static struct vector3 hv(const V3 &g) { struct vector3 h; vector3_setf3(&h, g.x, g.y, g.z); return h; }
static struct vector4 hv(const V4 &g) { struct vector4 h; vector4_setf4(&h, g.x, g.y, g.z, g.w); return h; }
static V2 gv(const struct vector2 &h) { return V2(h.x, h.y); }
static V3 gv(const struct vector3 &h) { return V3(h.x, h.y, h.z); }
static V4 gv(const struct vector4 &h) { return V4(h.x, h.y, h.z, h.w); }
static EV3 ev(const struct vector3 &h) { return EV3(h.x, h.y, h.z); }

/* ---------------------------------------------------------------- random inputs */

static struct vector2 rv2(double s = 10) { struct vector2 v; vector2_setf2(&v, r(-s, s), r(-s, s)); return v; }
static struct vector3 rv3(double s = 10) { struct vector3 v; vector3_setf3(&v, r(-s, s), r(-s, s), r(-s, s)); return v; }
static struct vector4 rv4(double s = 10) { struct vector4 v; vector4_setf4(&v, r(-s, s), r(-s, s), r(-s, s), r(-s, s)); return v; }
static struct vector3 runit3()
{
	struct vector3 v;
	do {
		v = rv3(1);
	} while (vector3_magnitude(&v) < 0.1);
	return *vector3_normalize(&v);
}
static struct quaternion rq_unit()
{
	struct quaternion q;
	do {
		quaternion_setf4(&q, r(-1, 1), r(-1, 1), r(-1, 1), r(-1, 1));
	} while (quaternion_magnitude(&q) < 0.1);
	return *quaternion_normalize(&q);
}
static struct quaternion rq_any()
{
	struct quaternion q = rq_unit();
	return *quaternion_multiplyf(&q, r(0.2, 5));
}
static struct matrix4 rm4(double s = 5)
{
	struct matrix4 m;
	for (int i = 0; i < 16; i++)
		m.m[i] = r(-s, s);
	return m;
}
static struct matrix3 rm3(double s = 5)
{
	struct matrix3 m;
	for (int i = 0; i < 9; i++)
		m.m[i] = r(-s, s);
	return m;
}
static struct matrix2 rm2(double s = 5)
{
	struct matrix2 m;
	for (int i = 0; i < 4; i++)
		m.m[i] = r(-s, s);
	return m;
}
/* translation, rotation and scale */
static struct matrix4 rtrs()
{
	struct vector3 s, t;
	struct quaternion q = rq_unit();
	vector3_setf3(&s, r(0.2, 4), r(0.2, 4), r(0.2, 4));
	t = rv3();
	struct matrix4 m;
	matrix4_transformation_compose(&m, &s, &q, &t);
	return m;
}

#include "compare_vectors.inc"
#include "compare_matrices.inc"
#include "compare_quaternions.inc"
#include "compare_edges.inc"
#include "compare_accuracy.inc"

/* ---------------------------------------------------------------- main */

int main()
{
	/* the layout the conversions rely on: m[] row by row, r01 is m[1] */
	struct matrix4 probe;
	matrix4_zero(&probe);
	probe.r01 = 1;
	if (probe.m[1] != 1) {
		std::printf("unexpected matrix layout\n");
		return 1;
	}

	vectors();
	matrices();
	quaternions();
	edge_cases();
	accuracy();

	std::printf("# hypatia comparison: %s precision%s\n\n", SINGLE ? "single" : "double",
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
		    ", HYP_DEPTH_MINUS_ONE_TO_ONE"
#else
		    ""
#endif
	);
	std::printf("Tolerance %g (difference relative to max(1, |reference|)); %d random inputs per row.\n\n", TOL, N);
	std::string area;
	for (const Row &w : rows) {
		if (w.area != area) {
			area = w.area;
			std::printf("\n## %s\n\n| test | compared with | n | over tol | max diff | worst case | note |\n|---|---|---|---|---|---|---|\n", area.c_str());
		}
		std::printf("| %s | %s | %ld | %s%ld%s | %s | %s | %s |\n", w.test.c_str(), w.partner.c_str(), w.n,
			    w.over ? "**" : "", w.over, w.over ? "**" : "", f(w.maxerr).c_str(),
			    w.over ? w.worst.c_str() : "", w.note.c_str());
	}
	std::printf("\n## Edge cases\n\n| function | input | hypatia | others |\n|---|---|---|---|\n");
	for (const Edge &e : edges)
		std::printf("| %s | %s | %s | %s |\n", e.what.c_str(), e.input.c_str(), e.hyp.c_str(), e.others.c_str());
	return 0;
}
