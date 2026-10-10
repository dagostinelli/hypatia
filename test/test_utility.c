/* SPDX-License-Identifier: MIT */

#include "random_source.h"

static const char *test_hyp_min_first_smaller(void)
{
	test_assert(scalar_equalsf(HYP_MIN(HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)), HYP_FLOAT_C(1.0)));
	return NULL;
}

static const char *test_hyp_min_second_smaller(void)
{
	test_assert(scalar_equalsf(HYP_MIN(HYP_FLOAT_C(5.0), HYP_FLOAT_C(3.0)), HYP_FLOAT_C(3.0)));
	return NULL;
}

static const char *test_hyp_min_equal(void)
{
	test_assert(scalar_equalsf(HYP_MIN(HYP_FLOAT_C(4.0), HYP_FLOAT_C(4.0)), HYP_FLOAT_C(4.0)));
	return NULL;
}

static const char *test_hyp_min_negative(void)
{
	test_assert(scalar_equalsf(HYP_MIN(-HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0)), -HYP_FLOAT_C(1.0)));
	return NULL;
}

static const char *test_hyp_max_first_larger(void)
{
	test_assert(scalar_equalsf(HYP_MAX(HYP_FLOAT_C(5.0), HYP_FLOAT_C(3.0)), HYP_FLOAT_C(5.0)));
	return NULL;
}

static const char *test_hyp_max_second_larger(void)
{
	test_assert(scalar_equalsf(HYP_MAX(HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)), HYP_FLOAT_C(2.0)));
	return NULL;
}

static const char *test_hyp_max_equal(void)
{
	test_assert(scalar_equalsf(HYP_MAX(HYP_FLOAT_C(4.0), HYP_FLOAT_C(4.0)), HYP_FLOAT_C(4.0)));
	return NULL;
}

static const char *test_hyp_max_negative(void)
{
	test_assert(scalar_equalsf(HYP_MAX(-HYP_FLOAT_C(5.0), -HYP_FLOAT_C(1.0)), -HYP_FLOAT_C(1.0)));
	return NULL;
}

static const char *test_hyp_swap_basic(void)
{
	HYP_FLOAT a = HYP_FLOAT_C(1.0), b = HYP_FLOAT_C(2.0);
	HYP_SWAP(&a, &b);
	test_assert(scalar_equalsf(a, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(b, HYP_FLOAT_C(1.0)));
	return NULL;
}

static const char *test_hyp_swap_same_value(void)
{
	HYP_FLOAT a = HYP_FLOAT_C(5.0), b = HYP_FLOAT_C(5.0);
	HYP_SWAP(&a, &b);
	test_assert(scalar_equalsf(a, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(b, HYP_FLOAT_C(5.0)));
	return NULL;
}

static const char *test_hyp_swap_negatives(void)
{
	HYP_FLOAT a = -HYP_FLOAT_C(3.0), b = -HYP_FLOAT_C(7.0);
	HYP_SWAP(&a, &b);
	test_assert(scalar_equalsf(a, -HYP_FLOAT_C(7.0)));
	test_assert(scalar_equalsf(b, -HYP_FLOAT_C(3.0)));
	return NULL;
}

static const char *test_hyp_square_positive(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(HYP_FLOAT_C(3.0)), HYP_FLOAT_C(9.0)));
	return NULL;
}

static const char *test_hyp_square_negative(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(-HYP_FLOAT_C(4.0)), HYP_FLOAT_C(16.0)));
	return NULL;
}

static const char *test_hyp_square_zero(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(HYP_FLOAT_C(0.0)), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_hyp_square_fractional(void)
{
	test_assert(scalar_equalsf(HYP_SQUARE(HYP_FLOAT_C(0.5)), HYP_FLOAT_C(0.25)));
	return NULL;
}

static const char *test_hyp_abs_positive(void)
{
	test_assert(scalar_equalsf(HYP_ABS(HYP_FLOAT_C(5.0)), HYP_FLOAT_C(5.0)));
	return NULL;
}

static const char *test_hyp_abs_negative(void)
{
	test_assert(scalar_equalsf(HYP_ABS(-HYP_FLOAT_C(5.0)), HYP_FLOAT_C(5.0)));
	return NULL;
}

static const char *test_hyp_abs_zero(void)
{
	test_assert(scalar_equalsf(HYP_ABS(HYP_FLOAT_C(0.0)), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_hyp_clamp_in_range(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(5.0)));
	return NULL;
}

static const char *test_hyp_clamp_below_range(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_hyp_clamp_above_range(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(HYP_FLOAT_C(15.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(10.0)));
	return NULL;
}

static const char *test_hyp_clamp_at_lower_boundary(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_hyp_clamp_at_upper_boundary(void)
{
	test_assert(scalar_equalsf(HYP_CLAMP(HYP_FLOAT_C(10.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(10.0)));
	return NULL;
}

static const char *test_hyp_wrap_in_range(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(5.0)));
	return NULL;
}

static const char *test_hyp_wrap_above_limit(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(HYP_FLOAT_C(12.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(2.0)));
	return NULL;
}

static const char *test_hyp_wrap_below_start(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(-HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(8.0)));
	return NULL;
}

static const char *test_hyp_wrap_at_limit(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(HYP_FLOAT_C(10.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_hyp_wrap_nonzero_start_above_limit(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(HYP_FLOAT_C(12.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(7.0)));
	return NULL;
}

static const char *test_hyp_wrap_nonzero_start_below_start(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(HYP_FLOAT_C(3.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(10.0)), HYP_FLOAT_C(8.0)));
	return NULL;
}

static const char *test_hyp_wrap_negative_start(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(-HYP_FLOAT_C(370.0), -HYP_FLOAT_C(180.0), HYP_FLOAT_C(180.0)), -HYP_FLOAT_C(10.0)));
	test_assert(scalar_equalsf(HYP_WRAP(HYP_FLOAT_C(190.0), -HYP_FLOAT_C(180.0), HYP_FLOAT_C(180.0)), -HYP_FLOAT_C(170.0)));
	return NULL;
}

static const char *test_hyp_wrap_just_below_start(void)
{
	/* -1e-16 + 10 rounds to exactly 10 in both precisions */
	HYP_FLOAT r = HYP_WRAP(-HYP_FLOAT_C(1e-16), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0));

	test_assert(r >= HYP_FLOAT_C(0.0) && r < HYP_FLOAT_C(10.0));
	return NULL;
}

static const char *test_hyp_wrap_zero_range(void)
{
	test_assert(scalar_equalsf(HYP_WRAP(HYP_FLOAT_C(7.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(3.0)), HYP_FLOAT_C(3.0)));
	return NULL;
}

static const char *test_hyp_deg_to_rad(void)
{
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_FLOAT_C(0.0)), HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_FLOAT_C(90.0)), HYP_PI_HALF));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_FLOAT_C(180.0)), HYP_PI));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_FLOAT_C(360.0)), HYP_FLOAT_C(2.0) * HYP_PI));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(-HYP_FLOAT_C(90.0)), -HYP_PI_HALF));
	return NULL;
}

static const char *test_hyp_rad_to_deg(void)
{
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_FLOAT_C(0.0)), HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_PI_HALF), HYP_FLOAT_C(90.0)));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_PI), HYP_FLOAT_C(180.0)));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_FLOAT_C(2.0) * HYP_PI), HYP_FLOAT_C(360.0)));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(-HYP_PI), -HYP_FLOAT_C(180.0)));
	return NULL;
}

static const char *test_hyp_deg_rad_roundtrip(void)
{
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_RAD_TO_DEG(HYP_FLOAT_C(1.0))), HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(HYP_DEG_TO_RAD(HYP_RAD_TO_DEG(HYP_PI)), HYP_PI));
	test_assert(scalar_equalsf(HYP_RAD_TO_DEG(HYP_DEG_TO_RAD(HYP_FLOAT_C(45.0))), HYP_FLOAT_C(45.0)));
	return NULL;
}

/* exact comparison without tripping -Wfloat-equal */
static int floats_identical(HYP_FLOAT a, HYP_FLOAT b)
{
	return !(a < b) && !(b < a);
}

static const char *test_hyp_constants_full_precision(void)
{
	/* each constant must be the closest HYP_FLOAT to the true value */
	test_assert(floats_identical(HYP_PI, HYP_FLOAT_C(3.141592653589793238462643383279502884197)));
	test_assert(floats_identical(HYP_TAU, HYP_FLOAT_C(6.283185307179586476925286766559005768394)));
	test_assert(floats_identical(HYP_PI_HALF, HYP_FLOAT_C(1.570796326794896619231321691639751442099)));
	test_assert(floats_identical(HYP_PI_SQUARED, HYP_FLOAT_C(9.869604401089358618834490999876151135314)));
	test_assert(floats_identical(HYP_E, HYP_FLOAT_C(2.718281828459045235360287471352662497757)));
	test_assert(floats_identical(HYP_RAD_PER_DEG, HYP_FLOAT_C(0.01745329251994329576923690768488612713443)));
	test_assert(floats_identical(HYP_DEG_PER_RAD, HYP_FLOAT_C(57.29577951308232087679815481410517033241)));
	return NULL;
}

static const char *test_scalar_random_rangef_scripted_values(void)
{
	static const long values[] = {0, 1073741824L};

	/* 0 maps to 0 and the midpoint maps to exactly 0.5 */
	test_random_script(values, 2);
	test_assert(floats_identical(scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)), HYP_FLOAT_C(0.0)));
	test_assert(floats_identical(scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)), HYP_FLOAT_C(0.5)));
	test_random_script(NULL, 0);
	return NULL;
}

static const char *test_scalar_random_rangef_below_one(void)
{
	static const long values[] = {TEST_RANDOM_MAX, 0};
	HYP_FLOAT r;

	/* in single precision TEST_RANDOM_MAX rounds to exactly 1 and is drawn again */
	test_random_script(values, 2);
	r = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0));
	test_assert(r >= HYP_FLOAT_C(0.0));
	test_assert(r < HYP_FLOAT_C(1.0));
	test_random_script(NULL, 0);
	return NULL;
}

static const char *test_scalar_random_rangef_bounds(void)
{
	static const long low[] = {0};
	static const long high[] = {TEST_RANDOM_MAX, 0};
	/* 2^31 - 2^11 gives r = 1 - 2^-20, exactly representable in single precision */
	static const long near_one[] = {2147481600L, 0};
	HYP_FLOAT r;

	test_random_script(low, 1);
	test_assert(floats_identical(scalar_random_rangef(HYP_FLOAT_C(-1.0), HYP_FLOAT_C(1.0)), HYP_FLOAT_C(-1.0)));

	test_random_script(high, 2);
	r = scalar_random_rangef(HYP_FLOAT_C(-1.0), HYP_FLOAT_C(1.0));
	test_assert(r >= HYP_FLOAT_C(-1.0));
	test_assert(r < HYP_FLOAT_C(1.0));

	/* in single precision 1000 + (1001 - 1000) * r rounds up to 1001 and is drawn again */
	test_random_script(near_one, 2);
	r = scalar_random_rangef(HYP_FLOAT_C(1000.0), HYP_FLOAT_C(1001.0));
	test_assert(r >= HYP_FLOAT_C(1000.0));
	test_assert(r < HYP_FLOAT_C(1001.0));

	test_random_script(NULL, 0);
	return NULL;
}

static const char *test_scalar_random_rangef_empty_range(void)
{
	test_assert(floats_identical(scalar_random_rangef(HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0)), HYP_FLOAT_C(2.0)));
	test_assert(floats_identical(scalar_random_rangef(HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.0)), HYP_FLOAT_C(3.0)));
	return NULL;
}

static const char *test_scalar_random_rangef_many_draws_in_range(void)
{
	HYP_FLOAT r;
	int i;

	for (i = 0; i < 10000; i++) {
		r = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0));
		test_assert(r >= HYP_FLOAT_C(0.0));
		test_assert(r < HYP_FLOAT_C(1.0));

		r = scalar_random_rangef(HYP_FLOAT_C(-1.0), HYP_FLOAT_C(1.0));
		test_assert(r >= HYP_FLOAT_C(-1.0));
		test_assert(r < HYP_FLOAT_C(1.0));
	}

	return NULL;
}

static const char *test_scalar_random_rangef_infinite_range(void)
{
	/* max - min can overflow (on x87 it may not): the call returns, in range */
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	HYP_FLOAT huge = HYP_FLOAT_C(3e38);
#elif defined(TEST_WIDE_RANGE)
	HYP_FLOAT huge = HYP_FLOAT_C(1e4932);
#else
	HYP_FLOAT huge = HYP_FLOAT_C(1e308);
#endif
	HYP_FLOAT r = scalar_random_rangef(-huge, huge);

	test_assert(r >= -huge && r < huge);

	return NULL;
}


static const char *test_scalar_equalsf(void)
{
	volatile HYP_FLOAT zero = HYP_FLOAT_C(0.0);
	HYP_FLOAT infinity = HYP_FLOAT_C(1.0) / zero;
	HYP_FLOAT nan = zero / zero;

	/* up to 1 in size the tolerance is HYP_EPSILON itself */
	test_assert(scalar_equalsf(HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.000005)));
	test_assert(!scalar_equalsf(HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.00002)));
	test_assert(scalar_equalsf(HYP_FLOAT_C(0.0), HYP_FLOAT_C(5e-6)));

	/* above 1 it is relative: large values one rounding apart are equal */
	test_assert(scalar_equalsf(HYP_FLOAT_C(1e6), HYP_FLOAT_C(1e6) + HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(-HYP_FLOAT_C(1e30), -HYP_FLOAT_C(1.000001e30)));
	test_assert(!scalar_equalsf(HYP_FLOAT_C(1e6), HYP_FLOAT_C(1.0001e6)));
	test_assert(!scalar_equalsf(HYP_FLOAT_C(1e6), -HYP_FLOAT_C(1e6)));

	/* infinities equal only themselves; NaN equals nothing */
	test_assert(scalar_equalsf(infinity, infinity));
	test_assert(!scalar_equalsf(infinity, -infinity));
	test_assert(!scalar_equalsf(infinity, HYP_FLOAT_C(1e30)));
	test_assert(!scalar_equalsf(nan, nan));
	test_assert(!scalar_equalsf(nan, HYP_FLOAT_C(1.0)));

	/* a custom epsilon */
	test_assert(scalar_equals_epsilonf(HYP_FLOAT_C(100.0), HYP_FLOAT_C(100.5), HYP_FLOAT_C(0.01)));
	test_assert(!scalar_equals_epsilonf(HYP_FLOAT_C(100.0), HYP_FLOAT_C(102.0), HYP_FLOAT_C(0.01)));

	return NULL;
}


static const char *utility_all_tests(void)
{
	run_test(test_scalar_random_rangef_infinite_range);
	run_test(test_scalar_equalsf);
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
	run_test(test_hyp_wrap_at_limit);
	run_test(test_hyp_wrap_nonzero_start_above_limit);
	run_test(test_hyp_wrap_nonzero_start_below_start);
	run_test(test_hyp_wrap_negative_start);
	run_test(test_hyp_wrap_just_below_start);
	run_test(test_hyp_wrap_zero_range);
	run_test(test_hyp_deg_to_rad);
	run_test(test_hyp_rad_to_deg);
	run_test(test_hyp_deg_rad_roundtrip);
	run_test(test_hyp_constants_full_precision);
	run_test(test_scalar_random_rangef_scripted_values);
	run_test(test_scalar_random_rangef_below_one);
	run_test(test_scalar_random_rangef_bounds);
	run_test(test_scalar_random_rangef_empty_range);
	run_test(test_scalar_random_rangef_many_draws_in_range);

	return NULL;
}
