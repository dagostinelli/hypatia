/* SPDX-License-Identifier: MIT */

#ifndef TEST_NO_C_MATH_H_
#define TEST_NO_C_MATH_H_

/* math functions the program supplies itself, for the HYP_NO_C_MATH build */
double test_sqrt(double x);
double test_fmod(double x, double y);
double test_sin(double x);
double test_cos(double x);
double test_tan(double x);
double test_asin(double x);
double test_acos(double x);
double test_atan2(double y, double x);

#endif /* TEST_NO_C_MATH_H_ */
