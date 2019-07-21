

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

static char *experimental_all_tests(void)
{
	run_test(test_matrix4_transformation_decompose_translation);
	run_test(test_matrix4_transformation_decompose_scaling);

	return 0;
}
