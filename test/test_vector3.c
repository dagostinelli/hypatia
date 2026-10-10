/* SPDX-License-Identifier: MIT */

#include "random_source.h"

static const char *test_vector3_set(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}};
	struct vector3 v2;

	vector3_set(&v2, &v1);
	test_assert(scalar_equalsf(v2.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v2.y, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v2.z, HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector3_setf3(void)
{
	struct vector3 v;

	vector3_setf3(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(3.0)));

	return NULL;
}

static const char *test_vector3_zero(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};

	vector3_zero(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector3_equals(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 v3 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(4.0)}};

	test_assert(vector3_equals(&v1, &v2));
	test_assert(!vector3_equals(&v1, &v3));

	return NULL;
}

static const char *test_vector3_negate(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}};

	vector3_negate(&v);
	test_assert(scalar_equalsf(v.x, -HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v.z, -HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector3_add(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}};

	vector3_add(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(7.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(9.0)));

	return NULL;
}

static const char *test_vector3_addf(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};

	vector3_addf(&v, HYP_FLOAT_C(5.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(7.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(8.0)));

	return NULL;
}

static const char *test_vector3_subtract(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(9.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector3_subtract(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector3_subtractf(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(9.0)}};

	vector3_subtractf(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(7.0)));

	return NULL;
}

static const char *test_vector3_multiply(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0)}};

	vector3_multiply(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(10.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(18.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(28.0)));

	return NULL;
}

static const char *test_vector3_multiplyf(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector3_multiplyf(&v, HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(9.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(12.0)));

	return NULL;
}

static const char *test_vector3_divide(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(10.0), HYP_FLOAT_C(18.0), HYP_FLOAT_C(28.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0)}};

	vector3_divide(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector3_dividef(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(6.0), HYP_FLOAT_C(9.0), HYP_FLOAT_C(12.0)}};

	vector3_dividef(&v, HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector3_magnitude(void)
{
	/* 3-4-5 right triangle extended: sqrt(1+4+4) = 3 */
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0)}};

	test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(3.0)));

	return NULL;
}

static const char *test_vector3_normalize(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0)}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0) / HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0) / HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(2.0) / HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(1.0)));

	return NULL;
}

static const char *test_vector3_normalize_zero(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector3_distance(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(3.0)}};

	/* sqrt(9 + 16 + 0) = 5 */
	test_assert(scalar_equalsf(vector3_distance(&v1, &v2), HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector3_dot_product(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}};

	/* 1*4 + 2*5 + 3*6 = 32 */
	test_assert(scalar_equalsf(vector3_dot_product(&v1, &v2), HYP_FLOAT_C(32.0)));

	return NULL;
}

static const char *test_vector3_dot_product_perpendicular(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector3_dot_product(&v1, &v2), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector3_cross_product(void)
{
	struct vector3 a;
	struct vector3 b;
	struct vector3 r;

	vector3_setf3(&a, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.0));
	vector3_setf3(&b, HYP_FLOAT_C(4.0), HYP_FLOAT_C(9.0), HYP_FLOAT_C(2.0));

	vector3_cross_product(&r, &a, &b);

	test_assert(scalar_equalsf(r.x, -HYP_FLOAT_C(15.0)));
	test_assert(scalar_equalsf(r.y, -HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(r.z, HYP_FLOAT_C(39.0)));

	/* tests lack of commutative property */
	vector3_cross_product(&r, &b, &a);

	test_assert(!scalar_equalsf(r.x, -HYP_FLOAT_C(15.0)));
	test_assert(!scalar_equalsf(r.y, -HYP_FLOAT_C(2.0)));
	test_assert(!scalar_equalsf(r.z, HYP_FLOAT_C(39.0)));

	return NULL;
}

static const char *test_vector3_angle_between_perpendicular(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_PI_HALF));
	return NULL;
}

static const char *test_vector3_angle_between_same(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_FLOAT_C(0.0)));
	return NULL;
}

static const char *test_vector3_angle_between_opposite(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 v2 = {.v = {-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_PI));
	return NULL;
}

static const char *test_vector3_multiplym4_identity(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct matrix4 m;

	matrix4_identity(&m);
	vector3_multiplym4(&v, &m);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(3.0)));

	return NULL;
}

static const char *test_vector3_multiplym4_scaling(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}};
	struct vector3 scale = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct matrix4 m;

	matrix4_make_transformation_scalingv3(&m, &scale);
	vector3_multiplym4(&v, &m);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(12.0)));

	return NULL;
}

static const char *test_vector3_find_normal_axis_between(void)
{
	struct vector3 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 v2 = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector3 vR;

	vector3_find_normal_axis_between(&vR, &v1, &v2);
	/* cross product of x and y axes is z axis, already normalized */
	test_assert(scalar_equalsf(vR.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vR.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vR.z, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(vector3_magnitude(&vR), HYP_FLOAT_C(1.0)));

	return NULL;
}

static const char *test_vector3_rotate_by_quaternion(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 axis = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};
	struct quaternion q;

	/* rotate (1,0,0) by 90 degrees around z axis -> (0,1,0) */
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI_HALF);
	vector3_rotate_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector3_rotate_by_quaternion_180(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector3 axis = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};
	struct quaternion q;

	/* rotate (1,0,0) by 180 degrees around z axis -> (-1,0,0) */
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI);
	vector3_rotate_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, -HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector3_reflect_by_quaternion(void)
{
	struct vector3 v;
	struct quaternion q;

	/* pure z quaternion reflects through the xy plane
	 * (0,0,1) should flip to (0,0,-1)
	 */
	q.w = HYP_FLOAT_C(0.0); q.x = HYP_FLOAT_C(0.0); q.y = HYP_FLOAT_C(0.0); q.z = HYP_FLOAT_C(1.0);
	v.x = HYP_FLOAT_C(0.0); v.y = HYP_FLOAT_C(0.0); v.z = HYP_FLOAT_C(1.0);
	vector3_reflect_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, -HYP_FLOAT_C(1.0)));

	/* vector in the plane is preserved */
	q.w = HYP_FLOAT_C(0.0); q.x = HYP_FLOAT_C(0.0); q.y = HYP_FLOAT_C(0.0); q.z = HYP_FLOAT_C(1.0);
	v.x = HYP_FLOAT_C(1.0); v.y = HYP_FLOAT_C(0.0); v.z = HYP_FLOAT_C(0.0);
	vector3_reflect_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	/* non-unit vector: (0,0,3) reflected through xy plane should be (0,0,-3)
	 * but normalize forces unit length, so we get (0,0,-1) instead
	 * this test documents the bug
	 */
	q.w = HYP_FLOAT_C(0.0); q.x = HYP_FLOAT_C(0.0); q.y = HYP_FLOAT_C(0.0); q.z = HYP_FLOAT_C(1.0);
	v.x = HYP_FLOAT_C(0.0); v.y = HYP_FLOAT_C(0.0); v.z = HYP_FLOAT_C(3.0);
	vector3_reflect_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, -HYP_FLOAT_C(3.0)));

	return NULL;
}

static const char *test_vector3_normalize_large(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1e15), HYP_FLOAT_C(1e15), HYP_FLOAT_C(1e15)}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(1.0)));

	return NULL;
}

static const char *test_vector3_normalize_small(void)
{
	struct vector3 v = {.v = {HYP_FLOAT_C(1e-15), HYP_FLOAT_C(1e-15), HYP_FLOAT_C(1e-15)}};

	/* only an exactly zero vector is left unchanged: this one normalizes */
	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_SQRT(HYP_FLOAT_C(1.0) / HYP_FLOAT_C(3.0))));
	test_assert(scalar_equalsf(v.y, HYP_SQRT(HYP_FLOAT_C(1.0) / HYP_FLOAT_C(3.0))));
	test_assert(scalar_equalsf(v.z, HYP_SQRT(HYP_FLOAT_C(1.0) / HYP_FLOAT_C(3.0))));

	return NULL;
}

static const char *test_vector3_set_random_unit_scripted(void)
{
	static const long bottom[] = {0, 0};
	static const long equator[] = {1073741824L, 0};
	static const long equator_half_turn[] = {1073741824L, 1073741824L};
	struct vector3 v;
	struct vector3 expected;

	/* draws: z, then the angle */
	test_random_script(bottom, 2);
	vector3_set_random_unit(&v);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(-1.0))));

	test_random_script(equator, 2);
	vector3_set_random_unit(&v);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	/* angle pi: sin(pi) is close to 0, not exactly 0 */
	test_random_script(equator_half_turn, 2);
	vector3_set_random_unit(&v);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(-1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	test_random_script(NULL, 0);
	return NULL;
}


static const char *test_vector3_set_random_unit_many(void)
{
	struct vector3 v;
	int counts[8];
	int small[3];
	int i;

	for (i = 0; i < 8; i++) {
		counts[i] = 0;
	}
	for (i = 0; i < 3; i++) {
		small[i] = 0;
	}

	for (i = 0; i < 10000; i++) {
		vector3_set_random_unit(&v);
		test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(1.0)));
		counts[(v.x >= HYP_FLOAT_C(0.0) ? 0 : 1) + (v.y >= HYP_FLOAT_C(0.0) ? 0 : 2) + (v.z >= HYP_FLOAT_C(0.0) ? 0 : 4)]++;
		if (HYP_ABS(v.x) < HYP_FLOAT_C(0.5)) {
			small[0]++;
		}
		if (HYP_ABS(v.y) < HYP_FLOAT_C(0.5)) {
			small[1]++;
		}
		if (HYP_ABS(v.z) < HYP_FLOAT_C(0.5)) {
			small[2]++;
		}
	}

	/* each octant of the sphere gets 12.5% +/- 2% */
	for (i = 0; i < 8; i++) {
		test_assert(counts[i] > 1050 && counts[i] < 1450);
	}

	/* the octants are symmetric even for an uneven spread (e.g. normalizing
	 * random components), so also check each component: evenly spread on the
	 * sphere, |component| < 0.5 half the time (50% +/- 3%)
	 */
	for (i = 0; i < 3; i++) {
		test_assert(small[i] > 4700 && small[i] < 5300);
	}

	return NULL;
}


static const char *test_vector3_lerp(void)
{
	struct vector3 start, end, r, e;

	vector3_setf3(&start, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(3.0));
	vector3_setf3(&end, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(5.0));
	test_assert(vector3_equals(vector3_lerp(&start, &end, HYP_FLOAT_C(0.0), &r), &start));
	test_assert(vector3_equals(vector3_lerp(&start, &end, HYP_FLOAT_C(1.0), &r), &end));
	test_assert(vector3_equals(vector3_lerp(&start, &end, HYP_FLOAT_C(0.25), &r), vector3_setf3(&e, HYP_FLOAT_C(1.5), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0))));

	return NULL;
}


static const char *test_vector3_clamp_min_max(void)
{
	struct vector3 v, lo, hi, e;

	vector3_setf3(&lo, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0));
	vector3_setf3(&hi, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	vector3_setf3(&v, -HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(7.0));
	test_assert(vector3_equals(vector3_clamp(&v, &lo, &hi), vector3_setf3(&e, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0))));

	vector3_setf3(&v, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0));
	vector3_setf3(&lo, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_equals(vector3_min(&v, &lo), vector3_setf3(&e, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0))));
	vector3_setf3(&v, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0));
	test_assert(vector3_equals(vector3_max(&v, &lo), vector3_setf3(&e, HYP_FLOAT_C(1.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(3.0))));

	return NULL;
}


static const char *test_vector3_reflect(void)
{
	struct vector3 v, normal, e, twice;
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-30), HYP_FLOAT_C(1e30) };
#else
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-200), HYP_FLOAT_C(1e200) };
#endif
	int i;

	/* off the floor: the part along the normal changes sign */
	vector3_setf3(&v, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0));
	vector3_setf3(&normal, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_equals(vector3_reflect(&v, &normal), vector3_setf3(&e, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0))));

	/* the length of the normal does not matter, including lengths whose
	 * squares underflow or overflow
	 */
	vector3_setf3(&normal, HYP_FLOAT_C(0.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.0));
	vector3_setf3(&v, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0));
	test_assert(vector3_equals(vector3_reflect(&v, &normal), vector3_setf3(&e, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0))));
	for (i = 0; i < 2; i++) {
		vector3_setf3(&normal, HYP_FLOAT_C(0.0), lengths[i], HYP_FLOAT_C(0.0));
		vector3_setf3(&v, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0));
		test_assert(vector3_equals(vector3_reflect(&v, &normal), vector3_setf3(&e, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0))));
	}

	/* reflecting twice gives the vector back, and the length is kept */
	vector3_setf3(&normal, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(2.0));
	vector3_setf3(&v, HYP_FLOAT_C(0.3), -HYP_FLOAT_C(0.7), HYP_FLOAT_C(0.2));
	vector3_reflect(vector3_set(&twice, &v), &normal);
	test_assert(scalar_equalsf(vector3_magnitude(&twice), vector3_magnitude(&v)));
	test_assert(vector3_equals(vector3_reflect(&twice, &normal), &v));

	/* a zero normal leaves the vector unchanged */
	vector3_zero(&normal);
	vector3_setf3(&v, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0));
	test_assert(vector3_equals(vector3_reflect(&v, &normal), vector3_setf3(&e, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0))));

	return NULL;
}


static const char *test_vector3_project(void)
{
	struct vector3 v, onto, e;

	vector3_setf3(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0));
	vector3_setf3(&onto, HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_equals(vector3_project(&v, &onto), vector3_setf3(&e, HYP_FLOAT_C(3.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	/* onto the zero vector */
	vector3_setf3(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0));
	vector3_zero(&onto);
	test_assert(vector3_equals(vector3_project(&v, &onto), vector3_zero(&e)));

	/* a perpendicular vector projects to zero */
	vector3_setf3(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	vector3_setf3(&onto, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_equals(vector3_project(&v, &onto), vector3_zero(&e)));

	return NULL;
}


static const char *test_vector3_set_random_in_ball(void)
{
	static const long scripted[] = {0, 0, 1073741824L, 0, 0};
	struct vector3 v;
	struct vector3 expected;
	int inner = 0;
	int i;

	/* draws: the direction (z, then the angle), then three; the radius is the
	 * largest of the three
	 */
	test_random_script(scripted, 5);
	vector3_set_random_in_ball(&v);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(0.5))));
	test_random_script(NULL, 0);

	/* evenly spread: an eighth of the points fall within radius 0.5 */
	for (i = 0; i < 10000; i++) {
		vector3_set_random_in_ball(&v);
		test_assert(vector3_magnitude(&v) < HYP_FLOAT_C(1.0));
		if (vector3_magnitude(&v) < HYP_FLOAT_C(0.5)) {
			inner++;
		}
	}
	test_assert(inner > 1050 && inner < 1450);

	return NULL;
}


static const char *test_vector3_set_random_in_cone(void)
{
	static const long scripted[] = {0, 0};
	struct vector3 v;
	struct vector3 axis;
	HYP_FLOAT angle = HYP_TAU / HYP_FLOAT_C(8.0);
	HYP_FLOAT middle = (HYP_COS(angle) + HYP_FLOAT_C(1.0)) / HYP_FLOAT_C(2.0);
	int upper = 0;
	int i;

	/* draws: the height along the axis, then the angle around it.  A quarter
	 * turn cone about Z, lowest height, angle 0: X.  About X instead of Z, the
	 * same draw turns X onto -Z.
	 */
	test_random_script(scripted, 2);
	vector3_set_random_in_cone(&v, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	test_assert(vector3_equals(&v, HYP_VECTOR3_UNIT_X));
	test_random_script(scripted, 2);
	vector3_set_random_in_cone(&v, HYP_VECTOR3_UNIT_X, HYP_TAU / HYP_FLOAT_C(4.0));
	test_assert(vector3_equals(&v, HYP_VECTOR3_UNIT_Z_NEGATIVE));
	test_random_script(NULL, 0);

	/* unit directions within angle of the axis, evenly spread over the cap:
	 * half fall above the middle height
	 */
	vector3_normalize(vector3_setf3(&axis, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0)));
	for (i = 0; i < 10000; i++) {
		vector3_set_random_in_cone(&v, &axis, angle);
		test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(1.0)));
		test_assert(vector3_dot_product(&v, &axis) >= HYP_COS(angle) - HYP_EPSILON);
		if (vector3_dot_product(&v, &axis) > middle) {
			upper++;
		}
	}
	test_assert(upper > 4700 && upper < 5300);

	return NULL;
}


static const char *test_vector3_normalize_small_and_zero(void)
{
	struct vector3 v;
	struct vector3 e;

	/* only an exactly zero vector cannot be normalized */
	vector3_setf3(&v, HYP_FLOAT_C(1e-6), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_equals(vector3_normalize(&v), HYP_VECTOR3_UNIT_X));
	vector3_setf3(&v, HYP_FLOAT_C(0.0), HYP_FLOAT_C(3e-30), HYP_FLOAT_C(4e-30));
	test_assert(vector3_equals(vector3_normalize(&v), vector3_setf3(&e, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.6), HYP_FLOAT_C(0.8))));
	vector3_zero(&v);
	test_assert(vector3_equals(vector3_normalize(&v), HYP_VECTOR3_ZERO));

	return NULL;
}


static const char *test_vector3_normalize_nan(void)
{
	struct vector3 v;
	volatile HYP_FLOAT zero = HYP_FLOAT_C(0.0); /* not folded at compile time */
	HYP_FLOAT nan = zero / zero;

	/* a vector with a NaN component is left unchanged, wherever the NaN is */
	vector3_setf3(&v, nan, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(1.0)));

	vector3_setf3(&v, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), nan);
	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(1.0)));

	return NULL;
}


static const char *test_vector3_set_random_in_cone_wide(void)
{
	struct vector3 v;
	int below = 0;
	int i;

	/* an angle of a full turn is the whole sphere: half the points are below */
	for (i = 0; i < 2000; i++) {
		vector3_set_random_in_cone(&v, HYP_VECTOR3_UNIT_Z, HYP_TAU);
		if (v.z < HYP_FLOAT_C(0.0)) {
			below++;
		}
	}
	test_assert(below > 850 && below < 1150);

	return NULL;
}


static const char *test_vector3_project_short_and_long(void)
{
	struct vector3 v, onto, e;
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-30), HYP_FLOAT_C(1e30) };
#else
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-200), HYP_FLOAT_C(1e200) };
#endif
	int i;

	/* onto vectors whose squares underflow or overflow */
	for (i = 0; i < 2; i++) {
		vector3_setf3(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0));
		vector3_setf3(&onto, HYP_FLOAT_C(0.0), lengths[i], HYP_FLOAT_C(0.0));
		test_assert(vector3_equals(vector3_project(&v, &onto), vector3_setf3(&e, HYP_FLOAT_C(0.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(0.0))));
	}

	return NULL;
}

static const char *test_vector3_normalize_infinite(void)
{
	struct vector3 v, e;
	volatile HYP_FLOAT zero = HYP_FLOAT_C(0.0); /* not folded at compile time */
	HYP_FLOAT infinity = HYP_FLOAT_C(1.0) / zero;

	/* the infinite components set the direction */
	vector3_setf3(&v, infinity, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_equals(vector3_normalize(&v), vector3_setf3(&e, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	vector3_setf3(&v, HYP_FLOAT_C(5.0), infinity, -infinity);
	test_assert(vector3_equals(vector3_normalize(&v), vector3_setf3(&e, HYP_FLOAT_C(0.0), HYP_SQRT(HYP_FLOAT_C(0.5)), -HYP_SQRT(HYP_FLOAT_C(0.5)))));

	return NULL;
}

static const char *test_vector3_angle_between_parallel_small_zero(void)
{
	struct vector3 a, b, zero;
	HYP_FLOAT angle;
	HYP_FLOAT i;

	/* parallel vectors: acos of a rounded cosine above 1 was NaN */
	for (i = HYP_FLOAT_C(1.0); i < HYP_FLOAT_C(20.5); i += HYP_FLOAT_C(1.0)) {
		vector3_setf3(&a, HYP_FLOAT_C(0.37) * i - HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.1) * i - HYP_FLOAT_C(7.0), HYP_FLOAT_C(0.53) * i + HYP_FLOAT_C(0.1));
		vector3_multiplyf(vector3_set(&b, &a), HYP_FLOAT_C(3.0));
		test_assert(scalar_equalsf(vector3_angle_between(&a, &b), HYP_FLOAT_C(0.0)));
	}

	/* 1e-4 radians apart, to a thousandth of the angle */
	vector3_setf3(&a, HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	vector3_setf3(&b, HYP_COS(HYP_FLOAT_C(1e-4)), HYP_SIN(HYP_FLOAT_C(1e-4)), HYP_FLOAT_C(0.0));
	angle = vector3_angle_between(&a, &b);
	test_assert(HYP_ABS(angle - HYP_FLOAT_C(1e-4)) < HYP_FLOAT_C(1e-7));

	/* any length; 0 for the zero vector */
	vector3_setf3(&a, HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	vector3_setf3(&b, HYP_FLOAT_C(0.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(0.0));
	test_assert(scalar_equalsf(vector3_angle_between(&a, &b), HYP_PI / HYP_FLOAT_C(2.0)));
	vector3_zero(&zero);
	test_assert(scalar_equalsf(vector3_angle_between(&a, &zero), HYP_FLOAT_C(0.0)));

	return NULL;
}


static const char *vector3_all_tests(void)
{
	run_test(test_vector3_angle_between_parallel_small_zero);
	run_test(test_vector3_normalize_infinite);
	run_test(test_vector3_project_short_and_long);
	run_test(test_vector3_set_random_in_cone_wide);
	run_test(test_vector3_normalize_nan);
	run_test(test_vector3_normalize_small_and_zero);
	run_test(test_vector3_lerp);
	run_test(test_vector3_clamp_min_max);
	run_test(test_vector3_project);
	run_test(test_vector3_reflect);
	run_test(test_vector3_set_random_unit_scripted);
	run_test(test_vector3_set_random_unit_many);
	run_test(test_vector3_set_random_in_ball);
	run_test(test_vector3_set_random_in_cone);
	run_test(test_vector3_set);
	run_test(test_vector3_setf3);
	run_test(test_vector3_zero);
	run_test(test_vector3_equals);
	run_test(test_vector3_negate);
	run_test(test_vector3_add);
	run_test(test_vector3_addf);
	run_test(test_vector3_subtract);
	run_test(test_vector3_subtractf);
	run_test(test_vector3_multiply);
	run_test(test_vector3_multiplyf);
	run_test(test_vector3_divide);
	run_test(test_vector3_dividef);
	run_test(test_vector3_magnitude);
	run_test(test_vector3_normalize);
	run_test(test_vector3_normalize_zero);
	run_test(test_vector3_distance);
	run_test(test_vector3_dot_product);
	run_test(test_vector3_dot_product_perpendicular);
	run_test(test_vector3_cross_product);
	run_test(test_vector3_angle_between_perpendicular);
	run_test(test_vector3_angle_between_same);
	run_test(test_vector3_angle_between_opposite);
	run_test(test_vector3_multiplym4_identity);
	run_test(test_vector3_multiplym4_scaling);
	run_test(test_vector3_find_normal_axis_between);
	run_test(test_vector3_rotate_by_quaternion);
	run_test(test_vector3_rotate_by_quaternion_180);
	run_test(test_vector3_reflect_by_quaternion);
	run_test(test_vector3_normalize_large);
	run_test(test_vector3_normalize_small);

	return NULL;
}
