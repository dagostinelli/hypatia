/* SPDX-License-Identifier: MIT */

#include "random_source.h"

static const char *test_vector4_set(void)
{
	struct vector4 v1, v2;

	/* Test vector4_setf4 */
	vector4_setf4(&v1, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));

	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v1.w, HYP_FLOAT_C(4.0)));

	/* Test vector4_set - copying from another vector4 */
	vector4_set(&v2, &v1);

	test_assert(scalar_equalsf(v2.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v2.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v2.z, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v2.w, HYP_FLOAT_C(4.0)));

	/* Test that all components are properly copied */
	test_assert(vector4_equals(&v1, &v2));

	return NULL;
}

static const char *test_vector4_zero(void)
{
	struct vector4 v;

	vector4_zero(&v);

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector4_negate(void)
{
	struct vector4 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector4_negate(&v);

	test_assert(scalar_equalsf(v.x, -HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, -HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, -HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.w, -HYP_FLOAT_C(4.0)));

	/* Negate negative values */
	vector4_negate(&v);

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(4.0)));

	/* Negate zero vector */
	vector4_zero(&v);
	vector4_negate(&v);

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector4_add(void)
{
	struct vector4 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct vector4 v2 = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(8.0)}};

	vector4_add(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(8.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(10.0)));
	test_assert(scalar_equalsf(v1.w, HYP_FLOAT_C(12.0)));

	/* Add with negative values */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0)}};
		struct vector4 b = {.v = {-HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

		vector4_add(&a, &b);

		test_assert(scalar_equalsf(a.x, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.y, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.z, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.w, HYP_FLOAT_C(0.0)));
	}

	return NULL;
}

static const char *test_vector4_addf(void)
{
	struct vector4 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector4_addf(&v, HYP_FLOAT_C(10.0));

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(11.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(12.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(13.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(14.0)));

	/* Add negative scalar */
	vector4_addf(&v, -HYP_FLOAT_C(10.0));

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(4.0)));

	/* Add zero */
	vector4_addf(&v, HYP_FLOAT_C(0.0));

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(4.0)));

	return NULL;
}

static const char *test_vector4_subtract(void)
{
	struct vector4 v1 = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(9.0), HYP_FLOAT_C(11.0)}};
	struct vector4 v2 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector4_subtract(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v1.w, HYP_FLOAT_C(7.0)));

	/* Subtract same vector yields zero */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}};
		struct vector4 b = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}};

		vector4_subtract(&a, &b);

		test_assert(scalar_equalsf(a.x, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.y, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.z, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.w, HYP_FLOAT_C(0.0)));
	}

	return NULL;
}

static const char *test_vector4_subtractf(void)
{
	struct vector4 v = {.v = {HYP_FLOAT_C(10.0), HYP_FLOAT_C(20.0), HYP_FLOAT_C(30.0), HYP_FLOAT_C(40.0)}};

	vector4_subtractf(&v, HYP_FLOAT_C(5.0));

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(15.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(25.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(35.0)));

	/* Subtract negative scalar (effectively adds) */
	vector4_subtractf(&v, -HYP_FLOAT_C(5.0));

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(10.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(20.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(30.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(40.0)));

	return NULL;
}

static const char *test_vector4_multiply(void)
{
	struct vector4 v1 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}};
	struct vector4 v2 = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}};

	vector4_multiply(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(12.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(20.0)));
	test_assert(scalar_equalsf(v1.w, HYP_FLOAT_C(30.0)));

	/* Multiply by zero vector */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
		struct vector4 z = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

		vector4_multiply(&a, &z);

		test_assert(scalar_equalsf(a.x, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.y, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.z, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(a.w, HYP_FLOAT_C(0.0)));
	}

	return NULL;
}

static const char *test_vector4_multiplyf(void)
{
	struct vector4 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

	vector4_multiplyf(&v, HYP_FLOAT_C(3.0));

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(9.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(12.0)));

	/* Multiply by zero */
	vector4_multiplyf(&v, HYP_FLOAT_C(0.0));

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(0.0)));

	/* Multiply by negative */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};

		vector4_multiplyf(&a, -HYP_FLOAT_C(2.0));

		test_assert(scalar_equalsf(a.x, -HYP_FLOAT_C(2.0)));
		test_assert(scalar_equalsf(a.y, -HYP_FLOAT_C(4.0)));
		test_assert(scalar_equalsf(a.z, -HYP_FLOAT_C(6.0)));
		test_assert(scalar_equalsf(a.w, -HYP_FLOAT_C(8.0)));
	}

	return NULL;
}

static const char *test_vector4_divide(void)
{
	struct vector4 v1 = {.v = {HYP_FLOAT_C(10.0), HYP_FLOAT_C(20.0), HYP_FLOAT_C(30.0), HYP_FLOAT_C(40.0)}};
	struct vector4 v2 = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(8.0)}};

	vector4_divide(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v1.w, HYP_FLOAT_C(5.0)));

	/* Divide by ones yields same vector */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(7.0), HYP_FLOAT_C(8.0), HYP_FLOAT_C(9.0), HYP_FLOAT_C(10.0)}};
		struct vector4 ones = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0)}};

		vector4_divide(&a, &ones);

		test_assert(scalar_equalsf(a.x, HYP_FLOAT_C(7.0)));
		test_assert(scalar_equalsf(a.y, HYP_FLOAT_C(8.0)));
		test_assert(scalar_equalsf(a.z, HYP_FLOAT_C(9.0)));
		test_assert(scalar_equalsf(a.w, HYP_FLOAT_C(10.0)));
	}

	return NULL;
}

static const char *test_vector4_dividef(void)
{
	struct vector4 v = {.v = {HYP_FLOAT_C(8.0), HYP_FLOAT_C(12.0), HYP_FLOAT_C(16.0), HYP_FLOAT_C(20.0)}};

	vector4_dividef(&v, HYP_FLOAT_C(4.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(5.0)));

	return NULL;
}

static const char *test_vector4_magnitude(void)
{
	/* Unit vector along x */
	struct vector4 vx = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	test_assert(scalar_equalsf(vector4_magnitude(&vx), HYP_FLOAT_C(1.0)));

	/* Known magnitude: sqrt(1+4+9+16) = sqrt(30) */
	{
		struct vector4 v = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
		HYP_FLOAT expected = HYP_SQRT(HYP_FLOAT_C(30.0));
		test_assert(scalar_equalsf(vector4_magnitude(&v), expected));
	}

	/* Zero vector */
	{
		struct vector4 z = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		test_assert(scalar_equalsf(vector4_magnitude(&z), HYP_FLOAT_C(0.0)));
	}

	return NULL;
}

static const char *test_vector4_normalize(void)
{
	/* Normalize a known vector */
	struct vector4 v = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	vector4_normalize(&v);

	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(0.0)));

	/* Normalize a general vector, magnitude should be 1 */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
		vector4_normalize(&a);
		test_assert(scalar_equalsf(vector4_magnitude(&a), HYP_FLOAT_C(1.0)));
	}

	/* Normalize zero vector should not crash and remain zero */
	{
		struct vector4 z = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		vector4_normalize(&z);
		test_assert(scalar_equalsf(z.x, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(z.y, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(z.z, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(z.w, HYP_FLOAT_C(0.0)));
	}

	return NULL;
}

static const char *test_vector4_distance(void)
{
	/* Distance between same point is zero */
	struct vector4 v1 = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	test_assert(scalar_equalsf(vector4_distance(&v1, &v1), HYP_FLOAT_C(0.0)));

	/* Distance between origin and a unit axis vector */
	{
		struct vector4 origin = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		struct vector4 point = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		test_assert(scalar_equalsf(vector4_distance(&origin, &point), HYP_FLOAT_C(1.0)));
	}

	/* Known distance: sqrt((4-1)^2 + (6-2)^2 + (8-3)^2 + (10-4)^2) = sqrt(9+16+25+36) = sqrt(86) */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
		struct vector4 b = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(8.0), HYP_FLOAT_C(10.0)}};
		HYP_FLOAT expected = HYP_SQRT(HYP_FLOAT_C(86.0));
		test_assert(scalar_equalsf(vector4_distance(&a, &b), expected));
	}

	/* Distance is symmetric */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
		struct vector4 b = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(8.0)}};
		test_assert(scalar_equalsf(vector4_distance(&a, &b), vector4_distance(&b, &a)));
	}

	return NULL;
}

static const char *test_vector4_dot_product(void)
{
	/* Dot product of orthogonal vectors is zero */
	struct vector4 vx = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector4 vy = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	test_assert(scalar_equalsf(vector4_dot_product(&vx, &vy), HYP_FLOAT_C(0.0)));

	/* Dot product of parallel vectors */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		struct vector4 b = {.v = {HYP_FLOAT_C(3.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		test_assert(scalar_equalsf(vector4_dot_product(&a, &b), HYP_FLOAT_C(6.0)));
	}

	/* General dot product: 1*5 + 2*6 + 3*7 + 4*8 = 5+12+21+32 = 70 */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
		struct vector4 b = {.v = {HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(8.0)}};
		test_assert(scalar_equalsf(vector4_dot_product(&a, &b), HYP_FLOAT_C(70.0)));
	}

	/* Dot product with zero vector */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
		struct vector4 z = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		test_assert(scalar_equalsf(vector4_dot_product(&a, &z), HYP_FLOAT_C(0.0)));
	}

	return NULL;
}

static const char *test_vector4_cross_product(void)
{
	struct vector4 result;

	/* Cross product of x and y unit vectors should be z */
	{
		struct vector4 vx = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		struct vector4 vy = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

		vector4_cross_product(&result, &vx, &vy);

		test_assert(scalar_equalsf(result.x, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(result.y, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(result.z, HYP_FLOAT_C(1.0)));
		test_assert(scalar_equalsf(result.w, HYP_FLOAT_C(0.0)));
	}

	/* Cross product of y and x unit vectors should be -z */
	{
		struct vector4 vx = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
		struct vector4 vy = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

		vector4_cross_product(&result, &vy, &vx);

		test_assert(scalar_equalsf(result.x, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(result.y, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(result.z, -HYP_FLOAT_C(1.0)));
		test_assert(scalar_equalsf(result.w, HYP_FLOAT_C(0.0)));
	}

	/* General cross product: (1,2,3,w) x (4,5,6,w) */
	/* x = 2*6 - 3*5 = 12-15 = -3 */
	/* y = 3*4 - 1*6 = 12-6 = 6 */
	/* z = 1*5 - 2*4 = 5-8 = -3 */
	/* w = w*w - w*w = 0 */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.0)}};
		struct vector4 b = {.v = {HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(1.0)}};

		vector4_cross_product(&result, &a, &b);

		test_assert(scalar_equalsf(result.x, -HYP_FLOAT_C(3.0)));
		test_assert(scalar_equalsf(result.y, HYP_FLOAT_C(6.0)));
		test_assert(scalar_equalsf(result.z, -HYP_FLOAT_C(3.0)));
		test_assert(scalar_equalsf(result.w, HYP_FLOAT_C(0.0)));
	}

	/* Cross product of parallel vectors is zero */
	{
		struct vector4 a = {.v = {HYP_FLOAT_C(2.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(0.0)}};
		struct vector4 b = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(0.0)}};

		vector4_cross_product(&result, &a, &b);

		test_assert(scalar_equalsf(result.x, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(result.y, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(result.z, HYP_FLOAT_C(0.0)));
		test_assert(scalar_equalsf(result.w, HYP_FLOAT_C(0.0)));
	}

	return NULL;
}

static const char *test_vector4_dot_product_perpendicular(void)
{
	/* All four pairs of standard basis vectors are perpendicular */
	struct vector4 vx = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector4 vy = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};
	struct vector4 vz = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}};
	struct vector4 vw = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}};

	test_assert(scalar_equalsf(vector4_dot_product(&vx, &vy), HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vector4_dot_product(&vx, &vz), HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vector4_dot_product(&vx, &vw), HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vector4_dot_product(&vy, &vz), HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vector4_dot_product(&vy, &vw), HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vector4_dot_product(&vz, &vw), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector4_normalize_zero(void)
{
	struct vector4 v = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	vector4_normalize(&v);

	/* Zero vector should remain zero (implementation guards against divide by zero) */
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.w, HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector4_magnitude_zero(void)
{
	struct vector4 v = {.v = {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}};

	test_assert(scalar_equalsf(vector4_magnitude(&v), HYP_FLOAT_C(0.0)));

	return NULL;
}

static const char *test_vector4_equals(void)
{
	struct vector4 a = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct vector4 b = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}};
	struct vector4 c = {.v = {HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(5.0)}};

	test_assert(vector4_equals(&a, &b));
	test_assert(!vector4_equals(&a, &c));

	/* Self-equality */
	test_assert(vector4_equals(&a, &a));

	return NULL;
}

static const char *test_vector4_set_random_unit_scripted(void)
{
	static const long zero[] = {0, 0, 0};
	static const long half[] = {1073741824L, 0, 0};
	struct vector4 v;
	struct vector4 expected;
	HYP_FLOAT root_half;

	/* draws: u, then two angles */
	test_random_script(zero, 3);
	vector4_set_random_unit(&v);
	test_assert(vector4_equals(&v, vector4_setf4(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	root_half = HYP_SQRT(HYP_FLOAT_C(0.5));
	test_random_script(half, 3);
	vector4_set_random_unit(&v);
	test_assert(vector4_equals(&v, vector4_setf4(&expected, HYP_FLOAT_C(0.0), root_half, HYP_FLOAT_C(0.0), root_half)));

	test_random_script(NULL, 0);
	return NULL;
}


static const char *test_vector4_set_random_unit_many(void)
{
	struct vector4 v;
	HYP_FLOAT c[4];
	int positive[4];
	int small[4];
	int i;
	int j;

	for (j = 0; j < 4; j++) {
		positive[j] = 0;
		small[j] = 0;
	}

	for (i = 0; i < 10000; i++) {
		vector4_set_random_unit(&v);
		test_assert(scalar_equalsf(vector4_magnitude(&v), HYP_FLOAT_C(1.0)));

		c[0] = v.x;
		c[1] = v.y;
		c[2] = v.z;
		c[3] = v.w;
		for (j = 0; j < 4; j++) {
			if (c[j] >= HYP_FLOAT_C(0.0)) {
				positive[j]++;
			}
			if (HYP_ABS(c[j]) < HYP_FLOAT_C(0.5)) {
				small[j]++;
			}
		}
	}

	/* evenly spread on the 4D sphere: each component is positive 50% of the time
	 * and has |component| < 0.5 60.9% of the time (each +/- 3%)
	 */
	for (j = 0; j < 4; j++) {
		test_assert(positive[j] > 4700 && positive[j] < 5300);
		test_assert(small[j] > 5790 && small[j] < 6390);
	}

	return NULL;
}


static const char *test_vector4_lerp(void)
{
	struct vector4 start, end, r, e;

	vector4_setf4(&start, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	vector4_setf4(&end, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.0));
	test_assert(vector4_equals(vector4_lerp(&start, &end, HYP_FLOAT_C(0.0), &r), &start));
	test_assert(vector4_equals(vector4_lerp(&start, &end, HYP_FLOAT_C(1.0), &r), &end));
	test_assert(vector4_equals(vector4_lerp(&start, &end, HYP_FLOAT_C(0.25), &r), vector4_setf4(&e, HYP_FLOAT_C(1.5), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0))));

	return NULL;
}


static const char *test_vector4_clamp_min_max(void)
{
	struct vector4 v, lo, hi, e;

	vector4_setf4(&lo, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0));
	vector4_setf4(&hi, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	vector4_setf4(&v, -HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(7.0), -HYP_FLOAT_C(0.25));
	test_assert(vector4_equals(vector4_clamp(&v, &lo, &hi), vector4_setf4(&e, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(0.25))));

	vector4_setf4(&v, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0));
	vector4_setf4(&lo, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(2.0));
	test_assert(vector4_equals(vector4_min(&v, &lo), vector4_setf4(&e, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(4.0))));
	vector4_setf4(&v, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0));
	test_assert(vector4_equals(vector4_max(&v, &lo), vector4_setf4(&e, HYP_FLOAT_C(1.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(2.0))));

	return NULL;
}


static const char *test_vector4_project(void)
{
	struct vector4 v, onto, e;

	vector4_setf4(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0));
	vector4_setf4(&onto, HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	test_assert(vector4_equals(vector4_project(&v, &onto), vector4_setf4(&e, HYP_FLOAT_C(3.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));

	/* onto the zero vector */
	vector4_setf4(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0));
	vector4_zero(&onto);
	test_assert(vector4_equals(vector4_project(&v, &onto), vector4_zero(&e)));

	/* a perpendicular vector projects to zero */
	vector4_setf4(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	vector4_setf4(&onto, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	test_assert(vector4_equals(vector4_project(&v, &onto), vector4_zero(&e)));

	return NULL;
}


static const char *test_vector4_normalize_small_and_zero(void)
{
	struct vector4 v;
	struct vector4 e;

	/* only an exactly zero vector cannot be normalized */
	vector4_setf4(&v, HYP_FLOAT_C(0.0), HYP_FLOAT_C(3e-7), HYP_FLOAT_C(0.0), HYP_FLOAT_C(4e-7));
	test_assert(vector4_equals(vector4_normalize(&v), vector4_setf4(&e, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.6), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.8))));
	vector4_zero(&v);
	test_assert(vector4_equals(vector4_normalize(&v), vector4_zero(&e)));

	return NULL;
}


static const char *test_vector4_project_short_and_long(void)
{
	struct vector4 v, onto, e;
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-30), HYP_FLOAT_C(1e30) };
#elif defined(TEST_LONG_DOUBLE_RANGE)
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-2500), HYP_FLOAT_C(1e2500) };
#else
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-200), HYP_FLOAT_C(1e200) };
#endif
	int i;

	/* onto vectors whose squares underflow or overflow */
	for (i = 0; i < 2; i++) {
		vector4_setf4(&v, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0));
		vector4_setf4(&onto, HYP_FLOAT_C(0.0), lengths[i], HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
		test_assert(vector4_equals(vector4_project(&v, &onto), vector4_setf4(&e, HYP_FLOAT_C(0.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0))));
	}

	return NULL;
}

static const char *vector4_all_tests(void)
{
	run_test(test_vector4_project_short_and_long);
	run_test(test_vector4_normalize_small_and_zero);
	run_test(test_vector4_lerp);
	run_test(test_vector4_clamp_min_max);
	run_test(test_vector4_project);
	run_test(test_vector4_set_random_unit_scripted);
	run_test(test_vector4_set_random_unit_many);
	run_test(test_vector4_set);
	run_test(test_vector4_zero);
	run_test(test_vector4_negate);
	run_test(test_vector4_add);
	run_test(test_vector4_addf);
	run_test(test_vector4_subtract);
	run_test(test_vector4_subtractf);
	run_test(test_vector4_multiply);
	run_test(test_vector4_multiplyf);
	run_test(test_vector4_divide);
	run_test(test_vector4_dividef);
	run_test(test_vector4_magnitude);
	run_test(test_vector4_normalize);
	run_test(test_vector4_distance);
	run_test(test_vector4_dot_product);
	run_test(test_vector4_cross_product);
	run_test(test_vector4_dot_product_perpendicular);
	run_test(test_vector4_normalize_zero);
	run_test(test_vector4_magnitude_zero);
	run_test(test_vector4_equals);

	return NULL;
}
