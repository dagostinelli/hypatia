// SPDX-License-Identifier: MIT
// quaternion comparisons (included by compare.cpp)

/* the rotation angle from a to b in long double, in [0, pi] */
static double oracle_rotation_angle(const struct quaternion &a, const struct quaternion &b)
{
	L na = std::sqrt((L)a.x * a.x + (L)a.y * a.y + (L)a.z * a.z + (L)a.w * a.w);
	L nb = std::sqrt((L)b.x * b.x + (L)b.y * b.y + (L)b.z * b.z + (L)b.w * b.w);
	L s = 0, c = 0;
	/* |a - b| and |a + b| for the unit versions, with the sign that gives d >= 0 */
	L sign = ((L)a.x * b.x + (L)a.y * b.y + (L)a.z * b.z + (L)a.w * b.w) < 0 ? -1 : 1;
	for (int i = 0; i < 4; i++) {
		L u = a.q[i] / na, v = sign * b.q[i] / nb;
		s += (u - v) * (u - v);
		c += (u + v) * (u + v);
	}
	return (double)(4 * std::atan2(std::sqrt(s), std::sqrt(c)));
}

static void quaternions()
{
	begin("quaternions", "quaternion_multiply(a, b)", "glm a * b");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), b = rq_any(), h = a;
		quaternion_multiply(&h, &b);
		check(err_q(h, hq(gq(a) * gq(b))), [&] { return sq(a) + ", " + sq(b) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_multiply(a, b)", "Eigen a * b");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), b = rq_any(), h = a;
		quaternion_multiply(&h, &b);
		check(err_q(h, hq(EQ(eq(a) * eq(b)))), [&] { return sq(a) + ", " + sq(b) + " -> " + sq(h); });
	}
#ifdef HAVE_CGLM
	begin("quaternions", "quaternion_multiply(a, b)", "cglm glm_quat_mul(a, b)");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), b = rq_any(), h = a;
		quaternion_multiply(&h, &b);
		versor ca = {a.x, a.y, a.z, a.w}, cb = {b.x, b.y, b.z, b.w}, cr;
		glm_quat_mul(ca, cb, cr);
		check(err_v(h.q, cr, 4), [&] { return sq(a) + ", " + sq(b) + " -> " + sq(h); });
	}
#endif
	begin("quaternions", "quaternion_multiplyv3(q, v)", "glm q * (v, 0)");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), h = a;
		struct vector3 v = rv3();
		quaternion_multiplyv3(&h, &v);
		check(err_q(h, hq(gq(a) * Q(0, v.x, v.y, v.z))), [&] { return sq(a) + ", " + sv(v.v, 3) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_conjugate", "glm::conjugate");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), h = a;
		quaternion_conjugate(&h);
		check(err_q(h, hq(glm::conjugate(gq(a)))), [&] { return sq(a) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_inverse", "glm::inverse", "not unit length");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), h = a;
		quaternion_inverse(&h);
		check(err_q(h, hq(glm::inverse(gq(a)))), [&] { return sq(a) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_inverse", "Eigen inverse", "not unit length");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), h = a;
		quaternion_inverse(&h);
		check(err_q(h, hq(eq(a).inverse())), [&] { return sq(a) + " -> " + sq(h); });
	}
#ifdef HAVE_CGLM
	begin("quaternions", "quaternion_inverse", "cglm glm_quat_inv", "not unit length");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), h = a;
		quaternion_inverse(&h);
		versor c = {a.x, a.y, a.z, a.w}, cr;
		glm_quat_inv(c, cr);
		check(err_v(h.q, cr, 4), [&] { return sq(a) + " -> " + sq(h); });
	}
#endif
	begin("quaternions", "quaternion_normalize", "glm::normalize");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), h = a;
		quaternion_normalize(&h);
		check(err_q(h, hq(glm::normalize(gq(a)))), [&] { return sq(a) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_dot_product, norm, magnitude", "glm::dot, dot(q, q), glm::length");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_any(), b = rq_any();
		double hd = quaternion_dot_product(&a, &b), hn = quaternion_norm(&a), hm = quaternion_magnitude(&a);
		double err = std::max(e1(hd, glm::dot(gq(a), gq(b))), std::max(e1(hn, glm::dot(gq(a), gq(a))), e1(hm, glm::length(gq(a)))));
		check(err, [&] { return sq(a) + ", " + sq(b); });
	}

	/* axis and angle */
	begin("quaternions", "quaternion_set_from_axis_anglev3", "glm::angleAxis");
	for (int i = 0; i < N; i++) {
		struct vector3 axis = runit3();
		H a = rangle();
		struct quaternion h;
		quaternion_set_from_axis_anglev3(&h, &axis, a);
		check(err_q(h, hq(glm::angleAxis(a, gv(axis)))), [&] { return sv(axis.v, 3) + ", " + f(a) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_set_from_axis_anglev3", "Eigen AngleAxis");
	for (int i = 0; i < N; i++) {
		struct vector3 axis = runit3();
		H a = rangle();
		struct quaternion h;
		quaternion_set_from_axis_anglev3(&h, &axis, a);
		check(err_q(h, hq(EQ(Eigen::AngleAxis<H>(a, ev(axis))))), [&] { return sv(axis.v, 3) + ", " + f(a) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_set_from_axis_anglev3", "glm::angleAxis(a, normalize(axis))", "axis not unit length");
	for (int i = 0; i < N; i++) {
		struct vector3 axis = rv3();
		H a = rangle();
		struct quaternion h;
		quaternion_set_from_axis_anglev3(&h, &axis, a);
		check(err_q(h, hq(glm::angleAxis(a, glm::normalize(gv(axis))))), [&] { return sv(axis.v, 3) + ", " + f(a) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_get_axis_anglev3", "glm::axis, glm::angle", "as a rotation: rebuilt with angleAxis, q or -q");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit();
		struct vector3 axis;
		H a;
		quaternion_get_axis_anglev3(&q, &axis, &a);
		struct quaternion back = hq(glm::angleAxis(a, gv(axis)));
		check(err_qr(back, q), [&] { return sq(q) + " -> " + sv(axis.v, 3) + ", " + f(a); });
	}
	begin("quaternions", "quaternion_get_axis_anglev3", "glm::angle", "the angle itself (hypatia [0, pi], GLM [0, 2 pi])");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit();
		struct vector3 axis;
		H a;
		quaternion_get_axis_anglev3(&q, &axis, &a);
		H g = glm::angle(gq(q));
		check(e1(a, g), [&] { return sq(q) + " -> " + f(a) + " vs " + f(g); });
	}
	begin("quaternions", "quaternion_get_axis_anglev3", "Eigen AngleAxis(q)", "as a rotation");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit();
		struct vector3 axis;
		H a;
		quaternion_get_axis_anglev3(&q, &axis, &a);
		Eigen::AngleAxis<H> e(eq(q));
		struct quaternion mine, theirs;
		quaternion_set_from_axis_anglev3(&mine, &axis, a);
		theirs = hq(EQ(e));
		check(err_qr(mine, theirs), [&] { return sq(q) + " -> " + sv(axis.v, 3) + ", " + f(a) + " vs angle " + f(e.angle()); });
	}

	/* Euler angles */
	begin("quaternions", "quaternion_set_from_euler_anglesf3(x, y, z)", "glm::quat(vec3(x, y, z))", "as a rotation");
	for (int i = 0; i < N; i++) {
		H x = rangle(), y = rangle(), z = rangle();
		struct quaternion h;
		quaternion_set_from_euler_anglesf3(&h, x, y, z);
		check(err_qr(h, hq(Q(V3(x, y, z)))), [&] { return f(x) + ", " + f(y) + ", " + f(z) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_set_from_euler_anglesf3(x, y, z)", "matrix4_set_from_euler_anglesf3(x, y, z)", "the matrix of the quaternion");
	for (int i = 0; i < N; i++) {
		H x = rangle(), y = rangle(), z = rangle();
		struct quaternion q;
		struct matrix4 hm, mm;
		quaternion_set_from_euler_anglesf3(&q, x, y, z);
		matrix4_set_from_quaternion(&hm, &q);
		matrix4_set_from_euler_anglesf3(&mm, x, y, z);
		check(err_v(hm.m, mm.m, 16), [&] { return f(x) + ", " + f(y) + ", " + f(z); });
	}
	begin("quaternions", "quaternion_get_euler_anglesf3", "glm::eulerAngles", "angles; |y| < 1.5");
	for (int i = 0; i < N; i++) {
		H x = r(-3, 3), y = r(-1.5, 1.5), z = r(-3, 3);
		struct quaternion q;
		quaternion_set_from_euler_anglesf3(&q, x, y, z);
		H hx, hy, hz;
		quaternion_get_euler_anglesf3(&q, &hx, &hy, &hz);
		V3 g = glm::eulerAngles(gq(q));
		H hh[3] = {hx, hy, hz}, gg[3] = {g.x, g.y, g.z};
		check(err_v(hh, gg, 3), [&] { return sq(q) + " -> " + sv(hh, 3) + " vs " + sv(gg, 3); });
	}
	begin("quaternions", "quaternion_get_euler_anglesf3", "the angles given to quaternion_set_from_euler_anglesf3", "round trip; |y| < 1.5");
	for (int i = 0; i < N; i++) {
		H x = r(-3, 3), y = r(-1.5, 1.5), z = r(-3, 3);
		struct quaternion q;
		quaternion_set_from_euler_anglesf3(&q, x, y, z);
		H hh[3], in[3] = {x, y, z};
		quaternion_get_euler_anglesf3(&q, &hh[0], &hh[1], &hh[2]);
		check(err_v(hh, in, 3) * (SINGLE ? 0.1 : 1e-3), [&] { return sv(in, 3) + " -> " + sv(hh, 3); });
	}

	/* from a matrix */
	begin("quaternions", "quaternion_set_from_matrix4", "glm::quat_cast", "q or -q");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit(), h;
		struct matrix4 m;
		matrix4_set_from_quaternion(&m, &q);
		quaternion_set_from_matrix4(&h, &m);
		check(err_qr(h, hq(glm::quat_cast(g4(m)))), [&] { return sm4(m) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_set_from_matrix4", "Eigen Quaternion(Matrix3)", "q or -q");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit(), h;
		struct matrix4 m;
		matrix4_set_from_quaternion(&m, &q);
		quaternion_set_from_matrix4(&h, &m);
		Eigen::Matrix<H, 3, 3> e3 = e4(m).block<3, 3>(0, 0);
		check(err_qr(h, hq(EQ(e3))), [&] { return sm4(m) + " -> " + sq(h); });
	}
#ifdef HAVE_CGLM
	begin("quaternions", "quaternion_set_from_matrix4", "cglm glm_mat4_quat", "q or -q");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit(), h;
		struct matrix4 m;
		matrix4_set_from_quaternion(&m, &q);
		quaternion_set_from_matrix4(&h, &m);
		M4 g = g4(m);
		mat4 cm;
		std::memcpy(cm, &g[0][0], sizeof cm);
		versor c;
		glm_mat4_quat(cm, c);
		struct quaternion cq;
		quaternion_setf4(&cq, c[0], c[1], c[2], c[3]);
		check(err_qr(h, cq), [&] { return sm4(m) + " -> " + sq(h) + " vs " + sq(cq); });
	}
#endif

	/* interpolation */
	begin("quaternions", "quaternion_lerp", "glm::lerp");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit(), h;
		H t = r(0, 1);
		quaternion_lerp(&a, &b, t, &h);
		check(err_q(h, hq(glm::lerp(gq(a), gq(b), t))), [&] { return sq(a) + ", " + sq(b) + ", " + f(t) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_nlerp", "glm::normalize(glm::lerp)");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit(), h;
		H t = r(0, 1);
		quaternion_nlerp(&a, &b, t, &h);
		check(err_q(h, hq(glm::normalize(glm::lerp(gq(a), gq(b), t)))), [&] { return sq(a) + ", " + sq(b) + ", " + f(t) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_slerp", "glm::slerp", "components");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit(), h;
		H t = r(0, 1);
		quaternion_slerp(&a, &b, t, &h);
		check(err_q(h, hq(glm::slerp(gq(a), gq(b), t))), [&] { return sq(a) + ", " + sq(b) + ", " + f(t) + " -> " + sq(h) + " vs " + sq(hq(glm::slerp(gq(a), gq(b), t))); });
	}
	begin("quaternions", "quaternion_slerp", "Eigen slerp", "components");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit(), h;
		H t = r(0, 1);
		quaternion_slerp(&a, &b, t, &h);
		struct quaternion e = hq(EQ(eq(a).slerp(t, eq(b))));
		check(err_q(h, e), [&] { return sq(a) + ", " + sq(b) + ", " + f(t) + " -> " + sq(h) + " vs " + sq(e); });
	}
#ifdef HAVE_CGLM
	begin("quaternions", "quaternion_slerp", "cglm glm_quat_slerp", "as a rotation (cglm does not take the shortest arc)");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit(), h;
		H t = r(0, 1);
		quaternion_slerp(&a, &b, t, &h);
		versor ca = {a.x, a.y, a.z, a.w}, cb = {b.x, b.y, b.z, b.w}, cr;
		glm_quat_slerp(ca, cb, t, cr);
		struct quaternion c;
		quaternion_setf4(&c, cr[0], cr[1], cr[2], cr[3]);
		check(err_qr(h, c), [&] { return sq(a) + ", " + sq(b) + ", " + f(t) + " -> " + sq(h) + " vs " + sq(c); });
	}
#endif
	begin("quaternions", "quaternion_slerp", "glm::slerp", "nearly the same: 1e-3 rad apart");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), d, b, h;
		struct vector3 axis = runit3();
		quaternion_set_from_axis_anglev3(&d, &axis, (H)1e-3);
		b = a;
		quaternion_multiply(&b, &d);
		H t = r(0, 1);
		quaternion_slerp(&a, &b, t, &h);
		check(err_q(h, hq(glm::slerp(gq(a), gq(b), t))), [&] { return sq(a) + ", " + sq(b) + ", " + f(t) + " -> " + sq(h); });
	}

	/* rotation between vectors */
	begin("quaternions", "quaternion_get_rotation_tov3", "glm::rotation(normalize(a), normalize(b))");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3();
		struct quaternion h;
		quaternion_get_rotation_tov3(&a, &b, &h);
		check(err_qr(h, hq(glm::rotation(glm::normalize(gv(a)), glm::normalize(gv(b))))), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_get_rotation_tov3", "Eigen FromTwoVectors");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3();
		struct quaternion h;
		quaternion_get_rotation_tov3(&a, &b, &h);
		check(err_qr(h, hq(EQ(EQ::FromTwoVectors(ev(a), ev(b))))), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + sq(h); });
	}
#ifdef HAVE_CGLM
	begin("quaternions", "quaternion_get_rotation_tov3", "cglm glm_quat_from_vecs(normalized)");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3();
		struct quaternion h;
		quaternion_get_rotation_tov3(&a, &b, &h);
		struct vector3 na = a, nb = b;
		vector3_normalize(&na);
		vector3_normalize(&nb);
		versor c;
		glm_quat_from_vecs(na.v, nb.v, c);
		struct quaternion cq;
		quaternion_setf4(&cq, c[0], c[1], c[2], c[3]);
		check(err_qr(h, cq), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + sq(h) + " vs " + sq(cq); });
	}
#endif
	begin("quaternions", "quaternion_get_rotation_tov3", "Eigen FromTwoVectors", "nearly opposite: 1e-3 rad from opposite; rotating a must give b");
	for (int i = 0; i < N; i++) {
		struct vector3 a = runit3(), b, axis;
		struct quaternion q, h;
		vector3_cross_product(&axis, &a, HYP_VECTOR3_UNIT_Y);
		vector3_normalize(&axis);
		quaternion_set_from_axis_anglev3(&q, &axis, (H)(PI - 1e-3));
		b = a;
		vector3_rotate_by_quaternion(&b, &q);
		quaternion_get_rotation_tov3(&a, &b, &h);
		struct vector3 hb = a, eb;
		vector3_rotate_by_quaternion(&hb, &h);
		EV3 e = EQ::FromTwoVectors(ev(a), ev(b)) * ev(a);
		vector3_setf3(&eb, e.x(), e.y(), e.z());
		/* how far each lands from b */
		double herr = err_v(hb.v, b.v, 3), eerr = err_v(eb.v, b.v, 3);
		check(herr, [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> hypatia lands " + f(herr) + " from b, Eigen " + f(eerr); });
	}

	/* rotations between quaternions */
	begin("quaternions", "quaternion_angle_between", "Eigen angularDistance");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit();
		H h = quaternion_angle_between(&a, &b), e = eq(a).angularDistance(eq(b));
		check(e1(h, e), [&] { return sq(a) + ", " + sq(b) + " -> " + f(h) + " vs " + f(e); });
	}
	begin("quaternions", "quaternion_angle_between", "long double", "1e-4 rad apart");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), d, b;
		struct vector3 axis = runit3();
		quaternion_set_from_axis_anglev3(&d, &axis, (H)1e-4);
		b = a;
		quaternion_multiply(&b, &d);
		H h = quaternion_angle_between(&a, &b);
		double o = oracle_rotation_angle(a, b);
		H e = eq(a).angularDistance(eq(b));
		check(std::fabs(h - o) / o * (SINGLE ? 1e-3 : 1e-6), [&] { return sq(a) + ", " + sq(b) + " -> " + f(h) + " vs " + f(o) + " (Eigen " + f(e) + ")"; });
	}
	begin("quaternions", "quaternion_difference", "min(|a - b|^2, |a + b|^2)");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit();
		H h = quaternion_difference(&a, &b);
		Q ga = gq(a), gb = gq(b);
		H g = std::min(glm::dot(ga - gb, ga - gb), glm::dot(ga + gb, ga + gb));
		check(e1(h, g), [&] { return sq(a) + ", " + sq(b) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("quaternions", "quaternion_rotate_by_quaternion(a, b)", "glm::normalize(a * b)");
	for (int i = 0; i < N; i++) {
		struct quaternion a = rq_unit(), b = rq_unit(), h = a;
		quaternion_rotate_by_quaternion(&h, &b);
		check(err_q(h, hq(glm::normalize(gq(a) * gq(b)))), [&] { return sq(a) + ", " + sq(b) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_rotate_by_axis_angle(q, axis, a)", "glm::rotate(q, a, axis) = q * angleAxis");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit(), h = q;
		struct vector3 axis = runit3();
		H a = rangle();
		quaternion_rotate_by_axis_angle(&h, &axis, a);
		check(err_q(h, hq(glm::rotate(gq(q), a, gv(axis)))), [&] { return sq(q) + ", " + sv(axis.v, 3) + ", " + f(a) + " -> " + sq(h); });
	}
	begin("quaternions", "quaternion_rotate_by_euler_angles(q, x, y, z)", "q * glm::quat(vec3(x, y, z))", "as a rotation");
	for (int i = 0; i < N; i++) {
		struct quaternion q = rq_unit(), h = q;
		H x = rangle(), y = rangle(), z = rangle();
		quaternion_rotate_by_euler_angles(&h, x, y, z);
		check(err_qr(h, hq(gq(q) * Q(V3(x, y, z)))), [&] { return sq(q) + ", " + f(x) + ", " + f(y) + ", " + f(z) + " -> " + sq(h); });
	}
}
