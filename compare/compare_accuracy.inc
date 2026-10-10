// SPDX-License-Identifier: MIT
// accuracy against long double (included by compare.cpp): where two
// libraries disagree, which one is closer to the exact answer

/* a row with the largest error of one library over the inputs */
[[maybe_unused]] static void acc_row(const char *area, const std::string &test, const char *note, double maxerr)
{
	begin(area, test.c_str(), "long double", note);
	cur->n = N;
	cur->maxerr = maxerr;
}

[[maybe_unused]] static double oracle_angle2(const struct vector2 &a, const struct vector2 &b)
{
	L ax = a.x, ay = a.y, bx = b.x, by = b.y;
	return (double)std::atan2(std::fabs(ax * by - ay * bx), ax * bx + ay * by);
}

/* how far rotating unit(from) by q lands from unit(to), in long double */
static double landing_error(const struct vector3 &from, const struct vector3 &to, const struct quaternion &q)
{
	L f[3] = {from.x, from.y, from.z}, t[3] = {to.x, to.y, to.z}, nf = 0, nt = 0, nq = 0;
	for (int i = 0; i < 3; i++) {
		nf += f[i] * f[i];
		nt += t[i] * t[i];
	}
	nf = std::sqrt(nf);
	nt = std::sqrt(nt);
	L qx = q.x, qy = q.y, qz = q.z, qw = q.w;
	nq = std::sqrt(qx * qx + qy * qy + qz * qz + qw * qw);
	qx /= nq;
	qy /= nq;
	qz /= nq;
	qw /= nq;
	L vx = f[0] / nf, vy = f[1] / nf, vz = f[2] / nf;
	/* v + 2 w (u x v) + 2 u x (u x v) */
	L cx = qy * vz - qz * vy, cy = qz * vx - qx * vz, cz = qx * vy - qy * vx;
	L rx = vx + 2 * (qw * cx + qy * cz - qz * cy);
	L ry = vy + 2 * (qw * cy + qz * cx - qx * cz);
	L rz = vz + 2 * (qw * cz + qx * cy - qy * cx);
	L ex = rx - t[0] / nt, ey = ry - t[1] / nt, ez = rz - t[2] / nt;
	return (double)std::sqrt(ex * ex + ey * ey + ez * ez);
}

[[maybe_unused]] static int zero_rcond;

[[maybe_unused]] static double rc_l(const struct matrix4 &a)
{
	double d[16], inv[16], det, rc;
	dbl<16>(a.m, d);
	lapack_inverse(d, 4, inv, &det, &rc);
	return rc;
}

#ifndef PRECISION_ONLY
static void accuracy()
{
	const char *area = "accuracy against long double (largest error over the inputs)";

	/* angle between vectors: random, nearly parallel, nearly opposite */
	{
		const char *notes[3] = {"random", "1e-3 rad apart", "1e-3 rad from opposite"};
		for (int kind = 0; kind < 3; kind++) {
			double h3 = 0, g3 = 0, h2 = 0, g2 = 0;
			for (int i = 0; i < N; i++) {
				struct vector3 a = rv3(), b = rv3();
				struct vector2 c = rv2(), d = rv2();
				if (kind > 0) {
					struct vector3 axis;
					struct quaternion q;
					vector3_cross_product(&axis, &a, &b);
					quaternion_set_from_axis_anglev3(&q, &axis, kind == 1 ? (H)1e-3 : (H)(PI - 1e-3));
					b = a;
					vector3_rotate_by_quaternion(&b, &q);
					vector3_multiplyf(&b, r(0.5, 2));
					H s = std::sin(kind == 1 ? (H)1e-3 : (H)(PI - 1e-3)), co = std::cos(kind == 1 ? (H)1e-3 : (H)(PI - 1e-3));
					vector2_setf2(&d, (c.x * co - c.y * s) * r(0.5, 2), (c.x * s + c.y * co) * r(0.5, 2));
				}
				double o3 = oracle_angle3(a, b), o2 = oracle_angle2(c, d);
				h3 = std::max(h3, std::fabs(vector3_angle_between(&a, &b) - o3));
				g3 = std::max(g3, std::fabs((double)glm::angle(glm::normalize(gv(a)), glm::normalize(gv(b))) - o3));
				h2 = std::max(h2, std::fabs(vector2_angle_between(&c, &d) - o2));
				g2 = std::max(g2, std::fabs((double)glm::angle(glm::normalize(gv(c)), glm::normalize(gv(d))) - o2));
			}
			acc_row(area, "vector3_angle_between", notes[kind], h3);
			acc_row(area, "glm::angle(normalize, normalize), 3D", notes[kind], g3);
			acc_row(area, "vector2_angle_between", notes[kind], h2);
			acc_row(area, "glm::angle(normalize, normalize), 2D", notes[kind], g2);
		}
	}

	/* rotation between vectors: how far the result turns from from to */
	{
		const char *notes[3] = {"random", "1e-3 rad from opposite", "exactly opposite (to = -2 from)"};
		for (int kind = 0; kind < 3; kind++) {
			double eh = 0, eg = 0, ee = 0;
#ifdef HAVE_CGLM
			double ec = 0;
#endif
			for (int i = 0; i < N; i++) {
				struct vector3 a = rv3(), b = rv3();
				if (kind == 1) {
					struct vector3 axis = runit3();
					struct quaternion q;
					quaternion_set_from_axis_anglev3(&q, &axis, (H)(PI - 1e-3));
					vector3_cross_product(&axis, &a, &axis);
					quaternion_set_from_axis_anglev3(&q, &axis, (H)(PI - 1e-3));
					b = a;
					vector3_rotate_by_quaternion(&b, &q);
				} else if (kind == 2) {
					b = a;
					vector3_multiplyf(&b, -2);
				}
				struct quaternion h;
				quaternion_get_rotation_tov3(&a, &b, &h);
				eh = std::max(eh, landing_error(a, b, h));
				eg = std::max(eg, landing_error(a, b, hq(glm::rotation(glm::normalize(gv(a)), glm::normalize(gv(b))))));
				ee = std::max(ee, landing_error(a, b, hq(EQ(EQ::FromTwoVectors(ev(a), ev(b))))));
#ifdef HAVE_CGLM
				struct vector3 na = a, nb = b;
				vector3_normalize(&na);
				vector3_normalize(&nb);
				versor c;
				glm_quat_from_vecs(na.v, nb.v, c);
				struct quaternion cq;
				quaternion_setf4(&cq, c[0], c[1], c[2], c[3]);
				ec = std::max(ec, landing_error(a, b, cq));
#endif
			}
			acc_row(area, "quaternion_get_rotation_tov3", notes[kind], eh);
			acc_row(area, "glm::rotation", notes[kind], eg);
			acc_row(area, "Eigen FromTwoVectors", notes[kind], ee);
#ifdef HAVE_CGLM
			acc_row(area, "cglm glm_quat_from_vecs", notes[kind], ec);
#endif
		}
	}

	/* determinant and reciprocal condition */
	{
		double dh = 0, dg = 0, de = 0, rh = 0, rg = 0, rl = 0;
		for (int i = 0; i < N; i++) {
			struct matrix4 a = rm4_cond(rnd() * 6);
			LM4 l;
			for (int k = 0; k < 16; k++)
				l(k / 4, k % 4) = a.m[k];
			double od = (double)l.determinant();
			/* relative to the size of the largest product, as rounding scales */
			double scale = 0;
			for (int k = 0; k < 16; k++)
				scale = std::max(scale, std::fabs((double)a.m[k]));
			scale = scale * scale * scale * scale;
			dh = std::max(dh, std::fabs(matrix4_determinant(&a) - od) / scale);
			dg = std::max(dg, std::fabs(glm::determinant(g4(a)) - od) / scale);
			de = std::max(de, std::fabs(e4(a).determinant() - od) / scale);

			LM4 li = l.inverse();
			L na = 0, ni = 0;
			for (int rr = 0; rr < 4; rr++) {
				na = std::max(na, l.row(rr).cwiseAbs().sum());
				ni = std::max(ni, li.row(rr).cwiseAbs().sum());
			}
			double orc = (double)(1 / (na * ni));
			H hrc = matrix4_reciprocal_condition(&a);
			if (!(hrc > 0) && zero_rcond++ < 3)
				edge("matrix4_reciprocal_condition", "condition " + f(1 / orc) + ": " + sm4(a), f(hrc), "exact " + f(orc) + ", determinant " + f(od) + ", LAPACK estimate " + f(rc_l(a)));
			rh = std::max(rh, std::fabs(std::log10(hrc / orc)));
			/* the same formula with GLM's inverse */
			M4 gi = glm::inverse(g4(a));
			double gn = 0, gin = 0;
			for (int rr = 0; rr < 4; rr++) {
				gn = std::max(gn, (double)(std::fabs(g4(a)[0][rr]) + std::fabs(g4(a)[1][rr]) + std::fabs(g4(a)[2][rr]) + std::fabs(g4(a)[3][rr])));
				gin = std::max(gin, (double)(std::fabs(gi[0][rr]) + std::fabs(gi[1][rr]) + std::fabs(gi[2][rr]) + std::fabs(gi[3][rr])));
			}
			rg = std::max(rg, std::fabs(std::log10(1 / (gn * gin) / orc)));
			double d[16], inv[16], det, rc;
			dbl<16>(a.m, d);
			lapack_inverse(d, 4, inv, &det, &rc);
			rl = std::max(rl, std::fabs(std::log10(rc / orc)));
		}
		acc_row(area, "matrix4_determinant", "condition up to 1e6; relative to the largest element^4", dh);
		acc_row(area, "glm::determinant", "condition up to 1e6; relative to the largest element^4", dg);
		acc_row(area, "Eigen determinant", "condition up to 1e6; relative to the largest element^4", de);
		acc_row(area, "matrix4_reciprocal_condition", "condition up to 1e6; |log10(result / exact)|", rh);
		acc_row(area, "1 / (norm(A) norm(glm::inverse(A)))", "condition up to 1e6; |log10(result / exact)|", rg);
		acc_row(area, "LAPACK dgecon estimate (double)", "condition up to 1e6; |log10(result / exact)|", rl);
	}
}
#endif
