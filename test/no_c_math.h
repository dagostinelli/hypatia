/* SPDX-License-Identifier: MIT */

#ifndef TEST_NO_C_MATH_H_
#define TEST_NO_C_MATH_H_

/* math functions the program supplies itself, for the HYP_NO_C_MATH build;
 * long double in long double precision, _Float128 in quad precision, double
 * otherwise
 */
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
typedef long double test_math_float;
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
typedef _Float128 test_math_float;
#else
typedef double test_math_float;
#endif

test_math_float test_sqrt(test_math_float x);
test_math_float test_fmod(test_math_float x, test_math_float y);
test_math_float test_sin(test_math_float x);
test_math_float test_cos(test_math_float x);
test_math_float test_tan(test_math_float x);
test_math_float test_asin(test_math_float x);
test_math_float test_acos(test_math_float x);
test_math_float test_atan2(test_math_float y, test_math_float x);

#endif /* TEST_NO_C_MATH_H_ */
