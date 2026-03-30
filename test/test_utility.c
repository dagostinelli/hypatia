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

	return NULL;
}
