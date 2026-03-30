/* SPDX-License-Identifier: MIT */

static char *test_matrix2_zero(void)
{
	struct matrix2 zero;
	uint8_t i;

	matrix2_zero(&zero);

	for (i = 0; i < 4; i++) {
		test_assert(scalar_equalsf(zero.m[i], 0.0f));
	}

	return NULL;
}


static char *test_matrix2_equals(void)
{
	struct matrix2 m, identity;

	matrix2_identity(&identity);
	_matrix2_set_random(&m);

	/* equal */
	test_assert(matrix2_equals(&identity, &identity));

	/* not-equal */
	test_assert(matrix2_equals(&identity, &m) == 0);

	return NULL;
}


static char *test_matrix2_multiply_identity(void)
{
	struct matrix2 m1, m2, identity;

	matrix2_identity(&identity);
	_matrix2_set_random(&m1);

	/* copy m1 -> m2 */
	matrix2_set(&m2, &m1);

	/* m2 = m2 * I */
	matrix2_multiply(&m2, &identity);

	/* equal */
	test_assert(matrix2_equals(&m2, &m1));

	return NULL;
}


static char *test_matrix2_multiply(void)
{
	struct matrix2 m1, m2, mR;

	m1.c00 = 1;  m1.c10 = 2;
	m1.c01 = 5;  m1.c11 = 6;

	mR.c00 = 11;  mR.c10 = 14;
	mR.c01 = 35;  mR.c11 = 46;

	/* copy m1 -> m2 */
	matrix2_set(&m2, &m1);

	matrix2_multiply(&m2, &m1);
	test_assert(matrix2_equals(&mR, &m2));

	return NULL;
}


static char *test_matrix2_determinant_trial1(void)
{
	struct matrix2 m = {5, 3, 7, 2};

	test_assert(scalar_equals(matrix2_determinant(&m), -11));
	return NULL;
}


static char *test_matrix2_determinant_trial2(void)
{
	struct matrix2 m = {8, 4, 3, -5};

	test_assert(scalar_equals(matrix2_determinant(&m), -52));
	return NULL;
}


static char *test_matrix2_determinant_trial3(void)
{
	struct matrix2 m = {2, -3, 1, 2};

	test_assert(scalar_equals(matrix2_determinant(&m), 7));
	return NULL;
}


static char *test_matrix2_determinant_trial4(void)
{
	struct matrix2 m = {(HYP_FLOAT)0.608088, (HYP_FLOAT)0.742654, (HYP_FLOAT)0.558388, (HYP_FLOAT)0.722123};

	test_assert(scalar_equals(matrix2_determinant(&m), (HYP_FLOAT)0.024425249));
	return NULL;
}


static char *test_matrix2_inverse(void)
{
	struct matrix2 originalMatrix = {2, 0, -1, 5};
	struct matrix2 identity;
	struct matrix2 inverted;
	struct matrix2 scratchMatrix;
	void *hasInverse = NULL;

	matrix2_identity(&identity);

	matrix2_identity(&originalMatrix);

	hasInverse = matrix2_invert(matrix2_set(&inverted, &originalMatrix));

	test_assert(hasInverse);

	matrix2_identity(&scratchMatrix);
	matrix2_multiply(&scratchMatrix, &inverted);
	matrix2_multiply(&scratchMatrix, &originalMatrix);

	test_assert(matrix2_equals(&identity, &scratchMatrix));

	return NULL;
}


static char *test_matrix2_invert(void)
{
	struct matrix2 m = {2, 0, -1, 5};
	struct matrix2 expected = {(HYP_FLOAT)0.5, (HYP_FLOAT)0.0, (HYP_FLOAT)0.1, (HYP_FLOAT)0.2};
	uint8_t i;

	matrix2_invert(&m);
	for (i = 0; i < 4; i++) {
		test_assert(scalar_equals(m.m[i], expected.m[i]));
	}

	return NULL;
}


static char *test_matrix2_columnrowcolumn(void)
{
	struct matrix2 c;
	struct matrix2 r;
	struct matrix2 m;

	matrix2_zero(&c);
	matrix2_zero(&r);
	matrix2_zero(&m);

	_matrix2_set_random(&m);

	matrix2_set(&c, &m);
	matrix2_set(&r, &m);

	test_assert(matrix2_equals(&c, &r));
	test_assert(matrix2_equals(&c, &m));
	test_assert(matrix2_equals(&m, &r));

	/* transpose only c and r, but not m */
	_matrix2_transpose_rowcolumn(&r);
	_matrix2_transpose_columnrow(&c);

	test_assert(matrix2_equals(&c, &r));
	test_assert(matrix2_equals(&c, &m) == 0);
	test_assert(matrix2_equals(&m, &r) == 0);

	matrix2_transpose(&m);

	test_assert(matrix2_equals(&c, &m));
	test_assert(matrix2_equals(&m, &r));

	return NULL;
}


static char *test_matrix2_transpose(void)
{
	struct matrix2 m = {0, 1, 2, 3};
	struct matrix2 e = {0, 2, 1, 3};

	matrix2_transpose(&m);
	test_assert(matrix2_equals(&m, &e));
	return NULL;
}


static char *test_matrix2_identity_with_vector(void)
{
	/* vector * identity_matrix = vector */
	struct matrix2 m;
	struct vector2 startingPosition = {4.3f, 1.4f};
	struct vector2 expectedPosition = {4.3f, 1.4f};

	matrix2_identity(&m);

	vector2_multiplym2(&startingPosition, &m);
	test_assert(vector2_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_matrix2_transformation_scalingv2(void)
{
	struct matrix2 transform;

	struct vector2 startingPosition = {0.0f, 1.0f};
	struct vector2 scale = {0.1f, 0.1f};
	struct vector2 expectedPosition = {0.0f, 0.1f};

	matrix2_make_transformation_scalingv2(&transform, &scale);
	vector2_multiplym2(&startingPosition, &transform);
	test_assert(vector2_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static char *test_vector2_rotate_by_matrix2_zx_quarter_turn(void)
{
	struct matrix2 m;
	struct vector2 r;

	matrix2_make_transformation_rotationf_z(&m, HYP_TAU / 4.0f);
	vector2_set(&r, HYP_VECTOR2_UNIT_X);
	vector2_multiplym2(&r, &m);
	test_assert(vector2_equals(&r, HYP_VECTOR2_UNIT_Y));

	return NULL;
}


static char *test_matrix2_rotatev3_xz_quarter_turn(void)
{
	struct matrix2 m;
	struct vector2 r;

	vector2_set(&r, HYP_VECTOR2_UNIT_X);
	matrix2_identity(&m);
	matrix2_rotate(&m, HYP_TAU / 4.0f);
	vector2_multiplym2(&r, &m);
	test_assert(vector2_equals(&r, HYP_VECTOR2_UNIT_Y));

	return NULL;
}


static char *test_matrix2_rotatev3_xz_quarter_turn_opposite(void)
{
	struct matrix2 m;
	struct vector2 r;

	vector2_set(&r, HYP_VECTOR2_UNIT_X);
	matrix2_identity(&m);
	matrix2_rotate(&m, -(HYP_TAU / 4.0f));
	vector2_multiplym2(&r, &m);
	test_assert(vector2_equals(&r, HYP_VECTOR2_UNIT_Y_NEGATIVE));

	return NULL;
}


static char *test_matrix2_add(void)
{
	struct matrix2 m1 = {1, 2, 3, 4};
	struct matrix2 m2 = {5, 6, 7, 8};
	struct matrix2 expected = {6, 8, 10, 12};

	matrix2_add(&m1, &m2);
	test_assert(matrix2_equals(&m1, &expected));

	return NULL;
}


static char *test_matrix2_subtract(void)
{
	struct matrix2 m1 = {5, 6, 7, 8};
	struct matrix2 m2 = {1, 2, 3, 4};
	struct matrix2 expected = {4, 4, 4, 4};

	matrix2_subtract(&m1, &m2);
	test_assert(matrix2_equals(&m1, &expected));

	return NULL;
}


static char *test_matrix2_multiplyf(void)
{
	struct matrix2 m = {1, 2, 3, 4};
	struct matrix2 expected = {2, 4, 6, 8};

	matrix2_multiplyf(&m, 2.0f);
	test_assert(matrix2_equals(&m, &expected));

	return NULL;
}


static char *test_matrix2_multiplyf_zero(void)
{
	struct matrix2 m = {1, 2, 3, 4};
	struct matrix2 expected;

	matrix2_zero(&expected);

	matrix2_multiplyf(&m, 0.0f);
	test_assert(matrix2_equals(&m, &expected));

	return NULL;
}


static char *test_matrix2_inverse_nonmutating(void)
{
	struct matrix2 original = {2, 0, -1, 5};
	struct matrix2 originalCopy = {2, 0, -1, 5};
	struct matrix2 inv;
	struct matrix2 product;
	struct matrix2 identity;
	void *result;

	matrix2_identity(&identity);

	result = matrix2_inverse(&original, &inv);
	test_assert(result);

	/* original should not be modified */
	test_assert(matrix2_equals(&original, &originalCopy));

	/* original * inv should be identity */
	matrix2_set(&product, &original);
	matrix2_multiply(&product, &inv);
	test_assert(matrix2_equals(&product, &identity));

	return NULL;
}


static char *test_matrix2_inverse_singular(void)
{
	struct matrix2 singular = {1, 2, 2, 4};
	struct matrix2 result;
	void *ret;

	ret = matrix2_inverse(&singular, &result);
	test_assert(ret == NULL);

	return NULL;
}


static char *test_matrix2_multiplyv2(void)
{
	struct matrix2 m = {1, 2, 3, 4};
	struct vector2 v = {2, 3};
	struct vector2 r;
	struct vector2 expected;

	/* vR.x = vT.x * c00 + vT.y * c01, vR.y = vT.x * c10 + vT.y * c11 */
	expected.x = 2 * 1 + 3 * 3;
	expected.y = 2 * 2 + 3 * 4;

	matrix2_multiplyv2(&m, &v, &r);
	test_assert(vector2_equals(&r, &expected));

	return NULL;
}


static char *test_matrix2_multiplyv2_identity(void)
{
	struct matrix2 m;
	struct vector2 v = {5.5f, -3.2f};
	struct vector2 r;

	matrix2_identity(&m);
	matrix2_multiplyv2(&m, &v, &r);
	test_assert(vector2_equals(&r, &v));

	return NULL;
}


static char *test_matrix2_scalev2(void)
{
	struct matrix2 m;
	struct vector2 scale = {2.0f, 3.0f};
	struct vector2 v = {4.0f, 5.0f};
	struct vector2 expected = {8.0f, 15.0f};

	matrix2_identity(&m);
	matrix2_scalev2(&m, &scale);
	vector2_multiplym2(&v, &m);
	test_assert(vector2_equals(&v, &expected));

	return NULL;
}


static char *matrix2_all_tests(void)
{
	run_test(test_matrix2_zero);
	run_test(test_matrix2_equals);
	run_test(test_matrix2_multiply_identity);
	run_test(test_matrix2_multiply);
	run_test(test_matrix2_identity_with_vector);
	run_test(test_matrix2_columnrowcolumn);
	run_test(test_matrix2_transpose);
	run_test(test_matrix2_determinant_trial1);
	run_test(test_matrix2_determinant_trial2);
	run_test(test_matrix2_determinant_trial3);
	run_test(test_matrix2_determinant_trial4);
	run_test(test_matrix2_inverse);
	run_test(test_matrix2_invert);

	run_test(test_matrix2_transformation_scalingv2);
	run_test(test_vector2_rotate_by_matrix2_zx_quarter_turn);

	run_test(test_matrix2_rotatev3_xz_quarter_turn);
	run_test(test_matrix2_rotatev3_xz_quarter_turn_opposite);

	run_test(test_matrix2_add);
	run_test(test_matrix2_subtract);
	run_test(test_matrix2_multiplyf);
	run_test(test_matrix2_multiplyf_zero);
	run_test(test_matrix2_inverse_nonmutating);
	run_test(test_matrix2_inverse_singular);
	run_test(test_matrix2_multiplyv2);
	run_test(test_matrix2_multiplyv2_identity);
	run_test(test_matrix2_scalev2);

	return NULL;
}
