/* SPDX-License-Identifier: MIT */

/* <math.h> included before hypatia.h: hypatia.h picks the same floating point
 * type and math functions as when it comes first, and the program links with
 * the same libraries.
 */

#include <math.h>
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include <hypatia.h>

int main(void)
{
	struct vector3 v;
	HYP_FLOAT length;
	HYP_FLOAT sine;

	vector3_setf3(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(0.0));
	length = vector3_magnitude(&v);
	if (length < HYP_FLOAT_C(5.0) || length > HYP_FLOAT_C(5.0)) {
		printf("magnitude\n");
		return 1;
	}

	sine = HYP_SIN(HYP_PI_HALF);
	if (sine < HYP_FLOAT_C(1.0) || sine > HYP_FLOAT_C(1.0)) {
		printf("sine\n");
		return 1;
	}

	return 0;
}
