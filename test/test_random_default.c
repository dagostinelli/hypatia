/* SPDX-License-Identifier: MIT */

/* Tests scalar_random_rangef with the default HYP_RANDOM (rand()), which the
 * main test program replaces with a scripted source.  Built with
 * HYP_NO_DEPRECATED to check that the header compiles without its deprecated
 * parts.
 */

#define HYP_NO_DEPRECATED
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include <hypatia.h>

#ifdef HYP_RANDOM_FLOAT
#	error "HYP_NO_DEPRECATED should remove HYP_RANDOM_FLOAT"
#endif

#ifdef matrix4_view_lookat_rh_EXP
#	error "HYP_NO_DEPRECATED should remove the _EXP names of promoted functions"
#endif

/* with HYP_NO_DEPRECATED the deprecated function names are free: if they
 * were still declared, these would not compile
 */
enum deprecated_names {
	quaternion_cross_product_EXP,
	quaternion_axis_between_EXP
};

#define DRAWS 100000

static int scalar_random_rangef_stays_in_range(HYP_FLOAT min, HYP_FLOAT max)
{
	HYP_FLOAT r;
	int i;

	for (i = 0; i < DRAWS; i++) {
		r = scalar_random_rangef(min, max);
		if (!(r >= min) || !(r < max)) {
			return 0;
		}
	}

	return 1;
}

static int scalar_random_rangef_spreads_evenly(void)
{
	int counts[10];
	HYP_FLOAT r;
	HYP_FLOAT edge;
	int bucket;
	int i;

	for (i = 0; i < 10; i++) {
		counts[i] = 0;
	}

	for (i = 0; i < DRAWS; i++) {
		r = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0));

		bucket = 0;
		edge = HYP_FLOAT_C(0.1);
		while (bucket < 9 && r >= edge) {
			bucket++;
			edge += HYP_FLOAT_C(0.1);
		}

		counts[bucket]++;
	}

	/* each tenth of [0, 1) should get 10% +/- 2% of the draws */
	for (i = 0; i < 10; i++) {
		if (counts[i] < DRAWS / 100 * 8 || counts[i] > DRAWS / 100 * 12) {
			return 0;
		}
	}

	return 1;
}

int main(void)
{
	if (!scalar_random_rangef_stays_in_range(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0))) {
		printf("scalar_random_rangef(0, 1) out of range\n");
		return 1;
	}

	if (!scalar_random_rangef_stays_in_range(HYP_FLOAT_C(-2.0), HYP_FLOAT_C(3.0))) {
		printf("scalar_random_rangef(-2, 3) out of range\n");
		return 1;
	}

	if (!scalar_random_rangef_spreads_evenly()) {
		printf("scalar_random_rangef(0, 1) is not evenly spread\n");
		return 1;
	}

	printf("ALL TESTS PASSED\n");
	return 0;
}
