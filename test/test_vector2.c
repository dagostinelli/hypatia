/* SPDX-License-Identifier: MIT */

#include "random_source.h"

static const char *test_vector2_set(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct vector2 v2;

	vector2_set(&v2, &v1);
	test_assert(scalar_equalsf(v2.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v2.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_setf2(void)
{
	struct vector2 v;

	vector2_setf2(&v, HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(6.0)));

	return NULL;
}

static const char *test_vector2_zero(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};

	vector2_zero(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_equals(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v3 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0)}};

	test_assert(vector2_equals(&v1, &v2));
	test_assert(!vector2_equals(&v1, &v3));

	return NULL;
}

static const char *test_vector2_negate(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0)}};

	vector2_negate(&v);
	test_assert(scalar_equalsf(v.x, -HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_add(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector2_add(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(6.0)));

	return NULL;
}

static const char *test_vector2_addf(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};

	vector2_addf(&v, HYP_FLOAT_C(5.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(7.0)));

	return NULL;
}

static const char *test_vector2_subtract(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};

	vector2_subtract(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_subtractf(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0)}};

	vector2_subtractf(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector2_multiply(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}};

	vector2_multiply(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(8.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(15.0)));

	return NULL;
}

static const char *test_vector2_multiplyf(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector2_multiplyf(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(8.0)));

	return NULL;
}

static const char *test_vector2_divide(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(8.0), HYP_FLOAT_C(15.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}};

	vector2_divide(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(3.0)));

	return NULL;
}

static const char *test_vector2_dividef(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(6.0), HYP_FLOAT_C(8.0)}};

	vector2_dividef(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector2_magnitude(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	test_assert(scalar_equalsf(vector2_magnitude(&v), HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector2_normalize(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector2_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.6)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.8)));
	test_assert(scalar_equalsf(vector2_magnitude(&v), HYP_FLOAT_C(1.0)));

	return NULL;
}

static const char *test_vector2_distance(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(6.0)}};

	/* distance = sqrt(9 + 16) = 5 */
	test_assert(scalar_equalsf(vector2_distance(&v1, &v2), HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector2_dot_product(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	/* 1*3 + 2*4 = 11 */
	test_assert(scalar_equalsf(vector2_dot_product(&v1, &v2), HYP_FLOAT_C(11.0)));

	return NULL;
}

static const char *test_vector2_dot_product_perpendicular(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};

	test_assert(scalar_equalsf(vector2_dot_product(&v1, &v2), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_cross_product(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};

	/* 1*1 - 0*0 = 1 */
	test_assert(scalar_equalsf(vector2_cross_product(&v1, &v2), HYP_FLOAT_C(1.0)));

	/* reversed: 0*0 - 1*1 = -1 (anti-commutative) */
	test_assert(scalar_equalsf(vector2_cross_product(&v2, &v1), -HYP_FLOAT_C(1.0)));

	/* parallel vectors: 1*0 - 0*1 = 0 */
	test_assert(scalar_equalsf(vector2_cross_product(&v1, &v1), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_normalize_zero(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	/* the zero vector cannot be normalized and is left unchanged */
	vector2_normalize(&v);

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_magnitude_zero(void)
{
	struct vector2 v = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector2_magnitude(&v), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector2_angle_between_perpendicular(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_PI_HALF));
	return NULL;
}

static const char *test_vector2_angle_between_same(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_vector2_angle_between_opposite(void)
{
	struct vector2 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector2 v2 = {.v = {-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector2_angle_between(&v1, &v2), HYP_PI));
	return NULL;
}

static const char *test_vector2_set_random_unit_scripted(void)
{
	static const long zero[] = {0};
	static const long half[] = {1073741824L};
	struct vector2 v;
	struct vector2 expected;

	test_random_script(zero, 1);
	vector2_set_random_unit(&v);
	test_assert(vector2_equals(&v, vector2_setf2(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));

	/* angle pi: sin(pi) is close to 0, not exactly 0 */
	test_random_script(half, 1);
	vector2_set_random_unit(&v);
	test_assert(vector2_equals(&v, vector2_setf2(&expected, HYP_FLOAT_C(-1.0), HYP_FLOAT_C(0.0))));

	test_random_script(NULL, 0);
	return NULL;
}


static const char *test_vector2_set_random_unit_many(void)
{
	struct vector2 v;
	int counts[4];
	int small_x = 0;
	int small_y = 0;
	int i;

	for (i = 0; i < 4; i++) {
		counts[i] = 0;
	}

	for (i = 0; i < 10000; i++) {
		vector2_set_random_unit(&v);
		test_assert(scalar_equalsf(vector2_magnitude(&v), HYP_FLOAT_C(1.0)));
		counts[(v.x >= HYP_FLOAT_C(0.0) ? 0 : 1) + (v.y >= HYP_FLOAT_C(0.0) ? 0 : 2)]++;
		if (HYP_ABS(v.x) < HYP_FLOAT_C(0.5)) {
			small_x++;
		}
		if (HYP_ABS(v.y) < HYP_FLOAT_C(0.5)) {
			small_y++;
		}
	}

	/* each quarter of the circle gets 25% +/- 3% */
	for (i = 0; i < 4; i++) {
		test_assert(counts[i] > 2200 && counts[i] < 2800);
	}

	/* the quarters are symmetric even for an uneven spread (e.g. normalizing
	 * random components), so also check |x| < 0.5 and |y| < 0.5: one third of
	 * the circle each (33.3% +/- 2.5%)
	 */
	test_assert(small_x > 3083 && small_x < 3583);
	test_assert(small_y > 3083 && small_y < 3583);

	return NULL;
}


static const char *test_vector2_lerp(void)
{
	struct vector2 start, end, r, e;

	vector2_setf2(&start, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0));
	vector2_setf2(&end, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(2.0));
	test_assert(vector2_equals(vector2_lerp(&start, &end, HYP_FLOAT_C(0.0), &r), &start));
	test_assert(vector2_equals(vector2_lerp(&start, &end, HYP_FLOAT_C(1.0), &r), &end));
	test_assert(vector2_equals(vector2_lerp(&start, &end, HYP_FLOAT_C(0.25), &r), vector2_setf2(&e, HYP_FLOAT_C(1.5), HYP_FLOAT_C(1.0))));

	return NULL;
}


static const char *test_vector2_clamp_min_max(void)
{
	struct vector2 v, lo, hi, e;

	vector2_setf2(&lo, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0));
	vector2_setf2(&hi, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	vector2_setf2(&v, -HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.5));
	test_assert(vector2_equals(vector2_clamp(&v, &lo, &hi), vector2_setf2(&e, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.5))));

	vector2_setf2(&v, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0));
	vector2_setf2(&lo, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(5.0));
	test_assert(vector2_equals(vector2_min(&v, &lo), vector2_setf2(&e, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0))));
	vector2_setf2(&v, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0));
	test_assert(vector2_equals(vector2_max(&v, &lo), vector2_setf2(&e, HYP_FLOAT_C(1.0), HYP_FLOAT_C(5.0))));

	return NULL;
}


static const char *test_vector2_project(void)
{
	struct vector2 v, onto, e;

	vector2_setf2(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	vector2_setf2(&onto, HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0));
	test_assert(vector2_equals(vector2_project(&v, &onto), vector2_setf2(&e, HYP_FLOAT_C(3.0), HYP_FLOAT_C(0.0))));

	/* onto the zero vector */
	vector2_setf2(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	vector2_zero(&onto);
	test_assert(vector2_equals(vector2_project(&v, &onto), vector2_zero(&e)));

	/* a perpendicular vector projects to zero */
	vector2_setf2(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	vector2_setf2(&onto, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0));
	test_assert(vector2_equals(vector2_project(&v, &onto), vector2_zero(&e)));

	return NULL;
}


static const char *test_vector2_set_random_in_disk(void)
{
	static const long half_way[] = {1073741824L, 0};
	struct vector2 v;
	struct vector2 expected;
	int inner = 0;
	int i;

	/* draws: u (the radius is sqrt(u)), then the angle */
	test_random_script(half_way, 2);
	vector2_set_random_in_disk(&v);
	test_assert(vector2_equals(&v, vector2_setf2(&expected, HYP_SQRT(HYP_FLOAT_C(0.5)), HYP_FLOAT_C(0.0))));
	test_random_script(NULL, 0);

	/* evenly spread: a quarter of the points fall within radius 0.5 */
	for (i = 0; i < 10000; i++) {
		vector2_set_random_in_disk(&v);
		test_assert(vector2_magnitude(&v) < HYP_FLOAT_C(1.0));
		if (vector2_magnitude(&v) < HYP_FLOAT_C(0.5)) {
			inner++;
		}
	}
	test_assert(inner > 2300 && inner < 2700);

	return NULL;
}


static const char *test_vector2_normalize_small_and_zero(void)
{
	struct vector2 v;
	struct vector2 e;

	/* only an exactly zero vector cannot be normalized */
	vector2_setf2(&v, HYP_FLOAT_C(3e-7), HYP_FLOAT_C(4e-7));
	test_assert(vector2_equals(vector2_normalize(&v), vector2_setf2(&e, HYP_FLOAT_C(0.6), HYP_FLOAT_C(0.8))));
	vector2_setf2(&v, HYP_FLOAT_C(3e-30), HYP_FLOAT_C(4e-30));
	test_assert(vector2_equals(vector2_normalize(&v), vector2_setf2(&e, HYP_FLOAT_C(0.6), HYP_FLOAT_C(0.8))));
	vector2_zero(&v);
	test_assert(vector2_equals(vector2_normalize(&v), vector2_zero(&e)));

	return NULL;
}


static const char *test_vector2_project_short_and_long(void)
{
	struct vector2 v, onto, e;
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-30), HYP_FLOAT_C(1e30) };
#else
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-200), HYP_FLOAT_C(1e200) };
#endif
	int i;

	/* onto vectors whose squares underflow or overflow */
	for (i = 0; i < 2; i++) {
		vector2_setf2(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
		vector2_setf2(&onto, HYP_FLOAT_C(0.0), lengths[i]);
		test_assert(vector2_equals(vector2_project(&v, &onto), vector2_setf2(&e, HYP_FLOAT_C(0.0), HYP_FLOAT_C(4.0))));
	}

	return NULL;
}

static const char *test_vector2_angle_between_parallel_small_zero(void)
{
	struct vector2 a, b, zero;
	HYP_FLOAT angle;
	HYP_FLOAT i;

	/* parallel vectors: acos of a rounded cosine above 1 was NaN */
	for (i = HYP_FLOAT_C(1.0); i < HYP_FLOAT_C(20.5); i += HYP_FLOAT_C(1.0)) {
		vector2_setf2(&a, HYP_FLOAT_C(0.37) * i - HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.1) * i - HYP_FLOAT_C(7.0));
		vector2_multiplyf(vector2_set(&b, &a), HYP_FLOAT_C(3.0));
		test_assert(scalar_equalsf(vector2_angle_between(&a, &b), HYP_FLOAT_C(0.0)));
	}

	/* 1e-4 radians apart, to a thousandth of the angle */
	vector2_setf2(&a, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	vector2_setf2(&b, HYP_COS(HYP_FLOAT_C(1e-4)), HYP_SIN(HYP_FLOAT_C(1e-4)));
	angle = vector2_angle_between(&a, &b);
	test_assert(HYP_ABS(angle - HYP_FLOAT_C(1e-4)) < HYP_FLOAT_C(1e-7));

	/* any length, either order; 0 for the zero vector */
	vector2_setf2(&a, HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0));
	vector2_setf2(&b, HYP_FLOAT_C(0.0), -HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(vector2_angle_between(&a, &b), HYP_PI / HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(vector2_angle_between(&b, &a), HYP_PI / HYP_FLOAT_C(2.0)));
	vector2_zero(&zero);
	test_assert(scalar_equalsf(vector2_angle_between(&a, &zero), HYP_FLOAT_C(0.0)));

	return NULL;
}


static const char *vector2_all_tests(void)
{
	run_test(test_vector2_angle_between_parallel_small_zero);
	run_test(test_vector2_project_short_and_long);
	run_test(test_vector2_normalize_small_and_zero);
	run_test(test_vector2_lerp);
	run_test(test_vector2_clamp_min_max);
	run_test(test_vector2_project);
	run_test(test_vector2_set_random_unit_scripted);
	run_test(test_vector2_set_random_unit_many);
	run_test(test_vector2_set_random_in_disk);
	run_test(test_vector2_set);
	run_test(test_vector2_setf2);
	run_test(test_vector2_zero);
	run_test(test_vector2_equals);
	run_test(test_vector2_negate);
	run_test(test_vector2_add);
	run_test(test_vector2_addf);
	run_test(test_vector2_subtract);
	run_test(test_vector2_subtractf);
	run_test(test_vector2_multiply);
	run_test(test_vector2_multiplyf);
	run_test(test_vector2_divide);
	run_test(test_vector2_dividef);
	run_test(test_vector2_magnitude);
	run_test(test_vector2_normalize);
	run_test(test_vector2_normalize_zero);
	run_test(test_vector2_magnitude_zero);
	run_test(test_vector2_distance);
	run_test(test_vector2_dot_product);
	run_test(test_vector2_dot_product_perpendicular);
	run_test(test_vector2_cross_product);
	run_test(test_vector2_angle_between_perpendicular);
	run_test(test_vector2_angle_between_same);
	run_test(test_vector2_angle_between_opposite);

	return NULL;
}
