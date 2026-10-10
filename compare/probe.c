/* SPDX-License-Identifier: MIT */
/* Known-answer checks for one hypatia.h, with no other library: each check
 * uses only the library's own functions (a matrix is applied with its own
 * matrix4_multiplyv3 or matrix4_multiplyv4), so it does not depend on a layout
 * convention.
 *
 * build:  cc -std=c90 -I<dir of hypatia.h> probe.c -lm            (now, before)
 *         cc -std=c90 -I<dir of hypatia.h> -DMASTER probe.c -lm   (master names)
 */
#include <stdio.h>
#include <math.h>
#define HYPATIA_IMPLEMENTATION
#define HYPATIA_DOUBLE_PRECISION_FLOATS
#include "hypatia.h"
#ifdef MASTER
#define matrix4_set_from_quaternion matrix4_make_transformation_rotationq
#define matrix4_set_from_euler_anglesf3 matrix4_set_from_euler_anglesf3_EXP
#define matrix4_set_from_axisv3_angle matrix4_set_from_axisv3_angle_EXP
#define matrix4_projection_perspective_fovy_rh matrix4_projection_perspective_fovy_rh_EXP
#define matrix4_view_lookat_rh matrix4_view_lookat_rh_EXP
#define quaternion_angle_between quaternion_angle_between_EXP
#endif

static int failed;

static void report(const char *what, double err, double tol, const char *got)
{
	int ok = err <= tol; /* false for NaN */
	if (!ok)
		failed++;
	printf("%-4s %s: %s (error %.3g)\n", ok ? "ok" : "FAIL", what, got, err);
}

static double dist3(const struct vector3 *a, double x, double y, double z)
{
	return sqrt((a->x - x) * (a->x - x) + (a->y - y) * (a->y - y) + (a->z - z) * (a->z - z));
}

int main(void)
{
	char got[200];
	double s = sqrt(0.5), e;
	struct vector3 v, a, b, axis;
	struct vector4 p, c;
	struct quaternion q, r, id;
	struct matrix4 m, mi;
	HYP_FLOAT angle;

	quaternion_identity(&id);
	quaternion_setf4(&q, 0, 0, s, s); /* 90 degrees about z */

	/* 1. a vector with components 1e-20 normalizes to length 1 */
	vector3_setf3(&v, 1e-20, 2e-20, 2e-20);
	vector3_normalize(&v);
	sprintf(got, "(1, 2, 2) 1e-20 -> (%g, %g, %g)", v.x, v.y, v.z);
	report("vector3_normalize, tiny", dist3(&v, 1.0 / 3, 2.0 / 3, 2.0 / 3), 1e-15, got);

	/* 2. a rotation keeps the length of the vector */
	quaternion_setf4(&r, 0.54845635901662571, 0.52026387912708161, -0.19409713479822688, 0.62517791115248667);
	vector3_setf3(&v, -0.329554, -0.604897, 0.823295);
	e = vector3_magnitude(&v);
	vector3_rotate_by_quaternion(&v, &r);
	sprintf(got, "length %.6g -> %.6g", e, vector3_magnitude(&v));
	report("vector3_rotate_by_quaternion keeps the length", fabs(vector3_magnitude(&v) - e), 1e-14, got);

	/* 3. the matrix of q moves a vector as vector3_rotate_by_quaternion does */
	matrix4_set_from_quaternion(&m, &q);
	vector3_setf3(&a, 1, 0, 0);
	matrix4_multiplyv3(&m, &a, &v);
	b = a;
	vector3_rotate_by_quaternion(&b, &q);
	sprintf(got, "x -> (%g, %g, %g) by the matrix, (%g, %g, %g) by the quaternion", v.x, v.y, v.z, b.x, b.y, b.z);
	report("matrix4_set_from_quaternion agrees with vector3_rotate_by_quaternion", dist3(&v, b.x, b.y, b.z), 1e-15, got);

	/* 4. euler (0, 0, a) is the rotation by a about z, as the axis-angle matrix */
	matrix4_set_from_euler_anglesf3(&m, 0, 0, 0.5);
	matrix4_multiplyv3(&m, &a, &v);
	vector3_setf3(&axis, 0, 0, 1);
	matrix4_set_from_axisv3_angle(&mi, &axis, 0.5);
	matrix4_multiplyv3(&mi, &a, &b);
	sprintf(got, "x -> (%g, %g, %g) by euler, (%g, %g, %g) by axis-angle", v.x, v.y, v.z, b.x, b.y, b.z);
	report("matrix4_set_from_euler_anglesf3 agrees with matrix4_set_from_axisv3_angle", dist3(&v, b.x, b.y, b.z), 1e-15, got);

	/* 5. an invertible matrix with a small determinant (1e-6) has an inverse */
	matrix4_identity(&m);
	m.m[0] = m.m[5] = m.m[10] = 0.01;
	matrix4_identity(&mi);
	matrix4_inverse(&m, &mi);
	sprintf(got, "diag(0.01, 0.01, 0.01, 1) -> diag(%g, %g, %g, %g)", mi.m[0], mi.m[5], mi.m[10], mi.m[15]);
	report("matrix4_inverse, determinant 1e-6", fabs(mi.m[0] - 100) + fabs(mi.m[5] - 100) + fabs(mi.m[10] - 100), 1e-12, got);

	/* 6. the perspective matrix maps the top of the field of view to y / w = 1 */
	matrix4_projection_perspective_fovy_rh(&m, HYP_PI / 3, 1, 1, 10);
	vector4_setf4(&p, 0, 5 * tan(HYP_PI / 6), -5, 1);
	matrix4_multiplyv4(&m, &p, &c);
	sprintf(got, "fovy 60 degrees, a point on the top edge -> y / w = %g", c.y / c.w);
	report("matrix4_projection_perspective_fovy_rh", fabs(c.y / c.w - 1), 1e-15, got);

	/* 7. the view matrix moves the eye to the origin */
	vector3_setf3(&v, 1, 2, 5);
	vector3_setf3(&a, 0, 0, 0);
	vector3_setf3(&b, 0, 1, 0);
	matrix4_view_lookat_rh(&m, &v, &a, &b);
	matrix4_multiplyv3(&m, &v, &a);
	sprintf(got, "eye (1, 2, 5) -> (%g, %g, %g)", a.x, a.y, a.z);
	report("matrix4_view_lookat_rh", dist3(&a, 0, 0, 0), 1e-14, got);

	/* 8. slerp at t = 1e-6 from the identity to 90 degrees turns by 1e-6 * 90 degrees */
	quaternion_slerp(&id, &q, 1e-6, &r);
	e = 2 * atan2(r.z, r.w);
	sprintf(got, "turns by %.10g rad, exact %.10g", e, 1e-6 * HYP_PI / 2);
	report("quaternion_slerp, t = 1e-6", fabs(e / (1e-6 * HYP_PI / 2) - 1), 1e-12, got);

	/* 9. slerp halfway between quaternions 1e-3 rad apart is of unit length */
	quaternion_setf4(&r, 0, 0, sin(0.5e-3), cos(0.5e-3));
	quaternion_slerp(&id, &r, 0.5, &r);
	sprintf(got, "length %.17g", quaternion_magnitude(&r));
	report("quaternion_slerp, 1e-3 rad apart", fabs(quaternion_magnitude(&r) - 1), 1e-15, got);

	/* 10. the rotation from x to y is the unit quaternion of 90 degrees about z */
	vector3_setf3(&a, 1, 0, 0);
	vector3_setf3(&b, 0, 1, 0);
	quaternion_get_rotation_tov3(&a, &b, &r);
	sprintf(got, "(%g, %g, %g, %g)", r.x, r.y, r.z, r.w);
	report("quaternion_get_rotation_tov3, x to y", fabs(r.x) + fabs(r.y) + fabs(r.z - s) + fabs(r.w - s), 1e-15, got);

	/* 11. the rotation from x to -x is a half turn (w = 0, unit length) */
	vector3_setf3(&b, -1, 0, 0);
	quaternion_get_rotation_tov3(&a, &b, &r);
	sprintf(got, "(%g, %g, %g, %g)", r.x, r.y, r.z, r.w);
	report("quaternion_get_rotation_tov3, x to -x", fabs(r.w) + fabs(quaternion_magnitude(&r) - 1) + fabs(r.x), 1e-15, got);

	/* 12. the angle between rotations 1e-4 rad apart */
	quaternion_setf4(&r, 0, 0, sin(0.5e-4), cos(0.5e-4));
	e = quaternion_angle_between(&id, &r);
	sprintf(got, "%.17g", e);
	report("quaternion_angle_between, 1e-4 rad", fabs(e / 1e-4 - 1), 1e-12, got);

	/* 13. the axis and angle of a rotation by 1e-4 rad about z */
	quaternion_get_axis_anglev3(&r, &axis, &angle);
	sprintf(got, "axis (%g, %g, %g), angle %.17g", axis.x, axis.y, axis.z, angle);
	report("quaternion_get_axis_anglev3, 1e-4 rad", fabs(angle / 1e-4 - 1) + dist3(&axis, 0, 0, 1), 1e-12, got);

	printf("%d failed\n", failed);
	return failed != 0;
}
