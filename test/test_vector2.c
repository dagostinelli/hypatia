/* SPDX-License-Identifier: MIT */

static char *test_vector2_set(void)
{
	struct vector2 v1 = {{3.0f, 4.0f}};
	struct vector2 v2;

	vector2_set(&v2, &v1);
	test_assert(scalar_equalsf(v2.x, 3.0f));
	test_assert(scalar_equalsf(v2.y, 4.0f));

	return NULL;
}

static char *test_vector2_setf2(void)
{
	struct vector2 v;

	vector2_setf2(&v, 5.0f, 6.0f);
	test_assert(scalar_equalsf(v.x, 5.0f));
	test_assert(scalar_equalsf(v.y, 6.0f));

	return NULL;
}

static char *test_vector2_zero(void)
{
	struct vector2 v = {{1.0f, 2.0f}};

	vector2_zero(&v);
	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));

	return NULL;
}

static char *test_vector2_equals(void)
{
	struct vector2 v1 = {{1.0f, 2.0f}};
	struct vector2 v2 = {{1.0f, 2.0f}};
	struct vector2 v3 = {{1.0f, 3.0f}};

	test_assert(vector2_equals(&v1, &v2));
	test_assert(!vector2_equals(&v1, &v3));

	return NULL;
}

static char *test_vector2_negate(void)
{
	struct vector2 v = {{3.0f, -4.0f}};

	vector2_negate(&v);
	test_assert(scalar_equalsf(v.x, -3.0f));
	test_assert(scalar_equalsf(v.y, 4.0f));

	return NULL;
}

static char *test_vector2_add(void)
{
	struct vector2 v1 = {{1.0f, 2.0f}};
	struct vector2 v2 = {{3.0f, 4.0f}};

	vector2_add(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 4.0f));
	test_assert(scalar_equalsf(v1.y, 6.0f));

	return NULL;
}

static char *test_vector2_addf(void)
{
	struct vector2 v = {{1.0f, 2.0f}};

	vector2_addf(&v, 5.0f);
	test_assert(scalar_equalsf(v.x, 6.0f));
	test_assert(scalar_equalsf(v.y, 7.0f));

	return NULL;
}

static char *test_vector2_subtract(void)
{
	struct vector2 v1 = {{5.0f, 7.0f}};
	struct vector2 v2 = {{2.0f, 3.0f}};

	vector2_subtract(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 3.0f));
	test_assert(scalar_equalsf(v1.y, 4.0f));

	return NULL;
}

static char *test_vector2_subtractf(void)
{
	struct vector2 v = {{5.0f, 7.0f}};

	vector2_subtractf(&v, 2.0f);
	test_assert(scalar_equalsf(v.x, 3.0f));
	test_assert(scalar_equalsf(v.y, 5.0f));

	return NULL;
}

static char *test_vector2_multiply(void)
{
	struct vector2 v1 = {{2.0f, 3.0f}};
	struct vector2 v2 = {{4.0f, 5.0f}};

	vector2_multiply(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 8.0f));
	test_assert(scalar_equalsf(v1.y, 15.0f));

	return NULL;
}

static char *test_vector2_multiplyf(void)
{
	struct vector2 v = {{3.0f, 4.0f}};

	vector2_multiplyf(&v, 2.0f);
	test_assert(scalar_equalsf(v.x, 6.0f));
	test_assert(scalar_equalsf(v.y, 8.0f));

	return NULL;
}

static char *test_vector2_divide(void)
{
	struct vector2 v1 = {{8.0f, 15.0f}};
	struct vector2 v2 = {{4.0f, 5.0f}};

	vector2_divide(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 2.0f));
	test_assert(scalar_equalsf(v1.y, 3.0f));

	return NULL;
}

static char *test_vector2_dividef(void)
{
	struct vector2 v = {{6.0f, 8.0f}};

	vector2_dividef(&v, 2.0f);
	test_assert(scalar_equalsf(v.x, 3.0f));
	test_assert(scalar_equalsf(v.y, 4.0f));

	return NULL;
}

static char *test_vector2_magnitude(void)
{
	struct vector2 v = {{3.0f, 4.0f}};

	test_assert(scalar_equalsf(vector2_magnitude(&v), 5.0f));

	return NULL;
}

static char *test_vector2_normalize(void)
{
	struct vector2 v = {{3.0f, 4.0f}};

	vector2_normalize(&v);
	test_assert(scalar_equalsf(v.x, 0.6f));
	test_assert(scalar_equalsf(v.y, 0.8f));
	test_assert(scalar_equalsf(vector2_magnitude(&v), 1.0f));

	return NULL;
}

static char *test_vector2_distance(void)
{
	struct vector2 v1 = {{1.0f, 2.0f}};
	struct vector2 v2 = {{4.0f, 6.0f}};

	/* distance = sqrt(9 + 16) = 5 */
	test_assert(scalar_equalsf(vector2_distance(&v1, &v2), 5.0f));

	return NULL;
}

static char *test_vector2_dot_product(void)
{
	struct vector2 v1 = {{1.0f, 2.0f}};
	struct vector2 v2 = {{3.0f, 4.0f}};

	/* 1*3 + 2*4 = 11 */
	test_assert(scalar_equalsf(vector2_dot_product(&v1, &v2), 11.0f));

	return NULL;
}

static char *test_vector2_dot_product_perpendicular(void)
{
	struct vector2 v1 = {{1.0f, 0.0f}};
	struct vector2 v2 = {{0.0f, 1.0f}};

	test_assert(scalar_equalsf(vector2_dot_product(&v1, &v2), 0.0f));

	return NULL;
}

static char *test_vector2_cross_product(void)
{
	struct vector2 v1 = {{1.0f, 0.0f}};
	struct vector2 v2 = {{0.0f, 1.0f}};

	/* 1*1 - 0*0 = 1 */
	test_assert(scalar_equalsf(vector2_cross_product(&v1, &v2), 1.0f));

	/* reversed: 0*0 - 1*1 = -1 (anti-commutative) */
	test_assert(scalar_equalsf(vector2_cross_product(&v2, &v1), -1.0f));

	/* parallel vectors: 1*0 - 0*1 = 0 */
	test_assert(scalar_equalsf(vector2_cross_product(&v1, &v1), 0.0f));

	return NULL;
}

static char *test_vector2_angle_between_perpendicular(void)
{
	struct vector2 v1 = {{1.0f, 0.0f}};
	struct vector2 v2 = {{0.0f, 1.0f}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_PI_HALF));
	return NULL;
}

static char *test_vector2_angle_between_same(void)
{
	struct vector2 v1 = {{1.0f, 0.0f}};
	struct vector2 v2 = {{1.0f, 0.0f}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), 0.0f));
	return NULL;
}

static char *test_vector2_angle_between_opposite(void)
{
	struct vector2 v1 = {{1.0f, 0.0f}};
	struct vector2 v2 = {{-1.0f, 0.0f}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_PI));
	return NULL;
}

static char *vector2_all_tests(void)
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
	run_test(test_vector2_distance);
	run_test(test_vector2_dot_product);
	run_test(test_vector2_dot_product_perpendicular);
	run_test(test_vector2_cross_product);
	run_test(test_vector2_angle_between_perpendicular);
	run_test(test_vector2_angle_between_same);
	run_test(test_vector2_angle_between_opposite);

	return NULL;
}
