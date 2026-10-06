/* SPDX-License-Identifier: MIT */

/* The math functions for the HYP_NO_C_MATH build.  This file does not include
 * hypatia.h; only this file includes <math.h>.
 */
#include <math.h>
#include "no_c_math.h"

double test_sqrt(double x)
{
	return sqrt(x);
}

double test_fmod(double x, double y)
{
	return fmod(x, y);
}

double test_sin(double x)
{
	return sin(x);
}

double test_cos(double x)
{
	return cos(x);
}

double test_tan(double x)
{
	return tan(x);
}

double test_asin(double x)
{
	return asin(x);
}

double test_acos(double x)
{
	return acos(x);
}

double test_atan2(double y, double x)
{
	return atan2(y, x);
}
