/* SPDX-License-Identifier: MIT */

#ifndef TEST_NO_C_MATH_H_
#define TEST_NO_C_MATH_H_

/* math functions the program supplies itself, for the HYP_NO_C_MATH build;
 * long double in long double precision, the type hypatia.h picks in quad
 * precision (with the same tests), double otherwise
 */
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
typedef long double test_math_float;
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
#	if defined(__FLT128_MANT_DIG__) && !defined(__clang__)
#		define TEST_QUAD_FUNCTION(name) name ## f128
typedef _Float128 test_math_float;
#	elif defined(__LDBL_MANT_DIG__) && __LDBL_MANT_DIG__ == 113
#		define TEST_QUAD_FUNCTION(name) name ## l
typedef long double test_math_float;
#	else
#		define TEST_QUAD_FUNCTION(name) name ## q
typedef __float128 test_math_float;
#	endif
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
