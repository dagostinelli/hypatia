/* SPDX-License-Identifier: MIT */

/* non-symmetric, not the identity, and invertible */
static const struct matrix4 test_matrix4_fixed = {.m = {1, 2, 3, 4, 0, 1, 2, 3, 0, 0, 1, 2, 0, 0, 0, 1}};

static const char *test_matrix4_zero(void)
{
	struct matrix4 zero;
	uint8_t i;

	matrix4_zero(&zero);

	for (i = 0; i < 16; i++) {
		test_assert(scalar_equalsf(zero.m[i], HYP_FLOAT_C(0.0)));
	}

	return NULL;
}


static const char *test_matrix4_equals(void)
{
	struct matrix4 m, identity;

	matrix4_identity(&identity);
	matrix4_set(&m, &test_matrix4_fixed);

	/* equal */
	test_assert(matrix4_equals(&identity, &identity));

	/* not-equal */
	test_assert(matrix4_equals(&identity, &m) == 0);

	return NULL;
}


static const char *test_matrix4_multiply_identity(void)
{
	struct matrix4 m1, m2, identity;

	matrix4_identity(&identity);
	matrix4_set(&m1, &test_matrix4_fixed);

	/* copy m1 -> m2 */
	matrix4_set(&m2, &m1);

	/* m2 = m2 * I */
	matrix4_multiply(&m2, &identity);

	/* equal */
	test_assert(matrix4_equals(&m2, &m1));

	return NULL;
}


/* small integer matrices with no 0 or 1 entries, so every term of a product
 * shows; every product and sum is exact
 */
static const struct matrix4 test_matrix4_a = {.m = {2, 3, -2, 5, -3, 4, 2, -5, 3, -2, 5, 4, -4, 2, 3, 2}};
static const struct matrix4 test_matrix4_b = {.m = {3, -2, 4, 2, 5, 2, -3, 3, -2, 4, 2, -3, 2, 3, -4, 5}};
static const struct matrix4 test_matrix4_c = {.m = {-2, 5, 3, -3, 4, -3, 2, 2, 3, 2, -5, 4, 2, -4, 3, -2}};


static const char *test_matrix4_identity_multiply(void)
{
	struct matrix4 m;
	struct matrix4 identity;

	matrix4_identity(&identity);

	/* I * A = A */
	matrix4_set(&m, &identity);
	matrix4_multiply(&m, &test_matrix4_a);
	test_assert(matrix4_equals(&m, &test_matrix4_a));

	/* A * I = A */
	matrix4_set(&m, &test_matrix4_a);
	matrix4_multiply(&m, &identity);
	test_assert(matrix4_equals(&m, &test_matrix4_a));

	return NULL;
}


static const char *test_matrix4_multiply_associative(void)
{
	struct matrix4 ab_c;
	struct matrix4 bc;
	struct matrix4 a_bc;

	/* (A B) C */
	matrix4_set(&ab_c, &test_matrix4_a);
	matrix4_multiply(&ab_c, &test_matrix4_b);
	matrix4_multiply(&ab_c, &test_matrix4_c);

	/* A (B C) */
	matrix4_set(&bc, &test_matrix4_b);
	matrix4_multiply(&bc, &test_matrix4_c);
	matrix4_set(&a_bc, &test_matrix4_a);
	matrix4_multiply(&a_bc, &bc);

	test_assert(matrix4_equals(&ab_c, &a_bc));

	return NULL;
}


static const char *test_matrix4_multiply_distributive(void)
{
	struct matrix4 b_plus_c;
	struct matrix4 left;
	struct matrix4 ac;
	struct matrix4 right;

	/* A (B + C) */
	matrix4_set(&b_plus_c, &test_matrix4_b);
	matrix4_add(&b_plus_c, &test_matrix4_c);
	matrix4_set(&left, &test_matrix4_a);
	matrix4_multiply(&left, &b_plus_c);

	/* A B + A C */
	matrix4_set(&right, &test_matrix4_a);
	matrix4_multiply(&right, &test_matrix4_b);
	matrix4_set(&ac, &test_matrix4_a);
	matrix4_multiply(&ac, &test_matrix4_c);
	matrix4_add(&right, &ac);

	test_assert(matrix4_equals(&left, &right));

	return NULL;
}


static const char *test_matrix4_multiply_not_commutative(void)
{
	struct matrix4 ab;
	struct matrix4 ba;

	matrix4_set(&ab, &test_matrix4_a);
	matrix4_multiply(&ab, &test_matrix4_b);
	matrix4_set(&ba, &test_matrix4_b);
	matrix4_multiply(&ba, &test_matrix4_a);

	/* A B and B A differ for these matrices */
	test_assert(!matrix4_equals(&ab, &ba));

	return NULL;
}


static const char *test_matrix4_multiplym4(void)
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


static const char *test_matrix4_identity_with_vector2(void)
{
	/* vector * identity_matrix = vector */
	struct matrix4 m;
	struct vector2 startingPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(1.4)}};
	struct vector2 expectedPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(1.4)}};
	struct vector2 r;

	matrix4_identity(&m);

	matrix4_multiplyv2(&m, &startingPosition, &r);
	test_assert(vector2_equals(&r, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_transformation_translatev3_with_vector2(void)
{
	struct matrix4 m;
	struct vector2 r;

	struct vector2 startingPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(7.4)}};
	struct vector3 translation = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.0)}};
	struct vector2 expectedPosition = {.v = {HYP_FLOAT_C(4.4), HYP_FLOAT_C(7.5)}};

	matrix4_make_transformation_translationv3(&m, &translation);
	matrix4_multiplyv2(&m, &startingPosition, &r);
	test_assert(vector2_equals(&r, &expectedPosition));

	return NULL;
}

static const char *test_matrix4_transformation_translatev3_with_vector2_2(void)
{
	struct matrix4 m;
	struct vector2 r;

	struct vector2 startingPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(7.4)}};
	struct vector3 translation = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(1.0)}};
	struct vector2 expectedPosition = {.v = {HYP_FLOAT_C(4.4), HYP_FLOAT_C(7.5)}};

	matrix4_make_transformation_translationv3(&m, &translation);
	matrix4_multiplyv2(&m, &startingPosition, &r);
	test_assert(vector2_equals(&r, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_multiplyv2_ignores_z(void)
{
	struct matrix4 m;
	struct vector2 v;
	struct vector2 r;
	struct vector2 expected;

	/* a 2D vector has z = 0: a quarter turn about Y takes (1, 0) to (0, 0) */
	matrix4_make_transformation_rotationf_y(&m, HYP_TAU / HYP_FLOAT_C(4.0));
	vector2_setf2(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	matrix4_multiplyv2(&m, &v, &r);
	test_assert(vector2_equals(&r, vector2_setf2(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	return NULL;
}


static const char *test_matrix4_identity_with_vector3(void)
{
	/* vector * identity_matrix = vector */
	struct matrix4 m;
	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(1.4), HYP_FLOAT_C(3.67)}};
	struct vector3 expectedPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(1.4), HYP_FLOAT_C(3.67)}};
	struct vector3 r;

	matrix4_identity(&m);

	matrix4_multiplyv3(&m, &startingPosition, &r);
	test_assert(vector3_equals(&r, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_identity_with_vector4(void)
{
	/* vector * identity_matrix = vector */
	struct matrix4 m;
	struct vector4 startingPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(1.4), HYP_FLOAT_C(3.67), HYP_FLOAT_C(2.4)}};
	struct vector4 expectedPosition = {.v = {HYP_FLOAT_C(4.3), HYP_FLOAT_C(1.4), HYP_FLOAT_C(3.67), HYP_FLOAT_C(2.4)}};
	struct vector4 r;

	matrix4_identity(&m);

	matrix4_multiplyv4(&m, &startingPosition, &r);
	test_assert(vector4_equals(&r, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_transpose(void)
{
	struct matrix4 m = {.m = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
	struct matrix4 e = {.m = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}};

	matrix4_transpose(&m);
	test_assert(matrix4_equals(&m, &e));

	return NULL;
}


static const char *test_matrix4_determinant_trial1(void)
{
	struct matrix4 m = {.m = {4, 3, 2, 2, 0, 1, -3, 3, 0, -1, 3, 3, 0, 3, 1, 1}};

	test_assert(scalar_equals(matrix4_determinant(&m), -240));
	return NULL;
}


static const char *test_matrix4_determinant_trial2(void)
{
	struct matrix4 m = {.m = {-1, 1, 4, 2, 2, -1, 2, 5, 1, 2, 3, 4, 3, 4, -1, 2}};

	test_assert(scalar_equals(matrix4_determinant(&m), -26));
	return NULL;
}


static const char *test_matrix4_determinant_trial3(void)
{
	struct matrix4 m = {.m = {1, 3, -2, 1, 5, 1, 0, -1, 0, 1, 0, -2, 2, -1, 0, 3}};

	test_assert(scalar_equals(matrix4_determinant(&m), -6));
	return NULL;
}


static const char *test_matrix4_columnrowcolumn(void)
{
	struct matrix4 c;
	struct matrix4 r;
	struct matrix4 m;

	matrix4_zero(&c);
	matrix4_zero(&r);
	matrix4_zero(&m);

	matrix4_set(&m, &test_matrix4_fixed);

	matrix4_set(&c, &m);
	matrix4_set(&r, &m);

	test_assert(matrix4_equals(&c, &r));
	test_assert(matrix4_equals(&c, &m));
	test_assert(matrix4_equals(&m, &r));

	/* transpose only c and r, but not m */
	hyp_matrix4_transpose_rowcolumn(&r);
	hyp_matrix4_transpose_columnrow(&c);

	test_assert(matrix4_equals(&c, &r));
	test_assert(matrix4_equals(&c, &m) == 0);
	test_assert(matrix4_equals(&m, &r) == 0);

	matrix4_transpose(&m);

	test_assert(matrix4_equals(&c, &m));
	test_assert(matrix4_equals(&m, &r));

	return NULL;
}


static const char *test_matrix4_transformation_translatev3(void)
{
	struct matrix4 transform;

	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector3 translation = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1)}};
	struct vector3 expectedPosition = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(1.1), HYP_FLOAT_C(0.1)}};

	matrix4_make_transformation_translationv3(&transform, &translation);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_transformation_translatev3_negative(void)
{
	struct matrix4 transform;

	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector3 translation = {.v = {-HYP_FLOAT_C(0.1), -HYP_FLOAT_C(0.1), -HYP_FLOAT_C(0.1)}};
	struct vector3 expectedPosition = {.v = {-HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.9), -HYP_FLOAT_C(0.1)}};

	matrix4_make_transformation_translationv3(&transform, &translation);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_transformation_scalingv3(void)
{
	struct matrix4 transform;

	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector3 scale = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1)}};
	struct vector3 expectedPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.0)}};

	matrix4_make_transformation_scalingv3(&transform, &scale);
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_transformation_scale_then_translatev3(void)
{
	struct matrix4 transform;
	struct matrix4 scratch;

	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0)}};
	struct vector3 scale = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1)}};
	struct vector3 translation = {.v = {-HYP_FLOAT_C(0.1), -HYP_FLOAT_C(0.1), -HYP_FLOAT_C(0.1)}};
	struct vector3 expectedPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(0.0)}};

	matrix4_identity(&transform);
	matrix4_multiply(&transform, matrix4_make_transformation_scalingv3(&scratch, &scale));
	matrix4_multiply(&transform, matrix4_make_transformation_translationv3(&scratch, &translation));
	vector3_multiplym4(&startingPosition, &transform);
	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static const char *test_vector3_rotate_by_matrix_xy_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_make_transformation_rotationf_x(&m, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_set(&r, HYP_VECTOR3_UNIT_Y);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));


	return NULL;
}


static const char *test_vector3_rotate_by_matrix_yx_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_make_transformation_rotationf_y(&m, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return NULL;
}


static const char *test_vector3_rotate_by_matrix_zx_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_make_transformation_rotationf_z(&m, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


static const char *test_matrix4_rotatev3_xz_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


static const char *test_matrix4_rotatev3_xz_quarter_turn_opposite(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Z, -(HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static const char *test_matrix4_rotatev3_xy_quarter_turn(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Y, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return NULL;
}


static const char *test_matrix4_rotatev3_xy_quarter_turn_opposite(void)
{
	struct matrix4 m;
	struct vector3 r;

	matrix4_identity(&m);
	matrix4_rotatev3(&m, HYP_VECTOR3_UNIT_Y, -(HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_set(&r, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&r, &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_xy_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_xz_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_yx_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_X), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_yz_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_zx_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_X), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_zy_quarter_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0)));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X_NEGATIVE));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_xy_half_turn(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	matrix4_make_transformation_rotationq(&m, quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(2.0)));
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return NULL;
}


/* rotationq and set_from_quaternion_EXP agree with set_from_axisf3_angle_EXP */
static int quaternion_builders_agree(HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT angle)
{
	struct matrix4 reference;
	struct matrix4 m;
	struct quaternion q;
	struct vector3 axis;

	vector3_setf3(&axis, x, y, z);
	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	matrix4_set_from_axisf3_angle(&reference, x, y, z, angle);

	return matrix4_equals(matrix4_make_transformation_rotationq(&m, &q), &reference) &&
		matrix4_equals(matrix4_set_from_quaternion(&m, &q), &reference);
}


static const char *test_matrix4_rotation_builders_agree(void)
{
	struct matrix4 reference;
	struct matrix4 m;
	HYP_FLOAT third = HYP_SQRT(HYP_FLOAT_C(1.0) / HYP_FLOAT_C(3.0));

	/* rotationf_x/_y/_z agree with the axis-angle builder on their axis */
	matrix4_set_from_axisf3_angle(&reference, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.7));
	test_assert(matrix4_equals(matrix4_make_transformation_rotationf_x(&m, HYP_FLOAT_C(0.7)), &reference));
	matrix4_set_from_axisf3_angle(&reference, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(2.0));
	test_assert(matrix4_equals(matrix4_make_transformation_rotationf_y(&m, -HYP_FLOAT_C(2.0)), &reference));
	matrix4_set_from_axisf3_angle(&reference, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0));
	test_assert(matrix4_equals(matrix4_make_transformation_rotationf_z(&m, HYP_FLOAT_C(3.0)), &reference));

	/* the quaternion builders agree with it on any axis */
	test_assert(quaternion_builders_agree(HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.7)));
	test_assert(quaternion_builders_agree(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(2.0)));
	test_assert(quaternion_builders_agree(HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0)));
	test_assert(quaternion_builders_agree(third, -third, third, HYP_FLOAT_C(1.1)));

	return NULL;
}


/* applies the upper 3x3 of m to a direction (w = 0) */
static struct vector3 *direction_by(const struct matrix4 *m, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, struct vector3 *vR)
{
	struct vector4 v;
	struct vector4 r;

	vector4_setf4(&v, x, y, z, HYP_FLOAT_C(0.0));
	matrix4_multiplyv4(m, &v, &r);

	return vector3_setf3(vR, r.x, r.y, r.z);
}


static const char *test_matrix4_normal_matrix(void)
{
	struct matrix4 m;
	struct matrix4 normal;
	struct matrix4 singular;
	struct vector3 scale;
	struct vector3 translation;
	struct vector3 tangent;
	struct vector3 n;
	struct quaternion q;

	/* a non-uniform scale, a rotation and a translation */
	vector3_setf3(&scale, HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.5));
	vector3_setf3(&translation, HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0));
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_FLOAT_C(0.5));
	matrix4_transformation_compose(&m, &scale, &q, &translation);
	test_assert(matrix4_normal_matrix(&m, &normal) != NULL);

	/* a surface with tangent (1, 1, 0) and normal (1, -1, 0): the transformed
	 * normal stays perpendicular to the transformed tangent
	 */
	direction_by(&m, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), &tangent);
	direction_by(&normal, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), &n);
	test_assert(scalar_equalsf(vector3_dot_product(&tangent, &n), HYP_FLOAT_C(0.0)));

	/* the plain matrix does not keep it perpendicular */
	direction_by(&m, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), &n);
	test_assert(!scalar_equalsf(vector3_dot_product(&tangent, &n), HYP_FLOAT_C(0.0)));

	/* the translation and the last row are not part of it */
	test_assert(scalar_equalsf(normal.r03, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(normal.r33, HYP_FLOAT_C(1.0)));

	/* a zero scale has no normal matrix */
	vector3_setf3(&scale, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	matrix4_make_transformation_scalingv3(&singular, &scale);
	test_assert(matrix4_normal_matrix(&singular, &normal) == NULL);

	return NULL;
}


static const char *test_matrix4_match_transformation_matrix_quaternion(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 vM, vQ;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_rotate_by_quaternion(vector3_set(&vQ, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&vQ, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	matrix4_make_transformation_rotationf_x(&m, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_set(&vM, HYP_VECTOR3_UNIT_Z);
	vector3_multiplym4(&vM, &m);
	test_assert(vector3_equals(&vM, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	test_assert(vector3_equals(&vQ, &vM));

	return NULL;
}


static const char *test_matrix4_transform_3d(void)
{
	struct quaternion orientation;
	struct matrix4 modelMatrix, worldMatrix, scaleM, rotateM, translateM;

	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector3 scale = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1)}};
	struct vector3 expectedPosition = {.v = {-HYP_FLOAT_C(1.1), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 translation = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	/* modelMatrix */
	matrix4_identity(&modelMatrix);
	matrix4_multiply(&modelMatrix, matrix4_make_transformation_scalingv3(&scaleM, &scale));

	/* world transformation */
	matrix4_identity(&worldMatrix);
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_translationv3(&translateM, &translation));
	matrix4_multiply(&worldMatrix,
			 matrix4_make_transformation_rotationq(&rotateM,
							       quaternion_set_from_axis_anglev3(&orientation, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0))));

	/* Read this right to left */
	/* vT = worldMatrix * modelMatrix * vT */
	vector3_multiplym4(&startingPosition, &modelMatrix);
	vector3_multiplym4(&startingPosition, &worldMatrix);

	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_transform_3d_combined(void)
{
	struct quaternion orientation;
	struct matrix4 worldMatrix, scaleM, rotateM, translateM;

	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector3 scale = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1)}};
	struct vector3 expectedPosition = {.v = {-HYP_FLOAT_C(1.1), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 translation = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	matrix4_identity(&worldMatrix);
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_scalingv3(&scaleM, &scale));
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_translationv3(&translateM, &translation));
	matrix4_multiply(&worldMatrix,
			 matrix4_make_transformation_rotationq(&rotateM,
							       quaternion_set_from_axis_anglev3(&orientation, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0))));

	vector3_multiplym4(&startingPosition, &worldMatrix);

	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_transform_3d_scale_translate(void)
{
	struct matrix4 worldMatrix, scaleM, translateM;

	struct vector3 startingPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector3 scale = {.v = {HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1), HYP_FLOAT_C(0.1)}};
	struct vector3 expectedPosition = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.1), HYP_FLOAT_C(0.0)}};
	struct vector3 translation = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	matrix4_identity(&worldMatrix);
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_scalingv3(&scaleM, &scale));
	matrix4_multiply(&worldMatrix, matrix4_make_transformation_translationv3(&translateM, &translation));

	vector3_multiplym4(&startingPosition, &worldMatrix);

	test_assert(vector3_equals(&startingPosition, &expectedPosition));

	return NULL;
}


static const char *test_matrix4_determinant_row_is_zero(void)
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


static const char *test_matrix4_inverse(void)
{
	struct matrix4 originalMatrix;
	struct matrix4 identity;
	struct matrix4 inverted;
	struct vector3 scratchVector;
	struct matrix4 scratchMatrix;
	void *hasInverse = NULL;

	matrix4_identity(&identity);

	matrix4_identity(&originalMatrix);
	matrix4_multiply(&originalMatrix, matrix4_make_transformation_scalingv3(&scratchMatrix, vector3_setf3(&scratchVector, HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5))));
	matrix4_multiply(&originalMatrix, matrix4_make_transformation_translationv3(&scratchMatrix, vector3_setf3(&scratchVector, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.8), HYP_FLOAT_C(0.3))));
	matrix4_multiply(&originalMatrix, matrix4_set_from_euler_anglesf3(&scratchMatrix, HYP_TAU / HYP_FLOAT_C(4.0), HYP_TAU / HYP_FLOAT_C(4.0), HYP_TAU / HYP_FLOAT_C(4.0)));

	hasInverse = matrix4_invert(matrix4_set(&inverted, &originalMatrix));

	test_assert(hasInverse);

	matrix4_identity(&scratchMatrix);
	matrix4_multiply(&scratchMatrix, &inverted);
	matrix4_multiply(&scratchMatrix, &originalMatrix);

	test_assert(matrix4_equals(&identity, &scratchMatrix));

	return NULL;
}


static const char *test_matrix4_add(void)
{
	struct matrix4 m1 = {.m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};
	struct matrix4 m2 = {.m = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};
	struct matrix4 expected = {.m = {17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17}};

	matrix4_add(&m1, &m2);
	test_assert(matrix4_equals(&m1, &expected));

	return NULL;
}


static const char *test_matrix4_subtract(void)
{
	struct matrix4 m1 = {.m = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};
	struct matrix4 m2 = {.m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};
	struct matrix4 expected = {.m = {15, 13, 11, 9, 7, 5, 3, 1, -1, -3, -5, -7, -9, -11, -13, -15}};

	matrix4_subtract(&m1, &m2);
	test_assert(matrix4_equals(&m1, &expected));

	return NULL;
}


static const char *test_matrix4_multiplyf(void)
{
	struct matrix4 m = {.m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};
	struct matrix4 expected = {.m = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32}};

	matrix4_multiplyf(&m, HYP_FLOAT_C(2.0));
	test_assert(matrix4_equals(&m, &expected));

	return NULL;
}


static const char *test_matrix4_multiplyf_zero(void)
{
	struct matrix4 m = {.m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};
	struct matrix4 expected;

	matrix4_zero(&expected);

	matrix4_multiplyf(&m, HYP_FLOAT_C(0.0));
	test_assert(matrix4_equals(&m, &expected));

	return NULL;
}


static const char *test_matrix4_inverse_nonmutating(void)
{
	struct matrix4 original;
	struct matrix4 originalCopy;
	struct matrix4 inv;
	struct matrix4 product;
	struct matrix4 identity;
	struct vector3 scratchVector;
	struct matrix4 scratchMatrix;
	void *result;

	matrix4_identity(&identity);

	matrix4_identity(&original);
	matrix4_multiply(&original, matrix4_make_transformation_scalingv3(&scratchMatrix, vector3_setf3(&scratchVector, HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5))));
	matrix4_multiply(&original, matrix4_make_transformation_translationv3(&scratchMatrix, vector3_setf3(&scratchVector, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.8), HYP_FLOAT_C(0.3))));

	matrix4_set(&originalCopy, &original);

	result = matrix4_inverse(&original, &inv);
	test_assert(result);

	/* original should not be modified */
	test_assert(matrix4_equals(&original, &originalCopy));

	/* original * inv should be identity */
	matrix4_set(&product, &original);
	matrix4_multiply(&product, &inv);
	test_assert(matrix4_equals(&product, &identity));

	return NULL;
}


static const char *test_matrix4_inverse_small_scale(void)
{
	struct matrix4 m;
	struct matrix4 inverse;
	struct matrix4 identity;
	struct vector3 scale;

	/* the fourth diagonal entry stays 1, so the determinant is 1e-6 */
	matrix4_make_transformation_scalingv3(&m, vector3_setf3(&scale, HYP_FLOAT_C(0.01), HYP_FLOAT_C(0.01), HYP_FLOAT_C(0.01)));

	matrix4_identity(&identity);

	/* the determinant is far below HYP_EPSILON, but the matrix is invertible */
	test_assert(matrix4_inverse(&m, &inverse) == &inverse);
	test_assert(matrix4_equals(matrix4_multiply(&m, &inverse), &identity));

	return NULL;
}


static const char *test_matrix4_inverse_zero_scale(void)
{
	struct matrix4 m;
	struct matrix4 original;
	struct matrix4 result;

	/* a zero scale on one axis: the determinant is exactly zero */
	matrix4_identity(&m);
	m.r11 = 0;

	test_assert(!matrix4_inverse(&m, &result));

	/* invert returns NULL for a matrix without an inverse and leaves it
	 * unchanged
	 */
	matrix4_set(&original, &m);
	test_assert(matrix4_invert(&m) == NULL);
	test_assert(matrix4_equals(&m, &original));

	return NULL;
}

static const char *test_matrix4_inverse_multiple_row(void)
{
	/* the second row is twice the first: the determinant is exactly zero */
	struct matrix4 m = {.m = {1, 2, 3, 4, 2, 4, 6, 8, 0, 0, 1, 0, 0, 0, 0, 1}};
	struct matrix4 result;

	test_assert(!matrix4_inverse(&m, &result));

	return NULL;
}


static const char *test_matrix4_inverse_singular(void)
{
	struct matrix4 singular;
	struct matrix4 result;
	void *ret;

	matrix4_zero(&singular);

	ret = matrix4_inverse(&singular, &result);
	test_assert(!ret);

	return NULL;
}


static const char *test_matrix4_translatev3(void)
{
	struct matrix4 m;
	struct vector3 translation = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 v = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}};
	struct vector3 expected = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(9.0)}};

	matrix4_identity(&m);
	matrix4_translatev3(&m, &translation);
	vector3_multiplym4(&v, &m);
	test_assert(vector3_equals(&v, &expected));

	return NULL;
}


static const char *test_matrix4_scalev3(void)
{
	struct matrix4 m;
	struct vector3 scale = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 expected = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(12.0)}};

	matrix4_identity(&m);
	matrix4_scalev3(&m, &scale);
	vector3_multiplym4(&v, &m);
	test_assert(vector3_equals(&v, &expected));

	return NULL;
}


static const char *test_matrix4_reciprocal_condition(void)
{
	struct matrix4 identity = {.m = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};
	struct matrix4 scaled = {.m = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};
	struct matrix4 singular = {.m = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct matrix4 nearly_singular = {.m = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.5), HYP_FLOAT_C(1.5), HYP_FLOAT_C(0.5), HYP_FLOAT_C(0.5001)}};

	test_assert(scalar_equalsf(matrix4_reciprocal_condition(&identity), HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(matrix4_reciprocal_condition(&scaled), HYP_FLOAT_C(0.5)));
	test_assert(scalar_equalsf(matrix4_reciprocal_condition(&singular), HYP_FLOAT_C(0.0)));
	test_assert(matrix4_reciprocal_condition(&nearly_singular) < HYP_FLOAT_C(0.001));
	test_assert(matrix4_reciprocal_condition(&nearly_singular) > HYP_FLOAT_C(0.0));

	return NULL;
}


static const char *test_matrix4_set_from_axis_angle_any_length(void)
{
	struct matrix4 m, identity;
	struct vector3 axis, v;

	/* an axis of length 10: a quarter turn about Z takes X to Y */
	vector3_setf3(&axis, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0));
	matrix4_set_from_axisv3_angle(&m, &axis, HYP_TAU / HYP_FLOAT_C(4.0));
	vector3_multiplym4(vector3_set(&v, HYP_VECTOR3_UNIT_X), &m);
	test_assert(vector3_equals(&v, HYP_VECTOR3_UNIT_Y));

	/* a zero axis: the identity */
	vector3_zero(&axis);
	matrix4_set_from_axisv3_angle(&m, &axis, HYP_TAU / HYP_FLOAT_C(4.0));
	test_assert(matrix4_equals(&m, matrix4_identity(&identity)));

	return NULL;
}


static const char *test_matrix4_inverse_ill_conditioned(void)
{
	/* the inverses were computed in long double; the error is measured
	 * relative to the largest element
	 */
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	/* condition number about 1e3 */
	static const struct matrix4 m = {.m = {
		HYP_FLOAT_C(0.0884399), HYP_FLOAT_C(0.137102), HYP_FLOAT_C(-0.360966), HYP_FLOAT_C(0.232208),
		HYP_FLOAT_C(0.091032), HYP_FLOAT_C(0.14769), HYP_FLOAT_C(-0.201904), HYP_FLOAT_C(0.233368),
		HYP_FLOAT_C(0.151817), HYP_FLOAT_C(0.221668), HYP_FLOAT_C(-0.384721), HYP_FLOAT_C(0.369558),
		HYP_FLOAT_C(0.137144), HYP_FLOAT_C(0.224133), HYP_FLOAT_C(-0.345436), HYP_FLOAT_C(0.358889)}};
	static const struct matrix4 expected = {.m = {
		HYP_FLOAT_C(54.13173437), HYP_FLOAT_C(367.8325088), HYP_FLOAT_C(31.52293775), HYP_FLOAT_C(-306.6678178),
		HYP_FLOAT_C(105.6770032), HYP_FLOAT_C(525.7416262), HYP_FLOAT_C(-89.04681015), HYP_FLOAT_C(-318.5451444),
		HYP_FLOAT_C(-13.47666289), HYP_FLOAT_C(-31.06593676), HYP_FLOAT_C(5.060489878), HYP_FLOAT_C(23.70939038),
		HYP_FLOAT_C(-99.65440438), HYP_FLOAT_C(-498.798669), HYP_FLOAT_C(48.43620642), HYP_FLOAT_C(341.7329656)}};
	const HYP_FLOAT tolerance = HYP_FLOAT_C(1e-4);
	const HYP_FLOAT largest = HYP_FLOAT_C(525.7416262);
#else
	/* condition number about 2e6 */
	static const struct matrix4 m = {.m = {
		HYP_FLOAT_C(0.296374), HYP_FLOAT_C(0.247638), HYP_FLOAT_C(-0.250437), HYP_FLOAT_C(0.172543),
		HYP_FLOAT_C(0.315098), HYP_FLOAT_C(0.276445), HYP_FLOAT_C(-0.277747), HYP_FLOAT_C(0.181612),
		HYP_FLOAT_C(0.313319), HYP_FLOAT_C(0.263054), HYP_FLOAT_C(-0.265772), HYP_FLOAT_C(0.18224),
		HYP_FLOAT_C(0.269044), HYP_FLOAT_C(0.227544), HYP_FLOAT_C(-0.229843), HYP_FLOAT_C(0.156244)}};
	static const struct matrix4 expected = {.m = {
		HYP_FLOAT_C(505853.3934), HYP_FLOAT_C(86940.40829), HYP_FLOAT_C(-341953.3394), HYP_FLOAT_C(-260831.1801),
		HYP_FLOAT_C(-56393.04431), HYP_FLOAT_C(-8861.098324), HYP_FLOAT_C(41926.07496), HYP_FLOAT_C(23673.86224),
		HYP_FLOAT_C(49632.39313), HYP_FLOAT_C(9374.479657), HYP_FLOAT_C(-29253.89652), HYP_FLOAT_C(-31585.27627),
		HYP_FLOAT_C(-715913.9766), HYP_FLOAT_C(-123011.744), HYP_FLOAT_C(484733.2641), HYP_FLOAT_C(368203.349)}};
	const HYP_FLOAT tolerance = HYP_FLOAT_C(1e-7);
	const HYP_FLOAT largest = HYP_FLOAT_C(715913.9766);
#endif
	struct matrix4 inverse;
	uint8_t i;

	test_assert(matrix4_inverse(&m, &inverse) != NULL);
	for (i = 0; i < 16; i++) {
		test_assert(HYP_ABS(inverse.m[i] - expected.m[i]) < tolerance * largest);
	}

	return NULL;
}


static const char *matrix4_all_tests(void)
{
	run_test(test_matrix4_inverse_ill_conditioned);
	run_test(test_matrix4_set_from_axis_angle_any_length);
	run_test(test_matrix4_reciprocal_condition);
	run_test(test_matrix4_zero);
	run_test(test_matrix4_equals);
	run_test(test_matrix4_multiply_identity);
	run_test(test_matrix4_multiplym4);
	run_test(test_matrix4_identity_multiply);
	run_test(test_matrix4_multiply_associative);
	run_test(test_matrix4_multiply_distributive);
	run_test(test_matrix4_multiply_not_commutative);
	run_test(test_matrix4_columnrowcolumn);
	run_test(test_matrix4_transpose);
	run_test(test_matrix4_determinant_trial1);
	run_test(test_matrix4_determinant_trial2);
	run_test(test_matrix4_determinant_trial3);

	run_test(test_matrix4_identity_with_vector2);
	run_test(test_matrix4_multiplyv2_ignores_z);
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
	run_test(test_matrix4_normal_matrix);
	run_test(test_matrix4_rotation_builders_agree);
	run_test(test_matrix4_transform_3d);
	run_test(test_matrix4_transform_3d_combined);

	run_test(test_matrix4_inverse);
	run_test(test_matrix4_determinant_row_is_zero);

	run_test(test_matrix4_add);
	run_test(test_matrix4_subtract);
	run_test(test_matrix4_multiplyf);
	run_test(test_matrix4_multiplyf_zero);
	run_test(test_matrix4_inverse_nonmutating);
	run_test(test_matrix4_inverse_singular);
	run_test(test_matrix4_inverse_small_scale);
	run_test(test_matrix4_inverse_zero_scale);
	run_test(test_matrix4_inverse_multiple_row);
	run_test(test_matrix4_translatev3);
	run_test(test_matrix4_scalev3);

	return NULL;
}
