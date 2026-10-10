#include "random_source.h"

#define HYP_RANDOM() test_random_source()
#define HYP_RANDOM_MAX TEST_RANDOM_MAX

/* included once without the implementation first, as through another header,
 * to check that the implementation is still compiled in below
 */
#include <hypatia.h>

#define HYPATIA_IMPLEMENTATION
#include <hypatia.h>

static const long *script_values;
static int script_count;

/* Park-Miller "minimal standard" generator, computed with Schrage's method so
 * every intermediate value fits in a 32-bit long.  It covers 1 .. 2^31 - 2 and
 * gives the same sequence on every platform, unlike rand().
 */
static long generator_state = 1;

static long test_random_generator(void)
{
	const long a = 48271;
	const long m = 2147483647L;
	const long q = m / a;
	const long r = m % a;
	long hi;
	long lo;
	long next;

	hi = generator_state / q;
	lo = generator_state % q;
	next = a * lo - r * hi;
	if (next <= 0) {
		next += m;
	}

	generator_state = next;
	return next;
}

void test_random_script(const long *values, int count)
{
	script_values = values;
	script_count = count;
}

long test_random_source(void)
{
	if (script_count > 0) {
		script_count--;
		return *script_values++;
	}

	return test_random_generator();
}
