/* SPDX-License-Identifier: MIT */

static char *test_hyp_min_first_smaller(void)
{
	test_assert(scalar_equalsf(HYP_MIN(1.0f, 2.0f), 1.0f));
	return NULL;
}

static char *test_hyp_min_second_smaller(void)
{
	test_assert(scalar_equalsf(HYP_MIN(5.0f, 3.0f), 3.0f));
	return NULL;
}

static char *test_hyp_min_equal(void)
{
	test_assert(scalar_equalsf(HYP_MIN(4.0f, 4.0f), 4.0f));
	return NULL;
}

static char *test_hyp_min_negative(void)
{
	test_assert(scalar_equalsf(HYP_MIN(-1.0f, 1.0f), -1.0f));
	return NULL;
}

static char *test_hyp_max_first_larger(void)
{
	test_assert(scalar_equalsf(HYP_MAX(5.0f, 3.0f), 5.0f));
	return NULL;
}

static char *test_hyp_max_second_larger(void)
{
	test_assert(scalar_equalsf(HYP_MAX(1.0f, 2.0f), 2.0f));
	return NULL;
}

static char *test_hyp_max_equal(void)
{
	test_assert(scalar_equalsf(HYP_MAX(4.0f, 4.0f), 4.0f));
	return NULL;
}

static char *test_hyp_max_negative(void)
{
	test_assert(scalar_equalsf(HYP_MAX(-5.0f, -1.0f), -1.0f));
	return NULL;
}

static char *test_hyp_swap_basic(void)
{
	HYP_FLOAT a = 1.0f, b = 2.0f;
	HYP_SWAP(&a, &b);
	test_assert(scalar_equalsf(a, 2.0f));
	test_assert(scalar_equalsf(b, 1.0f));
	return NULL;
}

static char *test_hyp_swap_same_value(void)
{
	HYP_FLOAT a = 5.0f, b = 5.0f;
	HYP_SWAP(&a, &b);
	test_assert(scalar_equalsf(a, 5.0f));
	test_assert(scalar_equalsf(b, 5.0f));
	return NULL;
}

static char *test_hyp_swap_negatives(void)
{
	HYP_FLOAT a = -3.0f, b = -7.0f;
	HYP_SWAP(&a, &b);
	test_assert(scalar_equalsf(a, -7.0f));
	test_assert(scalar_equalsf(b, -3.0f));
	return NULL;
}

static char *test_hyp_square_positive(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(3.0f), 9.0f));
	return NULL;
}

static char *test_hyp_square_negative(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(-4.0f), 16.0f));
	return NULL;
}

static char *test_hyp_square_zero(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(0.0f), 0.0f));
	return NULL;
}

static char *test_hyp_square_fractional(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(0.5f), 0.25f));
	return NULL;
}

static char *test_hyp_abs_positive(void)
{
	test_assert(scalar_equalsf(HYP_ABS(5.0f), 5.0f));
	return NULL;
}

static char *test_hyp_abs_negative(void)
{
	test_assert(scalar_equalsf(HYP_ABS(-5.0f), 5.0f));
	return NULL;
}

static char *test_hyp_abs_zero(void)
{
	test_assert(scalar_equalsf(HYP_ABS(0.0f), 0.0f));
	return NULL;
}

static char *test_hyp_clamp_in_range(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(5.0f, 0.0f, 10.0f), 5.0f));
	return NULL;
}

static char *test_hyp_clamp_below_range(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(-1.0f, 0.0f, 10.0f), 0.0f));
	return NULL;
}

static char *test_hyp_clamp_above_range(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(15.0f, 0.0f, 10.0f), 10.0f));
	return NULL;
}

static char *test_hyp_clamp_at_lower_boundary(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(0.0f, 0.0f, 10.0f), 0.0f));
	return NULL;
}

static char *test_hyp_clamp_at_upper_boundary(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(10.0f, 0.0f, 10.0f), 10.0f));
	return NULL;
}

static char *test_hyp_wrap_in_range(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(5.0f, 0.0f, 10.0f), 5.0f));
	return NULL;
}

static char *test_hyp_wrap_above_limit(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(12.0f, 0.0f, 10.0f), 2.0f));
	return NULL;
}

static char *test_hyp_wrap_below_start(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(-2.0f, 0.0f, 10.0f), -2.0f));
	return NULL;
}

static char *test_hyp_deg_to_rad(void)
{
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(0.0f), 0.0f));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(90.0f), HYP_PI_HALF));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(180.0f), HYP_PI));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(360.0f), 2.0f * HYP_PI));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(-90.0f), -HYP_PI_HALF));
	return NULL;
}

static char *test_hyp_rad_to_deg(void)
{
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(0.0f), 0.0f));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_PI_HALF), 90.0f));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_PI), 180.0f));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(2.0f * HYP_PI), 360.0f));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(-HYP_PI), -180.0f));
	return NULL;
}

static char *test_hyp_deg_rad_roundtrip(void)
{
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_RAD_TO_DEG(1.0f)), 1.0f));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_RAD_TO_DEG(HYP_PI)), HYP_PI));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_DEG_TO_RAD(45.0f)), 45.0f));
	return NULL;
}

static char *utility_all_tests(void)
{
	run_test(test_hyp_min_first_smaller);
	run_test(test_hyp_min_second_smaller);
	run_test(test_hyp_min_equal);
	run_test(test_hyp_min_negative);
	run_test(test_hyp_max_first_larger);
	run_test(test_hyp_max_second_larger);
	run_test(test_hyp_max_equal);
	run_test(test_hyp_max_negative);
	run_test(test_hyp_swap_basic);
	run_test(test_hyp_swap_same_value);
	run_test(test_hyp_swap_negatives);
	run_test(test_hyp_square_positive);
	run_test(test_hyp_square_negative);
	run_test(test_hyp_square_zero);
	run_test(test_hyp_square_fractional);
	run_test(test_hyp_abs_positive);
	run_test(test_hyp_abs_negative);
	run_test(test_hyp_abs_zero);
	run_test(test_hyp_clamp_in_range);
	run_test(test_hyp_clamp_below_range);
	run_test(test_hyp_clamp_above_range);
	run_test(test_hyp_clamp_at_lower_boundary);
	run_test(test_hyp_clamp_at_upper_boundary);
	run_test(test_hyp_wrap_in_range);
	run_test(test_hyp_wrap_above_limit);
	run_test(test_hyp_wrap_below_start);
	run_test(test_hyp_deg_to_rad);
	run_test(test_hyp_rad_to_deg);
	run_test(test_hyp_deg_rad_roundtrip);

	return NULL;
}
