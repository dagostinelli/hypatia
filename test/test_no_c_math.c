/* SPDX-License-Identifier: MIT */

/* The whole test suite with HYP_NO_C_MATH: hypatia.h does not include <math.h>
 * and the program supplies every math macro.  The macros have the same form as
 * the library's defaults.
 */
#define HYP_NO_C_MATH

#include "no_c_math.h"

#define HYP_SQRT(x) ((HYP_FLOAT)test_sqrt(x))
#define HYP_FMOD(x, y) ((HYP_FLOAT)test_fmod(x, y))
#define HYP_SIN(x) ((HYP_FLOAT)test_sin(x))
#define HYP_COS(x) ((HYP_FLOAT)test_cos(x))
#define HYP_TAN(x) ((HYP_FLOAT)test_tan(x))
#define HYP_ASIN(x) ((HYP_FLOAT)test_asin(x))
#define HYP_ACOS(x) ((HYP_FLOAT)test_acos(x))
#define HYP_ATAN2(y, x) ((HYP_FLOAT)test_atan2(y, x))

#include "implementation.c"
#include "main.c"
