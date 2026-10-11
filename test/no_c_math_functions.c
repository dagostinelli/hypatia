/* SPDX-License-Identifier: MIT */

/* The math functions for the HYP_NO_C_MATH build.  This file does not include
 * hypatia.h; only this file includes <math.h>.  In long double precision they
 * call the C99 long double functions, in quad precision the functions of the
 * type hypatia.h picks: sinf128, sinl or sinq.
 */
#if defined(HYPATIA_QUAD_PRECISION_FLOATS) && defined(__FLT128_MANT_DIG__) && !defined(__clang__)
#	define __STDC_WANT_IEC_60559_TYPES_EXT__
#endif
#include <math.h>
#include "no_c_math.h"

/* the choice hypatia.h makes: _Float128 where the C library has sinf128 and
 * the others, else a 113-bit long double, else __float128 and libquadmath
 */
#ifdef HYPATIA_QUAD_PRECISION_FLOATS
#	if defined(__FLT128_MANT_DIG__) && !defined(__clang__) && defined(HUGE_VAL_F128)
#		define TEST_QUAD_FUNCTION(name) name ## f128
#	elif defined(__LDBL_MANT_DIG__) && __LDBL_MANT_DIG__ == 113
#		define TEST_QUAD_FUNCTION(name) name ## l
#	else
#		define TEST_QUAD_FUNCTION(name) name ## q
#		define TEST_QUADMATH
#	endif
#endif

#ifdef TEST_QUADMATH
/* libquadmath, as declared in hypatia.h */
extern __float128 sqrtq(__float128 x);
extern __float128 fmodq(__float128 x, __float128 y);
extern __float128 sinq(__float128 x);
extern __float128 cosq(__float128 x);
extern __float128 tanq(__float128 x);
extern __float128 asinq(__float128 x);
extern __float128 acosq(__float128 x);
extern __float128 atan2q(__float128 y, __float128 x);
#endif

test_math_float test_sqrt(test_math_float x)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return sqrtl(x);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(sqrt)(x);
#else
	return sqrt(x);
#endif
}

test_math_float test_fmod(test_math_float x, test_math_float y)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return fmodl(x, y);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(fmod)(x, y);
#else
	return fmod(x, y);
#endif
}

test_math_float test_sin(test_math_float x)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return sinl(x);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(sin)(x);
#else
	return sin(x);
#endif
}

test_math_float test_cos(test_math_float x)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return cosl(x);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(cos)(x);
#else
	return cos(x);
#endif
}

test_math_float test_tan(test_math_float x)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return tanl(x);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(tan)(x);
#else
	return tan(x);
#endif
}

test_math_float test_asin(test_math_float x)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return asinl(x);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(asin)(x);
#else
	return asin(x);
#endif
}

test_math_float test_acos(test_math_float x)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return acosl(x);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(acos)(x);
#else
	return acos(x);
#endif
}

test_math_float test_atan2(test_math_float y, test_math_float x)
{
#if defined(HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	return atan2l(y, x);
#elif defined(HYPATIA_QUAD_PRECISION_FLOATS)
	return TEST_QUAD_FUNCTION(atan2)(y, x);
#else
	return atan2(y, x);
#endif
}
