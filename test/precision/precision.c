/* SPDX-License-Identifier: MIT */

/* The precision of hypatia, measured against references computed in
 * _Float128: for each function and kind of input, the largest and the mean
 * error over a fixed sequence of inputs, in epsilons of HYP_FLOAT.
 *
 *   precision                  prints the table
 *   precision --check FILE     compares with the table in FILE: fails when a
 *                              mean error is more than 1% above it, a largest
 *                              error more than 10% above it, or a row is
 *                              missing on either side
 *
 * The error is the largest component error divided by the largest component
 * of the reference.  Products, determinants and the inverse are scaled as in
 * the comments of each measurement.  Quaternions are compared up to sign.
 *
 * Needs GCC with _Float128 and the f128 math functions of glibc.
 */
#define __STDC_WANT_IEC_60559_TYPES_EXT__
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hypatia.h"

typedef _Float128 R;

#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#	define EPSILON ((R)FLT_EPSILON)
#else
#	define EPSILON ((R)DBL_EPSILON)
#endif

#define PI acosf128(-1)
#define N 20000
#define MAX_ROWS 128

struct row {
	const char *function;
	const char *inputs;
	double largest;
	double sum;
	long count;
};

static struct row rows[MAX_ROWS];
static int row_count;

static struct row *row(const char *function, const char *inputs)
{
	struct row *r = &rows[row_count++];

	r->function = function;
	r->inputs = inputs;
	r->largest = 0.0;
	r->sum = 0.0;
	r->count = 0;
	return r;
}

static void add(struct row *r, R error)
{
	double e = (double)error;

	if (!(e <= DBL_MAX)) {
		e = HUGE_VAL;
	}
	if (e > r->largest) {
		r->largest = e;
	}
	r->sum += e;
	r->count++;
}

/* ------------------------------------------------------------ inputs */

static unsigned long long state = 88172645463325252ULL;

/* a number in [0, 1) */
static R random01(void)
{
	state ^= state << 13;
	state ^= state >> 7;
	state ^= state << 17;
	return (R)(state >> 11) / (R)9007199254740992.0;
}

/* a number in [a, b) */
static R uniform(R a, R b)
{
	return a + (b - a) * random01();
}

static R rabs(R x)
{
	return x < 0 ? -x : x;
}

static R rlength(const R *v, int n)
{
	R s = 0;
	int i;

	for (i = 0; i < n; i++) {
		s += v[i] * v[i];
	}
	return sqrtf128(s);
}

static void rnormalize(R *v, int n)
{
	R l = rlength(v, n);
	int i;

	for (i = 0; i < n; i++) {
		v[i] /= l;
	}
}

/* a direction: a point in the unit ball, not too close to the center, scaled to length 1 */
static void random_unit(R *v, int n)
{
	R l;
	int i;

	do {
		for (i = 0; i < n; i++) {
			v[i] = uniform(-1, 1);
		}
		l = rlength(v, n);
	} while (l < (R)0.1 || l > 1);
	rnormalize(v, n);
}

static R rdot3(const R *a, const R *b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static void rcross(const R *a, const R *b, R *r)
{
	R x = a[1] * b[2] - a[2] * b[1];
	R y = a[2] * b[0] - a[0] * b[2];
	R z = a[0] * b[1] - a[1] * b[0];

	r[0] = x;
	r[1] = y;
	r[2] = z;
}

/* a unit vector perpendicular to the unit vector a */
static void random_perpendicular(const R *a, R *p)
{
	R r[3];

	random_unit(r, 3);
	rcross(a, r, p);
	rnormalize(p, 3);
}

/* the unit vector at angle t from the unit vector a */
static void turned(const R *a, R t, R *b)
{
	R p[3];
	int i;

	random_perpendicular(a, p);
	for (i = 0; i < 3; i++) {
		b[i] = a[i] * cosf128(t) + p[i] * sinf128(t);
	}
}

/* q (x, y, z, w) = a b */
static void rqmul(const R *a, const R *b, R *q)
{
	R x = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
	R y = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
	R z = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
	R w = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];

	q[0] = x;
	q[1] = y;
	q[2] = z;
	q[3] = w;
}

/* the unit quaternion of the rotation by t about a random axis */
static void rotation_by(R t, R *q)
{
	R axis[3];
	int i;

	random_unit(axis, 3);
	for (i = 0; i < 3; i++) {
		q[i] = axis[i] * sinf128(t / 2);
	}
	q[3] = cosf128(t / 2);
}

/* the 3x3 rotation matrix (row-major) of the quaternion q of any length */
static void rotation_matrix(const R *q, R *m)
{
	R x = q[0], y = q[1], z = q[2], w = q[3];
	R s = 2 / (x * x + y * y + z * z + w * w);

	m[0] = 1 - s * (y * y + z * z);
	m[1] = s * (x * y - z * w);
	m[2] = s * (x * z + y * w);
	m[3] = s * (x * y + z * w);
	m[4] = 1 - s * (x * x + z * z);
	m[5] = s * (y * z - x * w);
	m[6] = s * (x * z - y * w);
	m[7] = s * (y * z + x * w);
	m[8] = 1 - s * (x * x + y * y);
}

/* the unit quaternion of the 3x3 rotation matrix m (row-major), Shepperd */
static void matrix_quaternion(const R *m, R *q)
{
	R t = m[0] + m[4] + m[8];
	R s;

	if (t > 0) {
		s = 2 * sqrtf128(1 + t);
		q[3] = s / 4;
		q[0] = (m[7] - m[5]) / s;
		q[1] = (m[2] - m[6]) / s;
		q[2] = (m[3] - m[1]) / s;
	} else if (m[0] > m[4] && m[0] > m[8]) {
		s = 2 * sqrtf128(1 + m[0] - m[4] - m[8]);
		q[3] = (m[7] - m[5]) / s;
		q[0] = s / 4;
		q[1] = (m[1] + m[3]) / s;
		q[2] = (m[2] + m[6]) / s;
	} else if (m[4] > m[8]) {
		s = 2 * sqrtf128(1 + m[4] - m[0] - m[8]);
		q[3] = (m[2] - m[6]) / s;
		q[0] = (m[1] + m[3]) / s;
		q[1] = s / 4;
		q[2] = (m[5] + m[7]) / s;
	} else {
		s = 2 * sqrtf128(1 + m[8] - m[0] - m[4]);
		q[3] = (m[3] - m[1]) / s;
		q[0] = (m[2] + m[6]) / s;
		q[1] = (m[5] + m[7]) / s;
		q[2] = s / 4;
	}
	rnormalize(q, 4);
}

/* the inverse of the n x n matrix a (row-major) by Gauss-Jordan elimination;
 * returns the row-sum norm of the inverse
 */
static R rinverse(const R *a, R *inverse, int n)
{
	R m[16];
	R norm = 0;
	int i, j, k, p;

	for (i = 0; i < n * n; i++) {
		m[i] = a[i];
		inverse[i] = (i % (n + 1) == 0) ? 1 : 0;
	}
	for (k = 0; k < n; k++) {
		p = k;
		for (i = k + 1; i < n; i++) {
			if (rabs(m[i * n + k]) > rabs(m[p * n + k])) {
				p = i;
			}
		}
		for (j = 0; j < n; j++) {
			R t = m[k * n + j];
			m[k * n + j] = m[p * n + j];
			m[p * n + j] = t;
			t = inverse[k * n + j];
			inverse[k * n + j] = inverse[p * n + j];
			inverse[p * n + j] = t;
		}
		for (i = 0; i < n; i++) {
			R f;
			if (i == k) {
				continue;
			}
			f = m[i * n + k] / m[k * n + k];
			for (j = 0; j < n; j++) {
				m[i * n + j] -= f * m[k * n + j];
				inverse[i * n + j] -= f * inverse[k * n + j];
			}
		}
	}
	for (i = 0; i < n; i++) {
		R s = 0;
		R d = m[i * n + i];
		for (j = 0; j < n; j++) {
			inverse[i * n + j] /= d;
			s += rabs(inverse[i * n + j]);
		}
		if (s > norm) {
			norm = s;
		}
	}
	return norm;
}

static R row_sum_norm(const R *a, int n)
{
	R norm = 0;
	int i, j;

	for (i = 0; i < n; i++) {
		R s = 0;
		for (j = 0; j < n; j++) {
			s += rabs(a[i * n + j]);
		}
		if (s > norm) {
			norm = s;
		}
	}
	return norm;
}

/* ------------------------------------------------------------ errors */

/* the largest error of the n values, divided by the largest of the reference
 * (or by floor, when that is larger), in epsilons
 */
static R error(const HYP_FLOAT *x, const R *reference, int n, R floor)
{
	R big = floor;
	R worst = 0;
	int i;

	for (i = 0; i < n; i++) {
		if (rabs(reference[i]) > big) {
			big = rabs(reference[i]);
		}
	}
	if (!(big > 0)) {
		big = 1;
	}
	for (i = 0; i < n; i++) {
		R e = rabs((R)x[i] - reference[i]);
		if (!(e <= worst)) {
			worst = e;
		}
	}
	return worst / big / EPSILON;
}

static R error1(HYP_FLOAT x, R reference)
{
	return error(&x, &reference, 1, 0);
}

/* quaternions up to sign */
static R errorq(const HYP_FLOAT *x, const R *q)
{
	R minus[4];
	R a, b;
	int i;

	for (i = 0; i < 4; i++) {
		minus[i] = -q[i];
	}
	a = error(x, q, 4, 0);
	b = error(x, minus, 4, 0);
	return a < b ? a : b;
}

/* ------------------------------------------------------------ conversions */

static void set3(struct vector3 *v, const R *r)
{
	vector3_setf3(v, (HYP_FLOAT)r[0], (HYP_FLOAT)r[1], (HYP_FLOAT)r[2]);
}

static void get3(const struct vector3 *v, R *r)
{
	r[0] = (R)v->x;
	r[1] = (R)v->y;
	r[2] = (R)v->z;
}

static void setq(struct quaternion *q, const R *r)
{
	quaternion_setf4(q, (HYP_FLOAT)r[0], (HYP_FLOAT)r[1], (HYP_FLOAT)r[2], (HYP_FLOAT)r[3]);
}

static void getq(const struct quaternion *q, R *r)
{
	int i;

	for (i = 0; i < 4; i++) {
		r[i] = (R)q->q[i];
	}
}

/* m (row-major, 4x4) into a matrix4, and back: m[] holds r00, r01, ... */
static void setm4(struct matrix4 *m, const R *r)
{
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m->m[i * 4 + j] = (HYP_FLOAT)r[i * 4 + j];
		}
	}
}

static void getm4(const struct matrix4 *m, R *r)
{
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			r[i * 4 + j] = (R)m->m[i * 4 + j];
		}
	}
}

/* the values of a matrix4 in row-major order, for comparing with a reference */
static void rowmajor4(const struct matrix4 *m, HYP_FLOAT *h)
{
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			h[i * 4 + j] = m->m[i * 4 + j];
		}
	}
}

/* ------------------------------------------------------------ measurements */

static void vectors(void)
{
	struct row *r;
	int i, k, kind;

	for (kind = 0; kind < 3; kind++) {
		static const char *names[3] = {"random", "components 1e-15 .. 1e15", "(1, 1e-5, 1e-5) and its permutations"};
		r = row("vector3_normalize", names[kind]);
		for (i = 0; i < N; i++) {
			R v[3], o[3], l;
			struct vector3 h;
			for (k = 0; k < 3; k++) {
				if (kind == 0) {
					v[k] = uniform(-10, 10);
				} else if (kind == 1) {
					v[k] = powf128(10, uniform(-15, 15)) * (random01() < (R)0.5 ? -1 : 1);
				} else {
					v[k] = (k == i % 3 ? 1 : (R)1e-5) * uniform(1, 2);
				}
			}
			set3(&h, v);
			get3(&h, v);
			l = rlength(v, 3);
			for (k = 0; k < 3; k++) {
				o[k] = v[k] / l;
			}
			vector3_normalize(&h);
			add(r, error(h.v, o, 3, 0));
		}
	}

	r = row("vector2_normalize", "random");
	for (i = 0; i < N; i++) {
		R v[2], o[2], l;
		struct vector2 h;
		vector2_setf2(&h, (HYP_FLOAT)uniform(-10, 10), (HYP_FLOAT)uniform(-10, 10));
		v[0] = (R)h.x;
		v[1] = (R)h.y;
		l = rlength(v, 2);
		o[0] = v[0] / l;
		o[1] = v[1] / l;
		vector2_normalize(&h);
		add(r, error(h.v, o, 2, 0));
	}

	r = row("vector4_normalize", "random");
	for (i = 0; i < N; i++) {
		R v[4], o[4], l;
		struct vector4 h;
		vector4_setf4(&h, (HYP_FLOAT)uniform(-10, 10), (HYP_FLOAT)uniform(-10, 10), (HYP_FLOAT)uniform(-10, 10), (HYP_FLOAT)uniform(-10, 10));
		for (k = 0; k < 4; k++) {
			v[k] = (R)h.v[k];
		}
		l = rlength(v, 4);
		for (k = 0; k < 4; k++) {
			o[k] = v[k] / l;
		}
		vector4_normalize(&h);
		add(r, error(h.v, o, 4, 0));
	}

	/* relative to |a| |b| */
	r = row("vector3_cross_product", "random");
	for (i = 0; i < N; i++) {
		R a[3], b[3], o[3];
		struct vector3 ha, hb, hr;
		for (k = 0; k < 3; k++) {
			a[k] = uniform(-10, 10);
			b[k] = uniform(-10, 10);
		}
		set3(&ha, a);
		set3(&hb, b);
		get3(&ha, a);
		get3(&hb, b);
		rcross(a, b, o);
		vector3_cross_product(&hr, &ha, &hb);
		add(r, error(hr.v, o, 3, rlength(a, 3) * rlength(b, 3)));
	}

	/* relative to |a|, the size of the projection of a */
	r = row("vector3_project", "random");
	for (i = 0; i < N; i++) {
		R a[3], b[3], o[3], s;
		struct vector3 ha, hb;
		for (k = 0; k < 3; k++) {
			a[k] = uniform(-10, 10);
			b[k] = uniform(-10, 10);
		}
		set3(&ha, a);
		set3(&hb, b);
		get3(&ha, a);
		get3(&hb, b);
		s = rdot3(a, b) / rdot3(b, b);
		for (k = 0; k < 3; k++) {
			o[k] = s * b[k];
		}
		vector3_project(&ha, &hb);
		add(r, error(ha.v, o, 3, rlength(a, 3)));
	}

	/* relative to |v| */
	r = row("vector3_reflect", "random");
	for (i = 0; i < N; i++) {
		R a[3], n[3], o[3], s;
		struct vector3 ha, hn;
		for (k = 0; k < 3; k++) {
			a[k] = uniform(-10, 10);
			n[k] = uniform(-10, 10);
		}
		set3(&ha, a);
		set3(&hn, n);
		get3(&ha, a);
		get3(&hn, n);
		s = 2 * rdot3(a, n) / rdot3(n, n);
		for (k = 0; k < 3; k++) {
			o[k] = a[k] - s * n[k];
		}
		vector3_reflect(&ha, &hn);
		add(r, error(ha.v, o, 3, rlength(a, 3)));
	}

	/* the largest errors are near the critical angle, where a change of the
	 * input direction by one ulp changes the result by eta^2 |d| / sqrt(k) ulps
	 */
	r = row("vector3_refract", "random, eta 0.5 .. 2");
	for (i = 0; i < N; i++) {
		R a[3], n[3], o[3], d, kk, eta;
		struct vector3 ha, hn;
		HYP_FLOAT heta = (HYP_FLOAT)uniform((R)0.5, 2);
		eta = (R)heta;
		random_unit(n, 3);
		random_unit(a, 3);
		if (rdot3(a, n) > 0) {
			for (k = 0; k < 3; k++) {
				a[k] = -a[k];
			}
		}
		set3(&ha, a);
		set3(&hn, n);
		get3(&ha, a);
		get3(&hn, n);
		rnormalize(a, 3);
		rnormalize(n, 3);
		d = rdot3(n, a);
		kk = 1 - eta * eta * (1 - d * d);
		for (k = 0; k < 3; k++) {
			o[k] = kk < 0 ? 0 : eta * a[k] - (eta * d + sqrtf128(kk)) * n[k];
		}
		vector3_refract(&ha, &hn, heta);
		add(r, error(ha.v, o, 3, 1));
	}

	for (kind = 0; kind < 3; kind++) {
		static const char *names[3] = {"random", "1e-6 rad apart", "1e-3 rad from opposite"};
		r = row("vector3_angle_between", names[kind]);
		for (i = 0; i < N; i++) {
			R a[3], b[3], c[3];
			struct vector3 ha, hb;
			random_unit(a, 3);
			if (kind == 0) {
				random_unit(b, 3);
			} else {
				turned(a, kind == 1 ? (R)1e-6 : PI - (R)1e-3, b);
			}
			for (k = 0; k < 3; k++) {
				a[k] *= uniform(1, 10);
				b[k] *= uniform(1, 10);
			}
			set3(&ha, a);
			set3(&hb, b);
			get3(&ha, a);
			get3(&hb, b);
			rcross(a, b, c);
			add(r, error1(vector3_angle_between(&ha, &hb), atan2f128(rlength(c, 3), rdot3(a, b))));
		}
	}

	r = row("vector2_angle_between", "random");
	for (i = 0; i < N; i++) {
		R a[2], b[2];
		struct vector2 ha, hb;
		vector2_setf2(&ha, (HYP_FLOAT)uniform(-10, 10), (HYP_FLOAT)uniform(-10, 10));
		vector2_setf2(&hb, (HYP_FLOAT)uniform(-10, 10), (HYP_FLOAT)uniform(-10, 10));
		a[0] = (R)ha.x;
		a[1] = (R)ha.y;
		b[0] = (R)hb.x;
		b[1] = (R)hb.y;
		add(r, error1(vector2_angle_between(&ha, &hb), atan2f128(rabs(a[0] * b[1] - a[1] * b[0]), a[0] * b[0] + a[1] * b[1])));
	}
}

/* v rotated by q of any length: (2 (u.v) u + (w^2 - u.u) v + 2 w (u x v)) / |q|^2 */
static void rrotate(const R *q, const R *v, R *o)
{
	R c[3];
	R uu = rdot3(q, q);
	R d = rdot3(q, v);
	R m = uu + q[3] * q[3];
	int k;

	rcross(q, v, c);
	for (k = 0; k < 3; k++) {
		o[k] = (2 * d * q[k] + (q[3] * q[3] - uu) * v[k] + 2 * q[3] * c[k]) / m;
	}
}

static void quaternions(void)
{
	struct row *r;
	int i, k, kind;

	for (kind = 0; kind < 2; kind++) {
		r = row("vector3_rotate_by_quaternion", kind ? "q of length 1 +- 1e-6" : "unit q");
		for (i = 0; i < N; i++) {
			R q[4], v[3], o[3];
			struct quaternion hq;
			struct vector3 hv;
			random_unit(q, 4);
			if (kind) {
				for (k = 0; k < 4; k++) {
					q[k] *= 1 + uniform(-1, 1) * (R)1e-6;
				}
			}
			for (k = 0; k < 3; k++) {
				v[k] = uniform(-10, 10);
			}
			setq(&hq, q);
			getq(&hq, q);
			set3(&hv, v);
			get3(&hv, v);
			rrotate(q, v, o);
			vector3_rotate_by_quaternion(&hv, &hq);
			add(r, error(hv.v, o, 3, 0));
		}
	}

	r = row("quaternion_normalize", "random length");
	for (i = 0; i < N; i++) {
		R q[4], o[4], l;
		struct quaternion h;
		random_unit(q, 4);
		l = powf128(10, uniform(-3, 3));
		for (k = 0; k < 4; k++) {
			q[k] *= l;
		}
		setq(&h, q);
		getq(&h, q);
		l = rlength(q, 4);
		for (k = 0; k < 4; k++) {
			o[k] = q[k] / l;
		}
		quaternion_normalize(&h);
		add(r, error(h.q, o, 4, 0));
	}

	r = row("quaternion_inverse", "random length");
	for (i = 0; i < N; i++) {
		R q[4], o[4], l;
		struct quaternion h;
		random_unit(q, 4);
		l = powf128(10, uniform(-3, 3));
		for (k = 0; k < 4; k++) {
			q[k] *= l;
		}
		setq(&h, q);
		getq(&h, q);
		l = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
		for (k = 0; k < 3; k++) {
			o[k] = -q[k] / l;
		}
		o[3] = q[3] / l;
		quaternion_inverse(&h);
		add(r, error(h.q, o, 4, 0));
	}

	/* relative to |a| |b| */
	r = row("quaternion_multiply", "random");
	for (i = 0; i < N; i++) {
		R a[4], b[4], o[4];
		struct quaternion ha, hb;
		for (k = 0; k < 4; k++) {
			a[k] = uniform(-1, 1);
			b[k] = uniform(-1, 1);
		}
		setq(&ha, a);
		setq(&hb, b);
		getq(&ha, a);
		getq(&hb, b);
		rqmul(a, b, o);
		quaternion_multiply(&ha, &hb);
		add(r, error(ha.q, o, 4, rlength(a, 4) * rlength(b, 4)));
	}

	for (kind = 0; kind < 2; kind++) {
		r = row("quaternion_set_from_axis_anglev3", kind ? "angle 1e-4" : "random");
		for (i = 0; i < N; i++) {
			R axis[3], o[4], angle;
			struct vector3 ha;
			struct quaternion h;
			HYP_FLOAT a;
			random_unit(axis, 3);
			a = (HYP_FLOAT)(kind ? (R)1e-4 : uniform(-PI, PI));
			angle = (R)a;
			set3(&ha, axis);
			get3(&ha, axis);
			rnormalize(axis, 3);
			for (k = 0; k < 3; k++) {
				o[k] = axis[k] * sinf128(angle / 2);
			}
			o[3] = cosf128(angle / 2);
			quaternion_set_from_axis_anglev3(&h, &ha, a);
			add(r, errorq(h.q, o));
		}
	}

	/* axis * angle, the rotation vector, with the angle in [0, pi] */
	for (kind = 0; kind < 2; kind++) {
		r = row("quaternion_get_axis_anglev3", kind ? "angle 1e-4 (axis * angle)" : "random (axis * angle)");
		for (i = 0; i < N; i++) {
			R q[4], o[3], got[3], vn, angle;
			struct quaternion h;
			struct vector3 axis;
			HYP_FLOAT ha;
			HYP_FLOAT hg[3];
			if (kind) {
				rotation_by((R)1e-4, q);
			} else {
				random_unit(q, 4);
			}
			setq(&h, q);
			getq(&h, q);
			if (q[3] < 0) {
				for (k = 0; k < 4; k++) {
					q[k] = -q[k];
				}
			}
			vn = rlength(q, 3);
			angle = 2 * atan2f128(vn, q[3]);
			for (k = 0; k < 3; k++) {
				o[k] = q[k] / vn * angle;
			}
			quaternion_get_axis_anglev3(&h, &axis, &ha);
			for (k = 0; k < 3; k++) {
				got[k] = (R)axis.v[k] * (R)ha;
				if ((R)ha > PI) {
					got[k] = -(R)axis.v[k] * (2 * PI - (R)ha);
				}
				hg[k] = (HYP_FLOAT)got[k];
			}
			add(r, error(hg, o, 3, 0));
		}
	}

	for (kind = 0; kind < 2; kind++) {
		r = row("quaternion_set_from_matrix4", kind ? "near half turns (pi - 1e-3)" : "random rotation");
		for (i = 0; i < N; i++) {
			R q[4], m[9], m16[16], x[9], xi[9], o[4];
			struct matrix4 hm;
			struct quaternion h;
			int it;
			if (kind) {
				rotation_by(PI - (R)1e-3, q);
			} else {
				random_unit(q, 4);
			}
			rotation_matrix(q, m);
			for (k = 0; k < 16; k++) {
				m16[k] = (k % 5 == 0) ? 1 : 0;
			}
			for (k = 0; k < 9; k++) {
				m16[(k / 3) * 4 + k % 3] = m[k];
			}
			setm4(&hm, m16);
			getm4(&hm, m16);
			/* the reference: the nearest rotation to the rounded matrix, by the
			 * polar iteration x = (x + x^-T) / 2
			 */
			for (k = 0; k < 9; k++) {
				x[k] = m16[(k / 3) * 4 + k % 3];
			}
			for (it = 0; it < 6; it++) {
				rinverse(x, xi, 3);
				for (k = 0; k < 9; k++) {
					x[k] = (x[k] + xi[(k % 3) * 3 + k / 3]) / 2;
				}
			}
			matrix_quaternion(x, o);
			quaternion_set_from_matrix4(&h, &hm);
			add(r, errorq(h.q, o));
		}
	}

	for (kind = 0; kind < 3; kind++) {
		static const char *names[3] = {"random", "1e-3 rad apart", "1e-6 rad apart"};
		r = row("quaternion_slerp", names[kind]);
		for (i = 0; i < N; i++) {
			R s[4], t[4], d[4], o[4], ns, nt, dif = 0, sum = 0, theta, f1, f2, u;
			struct quaternion hs, ht, h;
			HYP_FLOAT hu = (HYP_FLOAT)random01();
			u = (R)hu;
			random_unit(s, 4);
			if (kind == 0) {
				random_unit(t, 4);
			} else {
				rotation_by(kind == 1 ? (R)1e-3 : (R)1e-6, d);
				rqmul(s, d, t);
			}
			setq(&hs, s);
			setq(&ht, t);
			getq(&hs, s);
			getq(&ht, t);
			if (s[0] * t[0] + s[1] * t[1] + s[2] * t[2] + s[3] * t[3] < 0) {
				for (k = 0; k < 4; k++) {
					t[k] = -t[k];
				}
			}
			ns = rlength(s, 4);
			nt = rlength(t, 4);
			for (k = 0; k < 4; k++) {
				dif += (s[k] / ns - t[k] / nt) * (s[k] / ns - t[k] / nt);
				sum += (s[k] / ns + t[k] / nt) * (s[k] / ns + t[k] / nt);
			}
			theta = 2 * atan2f128(sqrtf128(dif), sqrtf128(sum));
			if (theta > 0) {
				f1 = sinf128((1 - u) * theta) / sinf128(theta);
				f2 = sinf128(u * theta) / sinf128(theta);
			} else {
				f1 = 1 - u;
				f2 = u;
			}
			for (k = 0; k < 4; k++) {
				o[k] = f1 * s[k] + f2 * t[k];
			}
			quaternion_slerp(&hs, &ht, hu, &h);
			add(r, error(h.q, o, 4, 0));
		}
	}

	/* where the rotation takes the unit from, against the unit to */
	for (kind = 0; kind < 3; kind++) {
		static const char *names[3] = {"random (landing error)", "1e-3 rad from opposite (landing error)", "1e-6 rad apart (landing error)"};
		r = row("quaternion_get_rotation_tov3", names[kind]);
		for (i = 0; i < N; i++) {
			R a[3], b[3], q[4], landed[3];
			struct vector3 ha, hb;
			struct quaternion h;
			R e = 0;
			random_unit(a, 3);
			if (kind == 0) {
				random_unit(b, 3);
			} else {
				turned(a, kind == 1 ? PI - (R)1e-3 : (R)1e-6, b);
			}
			for (k = 0; k < 3; k++) {
				a[k] *= uniform(1, 10);
				b[k] *= uniform(1, 10);
			}
			set3(&ha, a);
			set3(&hb, b);
			get3(&ha, a);
			get3(&hb, b);
			rnormalize(a, 3);
			rnormalize(b, 3);
			quaternion_get_rotation_tov3(&ha, &hb, &h);
			getq(&h, q);
			rrotate(q, a, landed);
			/* the landing point, compared with b: its rounding is not hypatia's */
			for (k = 0; k < 3; k++) {
				if (rabs(landed[k] - b[k]) > e) {
					e = rabs(landed[k] - b[k]);
				}
			}
			add(r, e / EPSILON);
		}
	}

	for (kind = 0; kind < 2; kind++) {
		r = row("quaternion_angle_between", kind ? "1e-4 rad apart" : "random");
		for (i = 0; i < N; i++) {
			R a[4], b[4], d[4], na, nb, dif = 0, sum = 0;
			struct quaternion ha, hb;
			random_unit(a, 4);
			if (kind) {
				rotation_by((R)1e-4, d);
				rqmul(a, d, b);
			} else {
				random_unit(b, 4);
			}
			setq(&ha, a);
			setq(&hb, b);
			getq(&ha, a);
			getq(&hb, b);
			if (a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3] < 0) {
				for (k = 0; k < 4; k++) {
					b[k] = -b[k];
				}
			}
			na = rlength(a, 4);
			nb = rlength(b, 4);
			for (k = 0; k < 4; k++) {
				dif += (a[k] / na - b[k] / nb) * (a[k] / na - b[k] / nb);
				sum += (a[k] / na + b[k] / nb) * (a[k] / na + b[k] / nb);
			}
			add(r, error1(quaternion_angle_between(&ha, &hb), 4 * atan2f128(sqrtf128(dif), sqrtf128(sum))));
		}
	}

	/* the camera turns -Z into forward, with +Y toward up */
	/* the largest errors are where forward is nearly parallel to up: the
	 * result changes by about 1 / sin(forward, up) times a change of forward
	 */
	r = row("quaternion_set_look_rotation_rh", "random");
	for (i = 0; i < N; i++) {
		R f[3], up[3], right[3], above[3], m[9], o[4];
		struct vector3 hf, hu;
		struct quaternion h;
		random_unit(f, 3);
		random_unit(up, 3);
		for (k = 0; k < 3; k++) {
			f[k] *= uniform(1, 10);
		}
		set3(&hf, f);
		set3(&hu, up);
		get3(&hf, f);
		get3(&hu, up);
		rnormalize(f, 3);
		rcross(f, up, right);
		rnormalize(right, 3);
		rcross(right, f, above);
		for (k = 0; k < 3; k++) {
			m[k * 3 + 0] = right[k];
			m[k * 3 + 1] = above[k];
			m[k * 3 + 2] = -f[k];
		}
		matrix_quaternion(m, o);
		quaternion_set_look_rotation_rh(&h, &hf, &hu);
		add(r, errorq(h.q, o));
	}
}

/* a random 4x4 orthogonal matrix (row-major): the left and right products
 * with two unit quaternions
 */
static void random_orthogonal4(R *m)
{
	R p[4], q[4], e[4], x[4];
	int i, j;

	random_unit(p, 4);
	random_unit(q, 4);
	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++) {
			e[i] = (i == j) ? 1 : 0;
		}
		rqmul(p, e, x);
		rqmul(x, q, e);
		for (i = 0; i < 4; i++) {
			m[i * 4 + j] = e[i];
		}
	}
}

static void matrices(void)
{
	struct row *r;
	int i, j, k, kind;

	/* the error over the condition number |A| |A^-1| (row sums): what any
	 * method can lose
	 */
	for (kind = 0; kind < 3; kind++) {
		static const char *names[3] = {"random entries (error / condition number)", "rotation, scale and translation (error / condition number)", "condition ~1e4 (error / condition number)"};
		r = row("matrix4_inverse", names[kind]);
		for (i = 0; i < N; i++) {
			R a[16], o[16], u[16], v[16], q[4], rot[9];
			HYP_FLOAT h[16];
			struct matrix4 hm, hi;
			R condition;
			if (kind == 0) {
				for (k = 0; k < 16; k++) {
					a[k] = uniform(-1, 1);
				}
			} else if (kind == 1) {
				random_unit(q, 4);
				rotation_matrix(q, rot);
				for (k = 0; k < 16; k++) {
					a[k] = 0;
				}
				for (j = 0; j < 3; j++) {
					R s = uniform((R)0.5, 2);
					for (k = 0; k < 3; k++) {
						a[k * 4 + j] = rot[k * 3 + j] * s;
					}
					a[j * 4 + 3] = uniform(-10, 10);
				}
				a[15] = 1;
			} else {
				static const R d[4] = {1, (R)0.1, (R)0.01, (R)1e-4};
				random_orthogonal4(u);
				random_orthogonal4(v);
				for (j = 0; j < 4; j++) {
					for (k = 0; k < 4; k++) {
						int l;
						a[j * 4 + k] = 0;
						for (l = 0; l < 4; l++) {
							a[j * 4 + k] += u[j * 4 + l] * d[l] * v[k * 4 + l];
						}
					}
				}
			}
			setm4(&hm, a);
			getm4(&hm, a);
			condition = row_sum_norm(a, 4) * rinverse(a, o, 4);
			matrix4_inverse(&hm, &hi);
			rowmajor4(&hi, h);
			add(r, error(h, o, 16, 0) / condition);
		}
	}

	r = row("matrix3_inverse", "random entries (error / condition number)");
	for (i = 0; i < N; i++) {
		R a[9], o[9];
		HYP_FLOAT h[9];
		struct matrix3 hm, hi;
		R condition;
		for (k = 0; k < 9; k++) {
			hm.m[k] = (HYP_FLOAT)uniform(-1, 1);
		}
		for (k = 0; k < 9; k++) {
			a[k] = (R)hm.m[k];
		}
		condition = row_sum_norm(a, 3) * rinverse(a, o, 3);
		matrix3_inverse(&hm, &hi);
		for (k = 0; k < 9; k++) {
			h[k] = hi.m[k];
		}
		add(r, error(h, o, 9, 0) / condition);
	}

	r = row("matrix2_inverse", "random entries (error / condition number)");
	for (i = 0; i < N; i++) {
		R a[4], o[4];
		HYP_FLOAT h[4];
		struct matrix2 hm, hi;
		R condition;
		for (k = 0; k < 4; k++) {
			hm.m[k] = (HYP_FLOAT)uniform(-1, 1);
		}
		for (k = 0; k < 4; k++) {
			a[k] = (R)hm.m[k];
		}
		condition = row_sum_norm(a, 2) * rinverse(a, o, 2);
		matrix2_inverse(&hm, &hi);
		for (k = 0; k < 4; k++) {
			h[k] = hi.m[k];
		}
		add(r, error(h, o, 4, 0) / condition);
	}

	/* relative to the product of the row lengths (Hadamard's bound) */
	r = row("matrix4_determinant", "random entries");
	for (i = 0; i < N; i++) {
		R a[16], scale = 1, det;
		struct matrix4 hm;
		for (k = 0; k < 16; k++) {
			a[k] = uniform(-1, 1);
		}
		setm4(&hm, a);
		getm4(&hm, a);
		/* the determinant by elimination with partial pivoting */
		{
			R m[16];
			int p;
			det = 1;
			for (k = 0; k < 16; k++) {
				m[k] = a[k];
			}
			for (k = 0; k < 4; k++) {
				p = k;
				for (j = k + 1; j < 4; j++) {
					if (rabs(m[j * 4 + k]) > rabs(m[p * 4 + k])) {
						p = j;
					}
				}
				if (p != k) {
					for (j = 0; j < 4; j++) {
						R t = m[k * 4 + j];
						m[k * 4 + j] = m[p * 4 + j];
						m[p * 4 + j] = t;
					}
					det = -det;
				}
				det *= m[k * 4 + k];
				for (j = k + 1; j < 4; j++) {
					R f = m[j * 4 + k] / m[k * 4 + k];
					int l;
					for (l = k; l < 4; l++) {
						m[j * 4 + l] -= f * m[k * 4 + l];
					}
				}
			}
		}
		for (k = 0; k < 4; k++) {
			scale *= rlength(&a[k * 4], 4);
		}
		add(r, rabs((R)matrix4_determinant(&hm) - det) / scale / EPSILON);
	}

	/* each entry relative to the sum of the absolute values of its terms */
	r = row("matrix4_multiply", "random");
	for (i = 0; i < N; i++) {
		R a[16], b[16];
		struct matrix4 ha, hb;
		R worst = 0;
		for (k = 0; k < 16; k++) {
			a[k] = uniform(-1, 1);
			b[k] = uniform(-1, 1);
		}
		setm4(&ha, a);
		setm4(&hb, b);
		getm4(&ha, a);
		getm4(&hb, b);
		/* self = mT * self */
		matrix4_multiply(&ha, &hb);
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 4; k++) {
				R exact = 0, size = 0;
				int l;
				for (l = 0; l < 4; l++) {
					exact += b[j * 4 + l] * a[l * 4 + k];
					size += rabs(b[j * 4 + l] * a[l * 4 + k]);
				}
				if (rabs((R)ha.m[j * 4 + k] - exact) / size > worst) {
					worst = rabs((R)ha.m[j * 4 + k] - exact) / size;
				}
			}
		}
		add(r, worst / EPSILON);
	}

	r = row("matrix4_multiplyv4", "random");
	for (i = 0; i < N; i++) {
		R a[16], v[4];
		struct matrix4 hm;
		struct vector4 hv, hr;
		R worst = 0;
		for (k = 0; k < 16; k++) {
			a[k] = uniform(-1, 1);
		}
		for (k = 0; k < 4; k++) {
			v[k] = uniform(-10, 10);
		}
		setm4(&hm, a);
		getm4(&hm, a);
		vector4_setf4(&hv, (HYP_FLOAT)v[0], (HYP_FLOAT)v[1], (HYP_FLOAT)v[2], (HYP_FLOAT)v[3]);
		for (k = 0; k < 4; k++) {
			v[k] = (R)hv.v[k];
		}
		matrix4_multiplyv4(&hm, &hv, &hr);
		for (j = 0; j < 4; j++) {
			R exact = 0, size = 0;
			for (k = 0; k < 4; k++) {
				exact += a[j * 4 + k] * v[k];
				size += rabs(a[j * 4 + k] * v[k]);
			}
			if (rabs((R)hr.v[j] - exact) / size > worst) {
				worst = rabs((R)hr.v[j] - exact) / size;
			}
		}
		add(r, worst / EPSILON);
	}

	r = row("matrix4_set_from_quaternion", "unit q");
	for (i = 0; i < N; i++) {
		R q[4], m[9], o[16];
		HYP_FLOAT h[16];
		struct quaternion hq;
		struct matrix4 hm;
		random_unit(q, 4);
		setq(&hq, q);
		getq(&hq, q);
		rotation_matrix(q, m);
		for (k = 0; k < 16; k++) {
			o[k] = (k % 5 == 0) ? 1 : 0;
		}
		for (k = 0; k < 9; k++) {
			o[(k / 3) * 4 + k % 3] = m[k];
		}
		matrix4_set_from_quaternion(&hm, &hq);
		rowmajor4(&hm, h);
		add(r, error(h, o, 16, 0));
	}

	r = row("matrix4_set_from_axisv3_angle", "random");
	for (i = 0; i < N; i++) {
		R axis[3], q[4], m[9], o[16], angle;
		HYP_FLOAT h[16];
		struct vector3 ha;
		struct matrix4 hm;
		HYP_FLOAT a = (HYP_FLOAT)uniform(-PI, PI);
		angle = (R)a;
		random_unit(axis, 3);
		set3(&ha, axis);
		get3(&ha, axis);
		rnormalize(axis, 3);
		for (k = 0; k < 3; k++) {
			q[k] = axis[k] * sinf128(angle / 2);
		}
		q[3] = cosf128(angle / 2);
		rotation_matrix(q, m);
		for (k = 0; k < 16; k++) {
			o[k] = (k % 5 == 0) ? 1 : 0;
		}
		for (k = 0; k < 9; k++) {
			o[(k / 3) * 4 + k % 3] = m[k];
		}
		matrix4_set_from_axisv3_angle(&hm, &ha, a);
		rowmajor4(&hm, h);
		add(r, error(h, o, 16, 0));
	}

	/* multiplied by the sine of the angle between the view and up, which any
	 * method loses when they are nearly parallel
	 */
	r = row("matrix4_view_lookat_rh", "random (error * sin(view, up))");
	for (i = 0; i < N; i++) {
		R eye[3], target[3], up[3], z[3], x[3], y[3], o[16], s;
		HYP_FLOAT h[16];
		struct vector3 he, ht, hu;
		struct matrix4 hm;
		for (k = 0; k < 3; k++) {
			eye[k] = uniform(-10, 10);
			target[k] = uniform(-10, 10);
		}
		random_unit(up, 3);
		set3(&he, eye);
		set3(&ht, target);
		set3(&hu, up);
		get3(&he, eye);
		get3(&ht, target);
		get3(&hu, up);
		for (k = 0; k < 3; k++) {
			z[k] = target[k] - eye[k];
		}
		rnormalize(z, 3);
		rcross(z, up, x);
		s = rlength(x, 3) / rlength(up, 3);
		rnormalize(x, 3);
		rcross(x, z, y);
		for (k = 0; k < 3; k++) {
			o[0 * 4 + k] = x[k];
			o[1 * 4 + k] = y[k];
			o[2 * 4 + k] = -z[k];
			o[3 * 4 + k] = 0;
		}
		o[3] = -rdot3(x, eye);
		o[7] = -rdot3(y, eye);
		o[11] = rdot3(z, eye);
		o[15] = 1;
		matrix4_view_lookat_rh(&hm, &he, &ht, &hu);
		rowmajor4(&hm, h);
		add(r, error(h, o, 16, 0) * s);
	}

	r = row("matrix4_projection_perspective_fovy_rh", "random");
	for (i = 0; i < N; i++) {
		R o[16], fovy, aspect, n, f, hh;
		HYP_FLOAT h[16];
		struct matrix4 hm;
		HYP_FLOAT hf = (HYP_FLOAT)uniform((R)0.5, 2), ha = (HYP_FLOAT)uniform((R)0.5, 2), hn = (HYP_FLOAT)uniform((R)0.01, 1), hfar = (HYP_FLOAT)uniform(10, 1000);
		fovy = (R)hf;
		aspect = (R)ha;
		n = (R)hn;
		f = (R)hfar;
		hh = cosf128(fovy / 2) / sinf128(fovy / 2);
		for (k = 0; k < 16; k++) {
			o[k] = 0;
		}
		o[0] = hh / aspect;
		o[5] = hh;
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
		o[10] = (f + n) / (n - f);
		o[11] = 2 * f * n / (n - f);
#else
		o[10] = f / (n - f);
		o[11] = n * f / (n - f);
#endif
		o[14] = -1;
		matrix4_projection_perspective_fovy_rh(&hm, hf, ha, hn, hfar);
		rowmajor4(&hm, h);
		add(r, error(h, o, 16, 0));
	}
}

/* ------------------------------------------------------------ table */

static void print_table(FILE *out)
{
	int i;

#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	fprintf(out, "# single precision");
#else
	fprintf(out, "# double precision");
#endif
#ifdef __VERSION__
	fprintf(out, ", compiler %s", __VERSION__);
#endif
	fprintf(out, "\n# function | inputs | largest error | mean error, in epsilons\n");
	for (i = 0; i < row_count; i++) {
		fprintf(out, "%s | %s | %.4g | %.4g\n", rows[i].function, rows[i].inputs, rows[i].largest, rows[i].sum / (double)rows[i].count);
	}
}

/* the line "function | inputs | largest | mean" split in place */
static int parse(char *line, char **function, char **inputs, double *largest, double *mean)
{
	char *field[4];
	char *p = line;
	int i;

	for (i = 0; i < 4; i++) {
		char *bar = strstr(p, " | ");
		field[i] = p;
		if (i < 3) {
			if (bar == NULL) {
				return 0;
			}
			*bar = '\0';
			p = bar + 3;
		}
	}
	*function = field[0];
	*inputs = field[1];
	*largest = strtod(field[2], NULL);
	*mean = strtod(field[3], NULL);
	return 1;
}

static int check(const char *path)
{
	char line[512];
	int seen[MAX_ROWS];
	int failed = 0;
	int i;
	FILE *f = fopen(path, "r");

	if (f == NULL) {
		printf("cannot open %s\n", path);
		return 1;
	}
	for (i = 0; i < row_count; i++) {
		seen[i] = 0;
	}
	while (fgets(line, sizeof line, f) != NULL) {
		char *function, *inputs;
		double largest, mean;
		line[strcspn(line, "\n")] = '\0';
		if (line[0] == '\0' || line[0] == '#') {
			continue;
		}
		if (!parse(line, &function, &inputs, &largest, &mean)) {
			printf("not a row: %s\n", line);
			failed = 1;
			continue;
		}
		for (i = 0; i < row_count; i++) {
			if (strcmp(rows[i].function, function) == 0 && strcmp(rows[i].inputs, inputs) == 0) {
				break;
			}
		}
		if (i == row_count) {
			printf("FAIL  %s | %s: in the baseline, not measured\n", function, inputs);
			failed = 1;
			continue;
		}
		seen[i] = 1;
		{
			double now_mean = rows[i].sum / (double)rows[i].count;
			double now_largest = rows[i].largest;
			if (now_mean > mean * 1.01 + 1e-12 || now_largest > largest * 1.10 + 1e-12) {
				printf("FAIL  %s | %s: largest %.4g, mean %.4g; baseline %.4g, %.4g\n", function, inputs, now_largest, now_mean, largest, mean);
				failed = 1;
			} else if (now_mean < mean * 0.99 || now_largest < largest * 0.90) {
				printf("better %s | %s: largest %.4g, mean %.4g; baseline %.4g, %.4g: update the baseline\n", function, inputs, now_largest, now_mean, largest, mean);
			}
		}
	}
	fclose(f);
	for (i = 0; i < row_count; i++) {
		if (!seen[i]) {
			printf("FAIL  %s | %s: measured, not in the baseline\n", rows[i].function, rows[i].inputs);
			failed = 1;
		}
	}
	printf("%s: %d measurements, %s\n", path, row_count, failed ? "FAILED" : "none worse than the baseline");
	return failed;
}

int main(int argc, char **argv)
{
	vectors();
	quaternions();
	matrices();

	if (argc == 3 && strcmp(argv[1], "--check") == 0) {
		return check(argv[2]);
	}
	print_table(stdout);
	return 0;
}
