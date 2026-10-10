/* SPDX-License-Identifier: MIT */

/* The whole test suite with HYP_NO_C_MATH: hypatia.h does not include <math.h>
 * and the program supplies every math macro.  The macros have the same form as
 * the library's defaults.
 */
#define HYP_NO_C_MATH

#include "no_c_math.h"

#ifdef HYPATIA_LONG_DOUBLE_PRECISION_FLOATS
#	define HYP_SQRT(x) test_sqrt(x)
#	define HYP_FMOD(x, y) test_fmod(x, y)
#	define HYP_SIN(x) test_sin(x)
#	define HYP_COS(x) test_cos(x)
#	define HYP_TAN(x) test_tan(x)
#	define HYP_ASIN(x) test_asin(x)
#	define HYP_ACOS(x) test_acos(x)
#	define HYP_ATAN2(y, x) test_atan2(y, x)
#else
#	define HYP_SQRT(x) ((HYP_FLOAT)test_sqrt(x))
#	define HYP_FMOD(x, y) ((HYP_FLOAT)test_fmod(x, y))
#	define HYP_SIN(x) ((HYP_FLOAT)test_sin(x))
#	define HYP_COS(x) ((HYP_FLOAT)test_cos(x))
#	define HYP_TAN(x) ((HYP_FLOAT)test_tan(x))
#	define HYP_ASIN(x) ((HYP_FLOAT)test_asin(x))
#	define HYP_ACOS(x) ((HYP_FLOAT)test_acos(x))
#	define HYP_ATAN2(y, x) ((HYP_FLOAT)test_atan2(y, x))
#endif

#include "implementation.c"
#include "main.c"
