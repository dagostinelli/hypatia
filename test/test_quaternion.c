/* SPDX-License-Identifier: MIT */

/** [quaternion identity example] */
static char *test_quaternion_identity(void)
{
	struct quaternion q;

	quaternion_identity(&q);
	test_assert(scalar_equalsf(q.x, 0.0f));
	test_assert(scalar_equalsf(q.y, 0.0f));
	test_assert(scalar_equalsf(q.z, 0.0f));
	test_assert(scalar_equalsf(q.w, 1.0f));
	test_assert(scalar_equalsf(1.0f, quaternion_norm(&q)));
	test_assert(scalar_equalsf(1.0f, quaternion_magnitude(&q)));
	test_assert(quaternion_is_unit(&q));
	test_assert(!quaternion_is_pure(&q));

	return NULL;
}
/** [quaternion identity example] */


/** [quaternion conjugate example] */
static char *test_quaternion_conjugate(void)
{
	struct quaternion qA;
	struct quaternion qB;

	quaternion_set_from_axis_anglef3(&qA, 1.0f, 1.0f, 1.0f, HYP_TAU / 4.0f);
	quaternion_set_from_axis_anglef3(&qB, -1.0f, -1.0f, -1.0f, HYP_TAU / 4.0f);
	quaternion_conjugate(&qB);
	test_assert(quaternion_equals(&qA, &qB));

	return NULL;
}
/** [quaternion conjugate example] */


/** [quaternion inverse example] */
static char *test_quaternion_inverse(void)
{
	struct quaternion qA;
	struct quaternion qInverse;
	struct quaternion qIdentity;

	quaternion_set_from_axis_anglef3(&qA, 1.0f, 1.0f, 1.0f, HYP_TAU / 4.0f);
	quaternion_set(&qInverse, &qA);
	quaternion_inverse(&qInverse);
	quaternion_multiply(&qA, &qInverse);
	quaternion_normalize(&qA);
	quaternion_identity(&qIdentity);
	test_assert(quaternion_equals(&qA, &qIdentity));

	return NULL;
}
/** [quaternion inverse example] */


static char *test_quaternion_axis_anglev3(void)
{
	struct quaternion q, q1;
	HYP_FLOAT c;
	HYP_FLOAT s;
	HYP_FLOAT angle = HYP_TAU / 4.0f;
	struct vector3 axis;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, angle);

	vector3_set(&axis, HYP_VECTOR3_UNIT_X);

	c = HYP_COS(angle / 2.0f);
	s = HYP_SIN(angle / 2.0f);

	q1.x = axis.x * s;
	q1.y = axis.y * s;
	q1.z = axis.z * s;
	q1.w = c;

	quaternion_normalize(&q1);

	test_assert(quaternion_equals(&q, &q1));

	return NULL;
}


static char *test_quaternion_get_set_axis_anglev3(void)
{
	struct quaternion q;
	HYP_FLOAT angle, angle1;
	struct vector3 axis, axis1;

	vector3_set(&axis, HYP_VECTOR3_UNIT_X);
	angle = HYP_TAU / 4.0f;

	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	quaternion_get_axis_anglev3(&q, &axis1, &angle1);

	test_assert(vector3_equals(&axis, &axis1));
	test_assert(scalar_equalsf(angle, angle1));

	return NULL;
}


static char *test_quaternion_multiply(void)
{
	struct quaternion qA, qB;

	quaternion_set_from_axis_anglev3(&qA, HYP_VECTOR3_UNIT_X, HYP_TAU / 8.0f);
	quaternion_set_from_axis_anglev3(&qB, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);

	/* qA squared */
	quaternion_multiply(&qA, &qA);

	test_assert(quaternion_equals(&qA, &qB));

	return NULL;
}


static char *test_quaternion_multiply_identity(void)
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


static char *test_vector3_rotate_by_quaternion_yx_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return NULL;
}


static char *test_vector3_rotate_by_quaternion_zx_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static char *test_vector3_rotate_by_quaternion_xy_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return NULL;
}


static char *test_vector3_rotate_by_quaternion_zy_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X));

	return NULL;
}


static char *test_vector3_rotate_by_quaternion_xz_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


static char *test_vector3_rotate_by_quaternion_yz_quarter_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X_NEGATIVE));

	return NULL;
}


static char *test_vector3_rotate_by_quaternion_yx_half_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 2.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static char *test_vector3_rotate_by_quaternion_xy_half_turn(void)
{
	struct quaternion q;
	struct vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / 2.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X_NEGATIVE));

	return NULL;
}


static char *test_quaternion_slerp(void)
{
	struct quaternion q, q1, q2, q3;
	HYP_FLOAT angle;

	angle = HYP_TAU / 4.0f;

	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_X, angle);
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_X, angle * 1.1f);
	quaternion_set_from_axis_anglev3(&q3, HYP_VECTOR3_UNIT_X, angle * 1.2f);

	/* half-way */
	quaternion_slerp(&q1, &q3, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* none */
	quaternion_slerp(&q1, &q3, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q1));

	/* all the way */
	quaternion_slerp(&q1, &q3, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q3));


	/* swap order half-way */
	quaternion_slerp(&q3, &q1, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* swap order none */
	quaternion_slerp(&q3, &q1, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q3));

	/* swap order all the way */
	quaternion_slerp(&q3, &q1, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q1));


	/* go reverse around the sphere */
	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_X, angle);
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_X, angle * 0.9f);
	quaternion_set_from_axis_anglev3(&q3, HYP_VECTOR3_UNIT_X, angle * 0.8f);

	/* go reverse half-way */
	quaternion_slerp(&q1, &q3, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* go reverse none */
	quaternion_slerp(&q1, &q3, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q1));

	/* go reverse all the way */
	quaternion_slerp(&q1, &q3, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q3));


	/* swap order reverse half-way */
	quaternion_slerp(&q3, &q1, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* swap order reverse none */
	quaternion_slerp(&q3, &q1, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q3));

	/* swap order reverse all the way */
	quaternion_slerp(&q3, &q1, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q1));

	return NULL;
}


static char *test_quaternion_get_eulers_create_quaternion_ZYX(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT in_anglex, in_angley, in_anglez;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	in_anglex = 0.8f;
	in_angley = 0.7f;
	in_anglez = 0.432f;

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


static char *test_quaternion_rotate_by_quaternion_identity(void)
{
	struct quaternion scratchQuaternion;
	struct quaternion q1;
	HYP_FLOAT in_anglex, in_angley, in_anglez;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	in_anglex = 0.8f;
	in_angley = 0.8f;
	in_anglez = 0.8f;

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


static char *test_quaternion_get_eulers_from_axis_angle(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT in_anglex, in_angley, in_anglez;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	/* making the original quaternion out of an arbitrary axis angle */
	quaternion_set_from_axis_anglef3(&q1, 0.4f, 0.232f, 0.543f, HYP_TAU * 0.45f);

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


static char *test_quaternion_360_degree_eulers(void)
{
	struct quaternion q1;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	/* set the original quaternions with the eulers */
	quaternion_set_from_euler_anglesf3(&q1, 0.0f, HYP_DEG_TO_RAD(365.0f), 0.0f);

	/* get the angles */
	quaternion_get_euler_anglesf3(&q1, &out_anglex, &out_angley, &out_anglez);

	test_assert(scalar_equals(0.0f, out_anglex));

	/* should be 5 degrees, quaternion will normalize the value */
	test_assert(scalar_equals(5.0f, HYP_RAD_TO_DEG(out_angley)));

	test_assert(scalar_equals(0.0f, out_anglez));

	return NULL;
}


static char *test_quaternion_setf4(void)
{
	struct quaternion q;

	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	test_assert(scalar_equalsf(q.x, 1.0f));
	test_assert(scalar_equalsf(q.y, 2.0f));
	test_assert(scalar_equalsf(q.z, 3.0f));
	test_assert(scalar_equalsf(q.w, 4.0f));

	quaternion_setf4(&q, -0.5f, 0.0f, 0.5f, 1.0f);
	test_assert(scalar_equalsf(q.x, -0.5f));
	test_assert(scalar_equalsf(q.y, 0.0f));
	test_assert(scalar_equalsf(q.z, 0.5f));
	test_assert(scalar_equalsf(q.w, 1.0f));

	return NULL;
}


static char *test_quaternion_add(void)
{
	struct quaternion q, qT, qExpected;

	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	quaternion_setf4(&qT, 0.5f, 1.5f, 2.5f, 3.5f);
	quaternion_add(&q, &qT);

	quaternion_setf4(&qExpected, 1.5f, 3.5f, 5.5f, 7.5f);
	test_assert(quaternion_equals(&q, &qExpected));

	/* adding zero quaternion should not change anything */
	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	quaternion_setf4(&qT, 0.0f, 0.0f, 0.0f, 0.0f);
	quaternion_add(&q, &qT);
	quaternion_setf4(&qExpected, 1.0f, 2.0f, 3.0f, 4.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static char *test_quaternion_subtract(void)
{
	struct quaternion q, qT, qExpected;

	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	quaternion_setf4(&qT, 0.5f, 1.0f, 1.5f, 2.0f);
	quaternion_subtract(&q, &qT);

	quaternion_setf4(&qExpected, 0.5f, 1.0f, 1.5f, 2.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	/* subtracting from itself should yield zero */
	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	quaternion_subtract(&q, &q);
	quaternion_setf4(&qExpected, 0.0f, 0.0f, 0.0f, 0.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static char *test_quaternion_negate(void)
{
	struct quaternion q, qExpected;

	quaternion_setf4(&q, 1.0f, -2.0f, 3.0f, -4.0f);
	quaternion_negate(&q);
	quaternion_setf4(&qExpected, -1.0f, 2.0f, -3.0f, 4.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	/* double negate should return to original */
	quaternion_setf4(&q, 0.5f, 0.5f, 0.5f, 0.5f);
	quaternion_negate(&q);
	quaternion_negate(&q);
	quaternion_setf4(&qExpected, 0.5f, 0.5f, 0.5f, 0.5f);
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static char *test_quaternion_multiplyf(void)
{
	struct quaternion q, qExpected;

	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	quaternion_multiplyf(&q, 2.0f);
	quaternion_setf4(&qExpected, 2.0f, 4.0f, 6.0f, 8.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	/* multiply by zero */
	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	quaternion_multiplyf(&q, 0.0f);
	quaternion_setf4(&qExpected, 0.0f, 0.0f, 0.0f, 0.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	/* multiply by one (no change) */
	quaternion_setf4(&q, 1.0f, 2.0f, 3.0f, 4.0f);
	quaternion_multiplyf(&q, 1.0f);
	quaternion_setf4(&qExpected, 1.0f, 2.0f, 3.0f, 4.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static char *test_quaternion_multiplyv3(void)
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
	quaternion_setf4(&qExpected, 1.0f, 0.0f, 0.0f, 0.0f);
	test_assert(quaternion_equals(&q, &qExpected));

	return NULL;
}


static char *test_quaternion_dot_product(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT dot;

	/* dot product with itself */
	quaternion_setf4(&q1, 1.0f, 2.0f, 3.0f, 4.0f);
	dot = quaternion_dot_product(&q1, &q1);
	/* 1+4+9+16 = 30 */
	test_assert(scalar_equalsf(dot, 30.0f));

	/* dot product of orthogonal-ish quaternions */
	quaternion_setf4(&q1, 1.0f, 0.0f, 0.0f, 0.0f);
	quaternion_setf4(&q2, 0.0f, 1.0f, 0.0f, 0.0f);
	dot = quaternion_dot_product(&q1, &q2);
	test_assert(scalar_equalsf(dot, 0.0f));

	/* dot product of identity with itself should be 1 */
	quaternion_identity(&q1);
	dot = quaternion_dot_product(&q1, &q1);
	test_assert(scalar_equalsf(dot, 1.0f));

	return NULL;
}


static char *test_quaternion_lerp(void)
{
	struct quaternion q1, q2, qR, qExpected;

	quaternion_setf4(&q1, 0.0f, 0.0f, 0.0f, 1.0f);
	quaternion_setf4(&q2, 1.0f, 0.0f, 0.0f, 0.0f);

	/* at t=0, should return start */
	quaternion_lerp(&q1, &q2, 0.0f, &qR);
	test_assert(quaternion_equals(&qR, &q1));

	/* at t=1, should return end */
	quaternion_lerp(&q1, &q2, 1.0f, &qR);
	test_assert(quaternion_equals(&qR, &q2));

	/* at t=0.5, should be midpoint */
	quaternion_lerp(&q1, &q2, 0.5f, &qR);
	quaternion_setf4(&qExpected, 0.5f, 0.0f, 0.0f, 0.5f);
	test_assert(quaternion_equals(&qR, &qExpected));

	return NULL;
}


static char *test_quaternion_nlerp(void)
{
	struct quaternion q1, q2, qR;

	quaternion_setf4(&q1, 0.0f, 0.0f, 0.0f, 1.0f);
	quaternion_setf4(&q2, 1.0f, 0.0f, 0.0f, 0.0f);

	/* at t=0, should return start */
	quaternion_nlerp(&q1, &q2, 0.0f, &qR);
	test_assert(quaternion_equals(&qR, &q1));

	/* at t=1, should return end */
	quaternion_nlerp(&q1, &q2, 1.0f, &qR);
	test_assert(quaternion_equals(&qR, &q2));

	/* at t=0.5, should be normalized midpoint */
	quaternion_nlerp(&q1, &q2, 0.5f, &qR);
	/* lerp gives (0.5, 0, 0, 0.5), normalized magnitude = sqrt(0.5) */
	test_assert(scalar_equalsf(1.0f, quaternion_norm(&qR)));

	return NULL;
}


static char *test_quaternion_get_rotation_tov3(void)
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


static char *quaternion_all_tests(void)
{
	run_test(test_quaternion_identity);
	run_test(test_quaternion_conjugate);
	run_test(test_quaternion_inverse);
	run_test(test_quaternion_axis_anglev3);
	run_test(test_quaternion_multiply);
	run_test(test_quaternion_multiply_identity);
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

	return NULL;
}
