/* SPDX-License-Identifier: MIT */

/* The math functions for the HYP_NO_C_MATH build.  This file does not include
 * hypatia.h; only this file includes <math.h>.  In long double precision they
 * call the C99 long double functions.
 */
#include <math.h>
#include "no_c_math.h"

test_math_float test_sqrt(test_math_float x)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return sqrtl(x);
#else
	return sqrt(x);
#endif
}

test_math_float test_fmod(test_math_float x, test_math_float y)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return fmodl(x, y);
#else
	return fmod(x, y);
#endif
}

test_math_float test_sin(test_math_float x)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return sinl(x);
#else
	return sin(x);
#endif
}

test_math_float test_cos(test_math_float x)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return cosl(x);
#else
	return cos(x);
#endif
}

test_math_float test_tan(test_math_float x)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return tanl(x);
#else
	return tan(x);
#endif
}

test_math_float test_asin(test_math_float x)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return asinl(x);
#else
	return asin(x);
#endif
}

test_math_float test_acos(test_math_float x)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return acosl(x);
#else
	return acos(x);
#endif
}

test_math_float test_atan2(test_math_float y, test_math_float x)
{
#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
	return atan2l(y, x);
#else
	return atan2(y, x);
#endif
}
