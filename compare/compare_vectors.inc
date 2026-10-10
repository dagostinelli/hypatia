// SPDX-License-Identifier: MIT
// vector comparisons (included by compare.cpp)

/* the angle between a and b in long double: atan2(|a x b|, a . b) */
static double oracle_angle3(const struct vector3 &a, const struct vector3 &b)
{
	L ax = a.x, ay = a.y, az = a.z, bx = b.x, by = b.y, bz = b.z;
	L cx = ay * bz - az * by, cy = az * bx - ax * bz, cz = ax * by - ay * bx;
	return (double)std::atan2(std::sqrt(cx * cx + cy * cy + cz * cz), ax * bx + ay * by + az * bz);
}

#ifndef PRECISION_ONLY
static void vectors()
{
	/* normalize */
	begin("vectors", "vector2_normalize", "glm::normalize");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), h = a;
		vector2_normalize(&h);
		struct vector2 g = hv(glm::normalize(gv(a)));
		check(err_v(h.v, g.v, 2), [&] { return sv(a.v, 2) + " -> " + sv(h.v, 2) + " vs " + sv(g.v, 2); });
	}
	begin("vectors", "vector3_normalize", "glm::normalize");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		vector3_normalize(&h);
		struct vector3 g = hv(glm::normalize(gv(a)));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector3_normalize", "Eigen normalized");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		vector3_normalize(&h);
		EV3 e = ev(a).normalized();
		struct vector3 g;
		vector3_setf3(&g, e.x(), e.y(), e.z());
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector4_normalize", "glm::normalize");
	for (int i = 0; i < N; i++) {
		struct vector4 a = rv4(), h = a;
		vector4_normalize(&h);
		struct vector4 g = hv(glm::normalize(gv(a)));
		check(err_v(h.v, g.v, 4), [&] { return sv(a.v, 4) + " -> " + sv(h.v, 4) + " vs " + sv(g.v, 4); });
	}
#ifdef HAVE_CGLM
	begin("vectors", "vector3_normalize", "cglm glm_vec3_normalize");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		vector3_normalize(&h);
		vec3 c = {a.x, a.y, a.z};
		glm_vec3_normalize(c);
		check(err_v(h.v, c, 3), [&] { return sv(a.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(c, 3); });
	}
	begin("vectors", "vector4_normalize", "cglm glm_vec4_normalize");
	for (int i = 0; i < N; i++) {
		struct vector4 a = rv4(), h = a;
		vector4_normalize(&h);
		vec4 c = {a.x, a.y, a.z, a.w};
		glm_vec4_normalize(c);
		check(err_v(h.v, c, 4), [&] { return sv(a.v, 4) + " -> " + sv(h.v, 4) + " vs " + sv(c, 4); });
	}
#endif

	/* length, distance, dot, cross */
	begin("vectors", "vector3_magnitude", "glm::length");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3();
		H h = vector3_magnitude(&a), g = glm::length(gv(a));
		check(e1(h, g), [&] { return sv(a.v, 3) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector2_distance", "glm::distance");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), b = rv2();
		H h = vector2_distance(&a, &b), g = glm::distance(gv(a), gv(b));
		check(e1(h, g), [&] { return sv(a.v, 2) + ", " + sv(b.v, 2) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector3_distance", "glm::distance");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3();
		H h = vector3_distance(&a, &b), g = glm::distance(gv(a), gv(b));
		check(e1(h, g), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector4_distance", "glm::distance");
	for (int i = 0; i < N; i++) {
		struct vector4 a = rv4(), b = rv4();
		H h = vector4_distance(&a, &b), g = glm::distance(gv(a), gv(b));
		check(e1(h, g), [&] { return sv(a.v, 4) + ", " + sv(b.v, 4) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector4_dot_product", "glm::dot");
	for (int i = 0; i < N; i++) {
		struct vector4 a = rv4(), b = rv4();
		H h = vector4_dot_product(&a, &b), g = glm::dot(gv(a), gv(b));
		check(e1(h, g), [&] { return sv(a.v, 4) + ", " + sv(b.v, 4) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector3_cross_product", "glm::cross");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3(), h;
		vector3_cross_product(&h, &a, &b);
		struct vector3 g = hv(glm::cross(gv(a), gv(b)));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector2_cross_product", "a.x b.y - a.y b.x");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), b = rv2();
		H h = vector2_cross_product(&a, &b), g = a.x * b.y - a.y * b.x;
		check(e1(h, g), [&] { return sv(a.v, 2) + ", " + sv(b.v, 2) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector3_find_normal_axis_between", "glm::normalize(glm::cross)");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3(), h;
		vector3_find_normal_axis_between(&h, &a, &b);
		struct vector3 g = hv(glm::normalize(glm::cross(gv(a), gv(b))));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}

	/* angle between: random, then nearly parallel */
	begin("vectors", "vector3_angle_between", "glm::angle(normalize, normalize)", "GLM's acos loses accuracy near 0 and pi: see the accuracy table");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3();
		H h = vector3_angle_between(&a, &b), g = glm::angle(glm::normalize(gv(a)), glm::normalize(gv(b)));
		check(e1(h, g), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector3_angle_between", "long double atan2(|a x b|, a . b)", "a and 3a: parallel");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = a;
		vector3_multiplyf(&b, 3);
		H h = vector3_angle_between(&a, &b);
		double o = oracle_angle3(a, b);
		check(e1(h, o), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + f(h) + " vs " + f(o); });
	}
	begin("vectors", "vector3_angle_between", "long double atan2(|a x b|, a . b)", "1e-4 rad apart");
	for (int i = 0; i < N; i++) {
		struct vector3 a = runit3(), b, axis;
		struct quaternion q;
		vector3_cross_product(&axis, &a, HYP_VECTOR3_UNIT_X);
		vector3_normalize(&axis);
		quaternion_set_from_axis_anglev3(&q, &axis, (H)1e-4);
		b = a;
		vector3_rotate_by_quaternion(&b, &q);
		H h = vector3_angle_between(&a, &b);
		double o = oracle_angle3(a, b);
		/* the angle is small: compare absolute error relative to the angle */
		check(std::fabs(h - o) / o * (SINGLE ? 1e-3 : 1e-6), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + " -> " + f(h) + " vs " + f(o); });
	}
	begin("vectors", "vector2_angle_between", "glm::angle(normalize, normalize)", "GLM's acos loses accuracy near 0 and pi: see the accuracy table");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), b = rv2();
		H h = vector2_angle_between(&a, &b), g = glm::angle(glm::normalize(gv(a)), glm::normalize(gv(b)));
		check(e1(h, g), [&] { return sv(a.v, 2) + ", " + sv(b.v, 2) + " -> " + f(h) + " vs " + f(g); });
	}
	begin("vectors", "vector2_angle_between", "0", "a and 3a: parallel");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), b = a;
		vector2_multiplyf(&b, 3);
		H h = vector2_angle_between(&a, &b);
		check(e1(h, 0), [&] { return sv(a.v, 2) + ", " + sv(b.v, 2) + " -> " + f(h) + " vs 0"; });
	}

	/* lerp, clamp, min, max, project */
	begin("vectors", "vector3_lerp", "glm::mix");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3(), h;
		H t = r(-0.5, 1.5);
		vector3_lerp(&a, &b, t, &h);
		struct vector3 g = hv(glm::mix(gv(a), gv(b), t));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3) + ", " + f(t) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector4_lerp", "glm::mix");
	for (int i = 0; i < N; i++) {
		struct vector4 a = rv4(), b = rv4(), h;
		H t = r(-0.5, 1.5);
		vector4_lerp(&a, &b, t, &h);
		struct vector4 g = hv(glm::mix(gv(a), gv(b), t));
		check(err_v(h.v, g.v, 4), [&] { return sv(a.v, 4) + ", " + sv(b.v, 4) + ", " + f(t) + " -> " + sv(h.v, 4) + " vs " + sv(g.v, 4); });
	}
	begin("vectors", "vector3_clamp", "glm::clamp");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), lo = rv3(5), hi, h = a;
		hi = lo;
		struct vector3 d = rv3(5);
		for (int k = 0; k < 3; k++)
			hi.v[k] += std::fabs(d.v[k]);
		vector3_clamp(&h, &lo, &hi);
		struct vector3 g = hv(glm::clamp(gv(a), gv(lo), gv(hi)));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector3_min, vector3_max", "glm::min, glm::max");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3(), hmin = a, hmax = a;
		vector3_min(&hmin, &b);
		vector3_max(&hmax, &b);
		struct vector3 gmin = hv(glm::min(gv(a), gv(b))), gmax = hv(glm::max(gv(a), gv(b)));
		check(std::max(err_v(hmin.v, gmin.v, 3), err_v(hmax.v, gmax.v, 3)), [&] { return sv(a.v, 3) + ", " + sv(b.v, 3); });
	}
	begin("vectors", "vector2_project", "glm::proj");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), b = rv2(), h = a;
		vector2_project(&h, &b);
		struct vector2 g = hv(glm::proj(gv(a), gv(b)));
		check(err_v(h.v, g.v, 2), [&] { return sv(a.v, 2) + " onto " + sv(b.v, 2) + " -> " + sv(h.v, 2) + " vs " + sv(g.v, 2); });
	}
	begin("vectors", "vector3_project", "glm::proj");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), b = rv3(), h = a;
		vector3_project(&h, &b);
		struct vector3 g = hv(glm::proj(gv(a), gv(b)));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " onto " + sv(b.v, 3) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector4_project", "glm::proj");
	for (int i = 0; i < N; i++) {
		struct vector4 a = rv4(), b = rv4(), h = a;
		vector4_project(&h, &b);
		struct vector4 g = hv(glm::proj(gv(a), gv(b)));
		check(err_v(h.v, g.v, 4), [&] { return sv(a.v, 4) + " onto " + sv(b.v, 4) + " -> " + sv(h.v, 4) + " vs " + sv(g.v, 4); });
	}

	/* rotating and transforming vectors */
	begin("vectors", "vector3_rotate_by_quaternion", "glm q * v");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		struct quaternion q = rq_unit();
		vector3_rotate_by_quaternion(&h, &q);
		struct vector3 g = hv(gq(q) * gv(a));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " by " + sq(q) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector3_rotate_by_quaternion", "Eigen q * v");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		struct quaternion q = rq_unit();
		vector3_rotate_by_quaternion(&h, &q);
		EV3 e = eq(q) * ev(a);
		struct vector3 g;
		vector3_setf3(&g, e.x(), e.y(), e.z());
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " by " + sq(q) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector3_rotate_by_quaternion", "glm q * v", "q not unit length");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		struct quaternion q = rq_any();
		vector3_rotate_by_quaternion(&h, &q);
		struct vector3 g = hv(glm::normalize(gq(q)) * gv(a));
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " by " + sq(q) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3) + " (rotation by normalize(q))"; });
	}
#ifdef HAVE_CGLM
	begin("vectors", "vector3_rotate_by_quaternion", "cglm glm_quat_rotatev");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		struct quaternion q = rq_unit();
		vector3_rotate_by_quaternion(&h, &q);
		versor c = {q.x, q.y, q.z, q.w};
		vec3 v = {a.x, a.y, a.z}, out;
		glm_quat_rotatev(c, v, out);
		check(err_v(h.v, out, 3), [&] { return sv(a.v, 3) + " by " + sq(q) + " -> " + sv(h.v, 3) + " vs " + sv(out, 3); });
	}
#endif
	begin("vectors", "vector3_reflect_by_quaternion", "glm q * (v, 0) * q (as documented)");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		struct quaternion q = rq_unit();
		vector3_reflect_by_quaternion(&h, &q);
		Q p = gq(q) * Q(0, a.x, a.y, a.z) * gq(q);
		struct vector3 g;
		vector3_setf3(&g, p.x, p.y, p.z);
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " by " + sq(q) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector3_multiplym4", "glm (M * (v, 1)).xyz");
	for (int i = 0; i < N; i++) {
		struct vector3 a = rv3(), h = a;
		struct matrix4 m = rm4();
		vector3_multiplym4(&h, &m);
		V4 p = g4(m) * V4(gv(a), 1);
		struct vector3 g;
		vector3_setf3(&g, p.x, p.y, p.z);
		check(err_v(h.v, g.v, 3), [&] { return sv(a.v, 3) + " by " + sm4(m) + " -> " + sv(h.v, 3) + " vs " + sv(g.v, 3); });
	}
	begin("vectors", "vector2_multiplym2", "glm M * v");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), h = a;
		struct matrix2 m = rm2();
		vector2_multiplym2(&h, &m);
		struct vector2 g = hv(g2(m) * gv(a));
		check(err_v(h.v, g.v, 2), [&] { return sv(a.v, 2) + " by " + sm2(m) + " -> " + sv(h.v, 2) + " vs " + sv(g.v, 2); });
	}
	begin("vectors", "vector2_multiplym3", "glm (M * (v, 1)).xy");
	for (int i = 0; i < N; i++) {
		struct vector2 a = rv2(), h = a;
		struct matrix3 m = rm3();
		vector2_multiplym3(&h, &m);
		V3 p = g3(m) * V3(gv(a), 1);
		struct vector2 g;
		vector2_setf2(&g, p.x, p.y);
		check(err_v(h.v, g.v, 2), [&] { return sv(a.v, 2) + " by " + sm3(m) + " -> " + sv(h.v, 2) + " vs " + sv(g.v, 2); });
	}
}
#endif
