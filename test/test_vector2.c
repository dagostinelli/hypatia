/* SPDX-License-Identifier: MIT */

static const char *test_vector2_set(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct vector2 v2;

	vector2_set(&v2, &v1);
	test_assert(scalar_equalsf(v2.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v2.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_setf2(void)
{
	struct vector2 v;

	vector2_setf2(&v, HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(6.0)));

	return NULL;
}

static const char *test_vector2_zero(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};

	vector2_zero(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_equals(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v3 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0)}};

	test_assert(vector2_equals(&v1, &v2));
	test_assert(!vector2_equals(&v1, &v3));

	return NULL;
}

static const char *test_vector2_negate(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0)}};

	vector2_negate(&v);
	test_assert(scalar_equalsf(v.x, -HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_add(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector2_add(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(6.0)));

	return NULL;
}

static const char *test_vector2_addf(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};

	vector2_addf(&v, HYP_FLOAT_C(5.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(7.0)));

	return NULL;
}

static const char *test_vector2_subtract(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};

	vector2_subtract(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_subtractf(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0)}};

	vector2_subtractf(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector2_multiply(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}};

	vector2_multiply(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(8.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(15.0)));

	return NULL;
}

static const char *test_vector2_multiplyf(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector2_multiplyf(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(8.0)));

	return NULL;
}

static const char *test_vector2_divide(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(8.0), HYP_FLOAT_C(15.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}};

	vector2_divide(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(3.0)));

	return NULL;
}

static const char *test_vector2_dividef(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(6.0), HYP_FLOAT_C(8.0)}};

	vector2_dividef(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_magnitude(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	test_assert(scalar_equalsf(vector2_magnitude(&v), HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector2_normalize(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector2_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.6)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.8)));
	test_assert(scalar_equalsf(vector2_magnitude(&v), HYP_FLOAT_C(1.0)));

	return NULL;
}

static const char *test_vector2_distance(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(6.0)}};

	/* distance = sqrt(9 + 16) = 5 */
	test_assert(scalar_equalsf(vector2_distance(&v1, &v2), HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector2_dot_product(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	/* 1*3 + 2*4 = 11 */
	test_assert(scalar_equalsf(vector2_dot_product(&v1, &v2), HYP_FLOAT_C(11.0)));

	return NULL;
}

static const char *test_vector2_dot_product_perpendicular(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};

	test_assert(scalar_equalsf(vector2_dot_product(&v1, &v2), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_cross_product(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};

	/* 1*1 - 0*0 = 1 */
	test_assert(scalar_equalsf(vector2_cross_product(&v1, &v2), HYP_FLOAT_C(1.0)));

	/* reversed: 0*0 - 1*1 = -1 (anti-commutative) */
	test_assert(scalar_equalsf(vector2_cross_product(&v2, &v1), -HYP_FLOAT_C(1.0)));

	/* parallel vectors: 1*0 - 0*1 = 0 */
	test_assert(scalar_equalsf(vector2_cross_product(&v1, &v1), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_normalize_zero(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	/* vector2_normalize does not guard against zero magnitude,
	 * so this produces NaN via 0/0. Verify it does not crash.
	 */
	vector2_normalize(&v);

	/* Result is NaN (0/0 in IEEE 754), so scalar_equalsf with 0 is false */
	test_assert(!scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(!scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_magnitude_zero(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector2_magnitude(&v), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_angle_between_perpendicular(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_PI_HALF));
	return NULL;
}

static const char *test_vector2_angle_between_same(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_vector2_angle_between_opposite(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_PI));
	return NULL;
}

static const char *vector2_all_tests(void)
{
	run_test(test_vector2_set);
	run_test(test_vector2_setf2);
	run_test(test_vector2_zero);
	run_test(test_vector2_equals);
	run_test(test_vector2_negate);
	run_test(test_vector2_add);
	run_test(test_vector2_addf);
	run_test(test_vector2_subtract);
	run_test(test_vector2_subtractf);
	run_test(test_vector2_multiply);
	run_test(test_vector2_multiplyf);
	run_test(test_vector2_divide);
	run_test(test_vector2_dividef);
	run_test(test_vector2_magnitude);
	run_test(test_vector2_normalize);
	run_test(test_vector2_normalize_zero);
	run_test(test_vector2_magnitude_zero);
	run_test(test_vector2_distance);
	run_test(test_vector2_dot_product);
	run_test(test_vector2_dot_product_perpendicular);
	run_test(test_vector2_cross_product);
	run_test(test_vector2_angle_between_perpendicular);
	run_test(test_vector2_angle_between_same);
	run_test(test_vector2_angle_between_opposite);

	return NULL;
}
