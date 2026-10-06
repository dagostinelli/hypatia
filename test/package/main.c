/* SPDX-License-Identifier: MIT */

#define HYPATIA_IMPLEMENTATION
#include <hypatia.h>

int main(void)
{
	struct vector3 v;

	/* vector3_magnitude calls sqrt, so the C math library must be linked */
	vector3_setf3(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(0.0));

	return scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(5.0)) ? 0 : 1;
}
