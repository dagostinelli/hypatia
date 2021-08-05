/* SPDX-License-Identifier: MIT */

static char *test_matrix4_zero(void)
{
	struct matrix4 zero;
	uint8_t i;

	matrix4_zero(&zero);

	for (i = 0; i < 16; i++) {
		test_assert(scalar_equalsf(zero.m[i], 0.0f));
	}

	return NULL;
}


static char *test_matrix4_equals(void)
{
	struct matrix4 m, identity;

	matrix4_identity(&identity);
	_matrix4_set_random(&m);

	/* equal */
	test_assert(matrix4_equals(&identity, &identity));

	/* not-equal */
	test_assert(matrix4_equals(&identity, &m) == 0);

	return NULL;
}


static char *test_matrix4_multiplym4(void)
{
	struct matrix4 m1, m2, mR;

	m1.c00 = 1;  m1.c10 = 2;   m1.c20 = 3;  m1.c30 = 4;
	m1.c01 = 5;  m1.c11 = 6;   m1.c21 = 7;  m1.c31 = 8;
	m1.c02 = 9;  m1.c12 = 10;  m1.c22 = 11; m1.c32 = 12;
	m1.c03 = 13; m1.c13 = 14;  m1.c23 = 15; m1.c33 = 16;

	mR.c00 = 90;   mR.c10 = 100;  mR.c20 = 110; mR.c30 = 120;
	mR.c01 = 202;  mR.c11 = 228;  mR.c21 = 254; mR.c31 = 280;
	mR.c02 = 314;  mR.c12 = 356;  mR.c22 = 398; mR.c32 = 440;
	mR.c03 = 426;  mR.c13 = 484;  mR.c23 = 542; mR.c33 = 600;

	/* copy m1 -> m2 */
	matrix4_set(&m2, &m1);

	matrix4_multiply(&m2, &m1);
	test_assert(matrix4_equals(&mR, &m2));

	return NULL;
}


static char *test_matrix4_identity_with_vector2(void)
{
	/* vector * identity_matrix = vector */
	struct matrix4 m;
	struct vector2 startingPosition = {4.3f, 1.4f};
	struct vector2 expectedPosition = {4.3f, 1.4f};
	struct vector2 r;

	matrix4_identity(&m);

	matrix4_multiplyv2(&m, &startingPosition, &r);
	test_assert(vector2_equals(&r, &expectedPosition));

	return NULL;
}


static char *test_matrix4_transformation_translatev3_with_vector2(void)
{
	struct matrix4 m;
	struct vector2 r;

	struct vector2 startingPosition = {4.3f, 7.4f};
	struct vector3 translation = {0.1f, 0.1f, 0.0f};
	struct vector2 expectedPosition = {4.4f, 7.5f};

	matrix4_make_transformation_translationv3(&m, &translation);
	matrix4_multiplyv2(&m, &startingPosition, &r);
	test_assert(vector2_equals(&r, &expectedPosition));

	return NULL;
}

static char *test_matrix4_transformation_translatev3_with_vector2_2(void)
{
	struct matrix4 m;
	struct vector2 r;

	struct vector2 startingPosition = {4.3f, 7.4f};
	struct vector3 translation = {0.1f, 0.1f, 1.0f};
	struct vector2 expectedPosition = {4.4f, 7.5f};

	matrix4_make_transformation_translationv3(&m, &translation);
	matrix4_multiplyv2(&m, &startingPosition, &r);
	test_assert(vector2_equals(&r, &expectedPosition));

	return NULL;
}


static char *test_matrix4_identity_with_vector3(void)
{
	/* vector * identity_matrix = vector */
	struct matrix4 m;
	struct vector3 startingPosition = {4.3f, 1.4f, 3.67f};
	struct vector3 expectedPosition = {4.3f, 1.4f, 3.67f};
	struct vector3 r;

	matrix4_identity(&m);

	matrix4_multiplyv3(&m, &startingPosition, &r);
	test_assert(vector3_equals(&r, &expectedPosition));

	return NULL;
}


static char *test_matrix4_identity_with_vector4(void)
{
	/* vector * identity_matrix = vector */
	struct matrix4 m;
	struct vector4 startingPosition = {4.3f, 1.4f, 3.67f, 2.4f};
	struct vector4 expectedPosition = {4.3f, 1.4f, 3.67f, 2.4f};
	struct vector4 r;

	matrix4_identity(&m);

	matrix4_multiplyv4(&m, &startingPosition, &r);
	test_assert(vector4_equals(&r, &expectedPosition));

	return NULL;
}


static char *test_matrix4_transpose(void)
{
	struct matrix4 m = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	struct matrix4 e = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

	matrix4_transpose(&m);
	test_assert(matrix4_equals(&m, &e));

	return NULL;
}


static char *test_matrix4_determinant_trial1(void)
{
	struct matrix4 m = {4, 3, 2, 2, 0, 1, -3, 3, 0, -1, 3, 3, 0, 3, 1, 1};

	test_assert(scalar_equals(matrix4_determinant(&m), -240));
	return NULL;
}


static char *test_matrix4_determinant_trial2(void)
{
	struct matrix4 m = {-1, 1, 4, 2, 2, -1, 2, 5, 1, 2, 3, 4, 3, 4, -1, 2};

	test_assert(scalar_equals(matrix4_determinant(&m), -26));
	return NULL;
}


static char *test_matrix4_determinant_trial3(void)
{
	struct matrix4 m = {1, 3, -2, 1, 5, 1, 0, -1, 0, 1, 0, -2, 2, -1, 0, 3};

	test_assert(scalar_equals(matrix4_determinant(&m), -6));
	return NULL;
}


static char *test_matrix4_columnrowcolumn(void)
{
	struct matrix4 c;
	struct matrix4 r;
	struct matrix4 m;

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

	return NULL;
}


static char *test_matrix4_transformation_translatev3(void)
{
	struct matrix4 transform;

	struct vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	struct vector3 translation = {0.1f, 0.1f, 0.1f};
	struct vector3 expectedPosition = {0.1f, 1.1f, 0.1f};

	matrix4_make_transformation_translationv3(&transform, &translation);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_matrix4_transformation_translatev3_negative(void)
{
	struct matrix4 transform;

	struct vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	struct vector3 translation = {-0.1f, -0.1f, -0.1f};
	struct vector3 expectedPosition = {-0.1f, 0.9f, -0.1f};

	matrix4_make_transformation_translationv3(&transform, &translation);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_matrix4_transformation_scalingv3(void)
{
	struct matrix4 transform;

	struct vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	struct vector3 scale = {0.1f, 0.1f, 0.1f};
	struct vector3 expectedPosition = {0.0f, 0.1f, 0.0f};

	matrix4_make_transformation_scalingv3(&transform, &scale);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_matrix4_transformation_scale_then_translatev3(void)
{
	struct matrix4 transform;
	struct matrix4 scratch;

	struct vector3 startingPosition = {1.0f, 1.0f, 1.0f};
	struct vector3 scale = {0.1f, 0.1f, 0.1f};
	struct vector3 translation = {-0.1f, -0.1f, -0.1f};
	struct vector3 expectedPosition = {0.0f, 0.0f, -0.0f};

	matrix4_identity(&transform);
	matrix4_multiply(&transform, matrix4_make_transformation_scalingv3(&scratch, &scale));
	matrix4_multiply(&transform, matrix4_make_transformation_translationv3(&scratch, &translation));
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_vector3_rotate_by_matrix_xy_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_make_transformation_rotationf_x(&m, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_Y);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));


	return NULL;
}


static char *test_vector3_rotate_by_matrix_yx_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_make_transformation_rotationf_y(&m, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return NULL;
}


static char *test_vector3_rotate_by_matrix_zx_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_make_transformation_rotationf_z(&m, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static char *test_matrix4_rotatev3_xz_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static char *test_matrix4_rotatev3_xz_quarter_turn_opposite(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Z, -(HYP_TAU / 4.0f));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


static char *test_matrix4_rotatev3_xy_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f);
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return NULL;
}


static char *test_matrix4_rotatev3_xy_quarter_turn_opposite(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Y, -(HYP_TAU / 4.0f));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return NULL;
}


static char *test_matrix4_set_from_quaternion_xy_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return NULL;
}


static char *test_matrix4_set_from_quaternion_xz_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


static char *test_matrix4_set_from_quaternion_yx_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_X), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return NULL;
}


static char *test_matrix4_set_from_quaternion_yz_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X_NEGATIVE));

	return NULL;
}


static char *test_matrix4_set_from_quaternion_zx_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_X), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static char *test_matrix4_set_from_quaternion_zy_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X));

	return NULL;
}


static char *test_matrix4_set_from_quaternion_xy_half_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 2.0f));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static char *test_matrix4_match_transformation_matrix_quaternion(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 vM, vQ;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&vQ, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&vQ, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	matrix4_make_transformation_rotationf_z(&m, HYP_TAU / 4.0f);
	vector3_set(&vM, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&vM, &m);
	test_assert(vector3_equals(&vM, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	test_assert(vector3_equals(&vQ, &vM));

	return NULL;
}


static char *test_matrix4_transform_3d(void)
{
	struct quaternion orientation;
	struct matrix4 modelMatrix, worldMatrix, scaleM, rotateM, translateM;

	struct vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	struct vector3 scale = {0.1f, 0.1f, 0.1f};
	struct vector3 expectedPosition = {1.1f, 0.0f, 0.0f};
	struct vector3 translation = {0.0f, 1.0f, 0.0f};

	/* modelMatrix */
	matrix4_identity(&modelMatrix);
	matrix4_multiply(&modelMatrix, matrix4_make_transformation_scalingv3(&scaleM, &scale));

	/* world transformation */
	matrix4_identity(&worldMatrix);
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_translationv3(&translateM, &translation));
	matrix4_multiply(&worldMatrix,
			 matrix4_make_transformation_rotationq(&rotateM,
							       quaternion_set_from_axis_anglev3(&orientation, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f)));

	/* Read this right to left */
	/* vT = worldMatrix * modelMatrix * vT */
	vector3_multiplym4(&startingPosition, &modelMatrix);
	vector3_multiplym4(&startingPosition, &worldMatrix);

	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_matrix4_transform_3d_combined(void)
{
	struct quaternion orientation;
	struct matrix4 worldMatrix, scaleM, rotateM, translateM;

	struct vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	struct vector3 scale = {0.1f, 0.1f, 0.1f};
	struct vector3 expectedPosition = {1.1f, 0.0f, 0.0f};
	struct vector3 translation = {0.0f, 1.0f, 0.0f};

	matrix4_identity(&worldMatrix);
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_scalingv3(&scaleM, &scale));
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_translationv3(&translateM, &translation));
	matrix4_multiply(&worldMatrix,
			 matrix4_make_transformation_rotationq(&rotateM,
							       quaternion_set_from_axis_anglev3(&orientation, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f)));

	vector3_multiplym4(&startingPosition, &worldMatrix);

	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_matrix4_transform_3d_scale_translate(void)
{
	struct matrix4 worldMatrix, scaleM, translateM;

	struct vector3 startingPosition = {0.0f, 1.0f, 0.0f};
	struct vector3 scale = {0.1f, 0.1f, 0.1f};
	struct vector3 expectedPosition = {0.0f, 1.1f, 0.0f};
	struct vector3 translation = {0.0f, 1.0f, 0.0f};

	matrix4_identity(&worldMatrix);
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_scalingv3(&scaleM, &scale));
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_translationv3(&translateM, &translation));

	vector3_multiplym4(&startingPosition, &worldMatrix);

	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_matrix4_determinant_row_is_zero(void)
{
	struct matrix4 m;
	HYP_FLOAT det;

	/* identity is not zero */
	matrix4_identity(&m);
	det = matrix4_determinant(&m);
	test_assert(!scalar_equals(det, 0));
	test_assert(scalar_equals(det, 1));

	/* when any row is zero, the determinant is zero */
	matrix4_identity(&m);
	m.r00 = 0.0; m.r01 = 0.0; m.r02 = 0.0; m.r03 = 0.0;
	det = matrix4_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix4_identity(&m);
	m.r10 = 0.0; m.r11 = 0.0; m.r12 = 0.0; m.r13 = 0.0;
	det = matrix4_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix4_identity(&m);
	m.r20 = 0.0; m.r21 = 0.0; m.r22 = 0.0; m.r23 = 0.0;
	det = matrix4_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix4_identity(&m);
	m.r30 = 0.0; m.r31 = 0.0; m.r32 = 0.0; m.r33 = 0.0;
	det = matrix4_determinant(&m);
	test_assert(scalar_equals(det, 0));

	return NULL;
}


static char *test_matrix4_inverse(void)
{
	struct matrix4 originalMatrix;
	struct matrix4 identity;
	struct matrix4 inverted;
	struct vector3 scratchVector;
	struct matrix4 scratchMatrix;
	void *hasInverse = NULL;

	matrix4_identity(&identity);

	matrix4_identity(&originalMatrix);
	matrix4_multiply(&originalMatrix, matrix4_make_transformation_scalingv3(&scratchMatrix, vector3_setf3(&scratchVector, 0.5f, 0.5f, 0.5f)));
	matrix4_multiply(&originalMatrix, matrix4_make_transformation_translationv3(&scratchMatrix, vector3_setf3(&scratchVector, 1.0f, 0.8f, 0.3f)));
	matrix4_multiply(&originalMatrix, matrix4_set_from_euler_anglesf3_EXP(&scratchMatrix, HYP_TAU / 4.0f, HYP_TAU / 4.0f, HYP_TAU / 4.0f));

	hasInverse = matrix4_invert(matrix4_set(&inverted, &originalMatrix));

	test_assert(hasInverse);

	matrix4_identity(&scratchMatrix);
	matrix4_multiply(&scratchMatrix, &inverted);
	matrix4_multiply(&scratchMatrix, &originalMatrix);

	test_assert(matrix4_equals(&identity, &scratchMatrix));

	return NULL;
}


static char *matrix4_all_tests(void)
{
	run_test(test_matrix4_zero);
	run_test(test_matrix4_equals);
	run_test(test_matrix4_multiplym4);
	run_test(test_matrix4_columnrowcolumn);
	run_test(test_matrix4_transpose);
	run_test(test_matrix4_determinant_trial1);
	run_test(test_matrix4_determinant_trial2);
	run_test(test_matrix4_determinant_trial3);

	run_test(test_matrix4_identity_with_vector2);
	run_test(test_matrix4_identity_with_vector3);
	run_test(test_matrix4_identity_with_vector4);

	run_test(test_matrix4_transformation_translatev3);
	run_test(test_matrix4_transformation_translatev3_negative);
	run_test(test_matrix4_transformation_scalingv3);
	run_test(test_matrix4_transformation_scale_then_translatev3);
	run_test(test_matrix4_transform_3d_scale_translate);

	run_test(test_matrix4_transformation_translatev3_with_vector2);
	run_test(test_matrix4_transformation_translatev3_with_vector2_2);

	run_test(test_vector3_rotate_by_matrix_xy_quarter_turn);
	run_test(test_vector3_rotate_by_matrix_yx_quarter_turn);
	run_test(test_vector3_rotate_by_matrix_zx_quarter_turn);
	run_test(test_matrix4_rotatev3_xz_quarter_turn);
	run_test(test_matrix4_rotatev3_xz_quarter_turn_opposite);
	run_test(test_matrix4_rotatev3_xy_quarter_turn);
	run_test(test_matrix4_rotatev3_xy_quarter_turn_opposite);
	run_test(test_matrix4_set_from_quaternion_xy_quarter_turn);
	run_test(test_matrix4_set_from_quaternion_xz_quarter_turn);
	run_test(test_matrix4_set_from_quaternion_yx_quarter_turn);
	run_test(test_matrix4_set_from_quaternion_yz_quarter_turn);
	run_test(test_matrix4_set_from_quaternion_zx_quarter_turn);
	run_test(test_matrix4_set_from_quaternion_zy_quarter_turn);
	run_test(test_matrix4_set_from_quaternion_xy_half_turn);

	run_test(test_matrix4_match_transformation_matrix_quaternion);
	run_test(test_matrix4_transform_3d);
	run_test(test_matrix4_transform_3d_combined);

	run_test(test_matrix4_inverse);
	run_test(test_matrix4_determinant_row_is_zero);

	return NULL;
}
