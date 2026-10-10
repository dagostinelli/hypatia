/* SPDX-License-Identifier: MIT */

#ifndef TEST_RANDOM_SOURCE_H_
#define TEST_RANDOM_SOURCE_H_

/* The tests replace HYP_RANDOM with test_random_source so they can replay a
 * scripted sequence.  Once the script is used up it falls back to a fixed
 * generator that gives the same sequence on every platform.
 */
#define TEST_RANDOM_MAX 2147483647L

/* marks these functions for export or import when they are built into a
 * Windows DLL (the shared library tests)
 */
#ifndef TEST_RANDOM_API
#	define TEST_RANDOM_API
#endif

TEST_RANDOM_API void test_random_script(const long *values, int count);
TEST_RANDOM_API long test_random_source(void);

#endif /* TEST_RANDOM_SOURCE_H_ */
