/* SPDX-License-Identifier: MIT */

#include "random_source.h"

/** [quaternion identity example] */
static const char *test_quaternion_identity(void)
{
	struct quaternion q;

	quaternion_identity(&q);
	test_assert(scalar_equalsf(q.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(q.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(q.z, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(q.w, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(HYP_FLOAT_C(1.0), quaternion_norm(&q)));
	test_assert(scalar_equalsf(HYP_FLOAT_C(1.0), quaternion_magnitude(&q)));
	test_assert(quaternion_is_unit(&q));
	test_assert(!quaternion_is_pure(&q));

	return NULL;
}
/** [quaternion identity example] */


/** [quaternion conjugate example] */
static const char *test_quaternion_conjugate(void)
{
	struct quaternion qA;
	struct quaternion qB;

	quaternion_set_from_axis_anglef3(&qA, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_TAU / HYP_FLOAT_C(4.0));
	quaternion_set_from_axis_anglef3(&qB, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_TAU / HYP_FLOAT_C(4.0));
	quaternion_conjugate(&qB);
	test_assert(quaternion_equals(&qA, &qB));

	return NULL;
}
/** [quaternion conjugate example] */


/** [quaternion inverse example] */
static const char *test_quaternion_inverse(void)
{
	struct quaternion qA;
	struct quaternion qInverse;
	struct quaternion qIdentity;

	quaternion_set_from_axis_anglef3(&qA, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_TAU / HYP_FLOAT_C(4.0));
	quaternion_set(&qInverse, &qA);
	quaternion_inverse(&qInverse);
	quaternion_multiply(&qA, &qInverse);
	quaternion_normalize(&qA);
	quaternion_identity(&qIdentity);
	test_assert(quaternion_equals(&qA, &qIdentity));

	return NULL;
}


static const char *test_quaternion_inverse_unit(void)
{
	struct quaternion q;
	struct quaternion inverse;
	struct quaternion product;
	struct quaternion expected;

	/* for a unit quaternion the inverse is the conjugate */
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(3.0));
	quaternion_set(&inverse, &q);
	quaternion_inverse(&inverse);
	quaternion_conjugate(quaternion_set(&expected, &q));
	test_assert(quaternion_equals(&inverse, &expected));

	/* q * inverse(q) is the identity, without normalizing */
	quaternion_multiply(quaternion_set(&product, &q), &inverse);
	test_assert(quaternion_equals(&product, quaternion_identity(&expected)));

	return NULL;
}


static const char *test_quaternion_inverse_not_unit(void)
{
	struct quaternion q;
	struct quaternion inverse;
	struct quaternion product;
	struct quaternion expected;

	/* |q|^2 = 30: the inverse is the conjugate divided by 30 */
	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_set(&inverse, &q);
	quaternion_inverse(&inverse);
	quaternion_setf4(&expected, -HYP_FLOAT_C(1.0) / HYP_FLOAT_C(30.0), -HYP_FLOAT_C(2.0) / HYP_FLOAT_C(30.0), -HYP_FLOAT_C(3.0) / HYP_FLOAT_C(30.0), HYP_FLOAT_C(4.0) / HYP_FLOAT_C(30.0));
	test_assert(quaternion_equals(&inverse, &expected));

	/* q * inverse(q) and inverse(q) * q are the identity, without normalizing */
	quaternion_identity(&expected);
	quaternion_multiply(quaternion_set(&product, &q), &inverse);
	test_assert(quaternion_equals(&product, &expected));
	quaternion_multiply(quaternion_set(&product, &inverse), &q);
	test_assert(quaternion_equals(&product, &expected));

	return NULL;
}
/** [quaternion inverse example] */


static const char *test_quaternion_axis_anglev3(void)
{
	struct quaternion q, q1;
	HYP_FLOAT c;
	HYP_FLOAT s;
	HYP_FLOAT angle = HYP_TAU / HYP_FLOAT_C(4.0);
	struct vector3 axis;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, angle);

	vector3_set(&axis, HYP_VECTOR3_UNIT_X);

	c = HYP_COS(angle / HYP_FLOAT_C(2.0));
	s = HYP_SIN(angle / HYP_FLOAT_C(2.0));

	q1.x = axis.x * s;
	q1.y = axis.y * s;
	q1.z = axis.z * s;
	q1.w = c;

	quaternion_normalize(&q1);

	test_assert(quaternion_equals(&q, &q1));

	return NULL;
}


static const char *test_quaternion_get_set_axis_anglev3(void)
{
	struct quaternion q;
	HYP_FLOAT angle, angle1;
	struct vector3 axis, axis1;

	vector3_set(&axis, HYP_VECTOR3_UNIT_X);
	angle = HYP_TAU / HYP_FLOAT_C(4.0);

	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	quaternion_get_axis_anglev3(&q, &axis1, &angle1);

	test_assert(vector3_equals(&axis, &axis1));
	test_assert(scalar_equalsf(angle, angle1));

	return NULL;
}


static const char *test_quaternion_multiply(void)
{
	struct quaternion qA, qB;

	quaternion_set_from_axis_anglev3(&qA, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(8.0));
	quaternion_set_from_axis_anglev3(&qB, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0));

	/* qA squared */
	quaternion_multiply(&qA, &qA);

	test_assert(quaternion_equals(&qA, &qB));

	return NULL;
}


static const char *test_quaternion_multiply_identity(void)
{
	struct quaternion qA, qB, q;

	qA.x = 1;
	qA.y = 2;
	qA.z = 3;
	qA.w = 4;

	quaternion_identity(&qB);

	quaternion_set(&q, &qA);
	quaternion_multiply(&q, &qB);

	test_assert(quaternion_equals(&q, &qA));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_yx_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_zx_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_diagonal_third_turn(void)
{
	/* a third of a turn about (1, 1, 1) cycles the axes: X -> Y -> Z -> X.
	 * Rotations about a coordinate axis cannot catch a wrong sign on x * vx
	 * in quaternion_multiplyv3; this one does (PR #4).
	 */
	struct quaternion q;
	struct vector3 axis;
	struct vector3 v;

	vector3_normalize(vector3_setf3(&axis, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0)));
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_TAU / HYP_FLOAT_C(3.0));

	vector3_rotate_by_quaternion(vector3_set(&v, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&v, HYP_VECTOR3_UNIT_Y));

	vector3_rotate_by_quaternion(vector3_set(&v, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&v, HYP_VECTOR3_UNIT_Z));

	vector3_rotate_by_quaternion(vector3_set(&v, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&v, HYP_VECTOR3_UNIT_X));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_xy_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_zy_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_xz_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_yz_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X_NEGATIVE));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_yx_half_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(2.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static const char *test_vector3_rotate_by_quaternion_xy_half_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / HYP_FLOAT_C(2.0));
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X_NEGATIVE));

	return NULL;
}


static const char *test_quaternion_slerp(void)
{
	struct quaternion q, q1, q2, q3;
	HYP_FLOAT angle;

	angle = HYP_TAU / HYP_FLOAT_C(4.0);

	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_X, angle);
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_X, angle * HYP_FLOAT_C(1.1));
	quaternion_set_from_axis_anglev3(&q3, HYP_VECTOR3_UNIT_X, angle * HYP_FLOAT_C(1.2));

	/* half-way */
	quaternion_slerp(&q1, &q3, HYP_FLOAT_C(0.5), &q);
	test_assert(quaternion_equals(&q, &q2));

	/* none */
	quaternion_slerp(&q1, &q3, HYP_FLOAT_C(0.0), &q);
	test_assert(quaternion_equals(&q, &q1));

	/* all the way */
	quaternion_slerp(&q1, &q3, HYP_FLOAT_C(1.0), &q);
	test_assert(quaternion_equals(&q, &q3));


	/* swap order half-way */
	quaternion_slerp(&q3, &q1, HYP_FLOAT_C(0.5), &q);
	test_assert(quaternion_equals(&q, &q2));

	/* swap order none */
	quaternion_slerp(&q3, &q1, HYP_FLOAT_C(0.0), &q);
	test_assert(quaternion_equals(&q, &q3));

	/* swap order all the way */
	quaternion_slerp(&q3, &q1, HYP_FLOAT_C(1.0), &q);
	test_assert(quaternion_equals(&q, &q1));


	/* go reverse around the sphere */
	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_X, angle);
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_X, angle * HYP_FLOAT_C(0.9));
	quaternion_set_from_axis_anglev3(&q3, HYP_VECTOR3_UNIT_X, angle * HYP_FLOAT_C(0.8));

	/* go reverse half-way */
	quaternion_slerp(&q1, &q3, HYP_FLOAT_C(0.5), &q);
	test_assert(quaternion_equals(&q, &q2));

	/* go reverse none */
	quaternion_slerp(&q1, &q3, HYP_FLOAT_C(0.0), &q);
	test_assert(quaternion_equals(&q, &q1));

	/* go reverse all the way */
	quaternion_slerp(&q1, &q3, HYP_FLOAT_C(1.0), &q);
	test_assert(quaternion_equals(&q, &q3));


	/* swap order reverse half-way */
	quaternion_slerp(&q3, &q1, HYP_FLOAT_C(0.5), &q);
	test_assert(quaternion_equals(&q, &q2));

	/* swap order reverse none */
	quaternion_slerp(&q3, &q1, HYP_FLOAT_C(0.0), &q);
	test_assert(quaternion_equals(&q, &q3));

	/* swap order reverse all the way */
	quaternion_slerp(&q3, &q1, HYP_FLOAT_C(1.0), &q);
	test_assert(quaternion_equals(&q, &q1));

	return NULL;
}


static const char *test_quaternion_get_eulers_create_quaternion_ZYX(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT in_anglex, in_angley, in_anglez;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	in_anglex = HYP_FLOAT_C(0.8);
	in_angley = HYP_FLOAT_C(0.7);
	in_anglez = HYP_FLOAT_C(0.432);

	/* make a quaternion out of some arbitrary euler angles */
	quaternion_set_from_euler_anglesf3(&q1, in_anglex, in_angley, in_anglez);

	/* get the angles */
	quaternion_get_euler_anglesf3(&q1, &out_anglex, &out_angley, &out_anglez);

	/* test */
	test_assert(scalar_equals(in_anglex, out_anglex));
	test_assert(scalar_equals(in_angley, out_angley));
	test_assert(scalar_equals(in_anglez, out_anglez));

	/* compose new quaternions with the eulers */
	quaternion_set_from_euler_anglesf3(&q2, out_anglex, out_angley, out_anglez);

	/* same */
	test_assert(quaternion_equals(&q1, &q2));

	return NULL;
}


static const char *test_quaternion_rotate_by_quaternion_identity(void)
{
	struct quaternion scratchQuaternion;
	struct quaternion q1;
	HYP_FLOAT in_anglex, in_angley, in_anglez;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	in_anglex = HYP_FLOAT_C(0.8);
	in_angley = HYP_FLOAT_C(0.8);
	in_anglez = HYP_FLOAT_C(0.8);

	quaternion_set_from_euler_anglesf3(&q1,
		in_anglex, in_angley, in_anglez);

	quaternion_rotate_by_quaternion_EXP(&q1,
	    quaternion_identity(&scratchQuaternion));

	/* get the angles */
	quaternion_get_euler_anglesf3(&q1, &out_anglex, &out_angley, &out_anglez);

	/* test */
	test_assert(scalar_equals(in_anglex, out_anglex));
	test_assert(scalar_equals(in_angley, out_angley));
	test_assert(scalar_equals(in_anglez, out_anglez));


	quaternion_identity(&q1);
	quaternion_rotate_by_quaternion_EXP(&q1,
		quaternion_set_from_euler_anglesf3(&scratchQuaternion,
			in_anglex, in_angley, in_anglez));

	/* get the angles */
	quaternion_get_euler_anglesf3(&q1, &out_anglex, &out_angley, &out_anglez);

	/* test */
	test_assert(scalar_equals(in_anglex, out_anglex));
	test_assert(scalar_equals(in_angley, out_angley));
	test_assert(scalar_equals(in_anglez, out_anglez));

	return NULL;
}


static const char *test_quaternion_get_eulers_from_axis_angle(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT in_anglex, in_angley, in_anglez;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	/* making the original quaternion out of an arbitrary axis angle */
	quaternion_set_from_axis_anglef3(&q1, HYP_FLOAT_C(0.4), HYP_FLOAT_C(0.232), HYP_FLOAT_C(0.543), HYP_TAU * HYP_FLOAT_C(0.45));

	/* get the angles */
	quaternion_get_euler_anglesf3(&q1, &in_anglex, &in_angley, &in_anglez);

	/* compose new quaternions with the eulers */
	quaternion_set_from_euler_anglesf3(&q2, in_anglex, in_angley, in_anglez);

	/* get the angles */
	quaternion_get_euler_anglesf3(&q2, &out_anglex, &out_angley, &out_anglez);

	/* test */
	test_assert(scalar_equals(in_anglex, out_anglex));
	test_assert(scalar_equals(in_angley, out_angley));
	test_assert(scalar_equals(in_anglez, out_anglez));

	/* same */
	test_assert(quaternion_equals(&q1, &q2));

	return NULL;
}


static const char *test_quaternion_360_degree_eulers(void)
{
	struct quaternion q1;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	/* set the original quaternions with the eulers */
	quaternion_set_from_euler_anglesf3(&q1, HYP_FLOAT_C(0.0), HYP_DEG_TO_RAD(HYP_FLOAT_C(365.0)), HYP_FLOAT_C(0.0));

	/* get the angles */
	quaternion_get_euler_anglesf3(&q1, &out_anglex, &out_angley, &out_anglez);

	test_assert(scalar_equals(HYP_FLOAT_C(0.0), out_anglex));

	/* should be 5 degrees, quaternion will normalize the value */
	test_assert(scalar_equals(HYP_FLOAT_C(5.0), HYP_RAD_TO_DEG(out_angley)));

	test_assert(scalar_equals(HYP_FLOAT_C(0.0), out_anglez));

	return NULL;
}


static const char *test_quaternion_setf4(void)
{
	struct quaternion q;

	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	test_assert(scalar_equalsf(q.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(q.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(q.z, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(q.w, HYP_FLOAT_C(4.0)));

	quaternion_setf4(&q, -HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0));
	test_assert(scalar_equalsf(q.x, -HYP_FLOAT_C(0.5)));
	test_assert(scalar_equalsf(q.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(q.z, HYP_FLOAT_C(0.5)));
	test_assert(scalar_equalsf(q.w, HYP_FLOAT_C(1.0)));

	return NULL;
}


static const char *test_quaternion_add(void)
{
	struct quaternion q, qT, qExpected;

	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_setf4(&qT, HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.5), HYP_FLOAT_C(2.5), HYP_FLOAT_C(3.5));
	quaternion_add(&q, &qT);

	quaternion_setf4(&qExpected, HYP_FLOAT_C(1.5), HYP_FLOAT_C(3.5), HYP_FLOAT_C(5.5), HYP_FLOAT_C(7.5));
	test_assert(quaternion_equals(&q, &qExpected));

	/* adding zero quaternion should not change anything */
	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_setf4(&qT, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	quaternion_add(&q, &qT);
	quaternion_setf4(&qExpected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static const char *test_quaternion_subtract(void)
{
	struct quaternion q, qT, qExpected;

	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_setf4(&qT, HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.5), HYP_FLOAT_C(2.0));
	quaternion_subtract(&q, &qT);

	quaternion_setf4(&qExpected, HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.5), HYP_FLOAT_C(2.0));
	test_assert(quaternion_equals(&q, &qExpected));

	/* subtracting from itself should yield zero */
	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_subtract(&q, &q);
	quaternion_setf4(&qExpected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static const char *test_quaternion_negate(void)
{
	struct quaternion q, qExpected;

	quaternion_setf4(&q, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0));
	quaternion_negate(&q);
	quaternion_setf4(&qExpected, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	test_assert(quaternion_equals(&q, &qExpected));

	/* double negate should return to original */
	quaternion_setf4(&q, HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5));
	quaternion_negate(&q);
	quaternion_negate(&q);
	quaternion_setf4(&qExpected, HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5));
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static const char *test_quaternion_multiplyf(void)
{
	struct quaternion q, qExpected;

	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_multiplyf(&q, HYP_FLOAT_C(2.0));
	quaternion_setf4(&qExpected, HYP_FLOAT_C(2.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(8.0));
	test_assert(quaternion_equals(&q, &qExpected));

	/* multiply by zero */
	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_multiplyf(&q, HYP_FLOAT_C(0.0));
	quaternion_setf4(&qExpected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	test_assert(quaternion_equals(&q, &qExpected));

	/* multiply by one (no change) */
	quaternion_setf4(&q, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_multiplyf(&q, HYP_FLOAT_C(1.0));
	quaternion_setf4(&qExpected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static const char *test_quaternion_multiplyv3(void)
{
	struct quaternion q, qExpected;
	struct vector3 v;

	/* identity quaternion multiplied by unit x vector */
	quaternion_identity(&q);
	vector3_set(&v, HYP_VECTOR3_UNIT_X);
	quaternion_multiplyv3(&q, &v);

	/* manually compute: q=(0,0,0,1), v=(1,0,0)
	 * r.x = w*vx + y*vz - z*vy = 1*1 + 0*0 - 0*0 = 1
	 * r.y = w*vy - x*vz + z*vx = 1*0 - 0*0 + 0*1 = 0
	 * r.z = w*vz + x*vy - y*vx = 1*0 + 0*0 - 0*1 = 0
	 * r.w = x*vx - y*vy - z*vz = 0*1 - 0*0 - 0*0 = 0  (actually: +x*vx)
	 * Wait, let me re-read: r.w = self->x * vT->x - self->y * vT->y - self->z * vT->z
	 * = 0*1 - 0*0 - 0*0 = 0
	 */
	quaternion_setf4(&qExpected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static const char *test_quaternion_dot_product(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT dot;

	/* dot product with itself */
	quaternion_setf4(&q1, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	dot = quaternion_dot_product(&q1, &q1);
	/* 1+4+9+16 = 30 */
	test_assert(scalar_equalsf(dot, HYP_FLOAT_C(30.0)));

	/* dot product of orthogonal-ish quaternions */
	quaternion_setf4(&q1, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	quaternion_setf4(&q2, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	dot = quaternion_dot_product(&q1, &q2);
	test_assert(scalar_equalsf(dot, HYP_FLOAT_C(0.0)));

	/* dot product of identity with itself should be 1 */
	quaternion_identity(&q1);
	dot = quaternion_dot_product(&q1, &q1);
	test_assert(scalar_equalsf(dot, HYP_FLOAT_C(1.0)));

	return NULL;
}


static const char *test_quaternion_lerp(void)
{
	struct quaternion q1, q2, qR, qExpected;

	quaternion_setf4(&q1, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0));
	quaternion_setf4(&q2, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));

	/* at t=0, should return start */
	quaternion_lerp(&q1, &q2, HYP_FLOAT_C(0.0), &qR);
	test_assert(quaternion_equals(&qR, &q1));

	/* at t=1, should return end */
	quaternion_lerp(&q1, &q2, HYP_FLOAT_C(1.0), &qR);
	test_assert(quaternion_equals(&qR, &q2));

	/* at t=0.5, should be midpoint */
	quaternion_lerp(&q1, &q2, HYP_FLOAT_C(0.5), &qR);
	quaternion_setf4(&qExpected, HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.5));
	test_assert(quaternion_equals(&qR, &qExpected));

	return NULL;
}


static const char *test_quaternion_nlerp(void)
{
	struct quaternion q1, q2, qR;

	quaternion_setf4(&q1, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0));
	quaternion_setf4(&q2, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));

	/* at t=0, should return start */
	quaternion_nlerp(&q1, &q2, HYP_FLOAT_C(0.0), &qR);
	test_assert(quaternion_equals(&qR, &q1));

	/* at t=1, should return end */
	quaternion_nlerp(&q1, &q2, HYP_FLOAT_C(1.0), &qR);
	test_assert(quaternion_equals(&qR, &q2));

	/* at t=0.5, should be normalized midpoint */
	quaternion_nlerp(&q1, &q2, HYP_FLOAT_C(0.5), &qR);
	/* lerp gives (0.5, 0, 0, 0.5), normalized magnitude = sqrt(0.5) */
	test_assert(scalar_equalsf(HYP_FLOAT_C(1.0), quaternion_norm(&qR)));

	return NULL;
}


static const char *test_quaternion_get_rotation_tov3(void)
{
	struct quaternion qR;
	struct vector3 r;

	/* rotation from unit X to unit Y should rotate X to Y */
	quaternion_get_rotation_tov3(HYP_VECTOR3_UNIT_X, HYP_VECTOR3_UNIT_Y, &qR);
	quaternion_normalize(&qR);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_rotate_by_quaternion(&r, &qR);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	/* rotation from unit X to unit Z should rotate X to Z */
	quaternion_get_rotation_tov3(HYP_VECTOR3_UNIT_X, HYP_VECTOR3_UNIT_Z, &qR);
	quaternion_normalize(&qR);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_rotate_by_quaternion(&r, &qR);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	/* rotation from a vector to itself should be identity-like (no rotation) */
	quaternion_get_rotation_tov3(HYP_VECTOR3_UNIT_X, HYP_VECTOR3_UNIT_X, &qR);
	quaternion_normalize(&qR);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_rotate_by_quaternion(&r, &qR);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X));

	return NULL;
}


static const char *test_quaternion_slerp_nearly_identical(void)
{
	struct quaternion q1, q2, qR, expected;

	/* identity quaternion and a tiny rotation around X */
	quaternion_identity(&q1);
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_X, HYP_FLOAT_C(0.0001));

	/* SLERP at t=0.5 should produce half of the tiny rotation */
	quaternion_slerp(&q1, &q2, HYP_FLOAT_C(0.5), &qR);

	quaternion_set_from_axis_anglev3(&expected, HYP_VECTOR3_UNIT_X, HYP_FLOAT_C(0.00005));
	test_assert(quaternion_equals(&qR, &expected));
	test_assert(scalar_equalsf(qR.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(qR.z, HYP_FLOAT_C(0.0)));

	return NULL;
}


static const char *test_quaternion_slerp_opposite(void)
{
	struct quaternion q1, q2, qR;

	/* q and -q represent the same rotation */
	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0));
	quaternion_set(&q2, &q1);
	quaternion_negate(&q2);

	/* SLERP between q and -q should not produce NaN */
	quaternion_slerp(&q1, &q2, HYP_FLOAT_C(0.5), &qR);

	/* result should be a valid quaternion (not NaN) */
	test_assert(scalar_equalsf(qR.w, qR.w)); /* NaN != NaN */
	test_assert(scalar_equalsf(qR.x, qR.x));
	test_assert(scalar_equalsf(qR.y, qR.y));
	test_assert(scalar_equalsf(qR.z, qR.z));

	return NULL;
}


/* slerp from the identity to end at t = 0.5, applied to X */
static struct vector3 *slerp_halfway_applied_to_x(const struct quaternion *end, struct vector3 *vR)
{
	struct quaternion start;
	struct quaternion qR;

	quaternion_identity(&start);
	quaternion_slerp(&start, end, HYP_FLOAT_C(0.5), &qR);

	return vector3_rotate_by_quaternion(vector3_set(vR, HYP_VECTOR3_UNIT_X), &qR);
}


static const char *test_quaternion_slerp_shortest_arc(void)
{
	struct quaternion end;
	struct vector3 r;
	struct vector3 expected;
	HYP_FLOAT half = HYP_SQRT(HYP_FLOAT_C(0.5));

	/* +270 degrees about Z is -90 degrees the short way: halfway is -45 */
	quaternion_set_from_axis_anglev3(&end, HYP_VECTOR3_UNIT_Z, HYP_FLOAT_C(3.0) * HYP_TAU / HYP_FLOAT_C(4.0));
	slerp_halfway_applied_to_x(&end, &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, half, -half, HYP_FLOAT_C(0.0))));

	/* -q is the same rotation as q (+90 degrees about Z): halfway is +45 */
	quaternion_set_from_axis_anglev3(&end, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	quaternion_negate(&end);
	slerp_halfway_applied_to_x(&end, &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, half, half, HYP_FLOAT_C(0.0))));

	return NULL;
}


static const char *test_quaternion_slerp_at_endpoints(void)
{
	struct quaternion q1, q2, qR;

	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_Y, HYP_TAU / HYP_FLOAT_C(6.0));
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(3.0));

	/* t=0 should return start */
	quaternion_slerp(&q1, &q2, HYP_FLOAT_C(0.0), &qR);
	test_assert(quaternion_equals(&qR, &q1));

	/* t=1 should return end */
	quaternion_slerp(&q1, &q2, HYP_FLOAT_C(1.0), &qR);
	test_assert(quaternion_equals(&qR, &q2));

	return NULL;
}


static const char *test_quaternion_set_random_unit_scripted(void)
{
	static const long zero[] = {0, 0, 0};
	struct quaternion q;
	struct quaternion expected;

	/* draws: u, then two angles; this one is a half turn about Y */
	test_random_script(zero, 3);
	quaternion_set_random_unit(&q);
	test_assert(quaternion_equals(&q, quaternion_setf4(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	test_random_script(NULL, 0);
	return NULL;
}


static const char *test_quaternion_set_random_unit_many(void)
{
	struct quaternion q;
	struct vector3 v;
	int counts[4];
	int small_z = 0;
	int i;

	for (i = 0; i < 4; i++) {
		counts[i] = 0;
	}

	for (i = 0; i < 10000; i++) {
		quaternion_set_random_unit(&q);
		test_assert(quaternion_is_unit(&q));

		/* evenly spread rotations send the X axis evenly over the sphere */
		vector3_setf3(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
		vector3_rotate_by_quaternion(&v, &q);
		if (v.z < HYP_FLOAT_C(-0.5)) {
			counts[0]++;
		} else if (v.z < HYP_FLOAT_C(0.0)) {
			counts[1]++;
		} else if (v.z < HYP_FLOAT_C(0.5)) {
			counts[2]++;
		} else {
			counts[3]++;
		}
		if (HYP_ABS(v.z) < HYP_FLOAT_C(0.5)) {
			small_z++;
		}
	}

	/* each quarter of the z range gets 25% +/- 3% */
	for (i = 0; i < 4; i++) {
		test_assert(counts[i] > 2200 && counts[i] < 2800);
	}

	/* and |z| < 0.5 half the time (50% +/- 3%), which an uneven spread of
	 * rotations (e.g. normalizing random components) clearly misses
	 */
	test_assert(small_z > 4700 && small_z < 5300);

	return NULL;
}
static const char *quaternion_all_tests(void)
{
	run_test(test_quaternion_set_random_unit_scripted);
	run_test(test_quaternion_set_random_unit_many);
	run_test(test_quaternion_identity);
	run_test(test_quaternion_conjugate);
	run_test(test_quaternion_inverse);
	run_test(test_quaternion_inverse_unit);
	run_test(test_quaternion_inverse_not_unit);
	run_test(test_quaternion_axis_anglev3);
	run_test(test_quaternion_multiply);
	run_test(test_quaternion_multiply_identity);
	run_test(test_vector3_rotate_by_quaternion_diagonal_third_turn);
	run_test(test_vector3_rotate_by_quaternion_xy_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_xz_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_yx_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_yz_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_zx_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_zy_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_xy_half_turn);
	run_test(test_vector3_rotate_by_quaternion_yx_half_turn);
	run_test(test_quaternion_get_set_axis_anglev3);
	run_test(test_quaternion_slerp);
	run_test(test_quaternion_get_eulers_create_quaternion_ZYX);
	run_test(test_quaternion_rotate_by_quaternion_identity);
	run_test(test_quaternion_get_eulers_from_axis_angle);
	run_test(test_quaternion_360_degree_eulers);
	run_test(test_quaternion_setf4);
	run_test(test_quaternion_add);
	run_test(test_quaternion_subtract);
	run_test(test_quaternion_negate);
	run_test(test_quaternion_multiplyf);
	run_test(test_quaternion_multiplyv3);
	run_test(test_quaternion_dot_product);
	run_test(test_quaternion_lerp);
	run_test(test_quaternion_nlerp);
	run_test(test_quaternion_get_rotation_tov3);
	run_test(test_quaternion_slerp_nearly_identical);
	run_test(test_quaternion_slerp_opposite);
	run_test(test_quaternion_slerp_shortest_arc);
	run_test(test_quaternion_slerp_at_endpoints);

	return NULL;
}
