
static char * test_matrix3_zero()
{
	matrix3 zero;
	int i;
	
	matrix3_zero(&zero);	

	for (i = 0; i < 9; i++)
	{
		test_assert(zero.m[i] == 0);
	}
	
	return 0;
}


static char * test_matrix3_equals()
{
	matrix3 m, identity;	
	matrix3_identity(&identity);	
	_matrix3_set_random(&m);
	
	/* equal */
	test_assert(matrix3_equals(&identity, &identity));

	/* not-equal */
	test_assert(matrix3_equals(&identity, &m) == 0);
	
	return 0;
}


static char * test_matrix3_multiplym3()
{
	matrix3 m1, m2, mR;
	
	m1.c00 = 1;  m1.c10 = 2;   m1.c20 = 3;
	m1.c01 = 5;  m1.c11 = 6;   m1.c21 = 7;
	m1.c02 = 9;  m1.c12 = 10;  m1.c22 = 11;

	mR.c00 = 38;  mR.c10 = 44;   mR.c20 = 50;
	mR.c01 = 98;  mR.c11 = 116;   mR.c21 = 134;
	mR.c02 = 158;  mR.c12 = 188;  mR.c22 = 218;

	/* copy m1 -> m2 */
	matrix3_set(&m2, &m1);
	
	matrix3_multiply(&m2, &m1);	
	test_assert(matrix3_equals(&mR, &m2));
	
	return 0;
}


static char * test_matrix3_identity_with_vector()
{
	/* vector * identity_matrix = vector */
	matrix3 m;
	vector2 startingPosition = {4.3f, 1.4f};
	vector2 expectedPosition = {4.3f, 1.4f};
	
	matrix3_identity(&m);

	vector2_multiplym3(&startingPosition, &m);
	test_assert(vector2_equals(&startingPosition, &expectedPosition));

	return 0;
}


static char * test_matrix3_transpose()
{
	matrix3 m = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	matrix3 e = {0, 3, 6, 1, 4, 7, 2, 5, 8};
	matrix3_transpose(&m);
	test_assert(matrix3_equals(&m, &e));
	return 0;
}


static char * test_matrix3_columnrowcolumn()
{
	matrix3 c;
	matrix3 r;
	matrix3 m;
	
	matrix3_zero(&c);
	matrix3_zero(&r);
	matrix3_zero(&m);
	
	_matrix3_set_random(&m);
		
	matrix3_set(&c, &m);
	matrix3_set(&r, &m);
	
	test_assert(matrix3_equals(&c, &r));
	test_assert(matrix3_equals(&c, &m));
	test_assert(matrix3_equals(&m, &r));

	/* transpose only c and r, but not m */
	_matrix3_transpose_rowcolumn(&r);
	_matrix3_transpose_columnrow(&c);
	
	test_assert(matrix3_equals(&c, &r));
	test_assert(matrix3_equals(&c, &m) == 0);
	test_assert(matrix3_equals(&m, &r) == 0);
	
	matrix3_transpose(&m);
	
	test_assert(matrix3_equals(&c, &m));
	test_assert(matrix3_equals(&m, &r));
	
	return 0;
}


static char * test_matrix3_transformation_translatev2()
{
	matrix3 transform;

	vector2 startingPosition = {0.0f, 1.0f};
	vector2 translation = {0.1f, 0.1f};
	vector2 expectedPosition = {0.1f, 1.1f};

	matrix3_make_transformation_translationv2(&transform, &translation);
	vector2_multiplym3(&startingPosition, &transform);
	test_assert(vector2_equals(&startingPosition, &expectedPosition));

	return 0;
}


static char * test_matrix3_transformation_translatev2_negative()
{
	matrix3 transform;

	vector2 startingPosition = {0.0f, 1.0f};
	vector2 translation = {-0.1f, -0.1f};
	vector2 expectedPosition = {-0.1f, 0.9f};

	matrix3_make_transformation_translationv2(&transform, &translation);
	vector2_multiplym3(&startingPosition, &transform);
	test_assert(vector2_equals(&startingPosition, &expectedPosition));

	return 0;
}


static char * test_matrix3_transformation_scalingv2()
{
	matrix3 transform;

	vector2 startingPosition = {0.0f, 1.0f};
	vector2 scale = {0.1f, 0.1f};
	vector2 expectedPosition = {0.0f, 0.1f};

	matrix3_make_transformation_scalingv2(&transform, &scale);
	vector2_multiplym3(&startingPosition, &transform);
	test_assert(vector2_equals(&startingPosition, &expectedPosition));

	return 0;
}


static char * test_vector2_rotate_by_matrix_zx_quarter_turn()
{
	matrix3 m;
	vector2 r;

	matrix3_make_transformation_rotationf_z(&m, HYP_TAU / 4.0f);
	vector2_set(&r, HYP_VECTOR2_UNIT_X);
	vector2_multiplym3(&r, &m);
	test_assert(vector2_equals(&r, HYP_VECTOR2_UNIT_Y));

	return 0;
}


static char * test_matrix3_rotatev3_xz_quarter_turn()
{
	matrix3 m;
	vector2 r;
	
	vector2_set(&r, HYP_VECTOR2_UNIT_X);
	matrix3_identity(&m);
	matrix3_rotate(&m, HYP_TAU / 4.0f);
	vector2_multiplym3(&r, &m);
	test_assert(vector2_equals(&r, HYP_VECTOR2_UNIT_Y));
	
	return 0;
}


static char * test_matrix3_rotatev3_xz_quarter_turn_opposite()
{
	matrix3 m;
	vector2 r;
	
	vector2_set(&r, HYP_VECTOR2_UNIT_X);
	matrix3_identity(&m);
	matrix3_rotate(&m, -(HYP_TAU / 4.0f));
	vector2_multiplym3(&r, &m);
	test_assert(vector2_equals(&r, HYP_VECTOR2_UNIT_Y_NEGATIVE));
	
	return 0;
}


/*static char * test_matrix3_set_from_axisf2_angle()
{
	matrix3 m1;
	matrix3 m2;	
	vector3 axis;
	float angle;
	quaternion q;
	
	matrix3_identity(&m1);
	matrix3_identity(&m2);
	_vector3_set_random(&axis);
	angle = HYP_RANDOM_FLOAT;
	
	matrix3_set_from_axisv3_angle(&m1, &axis, angle);

	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	matrix3_set_from_quaternion(&m2, &q);
	
	test_assert(matrix3_equals(&m1, &m2));
	
	_matrix3_print_with_columnrow_indexer(&m1);
	_matrix3_print_with_columnrow_indexer(&m2);
	
	return 0;	
}*/


static char * matrix3_all_tests()
{
	run_test(test_matrix3_zero);
	run_test(test_matrix3_equals);
	run_test(test_matrix3_multiplym3);
	run_test(test_matrix3_identity_with_vector);
	run_test(test_matrix3_columnrowcolumn);
	run_test(test_matrix3_transpose);
	run_test(test_matrix3_transformation_translatev2);
	run_test(test_matrix3_transformation_translatev2_negative);
	run_test(test_matrix3_transformation_scalingv2);
	run_test(test_vector2_rotate_by_matrix_zx_quarter_turn);
	
	run_test(test_matrix3_rotatev3_xz_quarter_turn);
	run_test(test_matrix3_rotatev3_xz_quarter_turn_opposite);
	
	/*run_test(test_matrix3_set_from_axisf3_angle);*/
	
	return 0;
}


