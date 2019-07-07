static char *test_quaternion_get_eulers_create_quaternion_ZYX(void)
{
	struct quaternion q1, q2;
	HYP_FLOAT anglex, angley, anglez;

	/* make a quaternion out of some arbitrary euler angles */
	quaternion_set_from_euler_anglesf3_ZYX_EXP(&q1, 0.8f, 0.8f, 0.8f);

	/* get the angles */
	quaternion_get_euler_anglesf3_ZYX_EXP(&q1, &anglex, &angley, &anglez);

	/* compose new quaternions with the eulers */
	quaternion_set_from_euler_anglesf3_ZYX_EXP(&q2, anglex, angley, anglez);

	/* same */
	test_assert(quaternion_equals(&q1, &q2));

	return 0;
}

static char *experimental_all_tests(void)
{
	run_test(test_quaternion_get_eulers_create_quaternion_ZYX);

	return 0;
}
