/* SPDX-License-Identifier: MIT */

static char *test_matrix4_transformation_decompose_translation(void)
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

	return 0;
}

static char *test_matrix4_transformation_decompose_scaling(void)
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

	return 0;
}

static char *test_quaternion_get_eulers_from_axis_angle(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT in_anglex, in_angley, in_anglez;
	HYP_FLOAT out_anglex, out_angley, out_anglez;

	/* making the original quaternion out of an arbitrary axis angle */
	quaternion_set_from_axis_anglef3(&q1, 0.4f, 0.232f, 0.543f, HYP_TAU / 1.45f);

	/* get the angles */
	quaternion_get_euler_anglesf3_ZYX_EXP(&q1, &in_anglex, &in_angley, &in_anglez);

	/* compose new quaternions with the eulers */
	quaternion_set_from_euler_anglesf3_ZYX_EXP(&q2, in_anglex, in_angley, in_anglez);

	/* get the angles */
	quaternion_get_euler_anglesf3_ZYX_EXP(&q2, &out_anglex, &out_angley, &out_anglez);

	/*printf("%lf, %lf, %lf\n", in_anglex, in_angley, in_anglez);
	printf("%lf, %lf, %lf\n", out_anglex, out_angley, out_anglez);*/

	/* test */
	test_assert(scalar_equals(in_anglex, out_anglex));
	test_assert(scalar_equals(in_angley, out_angley));
	test_assert(scalar_equals(in_anglez, out_anglez));

	/*_quaternion_print(&q1);
	_quaternion_print(&q2);*/

	/* same */
	/*test_assert(quaternion_equals(&q1, &q2));*/

	return 0;
}

static char *experimental_all_tests(void)
{
	run_test(test_matrix4_transformation_decompose_translation);
	run_test(test_matrix4_transformation_decompose_scaling);
	run_test(test_quaternion_get_eulers_from_axis_angle);

	return 0;
}
