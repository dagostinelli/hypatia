
static char * test_matrix4_zero()
{
	matrix4 zero;
	int i;
	
	matrix4_zero(&zero);	

	for (i = 0; i < 16; i++)
	{
		test_assert(zero.m[i] == 0);
	}
	
	return 0;
}


static char * test_matrix4_equals()
{
	matrix4 m, identity;	
	matrix4_identity(&identity);	
	_matrix4_set_random(&m);
	
	/* equal */
	test_assert(matrix4_equals(&identity, &identity));

	/* not-equal */
	test_assert(matrix4_equals(&identity, &m) == 0);
	
	return 0;
}


static char * test_matrix4_multiplym4()
{
	matrix4 m1, m2, mR;
	
	m1.c00 = 1;  m1.c10 = 2;   m1.c20 = 3;  m1.c30 = 4;
	m1.c01 = 5;  m1.c11 = 6;   m1.c21 = 7;  m1.c31 = 8;
	m1.c02 = 9;  m1.c12 = 10;  m1.c22 = 11; m1.c32 = 12;
	m1.c03 = 13; m1.c13 = 14;  m1.c23 = 15; m1.c33 = 16;

	mR.c00 = 90;  mR.c10 = 100;   mR.c20 = 110;  mR.c30 = 120;
	mR.c01 = 202;  mR.c11 = 228;   mR.c21 = 254;  mR.c31 = 280;
	mR.c02 = 314;  mR.c12 = 356;  mR.c22 = 398; mR.c32 = 440;
	mR.c03 = 426; mR.c13 = 484;  mR.c23 = 542; mR.c33 = 600;

	/* copy m1 -> m2 */
	matrix4_set(&m2, &m1);
	
	matrix4_multiply(&m2, &m1);	
	test_assert(matrix4_equals(&mR, &m2));
	
	return 0;
}


static char * test_matrix4_identity_with_vector()
{
	/* vector * identity_matrix = vector */
	matrix4 m;
	vector3 startingPosition = {4.3f, 1.4f, 3.67f};
	vector3 expectedPosition = {4.3f, 1.4f, 3.67f};
	
	matrix4_identity(&m);

	vector3_multiplym4(&startingPosition, &m);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return 0;
}


static char * test_matrix4_transpose()
{
	matrix4 m = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	matrix4 e = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
	matrix4_transpose(&m);
	test_assert(matrix4_equals(&m, &e));
	return 0;
}


static char * test_matrix4_columnrowcolumn()
{
	matrix4 c;
	matrix4 r;
	matrix4 m;
	
	matrix4_zero(&c);
	matrix4_zero(&r);
	matrix4_zero(&m);
	
	_matrix4_set_random(&m);
		
	matrix4_set(&c, &m);
	matrix4_set(&r, &m);
	
	test_assert(matrix4_equals(&c, &r));
	test_assert(matrix4_equals(&c, &m));
	test_assert(matrix4_equals(&m, &r));

	/* transpose only c and r, but not m */
	_matrix4_transpose_rowcolumn(&r);
	_matrix4_transpose_columnrow(&c);
	
	test_assert(matrix4_equals(&c, &r));
	test_assert(matrix4_equals(&c, &m) == 0);
	test_assert(matrix4_equals(&m, &r) == 0);
	
	matrix4_transpose(&m);
	
	test_assert(matrix4_equals(&c, &m));
	test_assert(matrix4_equals(&m, &r));
	
	return 0;
}


static char * test_matrix4_transformation_translatev3()
{
	matrix4 transform;

	vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	vector3 translation = {0.1f, 0.1f, 0.1f};
	vector3 expectedPosition = {0.1f, 1.1f, 0.1f};

	matrix4_make_transformation_translationv3(&transform, &translation);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return 0;
}


static char * test_matrix4_transformation_translatev3_negative()
{
	matrix4 transform;

	vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	vector3 translation = {-0.1f, -0.1f, -0.1f};
	vector3 expectedPosition = {-0.1f, 0.9f, -0.1f};

	matrix4_make_transformation_translationv3(&transform, &translation);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return 0;
}


static char * test_matrix4_transformation_scalingv3()
{
	matrix4 transform;

	vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	vector3 scale = {0.1f, 0.1f, 0.1f};
	vector3 expectedPosition = {0.0f, 0.1f, 0.0f};

	matrix4_make_transformation_scalingv3(&transform, &scale);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return 0;
}

static char * test_vector3_rotate_by_matrix_xy_quarter_turn()
{
	matrix4 m;
	vector3 r;
	
	matrix4_make_transformation_rotationf_x(&m, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_Y);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return 0;
}


static char * test_vector3_rotate_by_matrix_yx_quarter_turn()
{
	matrix4 m;
	vector3 r;

	matrix4_make_transformation_rotationf_y(&m, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return 0;
}


static char * test_vector3_rotate_by_matrix_zx_quarter_turn()
{
	matrix4 m;
	vector3 r;

	matrix4_make_transformation_rotationf_z(&m, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return 0;
}

static char * test_matrix4_set_from_quaternion_xy_quarter_turn()
{
	matrix4 m;
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);
	matrix4_make_transformation_rotationq(&m, &q);
	vector3_set(&r, HYP_VECTOR3_UNIT_Y);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));
	
	return 0;
}


static char * test_matrix4_rotatev3_xz_quarter_turn()
{
	matrix4 m;
	vector3 r;
	
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));
	
	return 0;
}


static char * test_matrix4_rotatev3_xz_quarter_turn_opposite()
{
	matrix4 m;
	vector3 r;
	
	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Z, -(HYP_TAU / 4.0f));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));
	
	return 0;
}


static char * test_matrix4_rotatev3_xy_quarter_turn()
{
	matrix4 m;
	vector3 r;
	
	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));
	
	return 0;
}


static char * test_matrix4_rotatev3_xy_quarter_turn_opposite()
{
	matrix4 m;
	vector3 r;
	
	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Y, -(HYP_TAU / 4.0f));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));
	
	return 0;
}


/*static char * test_matrix4_set_from_axisf3_angle()
{
	matrix4 m1;
	matrix4 m2;	
	vector3 axis;
	HYP_FLOAT angle;
	quaternion q;
	
	matrix4_identity(&m1);
	matrix4_identity(&m2);
	_vector3_set_random(&axis);
	angle = HYP_RANDOM_FLOAT;
	
	matrix4_set_from_axisv3_angle(&m1, &axis, angle);

	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	matrix4_set_from_quaternion(&m2, &q);
	
	test_assert(matrix4_equals(&m1, &m2));
	
	_matrix4_print_with_columnrow_indexer(&m1);
	_matrix4_print_with_columnrow_indexer(&m2);
	
	return 0;	
}*/


static char * matrix4_all_tests()
{
	run_test(test_matrix4_zero);
	run_test(test_matrix4_equals);
	run_test(test_matrix4_multiplym4);
	run_test(test_matrix4_identity_with_vector);
	run_test(test_matrix4_columnrowcolumn);
	run_test(test_matrix4_transpose);
	run_test(test_matrix4_transformation_translatev3);
	run_test(test_matrix4_transformation_translatev3_negative);
	run_test(test_matrix4_transformation_scalingv3);
	run_test(test_vector3_rotate_by_matrix_xy_quarter_turn);
	run_test(test_vector3_rotate_by_matrix_yx_quarter_turn);
	run_test(test_vector3_rotate_by_matrix_zx_quarter_turn);
	run_test(test_matrix4_set_from_quaternion_xy_quarter_turn);
	
	run_test(test_matrix4_rotatev3_xz_quarter_turn);
	run_test(test_matrix4_rotatev3_xz_quarter_turn_opposite);
	run_test(test_matrix4_rotatev3_xy_quarter_turn);
	run_test(test_matrix4_rotatev3_xy_quarter_turn_opposite);
	
	/*run_test(test_matrix4_set_from_axisf3_angle);*/
	
	return 0;
}


