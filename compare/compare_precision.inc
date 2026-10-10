// SPDX-License-Identifier: MIT
// precision (included by compare.cpp): the error of each library against the
// exact answer for the same rounded inputs, computed in long double.  The error
// is in units of the HYP_FLOAT epsilon, relative to the largest component of
// the exact result (relative to the value itself for scalars).

using LV3 = Eigen::Matrix<L, 3, 1>;
using LV4 = Eigen::Matrix<L, 4, 1>;
using LM3 = Eigen::Matrix<L, 3, 3>;
using LQ = Eigen::Quaternion<L>;

static const double EPS = SINGLE ? 1.1920928955078125e-07 : 2.220446049250313e-16;

struct Acc {
	double max = -1, sum = 0;
	long n = 0;
	void add(double e)
	{
		if (std::isnan(e))
			e = INFINITY;
		max = std::max(max, e);
		sum += std::isinf(e) ? 0 : e;
		n++;
	}
	std::string str() const
	{
		if (n == 0)
			return "";
		char b[64];
		if (std::isinf(max))
			std::snprintf(b, sizeof b, "inf");
		else
			std::snprintf(b, sizeof b, "%.3g / %.2g", max, sum / n);
		return b;
	}
};

struct PRow {
	std::string fn, inputs;
	Acc h, g, e, c;
};
static std::deque<PRow> prows;

/* error of n components against the oracle, in epsilons of the largest |oracle| */
template <typename T> static double ulps(const T *x, const L *o, int n, L floor_ = 0)
{
	L big = floor_, m = 0;
	for (int i = 0; i < n; i++)
		big = std::max(big, std::fabs(o[i]));
	if (big == 0)
		big = 1;
	for (int i = 0; i < n; i++) {
		if (std::isnan((double)x[i]))
			return INFINITY;
		m = std::max(m, std::fabs((L)x[i] - o[i]));
	}
	return (double)(m / big) / EPS;
}
static double ulp1(double x, L o) { return ulps(&x, &o, 1); }
static double ulpsc(double x, L o, L scale) { return ulps(&x, &o, 1, scale); }

/* quaternion error up to sign */
static double ulpq(const H *x, const LQ &o)
{
	L a[4] = {o.x(), o.y(), o.z(), o.w()}, b[4] = {-o.x(), -o.y(), -o.z(), -o.w()};
	return std::min(ulps(x, a, 4), ulps(x, b, 4));
}

static LV3 lv(const struct vector3 &v) { return LV3(v.x, v.y, v.z); }
static LQ lq(const struct quaternion &q) { return LQ(q.w, q.x, q.y, q.z); }
static LM4 lm(const struct matrix4 &m)
{
	LM4 l;
	for (int i = 0; i < 16; i++)
		l(i / 4, i % 4) = m.m[i];
	return l;
}
/* row-major numbers of a long double matrix */
static void lrow(const LM4 &l, L *o)
{
	for (int i = 0; i < 16; i++)
		o[i] = l(i / 4, i % 4);
}
static void grow(const M4 &g, H *o)
{
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			o[r * 4 + c] = g[c][r];
}
static LM4 l4(const LM3 &r)
{
	LM4 l = LM4::Identity();
	l.block<3, 3>(0, 0) = r;
	return l;
}

#ifdef HAVE_CGLM
static void crow(mat4 m, H *o)
{
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			o[r * 4 + c] = m[c][r];
}
#endif

/* a unit vector a small angle from v (in long double, then rounded) */
static struct vector3 near_vector(const struct vector3 &v, double angle, bool opposite)
{
	LV3 a = lv(v).normalized(), p = a.unitOrthogonal();
	LV3 w = a * std::cos((L)angle) + p * std::sin((L)angle);
	if (opposite)
		w = -w;
	struct vector3 r;
	vector3_setf3(&r, (H)w.x(), (H)w.y(), (H)w.z());
	return r;
}

static struct quaternion near_quaternion(const struct quaternion &q, double angle)
{
	LQ d(Eigen::AngleAxis<L>((L)angle, lv(runit3())));
	LQ r = lq(q) * d;
	struct quaternion h;
	quaternion_setf4(&h, (H)r.x(), (H)r.y(), (H)r.z(), (H)r.w());
	return h;
}

/* the row-sum (infinity) norm */
static L row_norm(const LM4 &m)
{
	L n = 0;
	for (int r = 0; r < 4; r++)
		n = std::max(n, m.row(r).cwiseAbs().sum());
	return n;
}

static PRow &prow(const char *fn, const char *inputs)
{
	prows.push_back(PRow());
	prows.back().fn = fn;
	prows.back().inputs = inputs;
	return prows.back();
}

static void precision()
{
	const int N = 20000; /* more than the other sections: the maximum is noisy */

	/* ---------------------------------------------------------- vectors */
	for (int kind = 0; kind < 3; kind++) {
		static const char *in[3] = {"random", "components 1e-20 .. 1e20 (single 1e-15 .. 1e15)", "one large, two small (1, 1e-5, 1e-5)"};
		PRow &w = prow("vector3_normalize", in[kind]);
		for (int i = 0; i < N; i++) {
			struct vector3 v = rv3();
			if (kind == 1) {
				double e = SINGLE ? 15 : 20;
				for (int k = 0; k < 3; k++)
					v.v[k] = (H)(std::pow(10.0, r(-e, e)) * (rnd() < 0.5 ? -1 : 1));
			} else if (kind == 2) {
				vector3_setf3(&v, r(0.5, 2), r(-1e-5, 1e-5), r(-1e-5, 1e-5));
			}
			LV3 o3 = lv(v).normalized();
			L o[3] = {o3.x(), o3.y(), o3.z()};
			struct vector3 h = v;
			vector3_normalize(&h);
			w.h.add(ulps(h.v, o, 3));
			V3 g = glm::normalize(gv(v));
			w.g.add(ulps(&g[0], o, 3));
			EV3 e = ev(v).normalized();
			w.e.add(ulps(e.data(), o, 3));
#ifdef HAVE_CGLM
			vec3 c = {v.x, v.y, v.z};
			glm_vec3_normalize(c);
			w.c.add(ulps(c, o, 3));
#endif
		}
	}
	{
		PRow &w = prow("vector3_magnitude", "random");
		for (int i = 0; i < N; i++) {
			struct vector3 v = rv3();
			L o = lv(v).norm();
			w.h.add(ulp1(vector3_magnitude(&v), o));
			w.g.add(ulp1(glm::length(gv(v)), o));
			w.e.add(ulp1(ev(v).norm(), o));
#ifdef HAVE_CGLM
			vec3 c = {v.x, v.y, v.z};
			w.c.add(ulp1(glm_vec3_norm(c), o));
#endif
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("vector3_dot_product", kind ? "nearly perpendicular" : "random");
		for (int i = 0; i < N; i++) {
			struct vector3 a = rv3(), b = rv3();
			if (kind) {
				LV3 p = lv(a).cross(lv(rv3())).normalized() * (L)r(1, 10);
				LV3 q = p + lv(a).normalized() * (L)1e-4;
				vector3_setf3(&b, (H)q.x(), (H)q.y(), (H)q.z());
			}
			L o = lv(a).dot(lv(b));
			/* relative to |a| |b|: the size of the terms */
			L scale = lv(a).norm() * lv(b).norm();
			w.h.add(ulpsc(vector3_dot_product(&a, &b), o, scale));
			w.g.add(ulpsc(glm::dot(gv(a), gv(b)), o, scale));
			w.e.add(ulpsc(ev(a).dot(ev(b)), o, scale));
#ifdef HAVE_CGLM
			w.c.add(ulpsc(glm_vec3_dot(a.v, b.v), o, scale));
#endif
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("vector3_cross_product", kind ? "nearly parallel (1e-4 rad)" : "random");
		for (int i = 0; i < N; i++) {
			struct vector3 a = rv3(), b = rv3(), h;
			if (kind) {
				b = near_vector(a, 1e-4, false);
				vector3_multiplyf(&b, r(1, 10));
			}
			LV3 o3 = lv(a).cross(lv(b));
			L o[3] = {o3.x(), o3.y(), o3.z()};
			/* relative to |a| |b| */
			L scale = lv(a).norm() * lv(b).norm();
			vector3_cross_product(&h, &a, &b);
			w.h.add(ulps(h.v, o, 3, scale));
			V3 g = glm::cross(gv(a), gv(b));
			w.g.add(ulps(&g[0], o, 3, scale));
			EV3 e = ev(a).cross(ev(b));
			w.e.add(ulps(e.data(), o, 3, scale));
#ifdef HAVE_CGLM
			vec3 c;
			glm_vec3_cross(a.v, b.v, c);
			w.c.add(ulps(c, o, 3, scale));
#endif
		}
	}
	{
		PRow &w = prow("vector3_project", "random");
		for (int i = 0; i < N; i++) {
			struct vector3 a = rv3(), b = rv3(), h = a;
			LV3 ob = lv(b);
			LV3 o3 = ob * (lv(a).dot(ob) / ob.dot(ob));
			L o[3] = {o3.x(), o3.y(), o3.z()};
			L scale = lv(a).norm();
#ifndef HYP_MASTER
			vector3_project(&h, &b);
			w.h.add(ulps(h.v, o, 3, scale));
#endif
			V3 g = glm::proj(gv(a), gv(b));
			w.g.add(ulps(&g[0], o, 3, scale));
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("vector3_rotate_by_quaternion", kind ? "q of length 1 +- 1e-6 (drifted)" : "unit q");
		for (int i = 0; i < N; i++) {
			struct vector3 v = rv3(), h = v;
			struct quaternion q = rq_unit();
			if (kind)
				quaternion_multiplyf(&q, (H)(1 + r(-1e-6, 1e-6)));
			LV3 o3 = lq(q).normalized() * lv(v);
			L o[3] = {o3.x(), o3.y(), o3.z()};
			vector3_rotate_by_quaternion(&h, &q);
			w.h.add(ulps(h.v, o, 3));
			V3 g = gq(q) * gv(v);
			w.g.add(ulps(&g[0], o, 3));
			EV3 e = eq(q) * ev(v);
			w.e.add(ulps(e.data(), o, 3));
#ifdef HAVE_CGLM
			versor c = {q.x, q.y, q.z, q.w};
			vec3 out;
			glm_quat_rotatev(c, v.v, out);
			w.c.add(ulps(out, o, 3));
#endif
		}
	}

	/* ---------------------------------------------------------- matrices */
	{
		PRow &w = prow("matrix4_multiply", "random");
		for (int i = 0; i < N; i++) {
			struct matrix4 a = rm4(), b = rm4(), h = a;
			L o[16];
			lrow(lm(b) * lm(a), o);
			/* relative to the size of the terms: |B| |A| elementwise */
			L scale = (lm(b).cwiseAbs() * lm(a).cwiseAbs()).maxCoeff();
			matrix4_multiply(&h, &b);
			w.h.add(ulps(h.m, o, 16, scale));
			H g[16];
			grow(g4(b) * g4(a), g);
			w.g.add(ulps(g, o, 16, scale));
			EM4 e = e4(b) * e4(a);
			H eh[16];
			for (int k = 0; k < 16; k++)
				eh[k] = e(k / 4, k % 4);
			w.e.add(ulps(eh, o, 16, scale));
#ifdef HAVE_CGLM
			M4 ga = g4(a), gb = g4(b);
			mat4 ca, cb, cr;
			std::memcpy(ca, &ga[0][0], sizeof ca);
			std::memcpy(cb, &gb[0][0], sizeof cb);
			glm_mat4_mul(cb, ca, cr);
			H c[16];
			crow(cr, c);
			w.c.add(ulps(c, o, 16, scale));
#endif
		}
	}
	for (int kind = 0; kind < 3; kind++) {
		static const char *in[3] = {"random entries (error / condition number)", "rotation, scale and translation (error / condition number)", "condition ~1e4 (error / condition number)"};
		PRow &w = prow("matrix4_inverse", in[kind]);
		for (int i = 0; i < N; i++) {
			struct matrix4 a = kind == 0 ? rm4() : kind == 1 ? rtrs() : rm4_cond(4), h;
			L o[16];
			LM4 li = lm(a).inverse();
			lrow(li, o);
			/* the error any inverse can have grows with the condition number:
			 * measure the error per unit of it
			 */
			double k = (double)(row_norm(lm(a)) * row_norm(li));
			matrix4_inverse(&a, &h);
			w.h.add(1 / k * ulps(h.m, o, 16));
			H g[16];
			grow(glm::inverse(g4(a)), g);
			w.g.add(1 / k * ulps(g, o, 16));
			EM4 e = e4(a).inverse();
			H eh[16];
			for (int k = 0; k < 16; k++)
				eh[k] = e(k / 4, k % 4);
			w.e.add(1 / k * ulps(eh, o, 16));
#ifdef HAVE_CGLM
			M4 ga = g4(a);
			mat4 ca, cr;
			std::memcpy(ca, &ga[0][0], sizeof ca);
			glm_mat4_inv(ca, cr);
			H c[16];
			crow(cr, c);
			w.c.add(1 / k * ulps(c, o, 16));
#endif
		}
	}
	{
		PRow &w = prow("matrix3_inverse", "random entries (error / condition number)");
		for (int i = 0; i < N; i++) {
			struct matrix3 a = rm3(), h;
			LM3 l;
			for (int k = 0; k < 9; k++)
				l(k / 3, k % 3) = a.m[k];
			LM3 li = l.inverse();
			L n3 = 0, n3i = 0;
			for (int rr = 0; rr < 3; rr++) {
				n3 = std::max(n3, l.row(rr).cwiseAbs().sum());
				n3i = std::max(n3i, li.row(rr).cwiseAbs().sum());
			}
			double k = (double)(n3 * n3i);
			L o[9];
			for (int k = 0; k < 9; k++)
				o[k] = li(k / 3, k % 3);
			matrix3_inverse(&a, &h);
			w.h.add(1 / k * ulps(h.m, o, 9));
			struct matrix3 g = h3(glm::inverse(g3(a)));
			w.g.add(1 / k * ulps(g.m, o, 9));
			Eigen::Matrix<H, 3, 3> e3;
			for (int k = 0; k < 9; k++)
				e3(k / 3, k % 3) = a.m[k];
			Eigen::Matrix<H, 3, 3> ei = e3.inverse();
			H eh[9];
			for (int k = 0; k < 9; k++)
				eh[k] = ei(k / 3, k % 3);
			w.e.add(1 / k * ulps(eh, o, 9));
		}
	}
	{
		PRow &w = prow("matrix4_determinant", "random entries");
		for (int i = 0; i < N; i++) {
			struct matrix4 a = rm4();
			L o = lm(a).determinant();
			/* relative to the largest product of four entries */
			L big = lm(a).cwiseAbs().maxCoeff();
			L scale = big * big * big * big;
			w.h.add(ulpsc(matrix4_determinant(&a), o, scale));
			w.g.add(ulpsc(glm::determinant(g4(a)), o, scale));
			w.e.add(ulpsc(e4(a).determinant(), o, scale));
#ifdef HAVE_CGLM
			M4 ga = g4(a);
			mat4 ca;
			std::memcpy(ca, &ga[0][0], sizeof ca);
			w.c.add(ulpsc(glm_mat4_det(ca), o, scale));
#endif
		}
	}
	{
		PRow &w = prow("matrix4_normal_matrix", "rotation, scale and translation");
		for (int i = 0; i < N; i++) {
			struct matrix4 a = rtrs(), h;
			L o[16];
			lrow(l4(lm(a).block<3, 3>(0, 0).inverse().transpose()), o);
#ifndef HYP_MASTER
			matrix4_normal_matrix(&a, &h);
			w.h.add(ulps(h.m, o, 16));
#endif
			H g[16];
			grow(M4(glm::inverseTranspose(M3(g4(a)))), g);
			w.g.add(ulps(g, o, 16));
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("matrix4_set_from_axisv3_angle", kind ? "angle 1e-4" : "random");
		for (int i = 0; i < N; i++) {
			struct vector3 axis = runit3();
			H a = kind ? (H)1e-4 : rangle();
			struct matrix4 h;
			L o[16];
			lrow(l4(Eigen::AngleAxis<L>((L)a, lv(axis).normalized()).toRotationMatrix()), o);
			matrix4_set_from_axisv3_angle(&h, &axis, a);
			w.h.add(ulps(h.m, o, 16));
			H g[16];
			grow(glm::rotate(M4(1), a, gv(axis)), g);
			w.g.add(ulps(g, o, 16));
			Eigen::Matrix<H, 3, 3> e = Eigen::AngleAxis<H>(a, ev(axis)).toRotationMatrix();
			H eh[16];
			for (int k = 0; k < 16; k++)
				eh[k] = (k / 4 < 3 && k % 4 < 3) ? e(k / 4, k % 4) : (k == 15 ? 1 : 0);
			w.e.add(ulps(eh, o, 16));
#ifdef HAVE_CGLM
			mat4 cm;
			glm_rotate_make(cm, a, axis.v);
			H c[16];
			crow(cm, c);
			w.c.add(ulps(c, o, 16));
#endif
		}
	}
	{
		PRow &w = prow("matrix4_set_from_quaternion", "unit q");
		for (int i = 0; i < N; i++) {
			struct quaternion q = rq_unit();
			struct matrix4 h;
			L o[16];
			lrow(l4(lq(q).normalized().toRotationMatrix()), o);
			matrix4_set_from_quaternion(&h, &q);
			w.h.add(ulps(h.m, o, 16));
			H g[16];
			grow(glm::mat4_cast(gq(q)), g);
			w.g.add(ulps(g, o, 16));
			Eigen::Matrix<H, 3, 3> e = eq(q).toRotationMatrix();
			H eh[16];
			for (int k = 0; k < 16; k++)
				eh[k] = (k / 4 < 3 && k % 4 < 3) ? e(k / 4, k % 4) : (k == 15 ? 1 : 0);
			w.e.add(ulps(eh, o, 16));
#ifdef HAVE_CGLM
			versor c = {q.x, q.y, q.z, q.w};
			mat4 cm;
			glm_quat_mat4(c, cm);
			H cc[16];
			crow(cm, cc);
			w.c.add(ulps(cc, o, 16));
#endif
		}
	}
	{
		PRow &w = prow("matrix4_set_from_euler_anglesf3", "random");
		for (int i = 0; i < N; i++) {
			H x = rangle(), y = rangle(), z = rangle();
			struct matrix4 h;
			LM3 rz = Eigen::AngleAxis<L>((L)z, LV3::UnitZ()).toRotationMatrix();
			LM3 ry = Eigen::AngleAxis<L>((L)y, LV3::UnitY()).toRotationMatrix();
			LM3 rx = Eigen::AngleAxis<L>((L)x, LV3::UnitX()).toRotationMatrix();
			L o[16];
			lrow(l4(rz * ry * rx), o);
			matrix4_set_from_euler_anglesf3(&h, x, y, z);
			w.h.add(ulps(h.m, o, 16));
			H g[16];
			grow(glm::eulerAngleZYX(z, y, x), g);
			w.g.add(ulps(g, o, 16));
#ifdef HAVE_CGLM
			vec3 ang = {x, y, z};
			mat4 cm;
			glm_euler_zyx(ang, cm);
			H c[16];
			crow(cm, c);
			w.c.add(ulps(c, o, 16));
#endif
		}
	}
	{
		PRow &w = prow("matrix4_projection_perspective_fovy_rh", "random");
		for (int i = 0; i < N; i++) {
			H fovy = r(0.2, 2.5), aspect = r(0.5, 2.5), n = r(0.01, 2), fa = n + r(1, 1000);
			struct matrix4 h;
			L t = 1 / std::tan((L)fovy / 2);
			LM4 l = LM4::Zero();
			l(0, 0) = t / aspect;
			l(1, 1) = t;
			l(3, 2) = -1;
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
			l(2, 2) = ((L)fa + n) / ((L)n - fa);
			l(2, 3) = 2 * (L)fa * n / ((L)n - fa);
#else
			l(2, 2) = (L)fa / ((L)n - fa);
			l(2, 3) = (L)fa * n / ((L)n - fa);
#endif
			L o[16];
			lrow(l, o);
			matrix4_projection_perspective_fovy_rh(&h, fovy, aspect, n, fa);
			w.h.add(ulps(h.m, o, 16));
			H g[16];
			grow(GLM_PERSP_RH(fovy, aspect, n, fa), g);
			w.g.add(ulps(g, o, 16));
#ifdef HAVE_CGLM
			mat4 cm;
			glm_perspective_rh_zo(fovy, aspect, n, fa, cm);
			H c[16];
			crow(cm, c);
			w.c.add(ulps(c, o, 16));
#endif
		}
	}
	{
		PRow &w = prow("matrix4_view_lookat_rh", "random (error * sin(view, up))");
		for (int i = 0; i < N; i++) {
			struct vector3 eye = rv3(), target = rv3(), up = runit3();
			struct matrix4 h;
			LV3 f = (lv(target) - lv(eye)).normalized();
			LV3 s = f.cross(lv(up)).normalized();
			/* nearly parallel view and up directions magnify any rounding:
			 * measure the error per unit of 1 / sin(angle between them)
			 */
			double k = (double)(1 / f.cross(lv(up).normalized()).norm());
			LV3 u = s.cross(f);
			LM4 l = LM4::Identity();
			l.row(0) << s.x(), s.y(), s.z(), -s.dot(lv(eye));
			l.row(1) << u.x(), u.y(), u.z(), -u.dot(lv(eye));
			l.row(2) << -f.x(), -f.y(), -f.z(), f.dot(lv(eye));
			L o[16];
			lrow(l, o);
			matrix4_view_lookat_rh(&h, &eye, &target, &up);
			w.h.add(1 / k * ulps(h.m, o, 16));
			H g[16];
			grow(glm::lookAtRH(gv(eye), gv(target), gv(up)), g);
			w.g.add(1 / k * ulps(g, o, 16));
#ifdef HAVE_CGLM
			vec3 ce = {eye.x, eye.y, eye.z}, ct = {target.x, target.y, target.z}, cu = {up.x, up.y, up.z};
			mat4 cm;
			glm_lookat_rh(ce, ct, cu, cm);
			H c[16];
			crow(cm, c);
			w.c.add(1 / k * ulps(c, o, 16));
#endif
		}
	}

	/* ---------------------------------------------------------- quaternions */
	{
		PRow &w = prow("quaternion_multiply", "random");
		for (int i = 0; i < N; i++) {
			struct quaternion a = rq_any(), b = rq_any(), h = a;
			LQ o4 = lq(a) * lq(b);
			L o[4] = {o4.x(), o4.y(), o4.z(), o4.w()};
			L scale = lq(a).norm() * lq(b).norm();
			quaternion_multiply(&h, &b);
			w.h.add(ulps(h.q, o, 4, scale));
			struct quaternion g = hq(gq(a) * gq(b));
			w.g.add(ulps(g.q, o, 4, scale));
			struct quaternion e = hq(EQ(eq(a) * eq(b)));
			w.e.add(ulps(e.q, o, 4, scale));
#ifdef HAVE_CGLM
			versor ca = {a.x, a.y, a.z, a.w}, cb = {b.x, b.y, b.z, b.w}, cr;
			glm_quat_mul(ca, cb, cr);
			w.c.add(ulps(cr, o, 4, scale));
#endif
		}
	}
	{
		PRow &w = prow("quaternion_normalize", "random length");
		for (int i = 0; i < N; i++) {
			struct quaternion a = rq_any(), h = a;
			LQ o4 = lq(a).normalized();
			L o[4] = {o4.x(), o4.y(), o4.z(), o4.w()};
			quaternion_normalize(&h);
			w.h.add(ulps(h.q, o, 4));
			struct quaternion g = hq(glm::normalize(gq(a)));
			w.g.add(ulps(g.q, o, 4));
			struct quaternion e = hq(EQ(eq(a).normalized()));
			w.e.add(ulps(e.q, o, 4));
#ifdef HAVE_CGLM
			versor c = {a.x, a.y, a.z, a.w};
			glm_quat_normalize(c);
			w.c.add(ulps(c, o, 4));
#endif
		}
	}
	{
		PRow &w = prow("quaternion_inverse", "random length");
		for (int i = 0; i < N; i++) {
			struct quaternion a = rq_any(), h = a;
			LQ o4 = lq(a).inverse();
			L o[4] = {o4.x(), o4.y(), o4.z(), o4.w()};
			quaternion_inverse(&h);
			w.h.add(ulps(h.q, o, 4));
			struct quaternion g = hq(glm::inverse(gq(a)));
			w.g.add(ulps(g.q, o, 4));
			struct quaternion e = hq(EQ(eq(a).inverse()));
			w.e.add(ulps(e.q, o, 4));
#ifdef HAVE_CGLM
			versor c = {a.x, a.y, a.z, a.w}, cr;
			glm_quat_inv(c, cr);
			w.c.add(ulps(cr, o, 4));
#endif
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("quaternion_set_from_axis_anglev3", kind ? "angle 1e-4" : "random");
		for (int i = 0; i < N; i++) {
			struct vector3 axis = runit3();
			H a = kind ? (H)1e-4 : rangle();
			struct quaternion h;
			LQ o4(Eigen::AngleAxis<L>((L)a, lv(axis).normalized()));
			L o[4] = {o4.x(), o4.y(), o4.z(), o4.w()};
			quaternion_set_from_axis_anglev3(&h, &axis, a);
			w.h.add(ulps(h.q, o, 4));
			struct quaternion g = hq(glm::angleAxis(a, gv(axis)));
			w.g.add(ulps(g.q, o, 4));
			struct quaternion e = hq(EQ(Eigen::AngleAxis<H>(a, ev(axis))));
			w.e.add(ulps(e.q, o, 4));
#ifdef HAVE_CGLM
			versor c;
			glm_quatv(c, a, axis.v);
			w.c.add(ulps(c, o, 4));
#endif
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("quaternion_get_axis_anglev3", kind ? "angle 1e-4 (axis * angle)" : "random (axis * angle)");
		for (int i = 0; i < N; i++) {
			struct quaternion q = rq_unit();
			if (kind) {
				struct quaternion id;
				quaternion_identity(&id);
				q = near_quaternion(id, 1e-4);
			}
			/* the rotation vector: axis * angle with the angle in [0, pi] */
			LQ l = lq(q);
			if (l.w() < 0)
				l.coeffs() = -l.coeffs();
			L vn = l.vec().norm();
			LV3 rv = vn > 0 ? LV3(l.vec() / vn * (2 * std::atan2(vn, l.w()))) : LV3(0, 0, 0);
			L o[3] = {rv.x(), rv.y(), rv.z()};
			struct vector3 axis;
			H angle;
			quaternion_get_axis_anglev3(&q, &axis, &angle);
			H hv3[3] = {axis.x * angle, axis.y * angle, axis.z * angle};
			w.h.add(ulps(hv3, o, 3));
			H ga = glm::angle(gq(q));
			V3 gx = glm::axis(gq(q));
			if (ga > (H)PI) {
				ga = (H)(2 * PI) - ga;
				gx = -gx;
			}
			H gv3[3] = {gx.x * ga, gx.y * ga, gx.z * ga};
			w.g.add(ulps(gv3, o, 3));
			Eigen::AngleAxis<H> ea(eq(q));
			H ev3[3] = {ea.axis().x() * ea.angle(), ea.axis().y() * ea.angle(), ea.axis().z() * ea.angle()};
			w.e.add(ulps(ev3, o, 3));
#ifdef HAVE_CGLM
			versor c = {q.x, q.y, q.z, q.w};
			vec3 cx;
			float ca = glm_quat_angle(c);
			glm_quat_axis(c, cx);
			if (ca > (float)PI) {
				ca = (float)(2 * PI) - ca;
				glm_vec3_negate(cx);
			}
			H cv3[3] = {cx[0] * ca, cx[1] * ca, cx[2] * ca};
			w.c.add(ulps(cv3, o, 3));
#endif
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("quaternion_set_from_matrix4", kind ? "near half turns (pi - 1e-3)" : "random rotation");
		for (int i = 0; i < N; i++) {
			struct quaternion q = rq_unit(), h;
			if (kind) {
				struct vector3 axis = runit3();
				quaternion_set_from_axis_anglev3(&q, &axis, (H)(PI - 1e-3));
			}
			struct matrix4 m;
			matrix4_set_from_quaternion(&m, &q);
			/* the exact quaternion of the rounded matrix */
			LQ o(LM3(lm(m).block<3, 3>(0, 0)));
			o.normalize();
#ifndef HYP_MASTER
			quaternion_set_from_matrix4(&h, &m);
			w.h.add(ulpq(h.q, o));
#endif
			struct quaternion g = hq(glm::quat_cast(g4(m)));
			w.g.add(ulpq(g.q, o));
			struct quaternion e = hq(EQ(Eigen::Matrix<H, 3, 3>(e4(m).block<3, 3>(0, 0))));
			w.e.add(ulpq(e.q, o));
#ifdef HAVE_CGLM
			M4 gm = g4(m);
			mat4 cm;
			std::memcpy(cm, &gm[0][0], sizeof cm);
			versor c;
			glm_mat4_quat(cm, c);
			w.c.add(ulpq(c, o));
#endif
		}
	}
	for (int kind = 0; kind < 3; kind++) {
		static const char *in[3] = {"random", "1e-3 rad apart", "1e-6 rad apart"};
		PRow &w = prow("quaternion_slerp", in[kind]);
		for (int i = 0; i < N; i++) {
			struct quaternion a = rq_unit(), b = kind ? near_quaternion(a, kind == 1 ? 1e-3 : 1e-6) : rq_unit(), h;
			H t = r(0, 1);
			LQ la = lq(a).normalized(), lb = lq(b).normalized();
			LQ o4 = la.slerp((L)t, lb);
			L o[4] = {o4.x(), o4.y(), o4.z(), o4.w()};
			quaternion_slerp(&a, &b, t, &h);
			w.h.add(ulpq(h.q, o4));
			struct quaternion g = hq(glm::slerp(gq(a), gq(b), t));
			w.g.add(ulpq(g.q, o4));
			struct quaternion e = hq(EQ(eq(a).slerp(t, eq(b))));
			w.e.add(ulpq(e.q, o4));
#ifdef HAVE_CGLM
			versor ca = {a.x, a.y, a.z, a.w}, cb = {b.x, b.y, b.z, b.w}, cr;
			glm_quat_slerp(ca, cb, t, cr);
			w.c.add(ulpq(cr, o4));
#endif
			(void)o;
		}
	}
	for (int kind = 0; kind < 3; kind++) {
		static const char *in[3] = {"random (landing error)", "1e-3 rad from opposite (landing error)", "1e-6 rad apart (landing error)"};
		PRow &w = prow("quaternion_get_rotation_tov3", in[kind]);
		for (int i = 0; i < N; i++) {
			struct vector3 a = rv3(), b = kind == 0 ? rv3() : near_vector(a, kind == 1 ? 1e-3 : 1e-6, kind == 1);
			struct quaternion h;
			quaternion_get_rotation_tov3(&a, &b, &h);
			w.h.add(landing_error(a, b, h) / EPS);
			w.g.add(landing_error(a, b, hq(glm::rotation(glm::normalize(gv(a)), glm::normalize(gv(b))))) / EPS);
			w.e.add(landing_error(a, b, hq(EQ(EQ::FromTwoVectors(ev(a), ev(b))))) / EPS);
#ifdef HAVE_CGLM
			struct vector3 na = a, nb = b;
			vector3_normalize(&na);
			vector3_normalize(&nb);
			versor c;
			glm_quat_from_vecs(na.v, nb.v, c);
			struct quaternion cq;
			quaternion_setf4(&cq, c[0], c[1], c[2], c[3]);
			w.c.add(landing_error(a, b, cq) / EPS);
#endif
		}
	}
	for (int kind = 0; kind < 2; kind++) {
		PRow &w = prow("quaternion_angle_between", kind ? "1e-4 rad apart" : "random");
		for (int i = 0; i < N; i++) {
			struct quaternion a = rq_unit(), b = kind ? near_quaternion(a, 1e-4) : rq_unit();
			L o = lq(a).normalized().angularDistance(lq(b).normalized());
			w.h.add(ulp1(quaternion_angle_between(&a, &b), o));
			w.e.add(ulp1(eq(a).angularDistance(eq(b)), o));
		}
	}

	/* print */
	std::printf("\n## Precision against long double\n\n"
		    "Largest / mean error in units of the %s epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over %d inputs.  Lower is better; the best of each row is in bold.\n\n"
		    "| function | inputs | hypatia | GLM | Eigen | cglm |\n|---|---|---|---|---|---|\n",
		    SINGLE ? "float" : "double", N);
	for (PRow &w : prows) {
		double best = INFINITY;
		for (Acc *a : {&w.h, &w.g, &w.e, &w.c})
			if (a->n)
				best = std::min(best, a->max);
		auto cell = [&](Acc &a) {
			std::string s = a.str();
			if (a.n && a.max <= best * 1.0001 + 1e-9)
				s = "**" + s + "**";
			return s;
		};
		std::printf("| %s | %s | %s | %s | %s | %s |\n", w.fn.c_str(), w.inputs.c_str(), cell(w.h).c_str(), cell(w.g).c_str(), cell(w.e).c_str(), cell(w.c).c_str());
	}
}
