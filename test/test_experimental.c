
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
	/*run_test(test_matrix4_projection_perspective_fovy_rh_EXP());*/

	return 0;
}

