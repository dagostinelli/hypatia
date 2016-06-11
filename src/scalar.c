#include "hypatia.h"
#include "internal.h"


/**
 * @brief This checks for mathematical equality within EPSILON.
 *
 */
HYPAPI short scalar_equalsf(const HYP_FLOAT f1, const HYP_FLOAT f2)
{
	if ((HYP_ABS(f1 - f2) < HYP_EPSILON) == 0)
	{
		return 0;
	}

	return 1;
}
