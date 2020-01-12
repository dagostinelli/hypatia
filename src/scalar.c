/* SPDX-License-Identifier: MIT */

#include "hypatia.h"


/**
 * @brief This checks for mathematical equality within HYP_EPSILON.
 *
 */
HYPAPI short scalar_equalsf(const HYP_FLOAT f1, const HYP_FLOAT f2)
{
	return scalar_equals_epsilonf(f1, f2, HYP_EPSILON);
}

/**
 * @brief This checks for mathematical equality within a custom epsilon.
 *
 */
HYPAPI short scalar_equals_epsilonf(const HYP_FLOAT f1, const HYP_FLOAT f2, const HYP_FLOAT epsilon)
{
	if ((HYP_ABS(f1 - f2) < epsilon) == 0)
	{
		return 0;
	}

	return 1;
}
