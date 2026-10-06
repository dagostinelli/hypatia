/* SPDX-License-Identifier: MIT */

static const char *test_matrix4_transformation_decompose_translation(void)
{
	struct vector3 in_translation;
	struct vector3 out_translation;
	struct vector3 out_scale;
	struct quaternion out_rotation;
	struct matrix4 t;

	/* make a translation matrix */
	in_translation.x = 1.0;
	in_translation.y = 2.0;
	in_translation.z = 3.0;
	matrix4_make_transformation_translationv3(&t, &in_translation);

	/* decompose */
	matrix4_transformation_decompose_EXP(&t, &out_scale, &out_rotation, &out_translation);

	/* same */
	test_assert(vector3_equals(&in_translation, &out_translation));

	return NULL;
}

static const char *test_matrix4_transformation_decompose_scaling(void)
{
	struct vector3 in_scale;
	struct vector3 out_translation;
	struct vector3 out_scale;
	struct quaternion out_rotation;
	struct matrix4 t;

	/* make a scaling matrix */
	in_scale.x = 1.0;
	in_scale.y = 2.0;
	in_scale.z = 3.0;
	matrix4_make_transformation_scalingv3(&t, &in_scale);

	/* decompose */
	matrix4_transformation_decompose_EXP(&t, &out_scale, &out_rotation, &out_translation);

	/* same */
	test_assert(vector3_equals(&in_scale, &out_scale));

	return NULL;
}

static const char *test_matrix4_set_from_quaternion_quarter_turn_z(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	/* right hand rule: a quarter turn about Z takes X to Y */
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	matrix4_set_from_quaternion_EXP(&m, &q);
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_X), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


/* the matrix must rotate v the same way as the quaternion, and match
 * matrix4_set_from_axisv3_angle_EXP for the same axis and angle
 */
static int set_from_quaternion_matches_rotation(HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az, HYP_FLOAT angle)
{
	struct matrix4 m;
	struct matrix4 expected;
	struct quaternion q;
	struct vector3 axis;
	struct vector3 v;
	struct vector3 r;

	vector3_normalize(vector3_setf3(&axis, ax, ay, az));
	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	matrix4_set_from_quaternion_EXP(&m, &q);
	matrix4_set_from_axisv3_angle_EXP(&expected, &axis, angle);

	vector3_setf3(&v, HYP_FLOAT_C(0.3), -HYP_FLOAT_C(1.2), HYP_FLOAT_C(2.5));
	vector3_set(&r, &v);
	vector3_multiplym4(&r, &m);
	vector3_rotate_by_quaternion(&v, &q);

	return vector3_equals(&r, &v) && matrix4_equals(&m, &expected);
}


static const char *test_matrix4_set_from_quaternion_matches_rotation(void)
{
	test_assert(set_from_quaternion_matches_rotation(HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.7)));
	test_assert(set_from_quaternion_matches_rotation(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(2.0)));
	test_assert(set_from_quaternion_matches_rotation(HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.1)));
	test_assert(set_from_quaternion_matches_rotation(-HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0)));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_not_unit(void)
{
	struct matrix4 m;
	struct matrix4 expected;
	struct quaternion q;
	struct quaternion scaled;

	/* only the direction of the quaternion is used */
	quaternion_set_from_axis_anglef3(&q, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.6), HYP_FLOAT_C(0.8), HYP_FLOAT_C(1.3));
	matrix4_set_from_quaternion_EXP(&expected, &q);

	quaternion_setf4(&scaled, HYP_FLOAT_C(3.0) * q.x, HYP_FLOAT_C(3.0) * q.y, HYP_FLOAT_C(3.0) * q.z, HYP_FLOAT_C(3.0) * q.w);
	test_assert(matrix4_equals(matrix4_set_from_quaternion_EXP(&m, &scaled), &expected));

	quaternion_setf4(&scaled, HYP_FLOAT_C(0.001) * q.x, HYP_FLOAT_C(0.001) * q.y, HYP_FLOAT_C(0.001) * q.z, HYP_FLOAT_C(0.001) * q.w);
	test_assert(matrix4_equals(matrix4_set_from_quaternion_EXP(&m, &scaled), &expected));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_zero(void)
{
	struct matrix4 m;
	struct matrix4 identity;
	struct quaternion q;

	/* a zero quaternion describes no rotation */
	quaternion_setf4(&q, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	matrix4_identity(&identity);
	test_assert(matrix4_equals(matrix4_set_from_quaternion_EXP(&m, &q), &identity));

	return NULL;
}


static const char *experimental_all_tests(void)
{
	run_test(test_matrix4_set_from_quaternion_quarter_turn_z);
	run_test(test_matrix4_set_from_quaternion_matches_rotation);
	run_test(test_matrix4_set_from_quaternion_not_unit);
	run_test(test_matrix4_set_from_quaternion_zero);
	run_test(test_matrix4_transformation_decompose_translation);
	run_test(test_matrix4_transformation_decompose_scaling);

	return NULL;
}
