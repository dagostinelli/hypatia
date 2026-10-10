// SPDX-License-Identifier: MIT
// matrix comparisons (included by compare.cpp)

/* the GLM projections for the configured depth range */
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
#define GLM_PERSP_RH glm::perspectiveRH_NO
#define GLM_PERSP_LH glm::perspectiveLH_NO
#define GLM_ORTHO_RH glm::orthoRH_NO
#define GLM_ORTHO_LH glm::orthoLH_NO
#define DEPTH_NAME "_NO"
#else
#define GLM_PERSP_RH glm::perspectiveRH_ZO
#define GLM_PERSP_LH glm::perspectiveLH_ZO
#define GLM_ORTHO_RH glm::orthoRH_ZO
#define GLM_ORTHO_LH glm::orthoLH_ZO
#define DEPTH_NAME "_ZO"
#endif

/* LAPACK (double): inverse, determinant from the LU factors, and the
 * infinity-norm reciprocal condition estimate
 */
static bool lapack_inverse(const double *rowmajor, int n, double *inv_rowmajor, double *det, double *rcond)
{
	double a[16], work[64];
	int ipiv[4], iwork[4], info, lwork = 64;
	char norm = 'I';
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			a[c * n + r] = rowmajor[r * n + c];
	double anorm = dlange_(&norm, &n, &n, a, &n, work);
	dgetrf_(&n, &n, a, &n, ipiv, &info);
	if (info != 0)
		return false;
	*det = 1;
	for (int i = 0; i < n; i++)
		*det *= (ipiv[i] != i + 1 ? -1 : 1) * a[i * n + i];
	dgecon_(&norm, &n, a, &n, &anorm, rcond, work, iwork, &info);
	dgetri_(&n, a, &n, ipiv, work, &lwork, &info);
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			inv_rowmajor[r * n + c] = a[c * n + r];
	return info == 0;
}

/* a 4x4 with condition number about 10^k: Q1 diag(1 .. 10^-k) Q2 */
static struct matrix4 rm4_cond(double k)
{
	Eigen::Matrix<double, 4, 4> a, b, d = Eigen::Matrix<double, 4, 4>::Zero();
	for (int i = 0; i < 16; i++) {
		a(i / 4, i % 4) = rnd() - 0.5;
		b(i / 4, i % 4) = rnd() - 0.5;
	}
	Eigen::HouseholderQR<Eigen::Matrix<double, 4, 4> > qa(a), qb(b);
	for (int i = 0; i < 4; i++)
		d(i, i) = std::pow(10.0, -k * i / 3.0);
	Eigen::Matrix<double, 4, 4> m = qa.householderQ() * d * qb.householderQ().transpose();
	struct matrix4 h;
	for (int i = 0; i < 16; i++)
		h.m[i] = (H)m(i / 4, i % 4);
	return h;
}

/* the inverse in long double from the matrix as stored */
[[maybe_unused]] static LM4 oracle_inverse(const struct matrix4 &m)
{
	LM4 l;
	for (int i = 0; i < 16; i++)
		l(i / 4, i % 4) = m.m[i];
	return l.inverse();
}

/* the largest element error relative to the largest element of the oracle */
[[maybe_unused]] static double rel_to_max(const H *a, const LM4 &o)
{
	L big = o.cwiseAbs().maxCoeff(), m = 0;
	for (int i = 0; i < 16; i++)
		m = std::max(m, std::fabs(a[i] - o(i / 4, i % 4)));
	return (double)(m / big);
}

#ifndef PRECISION_ONLY
static void matrices()
{
	/* multiply: which order */
	begin("matrices", "matrix4_multiply(A, B)", "glm B * A", "README: sets self to mT * self");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4(), b = rm4(), h = a;
		matrix4_multiply(&h, &b);
		check(err_m4(h, g4(b) * g4(a)), [&] { return sm4(a) + ", " + sm4(b) + " -> " + sm4(h); });
	}
	begin("matrices", "matrix3_multiply(A, B)", "glm B * A");
	for (int i = 0; i < N; i++) {
		struct matrix3 a = rm3(), b = rm3(), h = a;
		matrix3_multiply(&h, &b);
		check(err_m3(h, g3(b) * g3(a)), [&] { return sm3(a) + ", " + sm3(b) + " -> " + sm3(h); });
	}
	begin("matrices", "matrix2_multiply(A, B)", "glm B * A");
	for (int i = 0; i < N; i++) {
		struct matrix2 a = rm2(), b = rm2(), h = a;
		matrix2_multiply(&h, &b);
		check(err_m2(h, g2(b) * g2(a)), [&] { return sm2(a) + ", " + sm2(b) + " -> " + sm2(h); });
	}
#ifdef HAVE_CGLM
	begin("matrices", "matrix4_multiply(A, B)", "cglm glm_mat4_mul(B, A)");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4(), b = rm4(), h = a;
		matrix4_multiply(&h, &b);
		mat4 ca, cb, cr;
		M4 ga = g4(a), gb = g4(b);
		std::memcpy(ca, &ga[0][0], sizeof ca);
		std::memcpy(cb, &gb[0][0], sizeof cb);
		glm_mat4_mul(cb, ca, cr);
		M4 gr;
		std::memcpy(&gr[0][0], cr, sizeof cr);
		check(err_m4(h, gr), [&] { return sm4(a) + ", " + sm4(b) + " -> " + sm4(h); });
	}
#endif
	begin("matrices", "matrix4_multiplyv4", "glm M * v");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rm4();
		struct vector4 a = rv4(), h;
		matrix4_multiplyv4(&m, &a, &h);
		struct vector4 g = hv(g4(m) * gv(a));
		check(err_v(h.v, g.v, 4), [&] { return sm4(m) + " * " + sv(a.v, 4) + " -> " + sv(h.v, 4) + " vs " + sv(g.v, 4); });
	}
	begin("matrices", "matrix4_multiplyv3", "glm (M * (v, 1)).xyz");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rm4();
		struct vector3 a = rv3(), h;
		matrix4_multiplyv3(&m, &a, &h);
		V4 p = g4(m) * V4(gv(a), 1);
		struct vector3 g;
		vector3_setf3(&g, p.x, p.y, p.z);
		check(err_v(h.v, g.v, 3), [&] { return sm4(m) + " * " + sv(a.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("matrices", "matrix4_multiplyv2", "glm (M * (v, 0, 1)).xy");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rm4();
		struct vector2 a = rv2(), h;
		matrix4_multiplyv2(&m, &a, &h);
		V4 p = g4(m) * V4(a.x, a.y, 0, 1);
		struct vector2 g;
		vector2_setf2(&g, p.x, p.y);
		check(err_v(h.v, g.v, 2), [&] { return sm4(m) + " * " + sv(a.v, 2) + " -> " + sv(h.v, 2) + " vs " + sv(g.v, 2); });
	}
	begin("matrices", "matrix3_multiplyv2", "glm (M * (v, 1)).xy");
	for (int i = 0; i < N; i++) {
		struct matrix3 m = rm3();
		struct vector2 a = rv2(), h;
		matrix3_multiplyv2(&m, &a, &h);
		V3 p = g3(m) * V3(gv(a), 1);
		struct vector2 g;
		vector2_setf2(&g, p.x, p.y);
		check(err_v(h.v, g.v, 2), [&] { return sm3(m) + " * " + sv(a.v, 2) + " -> " + sv(h.v, 2) + " vs " + sv(g.v, 2); });
	}
	begin("matrices", "matrix2_multiplyv2", "glm M * v");
	for (int i = 0; i < N; i++) {
		struct matrix2 m = rm2();
		struct vector2 a = rv2(), h;
		matrix2_multiplyv2(&m, &a, &h);
		struct vector2 g = hv(g2(m) * gv(a));
		check(err_v(h.v, g.v, 2), [&] { return sm2(m) + " * " + sv(a.v, 2) + " -> " + sv(h.v, 2) + " vs " + sv(g.v, 2); });
	}

	/* transpose, determinant */
	begin("matrices", "matrix4_transpose", "glm::transpose");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4(), h = a;
		matrix4_transpose(&h);
		check(err_m4(h, glm::transpose(g4(a))), [&] { return sm4(a) + " -> " + sm4(h); });
	}
	begin("matrices", "matrix3_transpose", "glm::transpose");
	for (int i = 0; i < N; i++) {
		struct matrix3 a = rm3(), h = a;
		matrix3_transpose(&h);
		check(err_m3(h, glm::transpose(g3(a))), [&] { return sm3(a) + " -> " + sm3(h); });
	}
	begin("matrices", "matrix4_determinant", "glm::determinant");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4();
		H h = matrix4_determinant(&a), g = glm::determinant(g4(a));
		check(e1(h, g) / 100, [&] { return sm4(a) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("matrices", "matrix4_determinant", "Eigen long double", "relative to |det|; entries up to 5");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4();
		H h = matrix4_determinant(&a);
		LM4 l;
		for (int k = 0; k < 16; k++)
			l(k / 4, k % 4) = a.m[k];
		double o = (double)l.determinant();
		check(std::fabs(h - o) / std::max(1.0, std::fabs(o)), [&] { return sm4(a) + " -> " + f(h) + " vs " + f(o); });
	}
	begin("matrices", "matrix4_determinant", "LAPACK dgetrf", "LAPACK in double; single precision rounding of the matrix products");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4();
		double d[16], inv[16], det, rc;
		dbl<16>(a.m, d);
		lapack_inverse(d, 4, inv, &det, &rc);
		H h = matrix4_determinant(&a);
		check(std::fabs(h - det) / std::max(1.0, std::fabs(det)), [&] { return sm4(a) + " -> " + f(h) + " vs " + f(det); });
	}
	begin("matrices", "matrix3_determinant", "glm::determinant");
	for (int i = 0; i < N; i++) {
		struct matrix3 a = rm3();
		H h = matrix3_determinant(&a), g = glm::determinant(g3(a));
		check(e1(h, g) / 10, [&] { return sm3(a) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("matrices", "matrix2_determinant", "glm::determinant");
	for (int i = 0; i < N; i++) {
		struct matrix2 a = rm2();
		H h = matrix2_determinant(&a), g = glm::determinant(g2(a));
		check(e1(h, g), [&] { return sm2(a) + " -> " + f(h) + " vs " + f(g); });
	}

	/* inverse: against each library, then the accuracy of each against long double */
	begin("matrices", "matrix4_inverse", "glm::inverse");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4(), h;
		matrix4_inverse(&a, &h);
		check(err_m4(h, glm::inverse(g4(a))) / 100, [&] { return sm4(a) + " -> " + sm4(h); });
	}
	begin("matrices", "matrix4_inverse", "Eigen inverse");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4(), h;
		matrix4_inverse(&a, &h);
		check(err_e4(h, e4(a).inverse()) / 100, [&] { return sm4(a) + " -> " + sm4(h); });
	}
	begin("matrices", "matrix4_inverse", "LAPACK dgetri");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4(), h;
		double d[16], inv[16], det, rc, hd[16];
		dbl<16>(a.m, d);
		lapack_inverse(d, 4, inv, &det, &rc);
		matrix4_inverse(&a, &h);
		dbl<16>(h.m, hd);
		check(en(hd, inv, 16) / 100, [&] { return sm4(a) + " -> " + sm4(h); });
	}
#ifdef HAVE_CGLM
	begin("matrices", "matrix4_inverse", "cglm glm_mat4_inv");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4(), h;
		matrix4_inverse(&a, &h);
		M4 ga = g4(a), gr;
		mat4 ca, cr;
		std::memcpy(ca, &ga[0][0], sizeof ca);
		glm_mat4_inv(ca, cr);
		std::memcpy(&gr[0][0], cr, sizeof cr);
		check(err_m4(h, gr) / 100, [&] { return sm4(a) + " -> " + sm4(h); });
	}
#endif
	begin("matrices", "matrix3_inverse", "glm::inverse");
	for (int i = 0; i < N; i++) {
		struct matrix3 a = rm3(), h;
		matrix3_inverse(&a, &h);
		check(err_m3(h, glm::inverse(g3(a))) / 100, [&] { return sm3(a) + " -> " + sm3(h); });
	}
	begin("matrices", "matrix2_inverse", "glm::inverse");
	for (int i = 0; i < N; i++) {
		struct matrix2 a = rm2(), h;
		matrix2_inverse(&a, &h);
		check(err_m2(h, glm::inverse(g2(a))) / 100, [&] { return sm2(a) + " -> " + sm2(h); });
	}

	/* accuracy against long double, for well and badly conditioned matrices */
	for (int k = 0; k <= 6; k += 3) {
		static char note[3][64];
		std::snprintf(note[k / 3], sizeof note[0], "condition ~1e%d; error relative to the largest element", k);
		double worst_h = 0, worst_g = 0, worst_e = 0, worst_l = 0;
#ifdef HAVE_CGLM
		double worst_c = 0;
#endif
		for (int i = 0; i < N; i++) {
			struct matrix4 a = rm4_cond(k), h;
			LM4 o = oracle_inverse(a);
			matrix4_inverse(&a, &h);
			worst_h = std::max(worst_h, rel_to_max(h.m, o));
			struct matrix4 g = h4(glm::inverse(g4(a)));
			worst_g = std::max(worst_g, rel_to_max(g.m, o));
			EM4 ei = e4(a).inverse();
			H eh[16];
			for (int j = 0; j < 16; j++)
				eh[j] = ei(j / 4, j % 4);
			worst_e = std::max(worst_e, rel_to_max(eh, o));
			double d[16], inv[16], det, rc;
			dbl<16>(a.m, d);
			lapack_inverse(d, 4, inv, &det, &rc);
			H lh[16];
			for (int j = 0; j < 16; j++)
				lh[j] = (H)inv[j];
			worst_l = std::max(worst_l, rel_to_max(lh, o));
#ifdef HAVE_CGLM
			M4 ga = g4(a), gr;
			mat4 ca, cr;
			std::memcpy(ca, &ga[0][0], sizeof ca);
			glm_mat4_inv(ca, cr);
			std::memcpy(&gr[0][0], cr, sizeof cr);
			struct matrix4 c = h4(gr);
			worst_c = std::max(worst_c, rel_to_max(c.m, o));
#endif
		}
		begin("inverse accuracy (largest error over the random matrices)", "matrix4_inverse", "long double", note[k / 3]);
		cur->n = N;
		cur->maxerr = worst_h;
		begin("inverse accuracy (largest error over the random matrices)", "glm::inverse", "long double", note[k / 3]);
		cur->n = N;
		cur->maxerr = worst_g;
		begin("inverse accuracy (largest error over the random matrices)", "Eigen inverse", "long double", note[k / 3]);
		cur->n = N;
		cur->maxerr = worst_e;
		begin("inverse accuracy (largest error over the random matrices)", "LAPACK dgetri (double), rounded", "long double", note[k / 3]);
		cur->n = N;
		cur->maxerr = worst_l;
#ifdef HAVE_CGLM
		begin("inverse accuracy (largest error over the random matrices)", "cglm glm_mat4_inv", "long double", note[k / 3]);
		cur->n = N;
		cur->maxerr = worst_c;
#endif
	}

	/* reciprocal condition */
	begin("matrices", "matrix4_reciprocal_condition", "1 / (norm(A) norm(A^-1)), infinity norm, in long double", "relative error up to 1e-2 (single) or 1e-5 (double); single precision is limited by the float inverse: see the accuracy table");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rm4_cond(rnd() * 6);
		H h = matrix4_reciprocal_condition(&a);
		LM4 l;
		for (int k = 0; k < 16; k++)
			l(k / 4, k % 4) = a.m[k];
		LM4 li = l.inverse();
		L na = 0, ni = 0;
		for (int rr = 0; rr < 4; rr++) {
			na = std::max(na, l.row(rr).cwiseAbs().sum());
			ni = std::max(ni, li.row(rr).cwiseAbs().sum());
		}
		double o = (double)(1 / (na * ni));
		check(std::fabs(h - o) / o * (SINGLE ? 1e-2 : 1e-5), [&] { return sm4(a) + " -> " + f(h) + " vs " + f(o); });
	}
	/* normal matrix */
	begin("matrices", "matrix4_normal_matrix", "glm::inverseTranspose(mat3(M))");
	for (int i = 0; i < N; i++) {
		struct matrix4 a = rtrs(), h;
		matrix4_normal_matrix(&a, &h);
		M4 g = M4(glm::inverseTranspose(M3(g4(a))));
		check(err_m4(h, g), [&] { return sm4(a) + " -> " + sm4(h); });
	}

	/* builders */
	begin("matrix builders", "matrix4_make_transformation_translationv3", "glm::translate(I, v)");
	for (int i = 0; i < N; i++) {
		struct vector3 v = rv3();
		struct matrix4 h;
		matrix4_make_transformation_translationv3(&h, &v);
		check(err_m4(h, glm::translate(M4(1), gv(v))), [&] { return sv(v.v, 3) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_make_transformation_scalingv3", "glm::scale(I, v)");
	for (int i = 0; i < N; i++) {
		struct vector3 v = rv3();
		struct matrix4 h;
		matrix4_make_transformation_scalingv3(&h, &v);
		check(err_m4(h, glm::scale(M4(1), gv(v))), [&] { return sv(v.v, 3) + " -> " + sm4(h); });
	}
	const char *axes[3] = {"x", "y", "z"};
	for (int ax = 0; ax < 3; ax++) {
		static char name[3][64];
		std::snprintf(name[ax], sizeof name[0], "matrix4_make_transformation_rotationf_%s", axes[ax]);
		begin("matrix builders", name[ax], "glm::rotate(I, a, axis)");
		for (int i = 0; i < N; i++) {
			H a = rangle();
			struct matrix4 h;
			V3 axis(ax == 0, ax == 1, ax == 2);
			if (ax == 0)
				matrix4_make_transformation_rotationf_x(&h, a);
			else if (ax == 1)
				matrix4_make_transformation_rotationf_y(&h, a);
			else
				matrix4_make_transformation_rotationf_z(&h, a);
			check(err_m4(h, glm::rotate(M4(1), a, axis)), [&] { return f(a) + " -> " + sm4(h); });
		}
	}
	begin("matrix builders", "matrix4_set_from_axisv3_angle", "glm::rotate(I, a, axis)");
	for (int i = 0; i < N; i++) {
		struct vector3 axis = runit3();
		H a = rangle();
		struct matrix4 h;
		matrix4_set_from_axisv3_angle(&h, &axis, a);
		check(err_m4(h, glm::rotate(M4(1), a, gv(axis))), [&] { return sv(axis.v, 3) + ", " + f(a) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_set_from_axisv3_angle", "glm::rotate(I, a, axis)", "axis not unit length");
	for (int i = 0; i < N; i++) {
		struct vector3 axis = rv3();
		H a = rangle();
		struct matrix4 h;
		matrix4_set_from_axisv3_angle(&h, &axis, a);
		check(err_m4(h, glm::rotate(M4(1), a, gv(axis))), [&] { return sv(axis.v, 3) + ", " + f(a) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_set_from_quaternion", "glm::mat4_cast");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit();
		struct matrix4 h;
		matrix4_set_from_quaternion(&h, &q);
		check(err_m4(h, glm::mat4_cast(gq(q))), [&] { return sq(q) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_set_from_quaternion", "glm::mat4_cast(normalize(q))", "q not unit length");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_any();
		struct matrix4 h;
		matrix4_set_from_quaternion(&h, &q);
		check(err_m4(h, glm::mat4_cast(glm::normalize(gq(q)))), [&] { return sq(q) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_make_transformation_rotationq", "Eigen toRotationMatrix");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit();
		struct matrix4 h;
		matrix4_make_transformation_rotationq(&h, &q);
		EM4 e = EM4::Identity();
		e.block<3, 3>(0, 0) = eq(q).toRotationMatrix();
		check(err_e4(h, e), [&] { return sq(q) + " -> " + sm4(h); });
	}
#ifdef HAVE_CGLM
	begin("matrix builders", "matrix4_make_transformation_rotationq", "cglm glm_quat_mat4");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit();
		struct matrix4 h;
		matrix4_make_transformation_rotationq(&h, &q);
		versor c = {q.x, q.y, q.z, q.w};
		mat4 cm;
		glm_quat_mat4(c, cm);
		M4 g;
		std::memcpy(&g[0][0], cm, sizeof cm);
		check(err_m4(h, g), [&] { return sq(q) + " -> " + sm4(h); });
	}
#endif
	begin("matrix builders", "matrix4_set_from_euler_anglesf3(x, y, z)", "glm::eulerAngleZYX(z, y, x) = Rz Ry Rx", "X first, then Y, then Z");
	for (int i = 0; i < N; i++) {
		H x = rangle(), y = rangle(), z = rangle();
		struct matrix4 h;
		matrix4_set_from_euler_anglesf3(&h, x, y, z);
		check(err_m4(h, glm::eulerAngleZYX(z, y, x)), [&] { return f(x) + ", " + f(y) + ", " + f(z) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_set_from_euler_anglesf3(x, y, z)", "glm::eulerAngleXYZ(x, y, z)", "for reference: GLM's XYZ is Rx Ry Rz");
	for (int i = 0; i < N; i++) {
		H x = rangle(), y = rangle(), z = rangle();
		struct matrix4 h;
		matrix4_set_from_euler_anglesf3(&h, x, y, z);
		check(err_m4(h, glm::eulerAngleXYZ(x, y, z)), [&] { return f(x) + ", " + f(y) + ", " + f(z) + " -> " + sm4(h); });
	}
#ifdef HAVE_CGLM
	for (int zyx = 0; zyx < 2; zyx++) {
		begin("matrix builders", "matrix4_set_from_euler_anglesf3(x, y, z)", zyx ? "cglm glm_euler_zyx" : "cglm glm_euler_xyz",
		      zyx ? "" : "for reference: cglm's xyz is GLM's XYZ");
		for (int i = 0; i < N; i++) {
			H x = rangle(), y = rangle(), z = rangle();
			struct matrix4 h;
			matrix4_set_from_euler_anglesf3(&h, x, y, z);
			vec3 ang = {x, y, z};
			mat4 cm;
			if (zyx)
				glm_euler_zyx(ang, cm);
			else
				glm_euler_xyz(ang, cm);
			M4 g;
			std::memcpy(&g[0][0], cm, sizeof cm);
			check(err_m4(h, g), [&] { return f(x) + ", " + f(y) + ", " + f(z) + " -> " + sm4(h); });
		}
	}
#endif
	begin("matrix builders", "matrix4_make_transformation_rotationv3(v)", "glm::eulerAngleZYX(v.z, v.y, v.x)");
	for (int i = 0; i < N; i++) {
		struct vector3 v;
		vector3_setf3(&v, rangle(), rangle(), rangle());
		struct matrix4 h;
		matrix4_make_transformation_rotationv3(&h, &v);
		check(err_m4(h, glm::eulerAngleZYX(v.z, v.y, v.x)), [&] { return sv(v.v, 3) + " -> " + sm4(h); });
	}

	/* composing: matrix4_translatev3(M, v) etc. */
	begin("matrix builders", "matrix4_translatev3(M, v)", "glm T(v) * M", "applies the translation after M");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rtrs(), h = m;
		struct vector3 v = rv3();
		matrix4_translatev3(&h, &v);
		check(err_m4(h, glm::translate(M4(1), gv(v)) * g4(m)), [&] { return sm4(m) + ", " + sv(v.v, 3) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_translatev3(M, v)", "glm::translate(M, v) = M * T(v)", "for reference: GLM applies it before M");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rtrs(), h = m;
		struct vector3 v = rv3();
		matrix4_translatev3(&h, &v);
		check(err_m4(h, glm::translate(g4(m), gv(v))), [&] { return sm4(m) + ", " + sv(v.v, 3) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_rotatev3(M, axis, a)", "glm R * M");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rtrs(), h = m;
		struct vector3 axis = runit3();
		H a = rangle();
		matrix4_rotatev3(&h, &axis, a);
		check(err_m4(h, glm::rotate(M4(1), a, gv(axis)) * g4(m)), [&] { return sm4(m) + ", " + sv(axis.v, 3) + ", " + f(a) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_scalev3(M, v)", "glm S * M");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rtrs(), h = m;
		struct vector3 v = rv3();
		matrix4_scalev3(&h, &v);
		check(err_m4(h, glm::scale(M4(1), gv(v)) * g4(m)), [&] { return sm4(m) + ", " + sv(v.v, 3) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_transformation_compose(s, q, t)", "glm T * R * S");
	for (int i = 0; i < N; i++) {
		struct vector3 s, t = rv3();
		vector3_setf3(&s, r(0.2, 4), r(0.2, 4), r(0.2, 4));
		struct quaternion q = rq_unit();
		struct matrix4 h;
		matrix4_transformation_compose(&h, &s, &q, &t);
		M4 g = glm::translate(M4(1), gv(t)) * glm::mat4_cast(gq(q)) * glm::scale(M4(1), gv(s));
		check(err_m4(h, g), [&] { return sv(s.v, 3) + ", " + sq(q) + ", " + sv(t.v, 3) + " -> " + sm4(h); });
	}
	begin("matrix builders", "matrix4_transformation_decompose", "glm::decompose", "scale, rotation (q or -q), translation");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rtrs();
		struct vector3 s, t;
		struct quaternion q;
		matrix4_transformation_decompose(&m, &s, &q, &t);
		V3 gs, gt, sk;
		V4 persp;
		Q gr;
		glm::decompose(g4(m), gs, gr, gt, sk, persp);
		struct vector3 hs = hv(gs), ht = hv(gt);
		check(std::max(std::max(err_v(s.v, hs.v, 3), err_v(t.v, ht.v, 3)), err_qr(q, hq(gr))),
		      [&] { return sm4(m) + " -> " + sv(s.v, 3) + sq(q) + sv(t.v, 3) + " vs " + sv(hs.v, 3) + sq(hq(gr)) + sv(ht.v, 3); });
	}
#ifdef HAVE_CGLM
	begin("matrix builders", "matrix4_transformation_decompose", "cglm glm_decompose", "scale and translation");
	for (int i = 0; i < N; i++) {
		struct matrix4 m = rtrs();
		struct vector3 s, t;
		struct quaternion q;
		matrix4_transformation_decompose(&m, &s, &q, &t);
		M4 g = g4(m);
		mat4 cm, rm;
		vec4 ct;
		vec3 cs;
		std::memcpy(cm, &g[0][0], sizeof cm);
		glm_decompose(cm, ct, rm, cs);
		check(std::max(err_v(s.v, cs, 3), err_v(t.v, ct, 3)), [&] { return sm4(m) + " -> " + sv(s.v, 3) + sv(t.v, 3) + " vs " + sv(cs, 3) + sv(ct, 3); });
	}
#endif

	/* 2D */
	begin("matrix builders", "matrix3_make_transformation_rotationf_z", "glm::rotate(mat3(1), a)");
	for (int i = 0; i < N; i++) {
		H a = rangle();
		struct matrix3 h;
		matrix3_make_transformation_rotationf_z(&h, a);
		check(err_m3(h, glm::rotate(M3(1), a)), [&] { return f(a) + " -> " + sm3(h); });
	}
	begin("matrix builders", "matrix3_make_transformation_translationv2", "glm::translate(mat3(1), v)");
	for (int i = 0; i < N; i++) {
		struct vector2 v = rv2();
		struct matrix3 h;
		matrix3_make_transformation_translationv2(&h, &v);
		check(err_m3(h, glm::translate(M3(1), gv(v))), [&] { return sv(v.v, 2) + " -> " + sm3(h); });
	}
	begin("matrix builders", "matrix3_make_transformation_scalingv2", "glm::scale(mat3(1), v)");
	for (int i = 0; i < N; i++) {
		struct vector2 v = rv2();
		struct matrix3 h;
		matrix3_make_transformation_scalingv2(&h, &v);
		check(err_m3(h, glm::scale(M3(1), gv(v))), [&] { return sv(v.v, 2) + " -> " + sm3(h); });
	}
	begin("matrix builders", "matrix3_rotate(M, a)", "glm R * M");
	for (int i = 0; i < N; i++) {
		struct matrix3 m = rm3(), h = m;
		H a = rangle();
		matrix3_rotate(&h, a);
		check(err_m3(h, glm::rotate(M3(1), a) * g3(m)), [&] { return sm3(m) + ", " + f(a) + " -> " + sm3(h); });
	}
	begin("matrix builders", "matrix3_translatev2(M, v)", "glm T * M");
	for (int i = 0; i < N; i++) {
		struct matrix3 m = rm3(), h = m;
		struct vector2 v = rv2();
		matrix3_translatev2(&h, &v);
		check(err_m3(h, glm::translate(M3(1), gv(v)) * g3(m)), [&] { return sm3(m) + ", " + sv(v.v, 2) + " -> " + sm3(h); });
	}
	begin("matrix builders", "matrix2_make_transformation_rotationf_z", "mat2 of glm::rotate(mat3(1), a)");
	for (int i = 0; i < N; i++) {
		H a = rangle();
		struct matrix2 h;
		matrix2_make_transformation_rotationf_z(&h, a);
		check(err_m2(h, M2(glm::rotate(M3(1), a))), [&] { return f(a) + " -> " + sm2(h); });
	}
	begin("matrix builders", "matrix2_rotate(M, a)", "glm R * M");
	for (int i = 0; i < N; i++) {
		struct matrix2 m = rm2(), h = m;
		H a = rangle();
		matrix2_rotate(&h, a);
		check(err_m2(h, M2(glm::rotate(M3(1), a)) * g2(m)), [&] { return sm2(m) + ", " + f(a) + " -> " + sm2(h); });
	}

	/* projections and views */
	begin("projections", "matrix4_projection_perspective_fovy_rh", "glm::perspectiveRH" DEPTH_NAME);
	for (int i = 0; i < N; i++) {
		H fovy = r(0.2, 2.5), aspect = r(0.5, 2.5), n = r(0.01, 2), fa = n + r(1, 1000);
		struct matrix4 h;
		matrix4_projection_perspective_fovy_rh(&h, fovy, aspect, n, fa);
		check(err_m4(h, GLM_PERSP_RH(fovy, aspect, n, fa)), [&] { return f(fovy) + ", " + f(aspect) + ", " + f(n) + ", " + f(fa) + " -> " + sm4(h); });
	}
	begin("projections", "matrix4_projection_perspective_fovy_lh", "glm::perspectiveLH" DEPTH_NAME);
	for (int i = 0; i < N; i++) {
		H fovy = r(0.2, 2.5), aspect = r(0.5, 2.5), n = r(0.01, 2), fa = n + r(1, 1000);
		struct matrix4 h;
		matrix4_projection_perspective_fovy_lh(&h, fovy, aspect, n, fa);
		check(err_m4(h, GLM_PERSP_LH(fovy, aspect, n, fa)), [&] { return f(fovy) + ", " + f(aspect) + ", " + f(n) + ", " + f(fa) + " -> " + sm4(h); });
	}
	begin("projections", "matrix4_projection_ortho3d_rh", "glm::orthoRH" DEPTH_NAME);
	for (int i = 0; i < N; i++) {
		H l = r(-10, 0), rr = l + r(0.5, 20), b = r(-10, 0), t = b + r(0.5, 20), n = r(-5, 5), fa = n + r(0.5, 100);
		struct matrix4 h;
		matrix4_projection_ortho3d_rh(&h, l, rr, b, t, n, fa);
		check(err_m4(h, GLM_ORTHO_RH(l, rr, b, t, n, fa)), [&] { return f(l) + ", " + f(rr) + ", " + f(b) + ", " + f(t) + ", " + f(n) + ", " + f(fa) + " -> " + sm4(h); });
	}
	begin("projections", "matrix4_projection_ortho3d_lh", "glm::orthoLH" DEPTH_NAME);
	for (int i = 0; i < N; i++) {
		H l = r(-10, 0), rr = l + r(0.5, 20), b = r(-10, 0), t = b + r(0.5, 20), n = r(-5, 5), fa = n + r(0.5, 100);
		struct matrix4 h;
		matrix4_projection_ortho3d_lh(&h, l, rr, b, t, n, fa);
		check(err_m4(h, GLM_ORTHO_LH(l, rr, b, t, n, fa)), [&] { return f(l) + ", " + f(rr) + ", " + f(b) + ", " + f(t) + ", " + f(n) + ", " + f(fa) + " -> " + sm4(h); });
	}
	begin("projections", "matrix4_view_lookat_rh", "glm::lookAtRH");
	for (int i = 0; i < N; i++) {
		struct vector3 eye = rv3(), target = rv3(), up = runit3();
		struct matrix4 h;
		matrix4_view_lookat_rh(&h, &eye, &target, &up);
		check(err_m4(h, glm::lookAtRH(gv(eye), gv(target), gv(up))), [&] { return sv(eye.v, 3) + sv(target.v, 3) + sv(up.v, 3) + " -> " + sm4(h); });
	}
	begin("projections", "matrix4_view_lookat_lh", "glm::lookAtLH");
	for (int i = 0; i < N; i++) {
		struct vector3 eye = rv3(), target = rv3(), up = runit3();
		struct matrix4 h;
		matrix4_view_lookat_lh(&h, &eye, &target, &up);
		check(err_m4(h, glm::lookAtLH(gv(eye), gv(target), gv(up))), [&] { return sv(eye.v, 3) + sv(target.v, 3) + sv(up.v, 3) + " -> " + sm4(h); });
	}
#ifdef HAVE_CGLM
	begin("projections", "matrix4_projection_perspective_fovy_rh", "cglm glm_perspective_rh_zo");
	for (int i = 0; i < N; i++) {
		H fovy = r(0.2, 2.5), aspect = r(0.5, 2.5), n = r(0.01, 2), fa = n + r(1, 1000);
		struct matrix4 h;
		matrix4_projection_perspective_fovy_rh(&h, fovy, aspect, n, fa);
		mat4 cm;
		glm_perspective_rh_zo(fovy, aspect, n, fa, cm);
		M4 g;
		std::memcpy(&g[0][0], cm, sizeof cm);
		check(err_m4(h, g), [&] { return f(fovy) + ", " + f(aspect) + ", " + f(n) + ", " + f(fa) + " -> " + sm4(h); });
	}
	begin("projections", "matrix4_projection_ortho3d_lh", "cglm glm_ortho_lh_zo");
	for (int i = 0; i < N; i++) {
		H l = r(-10, 0), rr = l + r(0.5, 20), b = r(-10, 0), t = b + r(0.5, 20), n = r(-5, 5), fa = n + r(0.5, 100);
		struct matrix4 h;
		matrix4_projection_ortho3d_lh(&h, l, rr, b, t, n, fa);
		mat4 cm;
		glm_ortho_lh_zo(l, rr, b, t, n, fa, cm);
		M4 g;
		std::memcpy(&g[0][0], cm, sizeof cm);
		check(err_m4(h, g), [&] { return f(l) + ", " + f(rr) + " -> " + sm4(h); });
	}
	begin("projections", "matrix4_view_lookat_rh", "cglm glm_lookat_rh");
	for (int i = 0; i < N; i++) {
		struct vector3 eye = rv3(), target = rv3(), up = runit3();
		struct matrix4 h;
		matrix4_view_lookat_rh(&h, &eye, &target, &up);
		vec3 ce = {eye.x, eye.y, eye.z}, ct = {target.x, target.y, target.z}, cu = {up.x, up.y, up.z};
		mat4 cm;
		glm_lookat_rh(ce, ct, cu, cm);
		M4 g;
		std::memcpy(&g[0][0], cm, sizeof cm);
		check(err_m4(h, g), [&] { return sv(eye.v, 3) + sv(target.v, 3) + sv(up.v, 3) + " -> " + sm4(h); });
	}
#endif
}
#endif
