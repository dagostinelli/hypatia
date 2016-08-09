static char *test_quaternion_get_eulers_create_quaternion_ZYX(void)
{
	quaternion q1, q2;
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


static char *test_quaternion_get_eulers_create_quaternion_ZYX_aa(void)
{
	quaternion q1, q2;
	HYP_FLOAT anglex, angley, anglez;

	/* making the original quaternion out of an arbitrary axis angle */
	quaternion_set_from_axis_anglef3(&q1, 0.4f, 0.232f, 0.543f, HYP_TAU / 1.45f);

	/* get the angles */
	quaternion_get_euler_anglesf3_ZYX_EXP(&q1, &anglex, &angley, &anglez);

	/* compose new quaternions with the eulers */
	quaternion_set_from_euler_anglesf3_ZYX_EXP(&q2, anglex, angley, anglez);

	/* same */
	test_assert(quaternion_equals(&q1, &q2));

	return 0;
}


/*
static char *test_matrix4_projection_perspective_fovy_rh_EXP()
{
	matrix4 projection;
	vector3 a;
	vector3 r;

	matrix4_identity(&projection);
	matrix4_projection_perspective_fovy_rh_EXP(&projection, 90.0f, 1.0, 0.0f, 20.0f);

	vector3_setf3(&a, -1.0f, 0.0f, 0.0f);
	vector3_multiplym4(&r, &projection);

	test_assert(r.x == -15);
	test_assert(r.y == -2);
	test_assert(r.z == 39);

	vector3_setf3(&a, 0.0f, 0.0f, 0.0f);
	vector3_multiplym4(&r, &projection);

	test_assert(r.x == -15);
	test_assert(r.y == -2);
	test_assert(r.z == 39);

	vector3_setf3(&a, 1.0f, 0.0f, 0.0f);
	vector3_multiplym4(&r, &projection);

	test_assert(r.x == -15);
	test_assert(r.y == -2);
	test_assert(r.z == 39);

	return 0;
}
*/


static char *experimental_all_tests()
{
	run_test(test_quaternion_get_eulers_create_quaternion_ZYX);
	run_test(test_quaternion_get_eulers_create_quaternion_ZYX_aa);

	/*run_test(test_matrix4_projection_perspective_fovy_rh_EXP());*/

	return 0;
}

