/* SPDX-License-Identifier: MIT */

#include <stdio.h>
#include <float.h> /* LDBL_MAX_EXP */
#include <hypatia.h>

/* the 15-bit exponent of x87 extended and IEEE quad, which reaches about
 * 1e4932: _Float128, and long double where it is one of the two; elsewhere
 * long double has the range of double
 */
#if defined(HYPATIA_QUAD_PRECISION_FLOATS) || (defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS) && LDBL_MAX_EXP >= 16384)
#	define TEST_WIDE_RANGE
#endif

#define UNUSED_VARIABLE(x) ((void)(x))

#include "unittest.h"

#include "test_vector2.c"
#include "test_vector3.c"
#include "test_vector4.c"
#include "test_quaternion.c"
#include "test_matrix2.c"
#include "test_matrix3.c"
#include "test_matrix4.c"
#include "test_experimental.c"
#include "test_utility.c"
#include "test_integration.c"

int tests_run;
const char *test_message;

static const char *all_testsuites(void)
{
	printf("quaternion_all_tests\n");
	run_test(quaternion_all_tests);
	printf("matrix2_all_tests\n");
	run_test(matrix2_all_tests);
	printf("matrix3_all_tests\n");
	run_test(matrix3_all_tests);
	printf("matrix4_all_tests\n");
	run_test(matrix4_all_tests);
	printf("vector2_all_tests\n");
	run_test(vector2_all_tests);
	printf("vector3_all_tests\n");
	run_test(vector3_all_tests);
	printf("vector4_all_tests\n");
	run_test(vector4_all_tests);
	printf("experimental_all_tests\n");
	run_test(experimental_all_tests);
	printf("utility_all_tests\n");
	run_test(utility_all_tests);
	printf("integration_all_tests\n");
	run_test(integration_all_tests);

	return NULL;
}

int main(int argc, char *argv[])
{
	const char *result;

	UNUSED_VARIABLE(argc);
	UNUSED_VARIABLE(argv);

	tests_run = 0;
	test_message = NULL;

	result = all_testsuites();
	if (result) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);

	return result ? 1 : 0;
}
