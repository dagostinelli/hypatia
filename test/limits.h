/* SPDX-License-Identifier: MIT */

#ifndef TEST_LIMITS_H
#define TEST_LIMITS_H

/* the limits of HYP_FLOAT, found by arithmetic that is rounded through a
 * volatile: excess precision (x87) and multiplies fused into adds would
 * otherwise hide an overflow or a subnormal
 */
static HYP_FLOAT test_rounded(HYP_FLOAT x)
{
	volatile HYP_FLOAT rounded = x;

	return rounded;
}

/* the largest finite power of two */
static HYP_FLOAT test_largest_power2(void)
{
	HYP_FLOAT largest = HYP_FLOAT_C(1.0);
	HYP_FLOAT next = HYP_FLOAT_C(2.0);

	while (next - next <= HYP_FLOAT_C(0.0)) {
		largest = next;
		next = test_rounded(largest * HYP_FLOAT_C(2.0));
	}

	return largest;
}

/* 1 when the power of two x is subnormal: x (1 + epsilon) rounds to x */
static int test_is_subnormal(HYP_FLOAT x)
{
	HYP_FLOAT epsilon = HYP_FLOAT_C(1.0);

	while (test_rounded(HYP_FLOAT_C(1.0) + epsilon / HYP_FLOAT_C(2.0)) > HYP_FLOAT_C(1.0)) {
		epsilon /= HYP_FLOAT_C(2.0);
	}

	return !(test_rounded(x * (HYP_FLOAT_C(1.0) + epsilon)) > x);
}

#endif
