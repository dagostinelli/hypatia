/* SPDX-License-Identifier: MIT */

#include <hypatia_mat6.h>

static char *test_matrix6_zero(void)
{
	struct matrix6 zero;
	uint8_t i;

	matrix6_zero(&zero);

	for (i = 0; i < 16; i++) {
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
	_matrix6_print_with_columnrow_indexer(&identity);printf("\n");
	_matrix6_set_random(&m1);
	_matrix6_print_with_columnrow_indexer(&m1);printf("\n");

	/* copy m1 -> m2 */
	matrix6_set(&m2, &m1);

	/* equal */
	test_assert(matrix6_equals(&m2, &m1));

	/* m2 = m2 * I */
	matrix6_multiply(&m2, &identity);

	_matrix6_print_with_columnrow_indexer(&m2);printf("\n");

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
	m1.c01 = 7;  m1.c11 = 8;   m1.c21 = 9;  m1.c31 = 10;  m1.c41 = 11;  m1.c41 = 12;
	m1.c02 = 13;  m1.c12 = 14;  m1.c22 = 15; m1.c32 = 16; m1.c42 = 17; m1.c52 = 18;
	m1.c03 = 19; m1.c13 = 20;  m1.c23 = 21; m1.c33 = 22; m1.c43 = 23; m1.c53 = 24;
	m1.c04 = 25; m1.c14 = 26;  m1.c24 = 27; m1.c34 = 28; m1.c44 = 29; m1.c54 = 30;
	m1.c05 = 31; m1.c15 = 32;  m1.c25 = 33; m1.c35 = 34; m1.c45 = 35; m1.c55 = 36;

	mR.c00 = 178;   mR.c10 = 184;  mR.c20 = 194; mR.c30 = 202; mR.c40 = 210; mR.c50 = 219;
	mR.c01 = 339;  mR.c11 = 358;  mR.c21 = 377; mR.c31 = 396; mR.c41 = 415; mR.c51 = 441;
	mR.c02 = 531;  mR.c12 = 562;  mR.c22 = 593; mR.c32 = 624; mR.c42 = 655; mR.c52 = 699;
	mR.c03 = 723;  mR.c13 = 766;  mR.c23 = 809; mR.c33 = 852; mR.c43 = 895; mR.c53 = 957;
	mR.c04 = 915;  mR.c14 = 970;  mR.c24 = 1025; mR.c34 = 1080; mR.c44 = 1135; mR.c54 = 1215;
	mR.c05 = 1107;  mR.c15 = 1174;  mR.c25 = 1241; mR.c35 = 1308; mR.c45 = 1375; mR.c55 = 1473;
	matrix6_multiplyf(&mR, 3.0f);

	/*matrix6_print_with_columnrow_indexer(&mR);*/

	printf("\n");

	/* copy m1 -> m2 */
	matrix6_set(&m2, &m1);

	matrix6_multiply(&m2, &m1);
	/*matrix6_print_with_columnrow_indexer(&m2);*/

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
	struct matrix6 m = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	struct matrix6 e = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

	matrix6_transpose(&m);
	test_assert(matrix6_equals(&m, &e));

	return NULL;
}


/*static char *test_matrix6_determinant_trial1(void)
{
	struct matrix6 m = {4, 3, 2, 2, 0, 1, -3, 3, 0, -1, 3, 3, 0, 3, 1, 1};

	test_assert(scalar_equals(matrix6_determinant(&m), -240));
	return NULL;
}


static char *test_matrix6_determinant_trial2(void)
{
	struct matrix6 m = {-1, 1, 4, 2, 2, -1, 2, 5, 1, 2, 3, 4, 3, 4, -1, 2};

	test_assert(scalar_equals(matrix6_determinant(&m), -26));
	return NULL;
}


static char *test_matrix6_determinant_trial3(void)
{
	struct matrix6 m = {1, 3, -2, 1, 5, 1, 0, -1, 0, 1, 0, -2, 2, -1, 0, 3};

	test_assert(scalar_equals(matrix6_determinant(&m), -6));
	return NULL;
}*/


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


/*static char *test_matrix6_determinant_row_is_zero(void)
{
	struct matrix6 m;
	HYP_FLOAT det;
*/
	/* identity is not zero */
/*	matrix6_identity(&m);
	det = matrix6_determinant(&m);
	test_assert(!scalar_equals(det, 0));
	test_assert(scalar_equals(det, 1));
*/
	/* when any row is zero, the determinant is zero */
/*	matrix6_identity(&m);
	m.r00 = 0.0; m.r01 = 0.0; m.r02 = 0.0; m.r03 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r10 = 0.0; m.r11 = 0.0; m.r12 = 0.0; m.r13 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r20 = 0.0; m.r21 = 0.0; m.r22 = 0.0; m.r23 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	matrix6_identity(&m);
	m.r30 = 0.0; m.r31 = 0.0; m.r32 = 0.0; m.r33 = 0.0;
	det = matrix6_determinant(&m);
	test_assert(scalar_equals(det, 0));

	return NULL;
}*/


/*static char *test_matrix6_inverse(void)
{
	struct matrix6 originalMatrix;
	struct matrix6 identity;
	struct matrix6 inverted;
	struct vector3 scratchVector;
	struct matrix6 scratchMatrix;
	void *hasInverse = NULL;

	matrix6_identity(&identity);

	matrix6_identity(&originalMatrix);
	matrix6_multiply(&originalMatrix, matrix6_make_transformation_scalingv3(&scratchMatrix, vector3_setf3(&scratchVector, 0.5f, 0.5f, 0.5f)));
	matrix6_multiply(&originalMatrix, matrix6_make_transformation_translationv3(&scratchMatrix, vector3_setf3(&scratchVector, 1.0f, 0.8f, 0.3f)));
	matrix6_multiply(&originalMatrix, matrix6_set_from_euler_anglesf3_EXP(&scratchMatrix, HYP_TAU / 4.0f, HYP_TAU / 4.0f, HYP_TAU / 4.0f));

	hasInverse = matrix6_invert(matrix6_set(&inverted, &originalMatrix));

	test_assert(hasInverse);

	matrix6_identity(&scratchMatrix);
	matrix6_multiply(&scratchMatrix, &inverted);
	matrix6_multiply(&scratchMatrix, &originalMatrix);

	test_assert(matrix6_equals(&identity, &scratchMatrix));

	return NULL;
}*/


static char *matrix6_all_tests(void)
{
	run_test(test_matrix6_zero);
	run_test(test_matrix6_equals);
	run_test(test_matrix6_multiply_identity);

	run_test(test_matrix6_multiplym6);
	run_test(test_matrix6_columnrowcolumn);
	run_test(test_matrix6_transpose);
	/*run_test(test_matrix6_determinant_trial1);
	run_test(test_matrix6_determinant_trial2);
	run_test(test_matrix6_determinant_trial3);*/

	run_test(test_matrix6_identity_with_vector2);
	run_test(test_matrix6_identity_with_vector3);
	run_test(test_matrix6_identity_with_vector4);

	/*run_test(test_matrix6_inverse);*/
	/*run_test(test_matrix6_determinant_row_is_zero);*/

	return NULL;
}
