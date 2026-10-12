/* SPDX-License-Identifier: MIT */

/* Every input size: the functions are given ordinary inputs multiplied by a
 * power of two s, from near the smallest normal number to near the largest,
 * and their results checked against the law that holds for s (the result is
 * the same, s times as large, or 1 / s times as large).  Multiplying by a
 * power of two is exact, so any difference beyond a few ulps comes from an
 * overflow or underflow inside the function.
 */

#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include <hypatia.h>
#include "limits.h"

static int failures;
static HYP_FLOAT tolerance;

/* the n values against expected times factor, within tolerance of the
 * largest of them
 */
static void check(const char *name, HYP_FLOAT s, const HYP_FLOAT *values, const HYP_FLOAT *expected, HYP_FLOAT factor, uint8_t n)
{
	HYP_FLOAT largest = HYP_FLOAT_C(0.0);
	HYP_FLOAT target;
	uint8_t i;
	int bad = 0;

	for (i = 0; i < n; i++) {
		largest = HYP_MAX(largest, HYP_ABS(expected[i] * factor));
	}
	for (i = 0; i < n; i++) {
		target = expected[i] * factor;
		if (!(HYP_ABS(values[i] - target) <= tolerance * largest)) {
			bad = 1;
		}
	}
	if (bad) {
		failures++;
		printf("%s at s = %g:", name, (double)s);
		for (i = 0; i < n; i++) {
			printf(" %g (%g)", (double)values[i], (double)(expected[i] * factor));
		}
		printf("\n");
	}
}

static void scaled3(struct vector3 *vR, const struct vector3 *v, HYP_FLOAT s)
{
	vector3_multiplyf(vector3_set(vR, v), s);
}

static void scaled4(struct vector4 *vR, const struct vector4 *v, HYP_FLOAT s)
{
	vector4_multiplyf(vector4_set(vR, v), s);
}

static void scaledq(struct quaternion *qR, const struct quaternion *q, HYP_FLOAT s)
{
	quaternion_multiplyf(quaternion_set(qR, q), s);
}

static void scaledm4(struct matrix4 *mR, const struct matrix4 *m, HYP_FLOAT s)
{
	uint8_t i;

	*mR = *m;
	for (i = 0; i < 16; i++) {
		mR->m[i] *= s;
	}
}

/* the functions at scale s, against their results at scale 1 */
static void at_scale(HYP_FLOAT s)
{
	struct vector3 a, b, n, r, ra, rb;
	struct vector4 a4, b4, r4, ra4;
	struct vector2 a2, b2, r2, ra2;
	struct quaternion p, q, qr, qa, qb;
	struct matrix2 matrix_a2, i2, e2;
	struct matrix3 matrix_a3, i3, e3;
	struct matrix4 matrix_a4, i4, e4, s4;
	HYP_FLOAT x[3], y[3], value, expected;
	uint8_t i;

	vector3_setf3(&a, HYP_FLOAT_C(0.3), -HYP_FLOAT_C(1.2), HYP_FLOAT_C(0.7));
	vector3_setf3(&b, HYP_FLOAT_C(1.1), HYP_FLOAT_C(0.4), -HYP_FLOAT_C(0.9));
	vector3_setf3(&n, -HYP_FLOAT_C(0.2), HYP_FLOAT_C(0.9), HYP_FLOAT_C(0.3));
	vector4_setf4(&a4, HYP_FLOAT_C(0.3), -HYP_FLOAT_C(1.2), HYP_FLOAT_C(0.7), HYP_FLOAT_C(2.1));
	vector4_setf4(&b4, HYP_FLOAT_C(1.1), HYP_FLOAT_C(0.4), -HYP_FLOAT_C(0.9), HYP_FLOAT_C(0.5));
	vector2_setf2(&a2, HYP_FLOAT_C(0.3), -HYP_FLOAT_C(1.2));
	vector2_setf2(&b2, HYP_FLOAT_C(1.1), HYP_FLOAT_C(0.4));
	quaternion_setf4(&p, HYP_FLOAT_C(0.2), -HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.4), HYP_FLOAT_C(0.9));
	quaternion_setf4(&q, -HYP_FLOAT_C(0.6), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.3), HYP_FLOAT_C(0.7));

	/* lengths and distances: s times */
	scaled3(&ra, &a, s);
	scaled3(&rb, &b, s);
	value = vector3_magnitude(&ra);
	expected = vector3_magnitude(&a);
	check("vector3_magnitude", s, &value, &expected, s, 1);
	value = vector3_distance(&ra, &rb);
	expected = vector3_distance(&a, &b);
	check("vector3_distance", s, &value, &expected, s, 1);
	scaled4(&ra4, &a4, s);
	value = vector4_magnitude(&ra4);
	expected = vector4_magnitude(&a4);
	check("vector4_magnitude", s, &value, &expected, s, 1);
	vector2_multiplyf(vector2_set(&ra2, &a2), s);
	value = vector2_magnitude(&ra2);
	expected = vector2_magnitude(&a2);
	check("vector2_magnitude", s, &value, &expected, s, 1);
	scaledq(&qa, &p, s);
	value = quaternion_magnitude(&qa);
	expected = quaternion_magnitude(&p);
	check("quaternion_magnitude", s, &value, &expected, s, 1);

	/* project, reflect, cross product, lerp: s times */
	vector3_project(vector3_set(&r, &ra), &rb);
	vector3_project(vector3_set(&n, &a), &b);
	check("vector3_project", s, r.v, n.v, s, 3);
	vector3_setf3(&n, -HYP_FLOAT_C(0.2), HYP_FLOAT_C(0.9), HYP_FLOAT_C(0.3));
	scaled3(&r, &a, s);
	vector3_reflect(&r, &n);
	vector3_reflect(vector3_set(&rb, &a), &n);
	check("vector3_reflect", s, r.v, rb.v, s, 3);
	vector3_cross_product(&r, &ra, &b);
	vector3_cross_product(&rb, &a, &b);
	check("vector3_cross_product", s, r.v, rb.v, s, 3);
	scaled3(&rb, &b, s);
	vector3_lerp(&ra, &rb, HYP_FLOAT_C(0.3), &r);
	vector3_lerp(&a, &b, HYP_FLOAT_C(0.3), &n);
	check("vector3_lerp", s, r.v, n.v, s, 3);
	vector2_set(&r2, &ra2);
	vector2_project(&r2, &b2);
	vector2_project(vector2_set(&ra2, &a2), &b2);
	check("vector2_project", s, r2.v, ra2.v, s, 2);
	vector4_project(vector4_set(&r4, &ra4), &b4);
	vector4_project(vector4_set(&ra4, &a4), &b4);
	check("vector4_project", s, r4.v, ra4.v, s, 4);

	/* applied to a vector: s times */
	matrix4_make_transformation_rotationq(&matrix_a4, &p);
	scaled4(&ra4, &a4, s);
	matrix4_multiplyv4(&matrix_a4, &ra4, &r4);
	matrix4_multiplyv4(&matrix_a4, &a4, &ra4);
	check("matrix4_multiplyv4", s, r4.v, ra4.v, s, 4);
	scaled3(&ra, &a, s);
	vector3_rotate_by_quaternion(vector3_set(&r, &ra), &p);
	vector3_rotate_by_quaternion(vector3_set(&n, &a), &p);
	check("vector3_rotate_by_quaternion (v)", s, r.v, n.v, s, 3);

	/* the same for a scaled quaternion, or scaled vectors */
	scaledq(&qa, &p, s);
	vector3_rotate_by_quaternion(vector3_set(&r, &a), &qa);
	vector3_rotate_by_quaternion(vector3_set(&n, &a), &p);
	check("vector3_rotate_by_quaternion (q)", s, r.v, n.v, HYP_FLOAT_C(1.0), 3);
	scaled3(&ra, &a, s);
	scaled3(&rb, &b, s);
	value = vector3_angle_between(&ra, &rb);
	expected = vector3_angle_between(&a, &b);
	check("vector3_angle_between", s, &value, &expected, HYP_FLOAT_C(1.0), 1);
	vector3_normalize(vector3_set(&r, &ra));
	vector3_normalize(vector3_set(&n, &a));
	check("vector3_normalize", s, r.v, n.v, HYP_FLOAT_C(1.0), 3);
	quaternion_get_rotation_tov3(&ra, &rb, &qr);
	quaternion_get_rotation_tov3(&a, &b, &qb);
	check("quaternion_get_rotation_tov3", s, qr.q, qb.q, HYP_FLOAT_C(1.0), 4);
	vector3_setf3(&n, -HYP_FLOAT_C(0.2), HYP_FLOAT_C(0.9), HYP_FLOAT_C(0.3));
	vector3_setf3(&r, HYP_FLOAT_C(0.3), -HYP_FLOAT_C(1.2), HYP_FLOAT_C(0.7));
	scaled3(&rb, &n, s);
	vector3_refract(vector3_set(&ra, &r), &rb, HYP_FLOAT_C(0.8));
	scaled3(&rb, &r, s);
	vector3_refract(&rb, &n, HYP_FLOAT_C(0.8));
	vector3_refract(&r, &n, HYP_FLOAT_C(0.8));
	check("vector3_refract (normal)", s, ra.v, r.v, HYP_FLOAT_C(1.0), 3);
	check("vector3_refract (direction)", s, rb.v, r.v, HYP_FLOAT_C(1.0), 3);
	scaled3(&ra, &a, s);
	scaled3(&rb, &b, s);
	quaternion_set_look_rotation_rh(&qr, &ra, &rb);
	quaternion_set_look_rotation_rh(&qb, &a, &b);
	check("quaternion_set_look_rotation_rh", s, qr.q, qb.q, HYP_FLOAT_C(1.0), 4);
	quaternion_set_from_axis_anglev3(&qr, &ra, HYP_FLOAT_C(0.7));
	quaternion_set_from_axis_anglev3(&qb, &a, HYP_FLOAT_C(0.7));
	check("quaternion_set_from_axis_anglev3", s, qr.q, qb.q, HYP_FLOAT_C(1.0), 4);

	/* functions of a rotation, given a scaled quaternion: the same */
	scaledq(&qa, &p, s);
	scaledq(&qb, &q, s);
	quaternion_get_euler_anglesf3(&qa, &x[0], &x[1], &x[2]);
	quaternion_get_euler_anglesf3(&p, &y[0], &y[1], &y[2]);
	check("quaternion_get_euler_anglesf3", s, x, y, HYP_FLOAT_C(1.0), 3);
	quaternion_get_axis_anglev3(&qa, &r, &x[0]);
	quaternion_get_axis_anglev3(&p, &n, &y[0]);
	check("quaternion_get_axis_anglev3 (axis)", s, r.v, n.v, HYP_FLOAT_C(1.0), 3);
	check("quaternion_get_axis_anglev3 (angle)", s, x, y, HYP_FLOAT_C(1.0), 1);
	value = quaternion_angle_between(&qa, &qb);
	expected = quaternion_angle_between(&p, &q);
	check("quaternion_angle_between", s, &value, &expected, HYP_FLOAT_C(1.0), 1);
	quaternion_normalize(quaternion_set(&qr, &qa));
	quaternion_normalize(quaternion_set(&qa, &p));
	check("quaternion_normalize", s, qr.q, qa.q, HYP_FLOAT_C(1.0), 4);
	scaledq(&qa, &p, s);
	quaternion_nlerp(&qa, &qb, HYP_FLOAT_C(0.3), &qr);
	quaternion_nlerp(&p, &q, HYP_FLOAT_C(0.3), &qa);
	check("quaternion_nlerp", s, qr.q, qa.q, HYP_FLOAT_C(1.0), 4);
	scaledq(&qa, &p, s);
	quaternion_slerp(&qa, &qb, HYP_FLOAT_C(0.3), &qr);
	quaternion_slerp(&p, &q, HYP_FLOAT_C(0.3), &qa);
	check("quaternion_slerp", s, qr.q, qa.q, s, 4);

	/* the same with a negative dot product, where q is turned to -q */
	scaledq(&qa, &p, s);
	scaledq(&qb, &q, -s);
	value = quaternion_angle_between(&qa, &qb);
	expected = quaternion_angle_between(&p, &q);
	check("quaternion_angle_between (-q)", s, &value, &expected, HYP_FLOAT_C(1.0), 1);
	quaternion_slerp(&qa, &qb, HYP_FLOAT_C(0.3), &qr);
	quaternion_slerp(&p, &q, HYP_FLOAT_C(0.3), &qa);
	check("quaternion_slerp (-q)", s, qr.q, qa.q, s, 4);
	scaledq(&qb, &q, s);
	scaledq(&qa, &p, s);
	matrix4_set_from_quaternion(&e4, &qa);
	matrix4_set_from_quaternion(&i4, &p);
	check("matrix4_set_from_quaternion", s, e4.m, i4.m, HYP_FLOAT_C(1.0), 16);

	/* inverses: 1 / s times */
	quaternion_inverse(quaternion_set(&qr, &qa));
	quaternion_inverse(quaternion_set(&qb, &p));
	check("quaternion_inverse", s, qr.q, qb.q, HYP_FLOAT_C(1.0) / s, 4);
	matrix2_identity(&matrix_a2);
	matrix_a2.r00 = HYP_FLOAT_C(1.3);
	matrix_a2.r01 = -HYP_FLOAT_C(0.4);
	matrix_a2.r10 = HYP_FLOAT_C(0.7);
	matrix_a2.r11 = HYP_FLOAT_C(0.9);
	matrix2_inverse(&matrix_a2, &e2);
	for (i = 0; i < 4; i++) {
		matrix_a2.m[i] *= s;
	}
	if (matrix2_inverse(&matrix_a2, &i2) == NULL) {
		matrix2_zero(&i2);
	}
	check("matrix2_inverse", s, i2.m, e2.m, HYP_FLOAT_C(1.0) / s, 4);
	matrix3_identity(&matrix_a3);
	matrix_a3.r00 = HYP_FLOAT_C(1.3);
	matrix_a3.r01 = -HYP_FLOAT_C(0.4);
	matrix_a3.r12 = HYP_FLOAT_C(0.7);
	matrix_a3.r20 = HYP_FLOAT_C(0.2);
	matrix_a3.r22 = HYP_FLOAT_C(0.9);
	matrix3_inverse(&matrix_a3, &e3);
	for (i = 0; i < 9; i++) {
		matrix_a3.m[i] *= s;
	}
	if (matrix3_inverse(&matrix_a3, &i3) == NULL) {
		matrix3_zero(&i3);
	}
	check("matrix3_inverse", s, i3.m, e3.m, HYP_FLOAT_C(1.0) / s, 9);
	matrix4_make_transformation_rotationq(&matrix_a4, &p);
	matrix_a4.r00 *= HYP_FLOAT_C(1.5);
	matrix_a4.r03 = HYP_FLOAT_C(0.5);
	matrix_a4.r33 = HYP_FLOAT_C(1.2);
	matrix4_inverse(&matrix_a4, &e4);
	scaledm4(&s4, &matrix_a4, s);
	if (matrix4_inverse(&s4, &i4) == NULL) {
		matrix4_zero(&i4);
	}
	check("matrix4_inverse", s, i4.m, e4.m, HYP_FLOAT_C(1.0) / s, 16);
	value = matrix4_reciprocal_condition(&s4);
	expected = matrix4_reciprocal_condition(&matrix_a4);
	check("matrix4_reciprocal_condition", s, &value, &expected, HYP_FLOAT_C(1.0), 1);
	matrix_a4.r33 = HYP_FLOAT_C(1.0);
	matrix4_normal_matrix(&matrix_a4, &e4);
	scaledm4(&s4, &matrix_a4, s);
	s4.r33 = HYP_FLOAT_C(1.0);
	if (matrix4_normal_matrix(&s4, &i4) == NULL) {
		matrix4_zero(&i4);
	}
	for (i = 0; i < 3; i++) {
		check("matrix4_normal_matrix", s, i4.m44[i], e4.m44[i], HYP_FLOAT_C(1.0) / s, 3);
	}

	/* decompose: the scale and the translation s times, the rotation the same */
	matrix4_make_transformation_rotationq(&matrix_a4, &p);
	matrix_a4.r03 = HYP_FLOAT_C(0.5);
	matrix_a4.r13 = -HYP_FLOAT_C(2.0);
	scaledm4(&s4, &matrix_a4, s);
	s4.r30 = HYP_FLOAT_C(0.0);
	s4.r31 = HYP_FLOAT_C(0.0);
	s4.r32 = HYP_FLOAT_C(0.0);
	s4.r33 = HYP_FLOAT_C(1.0);
	matrix4_transformation_decompose(&s4, &r, &qr, &rb);
	matrix4_transformation_decompose(&matrix_a4, &n, &qb, &ra);
	check("matrix4_transformation_decompose (scale)", s, r.v, n.v, s, 3);
	check("matrix4_transformation_decompose (rotation)", s, qr.q, qb.q, HYP_FLOAT_C(1.0), 4);
	check("matrix4_transformation_decompose (translation)", s, rb.v, ra.v, s, 3);
}

int main(void)
{
	HYP_FLOAT epsilon = HYP_FLOAT_C(1.0);
	HYP_FLOAT smallest = HYP_FLOAT_C(1.0);
	HYP_FLOAT largest = test_largest_power2();
	HYP_FLOAT s;

	while (test_rounded(HYP_FLOAT_C(1.0) + epsilon / HYP_FLOAT_C(2.0)) > HYP_FLOAT_C(1.0)) {
		epsilon /= HYP_FLOAT_C(2.0);
	}
	tolerance = HYP_FLOAT_C(16.0) * epsilon;
	while (!test_is_subnormal(smallest / HYP_FLOAT_C(2.0))) {
		smallest /= HYP_FLOAT_C(2.0);
	}

	/* s from 2^8 times the smallest normal number to 2^-8 times the largest */
	for (s = smallest * HYP_FLOAT_C(256.0); s <= largest / HYP_FLOAT_C(256.0); s *= HYP_FLOAT_C(8.0)) {
		at_scale(s);
	}

	if (failures != 0) {
		printf("%d failed\n", failures);
		return 1;
	}
	return 0;
}
