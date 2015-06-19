#include "hypatia.h"
#include "internal.h"


/**
 * @brief This checks for mathematical equality within EPSILON.
 *
 */
HYPAPI int scalar_equalsf(const float f1, const float f2)
{
	return HYP_ABS(f1 - f2) < HYP_EPSILON;
}
