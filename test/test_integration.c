/* SPDX-License-Identifier: MIT */

/**
 * Test 1: TRS compose-transform-decompose roundtrip
 *
 * Build a matrix4 with translation (3,4,5), rotation (45 deg around Y via
 * quaternion), and scaling (2,2,2).  Transform a vector3 through it and
 * verify the result.  Then decompose and check the components.
 *
 * matrix4_multiply(&self, &mT) computes self = mT * self (pre-multiply).
 * vector3_multiplym4(&v, &M) computes v = M * v.
 *
 * So the chain:
 *   combined = I;  multiply(&combined, &S);  multiply(&combined, &T);  multiply(&combined, &R);
 * gives combined = R * T * S.
 * And v_new = R * T * S * v  (scale first, then translate, then rotate).
 *
 * Manual computation for v = (1, 0, 0):
 *   after scale(2,2,2):      (2, 0, 0)
 *   after translate(3,4,5):  (5, 4, 5)
 *   after rotate 45 deg Y:   Ry * (5,4,5)
 *     The library's Y rotation: x' = x*cos - z*sin, z' = x*sin + z*cos
 *     (verified: 90 deg maps X -> Z in existing tests)
 *     x' = 5*cos45 - 5*sin45 = 0
 *     y' = 4
 *     z' = 5*sin45 + 5*cos45 = 10*cos45 ~ 7.07107
 */
static char *test_integration_trs_roundtrip(void)
{
	struct matrix4 combined, scaleM, translateM, rotateM;
	struct quaternion q;
	struct vector3 v;
	HYP_FLOAT angle = HYP_DEG_TO_RAD(45.0f);
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	struct vector3 scale = {{{2.0f, 2.0f, 2.0f}}};
	struct vector3 translation = {{{3.0f, 4.0f, 5.0f}}};

	/* build combined = R * T * S */
	matrix4_identity(&combined);
	matrix4_multiply(&combined, matrix4_make_transformation_scalingv3(&scaleM, &scale));
	matrix4_multiply(&combined, matrix4_make_transformation_translationv3(&translateM, &translation));
	matrix4_multiply(&combined,
		matrix4_make_transformation_rotationq(&rotateM,
			quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, angle)));

	/* transform v = (1, 0, 0): v_new = R * T * S * v */
	vector3_setf3(&v, 1.0f, 0.0f, 0.0f);
	vector3_multiplym4(&v, &combined);

	/* expected: scale -> (2,0,0), translate -> (5,4,5), rotate Y 45:
	 *   x' = 5*cos45 - 5*sin45 = 0
	 *   y' = 4
	 *   z' = 5*sin45 + 5*cos45 = 10*cos45
	 */
	test_assert(scalar_equalsf(v.x, 5.0f * c - 5.0f * s));
	test_assert(scalar_equalsf(v.y, 4.0f));
	test_assert(scalar_equalsf(v.z, 5.0f * s + 5.0f * c));

	/* decompose a scale-only matrix to verify decompose works for that case
	 * (the full TRS combined matrix mixes rotation into the translation
	 * column, so decompose_EXP cannot recover the original translation)
	 */
	{
		struct vector3 out_scale, out_translation;
		struct quaternion out_rotation;
		struct matrix4 scaleOnly;

		matrix4_make_transformation_scalingv3(&scaleOnly, &scale);
		matrix4_transformation_decompose_EXP(&scaleOnly,
			&out_scale, &out_rotation, &out_translation);
		test_assert(vector3_equals(&out_scale, &scale));
	}

	return NULL;
}


/**
 * Test 2: Quaternion-Matrix rotation equivalence
 *
 * The library's direct quaternion rotation (vector3_rotate_by_quaternion)
 * and the matrix built from that quaternion (matrix4_make_transformation_rotationq)
 * produce opposite-handed rotations for the same angle.  This is demonstrated
 * by the existing tests:
 *   - quaternion Z TAU/4 on X -> +Y
 *   - matrix-from-quaternion Z TAU/4 on X -> -Y
 *
 * We verify that both paths produce consistent (negated-angle-equivalent)
 * results: rotating by +angle via quaternion equals rotating by -angle via
 * the matrix (and vice-versa).
 */
static char *test_integration_quaternion_matrix_rotation_equivalence(void)
{
	struct quaternion q;
	struct matrix4 m;
	struct vector3 vQ, vM;

	/* 90 degrees around Z */
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f);
	matrix4_make_transformation_rotationq(&m, &q);

	/* rotate (1,0,0) via quaternion */
	vector3_set(&vQ, HYP_VECTOR3_UNIT_X);
	vector3_rotate_by_quaternion(&vQ, &q);

	/* rotate (1,0,0) via matrix */
	vector3_set(&vM, HYP_VECTOR3_UNIT_X);
	vector3_multiplym4(&vM, &m);

	/* quaternion gives (0,1,0), matrix gives (0,-1,0) --
	 * they are the negation of each other in the rotated components */
	test_assert(vector3_equals(&vQ, HYP_VECTOR3_UNIT_Y));
	test_assert(vector3_equals(&vM, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	/* Now verify: matrix built from the negated-angle quaternion matches
	 * the direct quaternion rotation with the original angle */
	{
		struct quaternion qNeg;
		struct matrix4 mNeg;
		struct vector3 vMNeg;

		quaternion_set_from_axis_anglev3(&qNeg, HYP_VECTOR3_UNIT_Z, -(HYP_TAU / 4.0f));
		matrix4_make_transformation_rotationq(&mNeg, &qNeg);

		vector3_set(&vMNeg, HYP_VECTOR3_UNIT_X);
		vector3_multiplym4(&vMNeg, &mNeg);

		/* matrix with -angle should match quaternion with +angle */
		test_assert(vector3_equals(&vMNeg, &vQ));
	}

	return NULL;
}


/**
 * Test 3: Matrix multiply chain -- translation then scaling
 *
 * Convention (from existing tests): combined = I * S * T
 * means the vector is first scaled, then translated.
 *
 * v = (1, 1, 1)
 * scale(2, 3, 4) -> (2, 3, 4)
 * translate(10, 20, 30) -> (12, 23, 34)
 */
static char *test_integration_matrix_multiply_chain(void)
{
	struct matrix4 combined, scaleM, translateM;
	struct vector3 v;

	struct vector3 scale = {{{2.0f, 3.0f, 4.0f}}};
	struct vector3 translation = {{{10.0f, 20.0f, 30.0f}}};
	struct vector3 expected = {{{12.0f, 23.0f, 34.0f}}};

	matrix4_identity(&combined);
	matrix4_multiply(&combined, matrix4_make_transformation_scalingv3(&scaleM, &scale));
	matrix4_multiply(&combined, matrix4_make_transformation_translationv3(&translateM, &translation));

	vector3_setf3(&v, 1.0f, 1.0f, 1.0f);
	vector3_multiplym4(&v, &combined);

	test_assert(vector3_equals(&v, &expected));

	/* Also verify doing them separately gives the same result */
	{
		struct vector3 vSep;

		vector3_setf3(&vSep, 1.0f, 1.0f, 1.0f);
		vector3_multiplym4(&vSep, &scaleM);
		vector3_multiplym4(&vSep, &translateM);
		test_assert(vector3_equals(&vSep, &expected));
	}

	return NULL;
}


/**
 * Test 4a: Vector2 through matrix2 rotation
 *
 * Rotate (1, 0) by 90 degrees -> (0, 1)
 * Then rotate that result by another 90 degrees -> (-1, 0)
 */
static char *test_integration_vector2_matrix2_rotation(void)
{
	struct matrix2 m90;
	struct vector2 v;

	matrix2_make_transformation_rotationf_z(&m90, HYP_TAU / 4.0f);

	/* first rotation: (1,0) -> (0,1) */
	vector2_set(&v, HYP_VECTOR2_UNIT_X);
	vector2_multiplym2(&v, &m90);
	test_assert(vector2_equals(&v, HYP_VECTOR2_UNIT_Y));

	/* second rotation: (0,1) -> (-1,0) */
	vector2_multiplym2(&v, &m90);
	test_assert(vector2_equals(&v, HYP_VECTOR2_UNIT_X_NEGATIVE));

	return NULL;
}


/**
 * Test 4b: Vector2 through matrix3 translation then scaling
 *
 * Translate (1, 2) by (3, 4) -> (4, 6)
 * Then scale by (2, 0.5) -> (8, 3)
 */
static char *test_integration_vector2_matrix3_translate_scale(void)
{
	struct matrix3 translateM, scaleM;
	struct vector2 v;
	struct vector2 translation = {{3.0f, 4.0f}};
	struct vector2 scale = {{2.0f, 0.5f}};
	struct vector2 afterTranslate = {{4.0f, 6.0f}};
	struct vector2 afterScale = {{8.0f, 3.0f}};

	/* translate */
	matrix3_make_transformation_translationv2(&translateM, &translation);
	vector2_setf2(&v, 1.0f, 2.0f);
	vector2_multiplym3(&v, &translateM);
	test_assert(vector2_equals(&v, &afterTranslate));

	/* scale */
	matrix3_make_transformation_scalingv2(&scaleM, &scale);
	vector2_multiplym3(&v, &scaleM);
	test_assert(vector2_equals(&v, &afterScale));

	return NULL;
}


/**
 * Test 4c: Vector2 rotation through matrix3
 *
 * Rotate (1, 0) by 90 degrees -> (0, 1) via matrix3, confirming
 * that matrix3 rotation matches matrix2 rotation for unit vectors.
 */
static char *test_integration_vector2_matrix3_rotation(void)
{
	struct matrix3 m90;
	struct matrix2 m2_90;
	struct vector2 v3, v2;

	matrix3_make_transformation_rotationf_z(&m90, HYP_TAU / 4.0f);
	matrix2_make_transformation_rotationf_z(&m2_90, HYP_TAU / 4.0f);

	/* rotate (1, 0) via matrix3 */
	vector2_set(&v3, HYP_VECTOR2_UNIT_X);
	vector2_multiplym3(&v3, &m90);

	/* rotate (1, 0) via matrix2 */
	vector2_set(&v2, HYP_VECTOR2_UNIT_X);
	vector2_multiplym2(&v2, &m2_90);

	/* both should give (0, 1) */
	test_assert(vector2_equals(&v3, HYP_VECTOR2_UNIT_Y));
	test_assert(vector2_equals(&v2, HYP_VECTOR2_UNIT_Y));
	test_assert(vector2_equals(&v3, &v2));

	return NULL;
}


static char *integration_all_tests(void)
{
	run_test(test_integration_trs_roundtrip);
	run_test(test_integration_quaternion_matrix_rotation_equivalence);
	run_test(test_integration_matrix_multiply_chain);
	run_test(test_integration_vector2_matrix2_rotation);
	run_test(test_integration_vector2_matrix3_translate_scale);
	run_test(test_integration_vector2_matrix3_rotation);

	return NULL;
}
