/* SPDX-License-Identifier: MIT */

#include <hypatia_mat6.h>

static char *test_matrix6_zero(void)
{
	struct matrix6 zero;
	uint8_t i;

	matrix6_zero(&zero);

	for (i = 0; i < 36; i++) {
		test_assert(scalar_equalsf(zero.m[i], 0.0f));
	}

	return NULL;
}


static char *test_matrix6_equals(void)
{
	struct matrix6 m, identity;

	matrix6_identity(&identity);
	_matrix6_set_random(&m);

	/* equal */
	test_assert(matrix6_equals(&identity, &identity));

	/* not-equal */
	test_assert(matrix6_equals(&identity, &m) == 0);

	return NULL;
}


static char *test_matrix6_multiply_identity(void)
{
	struct matrix6 m1, m2, identity;

	matrix6_identity(&identity);
	_matrix6_set_random(&m1);

	/* copy m1 -> m2 */
	matrix6_set(&m2, &m1);

	/* equal */
	test_assert(matrix6_equals(&m2, &m1));

	/* m2 = m2 * I */
	matrix6_multiply(&m2, &identity);

	/* equal */
	test_assert(matrix6_equals(&m2, &m1));

	return NULL;
}


static char *test_matrix6_multiplym6(void)
{
	struct matrix6 m1, m2, mR;

	matrix6_identity(&m1);
	matrix6_identity(&m2);

	m1.c00 = 1;  m1.c10 = 2;   m1.c20 = 3;  m1.c30 = 4; m1.c40 = 5; m1.c50 = 6;
	m1.c01 = 7;  m1.c11 = 8;   m1.c21 = 9;  m1.c31 = 10;  m1.c41 = 11;  m1.c51 = 12;
	m1.c02 = 13;  m1.c12 = 14;  m1.c22 = 15; m1.c32 = 16; m1.c42 = 17; m1.c52 = 18;
	m1.c03 = 19; m1.c13 = 20;  m1.c23 = 21; m1.c33 = 22; m1.c43 = 23; m1.c53 = 24;
	m1.c04 = 25; m1.c14 = 26;  m1.c24 = 27; m1.c34 = 28; m1.c44 = 29; m1.c54 = 30;
	m1.c05 = 31; m1.c15 = 32;  m1.c25 = 33; m1.c35 = 34; m1.c45 = 35; m1.c55 = 36;

	mR.c00 = 147;   mR.c10 = 154;  mR.c20 = 161; mR.c30 = 168; mR.c40 = 175; mR.c50 = 182;
	mR.c01 = 339;  mR.c11 = 358;  mR.c21 = 377; mR.c31 = 396; mR.c41 = 415; mR.c51 = 434;
	mR.c02 = 531;  mR.c12 = 562;  mR.c22 = 593; mR.c32 = 624; mR.c42 = 655; mR.c52 = 686;
	mR.c03 = 723;  mR.c13 = 766;  mR.c23 = 809; mR.c33 = 852; mR.c43 = 895; mR.c53 = 938;
	mR.c04 = 915;  mR.c14 = 970;  mR.c24 = 1025; mR.c34 = 1080; mR.c44 = 1135; mR.c54 = 1190;
	mR.c05 = 1107;  mR.c15 = 1174;  mR.c25 = 1241; mR.c35 = 1308; mR.c45 = 1375; mR.c55 = 1442;
	matrix6_multiplyf(&mR, 3.0f);

	/* copy m1 -> m2 */
	matrix6_set(&m2, &m1);

	matrix6_multiply(&m2, &m1);

	test_assert(matrix6_equals(&mR, &m2));

	return NULL;
}


static char *test_matrix6_identity_with_vector2(void)
{
	/* vector * identity_matrix = vector */
	struct matrix6 m;
	struct vector2 startingPosition = {4.3f, 1.4f};
	struct vector2 expectedPosition = {4.3f, 1.4f};
	struct vector2 r;

	matrix6_identity(&m);

	matrix6_multiplyv2(&m, &startingPosition, &r);
	test_assert(vector2_equals(&r, &expectedPosition));

	return NULL;
}


static char *test_matrix6_identity_with_vector3(void)
{
	/* vector * identity_matrix = vector */
	struct matrix6 m;
	struct vector3 startingPosition = {4.3f, 1.4f, 3.67f};
	struct vector3 expectedPosition = {4.3f, 1.4f, 3.67f};
	struct vector3 r;

	matrix6_identity(&m);

	matrix6_multiplyv3(&m, &startingPosition, &r);
	test_assert(vector3_equals(&r, &expectedPosition));

	return NULL;
}


static char *test_matrix6_identity_with_vector4(void)
{
	/* vector * identity_matrix = vector */
	struct matrix6 m;
	struct vector4 startingPosition = {4.3f, 1.4f, 3.67f, 2.4f};
	struct vector4 expectedPosition = {4.3f, 1.4f, 3.67f, 2.4f};
	struct vector4 r;

	matrix6_identity(&m);

	matrix6_multiplyv4(&m, &startingPosition, &r);
	test_assert(vector4_equals(&r, &expectedPosition));

	return NULL;
}


static char *test_matrix6_transpose(void)
{
	struct matrix6 m = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
	struct matrix6 e = {0, 6, 12, 18, 24, 30, 1, 7, 13, 19, 25, 31, 2, 8, 14, 20, 26, 32, 3, 9, 15, 21, 27, 33, 4, 10, 16, 22, 28, 34, 5, 11, 17, 23, 29, 35};

	matrix6_transpose(&m);
	test_assert(matrix6_equals(&m, &e));

	return NULL;
}


static char *test_matrix6_determinant_trial1(void)
{
	struct matrix6 m = {1, 2, 3, 4, 5, 9, 7, 8, 7, 5, 8, 4, 7, 8, 2, 6, 4, 5, 2, 1, 5, 1, 5, 7, 5, 8, 9, 7, 3, 5, 6, 9, 8, 7, 1, 3};

	test_assert(scalar_equals(matrix6_determinant(&m), -503));
	return NULL;
}


static char *test_matrix6_determinant_trial2(void)
{
	struct matrix6 m = {1, 2, 2, 8, 5, 2, -4, 5, 7, 7, 4, 3, 4, 1, 1, 8, 3, 5, 1, 2, 3, 1, 4, 4, 7, 8, 9, 1, 2, 7, 2, 4, 5, 8, 9, 8};

	test_assert(scalar_equals(matrix6_determinant(&m), 4621));
	return NULL;
}


static char *test_matrix6_determinant_trial3(void)
{
	struct matrix6 m = {-1, -2, -2, 5, 7, 7, 1, 4, -1, 2, -4, 5, -4, 5, 7, 1, -4, -5, 4, 2, 5, 1, 1, 5, 5, 5, 9, 8, 7, 8, -2, -7, -8, 8, 9, 9};

	test_assert(scalar_equals(matrix6_determinant(&m), 66240));
	return NULL;
}


static char *test_matrix6_columnrowcolumn(void)
{
	struct matrix6 c;
	struct matrix6 r;
	struct matrix6 m;

	matrix6_zero(&c);
	matrix6_zero(&r);
	matrix6_zero(&m);

	_matrix6_set_random(&m);

	matrix6_set(&c, &m);
	matrix6_set(&r, &m);

	test_assert(matrix6_equals(&c, &r));
	test_assert(matrix6_equals(&c, &m));
	test_assert(matrix6_equals(&m, &r));

	/* transpose only c and r, but not m */
	_matrix6_transpose_rowcolumn(&r);
	_matrix6_transpose_columnrow(&c);

	test_assert(matrix6_equals(&c, &r));
	test_assert(matrix6_equals(&c, &m) == 0);
	test_assert(matrix6_equals(&m, &r) == 0);

	matrix6_transpose(&m);

	test_assert(matrix6_equals(&c, &m));
	test_assert(matrix6_equals(&m, &r));

	return NULL;
}


static char *test_matrix6_determinant_row_is_zero(void)
{
	struct matrix6 m;
	HYP_FLOAT det;

	/* identity is not zero */
	matrix6_identity(&m);
	det = matrix6_determinant(&m);
	test_assert(!scalar_equals(det, 0));
	test_assert(scalar_equals(det, 1));

	/* when any row is zero, the determinant is zero */
	matrix6_identity(&m);
	m.r00 = 0.0; m.r01 = 0.0; m.r02 = 0.0; m.r03 = 0.0; m.r04 = 0.0; m.r05 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r10 = 0.0; m.r11 = 0.0; m.r12 = 0.0; m.r13 = 0.0; m.r14 = 0.0; m.r15 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r20 = 0.0; m.r21 = 0.0; m.r22 = 0.0; m.r23 = 0.0; m.r24 = 0.0; m.r25 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r30 = 0.0; m.r31 = 0.0; m.r32 = 0.0; m.r33 = 0.0; m.r34 = 0.0; m.r35 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r40 = 0.0; m.r41 = 0.0; m.r42 = 0.0; m.r43 = 0.0; m.r44 = 0.0; m.r45 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r50 = 0.0; m.r51 = 0.0; m.r52 = 0.0; m.r53 = 0.0; m.r54 = 0.0; m.r55 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	return NULL;
}


/* float rounding in a 6x6 elimination exceeds HYP_EPSILON, so allow more slack */
static int matrix6_equals_tolerance(const struct matrix6 *a, const struct matrix6 *b)
{
	uint8_t i;

	for (i = 0; i < 36; i++) {
		if (scalar_equals_epsilonf(a->m[i], b->m[i], 1E-4f) == 0) {
			return 0;
		}
	}

	return 1;
}


static char *test_matrix6_inverse(void)
{
	struct matrix6 m = {1, 2, 3, 4, 5, 9, 7, 8, 7, 5, 8, 4, 7, 8, 2, 6, 4, 5, 2, 1, 5, 1, 5, 7, 5, 8, 9, 7, 3, 5, 6, 9, 8, 7, 1, 3};
	struct matrix6 identity;
	struct matrix6 inverted;
	struct matrix6 product;

	matrix6_identity(&identity);

	test_assert(matrix6_inverse(&m, &inverted) == &inverted);

	/* M * M^-1 = I */
	matrix6_set(&product, &m);
	matrix6_multiply(&product, &inverted);
	test_assert(matrix6_equals_tolerance(&product, &identity));

	/* M^-1 * M = I */
	matrix6_set(&product, &inverted);
	matrix6_multiply(&product, &m);
	test_assert(matrix6_equals_tolerance(&product, &identity));

	return NULL;
}


static char *test_matrix6_inverse_diagonal(void)
{
	struct matrix6 m;
	struct matrix6 expected;

	/* every diagonal element, including rows 5 and 6, must be inverted */
	matrix6_identity(&m);
	m.r00 = 2.0f; m.r11 = 4.0f; m.r22 = 5.0f; m.r33 = 8.0f; m.r44 = 10.0f; m.r55 = 16.0f;

	matrix6_identity(&expected);
	expected.r00 = 0.5f; expected.r11 = 0.25f; expected.r22 = 0.2f;
	expected.r33 = 0.125f; expected.r44 = 0.1f; expected.r55 = 0.0625f;

	matrix6_invert(&m);
	test_assert(matrix6_equals(&m, &expected));

	return NULL;
}


static char *test_matrix6_inverse_needs_pivot(void)
{
	struct matrix6 m;
	struct matrix6 inverted;
	struct matrix6 product;
	struct matrix6 identity;

	/* a permutation matrix has zeros on the diagonal */
	matrix6_zero(&m);
	m.r01 = 1.0f; m.r12 = 1.0f; m.r23 = 1.0f; m.r34 = 1.0f; m.r45 = 1.0f; m.r50 = 1.0f;

	matrix6_identity(&identity);

	test_assert(matrix6_inverse(&m, &inverted) != NULL);

	matrix6_set(&product, &m);
	matrix6_multiply(&product, &inverted);
	test_assert(matrix6_equals(&product, &identity));

	return NULL;
}


static char *test_matrix6_inverse_singular(void)
{
	struct matrix6 m;
	struct matrix6 inverted;
	struct matrix6 original;

	/* the last row duplicates the first, so there is no inverse */
	matrix6_identity(&m);
	m.r50 = 1.0f; m.r55 = 0.0f;

	test_assert(matrix6_inverse(&m, &inverted) == NULL);

	/* invert leaves a singular matrix unchanged */
	matrix6_set(&original, &m);
	matrix6_invert(&m);
	test_assert(matrix6_equals(&m, &original));

	return NULL;
}


static char *test_matrix6_multiplyv2(void)
{
	struct matrix6 m;
	struct vector2 v = {1.0f, 2.0f};
	struct vector2 expected;
	struct vector2 r;

	/* missing vector components are treated as 1 */
	matrix6_identity(&m);
	m.r02 = 1.0f; m.r03 = 2.0f; m.r04 = 3.0f; m.r05 = 4.0f;
	m.r12 = 5.0f; m.r13 = 6.0f; m.r14 = 7.0f; m.r15 = 8.0f;

	expected.x = 1.0f + 1.0f + 2.0f + 3.0f + 4.0f;
	expected.y = 2.0f + 5.0f + 6.0f + 7.0f + 8.0f;

	matrix6_multiplyv2(&m, &v, &r);
	test_assert(vector2_equals(&r, &expected));

	return NULL;
}


static char *test_matrix6_multiply_matches_matrix4(void)
{
	/* a 4x4 block inside a 6x6 identity multiplies like a matrix4 */
	struct matrix4 a4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	struct matrix4 b4 = {2, 0, 1, 3, 1, 4, 0, 2, 5, 1, 3, 0, 0, 2, 1, 6};
	struct matrix6 a6;
	struct matrix6 b6;
	uint8_t row;
	uint8_t col;

	matrix6_identity(&a6);
	matrix6_identity(&b6);

	for (row = 0; row < 4; row++) {
		for (col = 0; col < 4; col++) {
			a6.m66[row][col] = a4.m[row * 4 + col];
			b6.m66[row][col] = b4.m[row * 4 + col];
		}
	}

	matrix4_multiply(&a4, &b4);
	matrix6_multiply(&a6, &b6);

	for (row = 0; row < 4; row++) {
		for (col = 0; col < 4; col++) {
			test_assert(scalar_equalsf(a6.m66[row][col], a4.m[row * 4 + col]));
		}
	}

	return NULL;
}


static char *matrix6_all_tests(void)
{
	run_test(test_matrix6_zero);
	run_test(test_matrix6_equals);
	run_test(test_matrix6_multiply_identity);

	run_test(test_matrix6_multiplym6);
	run_test(test_matrix6_columnrowcolumn);
	run_test(test_matrix6_transpose);
	run_test(test_matrix6_determinant_trial1);
	run_test(test_matrix6_determinant_trial2);
	run_test(test_matrix6_determinant_trial3);

	run_test(test_matrix6_identity_with_vector2);
	run_test(test_matrix6_identity_with_vector3);
	run_test(test_matrix6_identity_with_vector4);

	run_test(test_matrix6_multiplyv2);
	run_test(test_matrix6_multiply_matches_matrix4);

	run_test(test_matrix6_inverse);
	run_test(test_matrix6_inverse_diagonal);
	run_test(test_matrix6_inverse_needs_pivot);
	run_test(test_matrix6_inverse_singular);
	run_test(test_matrix6_determinant_row_is_zero);

	return NULL;
}
