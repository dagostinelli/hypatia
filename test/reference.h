/* SPDX-License-Identifier: MIT */

#ifndef TEST_REFERENCE_H_
#define TEST_REFERENCE_H_

/* References in double for the single precision tests that check precision
 * to the ulp.  Without <math.h>: the HYP_NO_C_MATH build runs them too.
 */
#if defined(HYPATIA_SINGLE_PRECISION_FLOATS) && !defined(HYP_FAST)

/* sqrt in double: Newton steps from the float square root */
static double reference_sqrt(double x)
{
	double y = HYP_SQRT((HYP_FLOAT)x);
	int i;

	if (!(x > 0.0)) {
		return 0.0;
	}
	for (i = 0; i < 3; i++) {
		y = 0.5 * (y + x / y);
	}

	return y;
}


/* the spacing of floats at |x|, for normal floats */
static double reference_float_ulp(double x)
{
	double a = x < 0.0 ? -x : x;
	double power = 1.0;

	while (power <= a) {
		power *= 2.0;
	}
	while (power / 2.0 > a) {
		power /= 2.0;
	}

	/* a is in [power / 2, power): the spacing is power 2^-24 */
	return power / 16777216.0;
}


/* a fixed sequence of numbers in [0, 1) */
static double reference_random(void)
{
	static unsigned long state = 12345;

	state = (state * 1103515245UL + 12345UL) & 0x7fffffffUL;
	return (double)state / 2147483648.0;
}

#endif

#endif /* TEST_REFERENCE_H_ */
