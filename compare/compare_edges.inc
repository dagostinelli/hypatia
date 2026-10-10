// SPDX-License-Identifier: MIT
// edge cases (included by compare.cpp): each library's answer side by side

static volatile H zero_v = 0;

static std::string gs3(const V3 &v) { struct vector3 h = hv(v); return sv(h.v, 3); }
static std::string gsq(const Q &q) { return sq(hq(q)); }

static void edge_cases()
{
	const H nan = zero_v / zero_v, inf = 1 / zero_v;
	const H tiny = SINGLE ? (H)1e-30 : (H)1e-200, huge = SINGLE ? (H)1e30 : (H)1e200;

	/* normalize */
	{
		struct vector3 ins[5];
		const char *names[5] = {"zero", "tiny (1e-30 / 1e-200)", "huge (1e30 / 1e200)", "one component inf", "one component NaN"};
		vector3_setf3(&ins[0], 0, 0, 0);
		vector3_setf3(&ins[1], tiny, tiny, 0);
		vector3_setf3(&ins[2], huge, huge, 0);
		vector3_setf3(&ins[3], inf, 1, 0);
		vector3_setf3(&ins[4], nan, 1, 0);
		for (int i = 0; i < 5; i++) {
			struct vector3 h = ins[i];
			vector3_normalize(&h);
			std::string o = "GLM " + gs3(glm::normalize(gv(ins[i])));
			EV3 e = ev(ins[i]).normalized();
			o += "; Eigen " + gs3(V3(e.x(), e.y(), e.z()));
#ifdef HAVE_CGLM
			vec3 c = {ins[i].x, ins[i].y, ins[i].z};
			glm_vec3_normalize(c);
			o += "; cglm " + sv(c, 3);
#endif
			edge("vector3_normalize", names[i] + std::string(" ") + sv(ins[i].v, 3), sv(h.v, 3), o);
		}
	}

	/* angle between */
	{
		struct vector3 a, b;
		vector3_setf3(&a, 1, 2, 3);
		b = a;
		vector3_multiplyf(&b, 3);
		edge("vector3_angle_between", "a, 3a: parallel", f(vector3_angle_between(&a, &b)),
		     "GLM angle(normalize, normalize) " + f(glm::angle(glm::normalize(gv(a)), glm::normalize(gv(b)))));
		vector3_setf3(&b, 0, 0, 0);
		edge("vector3_angle_between", "a, zero", f(vector3_angle_between(&a, &b)), "GLM " + f(glm::angle(glm::normalize(gv(a)), glm::normalize(gv(b)))));
		struct vector2 c, d;
		vector2_setf2(&c, 0.1, 0.7);
		d = c;
		vector2_multiplyf(&d, 7);
		edge("vector2_angle_between", "a, 7a: parallel", f(vector2_angle_between(&c, &d)),
		     "GLM angle(normalize, normalize) " + f(glm::angle(glm::normalize(gv(c)), glm::normalize(gv(d)))));
	}

	/* project onto zero */
	{
		struct vector3 a, z, h;
		vector3_setf3(&a, 1, 2, 3);
		vector3_zero(&z);
		h = a;
		vector3_project(&h, &z);
		edge("vector3_project", "onto zero", sv(h.v, 3), "GLM proj " + gs3(glm::proj(gv(a), gv(z))));
	}

	/* quaternions */
	{
		struct quaternion z, h;
		quaternion_setf4(&z, 0, 0, 0, 0);
		h = z;
		quaternion_normalize(&h);
		edge("quaternion_normalize", "zero", sq(h), "GLM " + gsq(glm::normalize(gq(z))) + "; Eigen " + sq(hq(EQ(eq(z).normalized()))));
		h = z;
		quaternion_inverse(&h);
		edge("quaternion_inverse", "zero", sq(h), "GLM " + gsq(glm::inverse(gq(z))) + "; Eigen " + sq(hq(EQ(eq(z).inverse()))));

		struct quaternion q, nq, r;
		struct vector3 axis;
		vector3_setf3(&axis, 0, 0, 1);
		quaternion_set_from_axis_anglev3(&q, &axis, 1);
		nq = q;
		quaternion_negate(&nq);
		quaternion_slerp(&q, &nq, 0.5, &r);
		edge("quaternion_slerp", "q, -q, 0.5 (same rotation)", sq(r),
		     "GLM slerp " + gsq(glm::slerp(gq(q), gq(nq), (H)0.5)) + "; GLM mix " + gsq(glm::mix(gq(q), gq(nq), (H)0.5)) + "; Eigen " + sq(hq(EQ(eq(q).slerp(0.5, eq(nq))))));
		quaternion_slerp(&q, &q, 0.5, &r);
		edge("quaternion_slerp", "q, q, 0.5", sq(r), "GLM " + gsq(glm::slerp(gq(q), gq(q), (H)0.5)) + "; Eigen " + sq(hq(EQ(eq(q).slerp(0.5, eq(q))))));
		struct quaternion q2;
		quaternion_set_from_axis_anglev3(&q2, &axis, 2.5);
		quaternion_slerp(&q, &q2, 1.5, &r);
		edge("quaternion_slerp", "t = 1.5 (beyond end)", sq(r), "GLM " + gsq(glm::slerp(gq(q), gq(q2), (H)1.5)) + "; Eigen " + sq(hq(EQ(eq(q).slerp(1.5, eq(q2))))));

		struct quaternion id, nid;
		quaternion_identity(&id);
		nid = id;
		quaternion_negate(&nid);
		struct vector3 ax;
		H an;
		quaternion_get_axis_anglev3(&id, &ax, &an);
		edge("quaternion_get_axis_anglev3", "identity", sv(ax.v, 3) + ", " + f(an), "GLM axis " + gs3(glm::axis(gq(id))) + ", angle " + f(glm::angle(gq(id))));
		quaternion_get_axis_anglev3(&nid, &ax, &an);
		edge("quaternion_get_axis_anglev3", "-identity", sv(ax.v, 3) + ", " + f(an), "GLM axis " + gs3(glm::axis(gq(nid))) + ", angle " + f(glm::angle(gq(nid))));
		quaternion_get_axis_anglev3(&nq, &ax, &an);
		edge("quaternion_get_axis_anglev3", "-(1 rad about Z)", sv(ax.v, 3) + ", " + f(an), "GLM axis " + gs3(glm::axis(gq(nq))) + ", angle " + f(glm::angle(gq(nq))));
	}

	/* rotation between vectors, degenerate */
	{
		struct vector3 a, b, z;
		struct quaternion h;
		vector3_setf3(&a, 1, 2, 3);
		b = a;
		vector3_negate(&b);
		vector3_zero(&z);
		quaternion_get_rotation_tov3(&a, &b, &h);
		struct vector3 ra = a;
		vector3_rotate_by_quaternion(&ra, &h);
		Q g = glm::rotation(glm::normalize(gv(a)), glm::normalize(gv(b)));
		EQ e = EQ::FromTwoVectors(ev(a), ev(b));
		edge("quaternion_get_rotation_tov3", "a, -a: opposite", sq(h) + " (turns a to " + sv(ra.v, 3) + ")",
		     "GLM " + gsq(g) + " (turns a to " + gs3(g * gv(a)) + "); Eigen " + sq(hq(e)));
		quaternion_get_rotation_tov3(&a, &z, &h);
		edge("quaternion_get_rotation_tov3", "a, zero", sq(h), "GLM " + gsq(glm::rotation(glm::normalize(gv(a)), glm::normalize(gv(z)))) + "; Eigen " + sq(hq(EQ(EQ::FromTwoVectors(ev(a), ev(z))))));
	}

	/* matrices */
	{
		struct matrix4 s, h;
		for (int i = 0; i < 16; i++)
			s.m[i] = (H)(i + 1);
		const struct matrix4 *p = matrix4_inverse(&s, &h);
		M4 gi = glm::inverse(g4(s));
		edge("matrix4_inverse", "singular (1 .. 16)", p ? "returns the matrix " + sm4(h) : std::string("NULL"),
		     "GLM " + sm4(h4(gi)) + "; determinant: hypatia " + f(matrix4_determinant(&s)) + ", GLM " + f(glm::determinant(g4(s))));
		/* nearly singular, but invertible */
		struct matrix4 ns = s;
		ns.r00 += SINGLE ? (H)1e-3 : (H)1e-9;
		p = matrix4_inverse(&ns, &h);
		edge("matrix4_inverse", "nearly singular", p ? "rcond " + f(matrix4_reciprocal_condition(&ns)) : std::string("NULL"),
		     "LAPACK rcond (double) see table; GLM determinant " + f(glm::determinant(g4(ns))));

		struct vector3 sc, t, zero;
		struct quaternion q;
		vector3_setf3(&sc, 0, 1, 1);
		vector3_zero(&zero);
		matrix4_make_transformation_scalingv3(&h, &sc);
		uint8_t ok = matrix4_transformation_decompose(&h, &sc, &q, &t);
		V3 gs, gt, sk;
		V4 pe;
		Q gr;
		bool gok = glm::decompose(g4(h), gs, gr, gt, sk, pe);
		edge("matrix4_transformation_decompose", "zero scale on x", ok ? "1" : "0 (fails)", std::string("GLM ") + (gok ? "true" : "false") + ", scale " + gs3(gs));

		struct vector3 eye, up;
		vector3_setf3(&eye, 1, 2, 3);
		vector3_setf3(&up, 0, 1, 0);
		matrix4_view_lookat_rh(&h, &eye, &eye, &up);
		edge("matrix4_view_lookat_rh", "eye == target", sm4(h), "GLM " + sm4(h4(glm::lookAtRH(gv(eye), gv(eye), gv(up)))));
		struct vector3 target;
		vector3_setf3(&target, 1, 5, 3);
		matrix4_view_lookat_rh(&h, &eye, &target, &up);
		edge("matrix4_view_lookat_rh", "looking along up", sm4(h), "GLM " + sm4(h4(glm::lookAtRH(gv(eye), gv(target), gv(up)))));

		matrix4_projection_perspective_fovy_rh(&h, 1, 1, 1, 1);
		edge("matrix4_projection_perspective_fovy_rh", "zNear == zFar", sm4(h), "GLM " + sm4(h4(GLM_PERSP_RH((H)1, (H)1, (H)1, (H)1))));
	}
}
