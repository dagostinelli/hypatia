/* SPDX-License-Identifier: MIT */

static char *test_vector4_set(void)
{
	struct vector4 v1, v2;

	/* Test vector4_setf4 */
	vector4_setf4(&v1, 1.0f, 2.0f, 3.0f, 4.0f);

	test_assert(scalar_equalsf(v1.x, 1.0f));
	test_assert(scalar_equalsf(v1.y, 2.0f));
	test_assert(scalar_equalsf(v1.z, 3.0f));
	test_assert(scalar_equalsf(v1.w, 4.0f));

	/* Test vector4_set - copying from another vector4 */
	vector4_set(&v2, &v1);

	test_assert(scalar_equalsf(v2.x, 1.0f));
	test_assert(scalar_equalsf(v2.y, 2.0f));
	test_assert(scalar_equalsf(v2.z, 3.0f));
	test_assert(scalar_equalsf(v2.w, 4.0f));

	/* Test that all components are properly copied */
	test_assert(vector4_equals(&v1, &v2));

	return NULL;
}

static char *test_vector4_zero(void)
{
	struct vector4 v;

	vector4_zero(&v);

	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));
	test_assert(scalar_equalsf(v.w, 0.0f));

	return NULL;
}

static char *test_vector4_negate(void)
{
	struct vector4 v = {{{1.0f, 2.0f, 3.0f, 4.0f}}};

	vector4_negate(&v);

	test_assert(scalar_equalsf(v.x, -1.0f));
	test_assert(scalar_equalsf(v.y, -2.0f));
	test_assert(scalar_equalsf(v.z, -3.0f));
	test_assert(scalar_equalsf(v.w, -4.0f));

	/* Negate negative values */
	vector4_negate(&v);

	test_assert(scalar_equalsf(v.x, 1.0f));
	test_assert(scalar_equalsf(v.y, 2.0f));
	test_assert(scalar_equalsf(v.z, 3.0f));
	test_assert(scalar_equalsf(v.w, 4.0f));

	/* Negate zero vector */
	vector4_zero(&v);
	vector4_negate(&v);

	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));
	test_assert(scalar_equalsf(v.w, 0.0f));

	return NULL;
}

static char *test_vector4_add(void)
{
	struct vector4 v1 = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
	struct vector4 v2 = {{{5.0f, 6.0f, 7.0f, 8.0f}}};

	vector4_add(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, 6.0f));
	test_assert(scalar_equalsf(v1.y, 8.0f));
	test_assert(scalar_equalsf(v1.z, 10.0f));
	test_assert(scalar_equalsf(v1.w, 12.0f));

	/* Add with negative values */
	{
		struct vector4 a = {{{1.0f, -2.0f, 3.0f, -4.0f}}};
		struct vector4 b = {{{-1.0f, 2.0f, -3.0f, 4.0f}}};

		vector4_add(&a, &b);

		test_assert(scalar_equalsf(a.x, 0.0f));
		test_assert(scalar_equalsf(a.y, 0.0f));
		test_assert(scalar_equalsf(a.z, 0.0f));
		test_assert(scalar_equalsf(a.w, 0.0f));
	}

	return NULL;
}

static char *test_vector4_addf(void)
{
	struct vector4 v = {{{1.0f, 2.0f, 3.0f, 4.0f}}};

	vector4_addf(&v, 10.0f);

	test_assert(scalar_equalsf(v.x, 11.0f));
	test_assert(scalar_equalsf(v.y, 12.0f));
	test_assert(scalar_equalsf(v.z, 13.0f));
	test_assert(scalar_equalsf(v.w, 14.0f));

	/* Add negative scalar */
	vector4_addf(&v, -10.0f);

	test_assert(scalar_equalsf(v.x, 1.0f));
	test_assert(scalar_equalsf(v.y, 2.0f));
	test_assert(scalar_equalsf(v.z, 3.0f));
	test_assert(scalar_equalsf(v.w, 4.0f));

	/* Add zero */
	vector4_addf(&v, 0.0f);

	test_assert(scalar_equalsf(v.x, 1.0f));
	test_assert(scalar_equalsf(v.y, 2.0f));
	test_assert(scalar_equalsf(v.z, 3.0f));
	test_assert(scalar_equalsf(v.w, 4.0f));

	return NULL;
}

static char *test_vector4_subtract(void)
{
	struct vector4 v1 = {{{5.0f, 7.0f, 9.0f, 11.0f}}};
	struct vector4 v2 = {{{1.0f, 2.0f, 3.0f, 4.0f}}};

	vector4_subtract(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, 4.0f));
	test_assert(scalar_equalsf(v1.y, 5.0f));
	test_assert(scalar_equalsf(v1.z, 6.0f));
	test_assert(scalar_equalsf(v1.w, 7.0f));

	/* Subtract same vector yields zero */
	{
		struct vector4 a = {{{3.0f, 4.0f, 5.0f, 6.0f}}};
		struct vector4 b = {{{3.0f, 4.0f, 5.0f, 6.0f}}};

		vector4_subtract(&a, &b);

		test_assert(scalar_equalsf(a.x, 0.0f));
		test_assert(scalar_equalsf(a.y, 0.0f));
		test_assert(scalar_equalsf(a.z, 0.0f));
		test_assert(scalar_equalsf(a.w, 0.0f));
	}

	return NULL;
}

static char *test_vector4_subtractf(void)
{
	struct vector4 v = {{{10.0f, 20.0f, 30.0f, 40.0f}}};

	vector4_subtractf(&v, 5.0f);

	test_assert(scalar_equalsf(v.x, 5.0f));
	test_assert(scalar_equalsf(v.y, 15.0f));
	test_assert(scalar_equalsf(v.z, 25.0f));
	test_assert(scalar_equalsf(v.w, 35.0f));

	/* Subtract negative scalar (effectively adds) */
	vector4_subtractf(&v, -5.0f);

	test_assert(scalar_equalsf(v.x, 10.0f));
	test_assert(scalar_equalsf(v.y, 20.0f));
	test_assert(scalar_equalsf(v.z, 30.0f));
	test_assert(scalar_equalsf(v.w, 40.0f));

	return NULL;
}

static char *test_vector4_multiply(void)
{
	struct vector4 v1 = {{{2.0f, 3.0f, 4.0f, 5.0f}}};
	struct vector4 v2 = {{{3.0f, 4.0f, 5.0f, 6.0f}}};

	vector4_multiply(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, 6.0f));
	test_assert(scalar_equalsf(v1.y, 12.0f));
	test_assert(scalar_equalsf(v1.z, 20.0f));
	test_assert(scalar_equalsf(v1.w, 30.0f));

	/* Multiply by zero vector */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
		struct vector4 z = {{{0.0f, 0.0f, 0.0f, 0.0f}}};

		vector4_multiply(&a, &z);

		test_assert(scalar_equalsf(a.x, 0.0f));
		test_assert(scalar_equalsf(a.y, 0.0f));
		test_assert(scalar_equalsf(a.z, 0.0f));
		test_assert(scalar_equalsf(a.w, 0.0f));
	}

	return NULL;
}

static char *test_vector4_multiplyf(void)
{
	struct vector4 v = {{{1.0f, 2.0f, 3.0f, 4.0f}}};

	vector4_multiplyf(&v, 3.0f);

	test_assert(scalar_equalsf(v.x, 3.0f));
	test_assert(scalar_equalsf(v.y, 6.0f));
	test_assert(scalar_equalsf(v.z, 9.0f));
	test_assert(scalar_equalsf(v.w, 12.0f));

	/* Multiply by zero */
	vector4_multiplyf(&v, 0.0f);

	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));
	test_assert(scalar_equalsf(v.w, 0.0f));

	/* Multiply by negative */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};

		vector4_multiplyf(&a, -2.0f);

		test_assert(scalar_equalsf(a.x, -2.0f));
		test_assert(scalar_equalsf(a.y, -4.0f));
		test_assert(scalar_equalsf(a.z, -6.0f));
		test_assert(scalar_equalsf(a.w, -8.0f));
	}

	return NULL;
}

static char *test_vector4_divide(void)
{
	struct vector4 v1 = {{{10.0f, 20.0f, 30.0f, 40.0f}}};
	struct vector4 v2 = {{{2.0f, 4.0f, 5.0f, 8.0f}}};

	vector4_divide(&v1, &v2);

	test_assert(scalar_equalsf(v1.x, 5.0f));
	test_assert(scalar_equalsf(v1.y, 5.0f));
	test_assert(scalar_equalsf(v1.z, 6.0f));
	test_assert(scalar_equalsf(v1.w, 5.0f));

	/* Divide by ones yields same vector */
	{
		struct vector4 a = {{{7.0f, 8.0f, 9.0f, 10.0f}}};
		struct vector4 ones = {{{1.0f, 1.0f, 1.0f, 1.0f}}};

		vector4_divide(&a, &ones);

		test_assert(scalar_equalsf(a.x, 7.0f));
		test_assert(scalar_equalsf(a.y, 8.0f));
		test_assert(scalar_equalsf(a.z, 9.0f));
		test_assert(scalar_equalsf(a.w, 10.0f));
	}

	return NULL;
}

static char *test_vector4_dividef(void)
{
	struct vector4 v = {{{8.0f, 12.0f, 16.0f, 20.0f}}};

	vector4_dividef(&v, 4.0f);
	test_assert(scalar_equalsf(v.x, 2.0f));
	test_assert(scalar_equalsf(v.y, 3.0f));
	test_assert(scalar_equalsf(v.z, 4.0f));
	test_assert(scalar_equalsf(v.w, 5.0f));

	return NULL;
}

static char *test_vector4_magnitude(void)
{
	/* Unit vector along x */
	struct vector4 vx = {{{1.0f, 0.0f, 0.0f, 0.0f}}};
	test_assert(scalar_equalsf(vector4_magnitude(&vx), 1.0f));

	/* Known magnitude: sqrt(1+4+9+16) = sqrt(30) */
	{
		struct vector4 v = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
		HYP_FLOAT expected = HYP_SQRT(30.0f);
		test_assert(scalar_equalsf(vector4_magnitude(&v), expected));
	}

	/* Zero vector */
	{
		struct vector4 z = {{{0.0f, 0.0f, 0.0f, 0.0f}}};
		test_assert(scalar_equalsf(vector4_magnitude(&z), 0.0f));
	}

	return NULL;
}

static char *test_vector4_normalize(void)
{
	/* Normalize a known vector */
	struct vector4 v = {{{3.0f, 0.0f, 0.0f, 0.0f}}};
	vector4_normalize(&v);

	test_assert(scalar_equalsf(v.x, 1.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));
	test_assert(scalar_equalsf(v.w, 0.0f));

	/* Normalize a general vector, magnitude should be 1 */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
		vector4_normalize(&a);
		test_assert(scalar_equalsf(vector4_magnitude(&a), 1.0f));
	}

	/* Normalize zero vector should not crash and remain zero */
	{
		struct vector4 z = {{{0.0f, 0.0f, 0.0f, 0.0f}}};
		vector4_normalize(&z);
		test_assert(scalar_equalsf(z.x, 0.0f));
		test_assert(scalar_equalsf(z.y, 0.0f));
		test_assert(scalar_equalsf(z.z, 0.0f));
		test_assert(scalar_equalsf(z.w, 0.0f));
	}

	return NULL;
}

static char *test_vector4_distance(void)
{
	/* Distance between same point is zero */
	struct vector4 v1 = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
	test_assert(scalar_equalsf(vector4_distance(&v1, &v1), 0.0f));

	/* Distance between origin and a unit axis vector */
	{
		struct vector4 origin = {{{0.0f, 0.0f, 0.0f, 0.0f}}};
		struct vector4 point = {{{1.0f, 0.0f, 0.0f, 0.0f}}};
		test_assert(scalar_equalsf(vector4_distance(&origin, &point), 1.0f));
	}

	/* Known distance: sqrt((4-1)^2 + (6-2)^2 + (8-3)^2 + (10-4)^2) = sqrt(9+16+25+36) = sqrt(86) */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
		struct vector4 b = {{{4.0f, 6.0f, 8.0f, 10.0f}}};
		HYP_FLOAT expected = HYP_SQRT(86.0f);
		test_assert(scalar_equalsf(vector4_distance(&a, &b), expected));
	}

	/* Distance is symmetric */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
		struct vector4 b = {{{5.0f, 6.0f, 7.0f, 8.0f}}};
		test_assert(scalar_equalsf(vector4_distance(&a, &b), vector4_distance(&b, &a)));
	}

	return NULL;
}

static char *test_vector4_dot_product(void)
{
	/* Dot product of orthogonal vectors is zero */
	struct vector4 vx = {{{1.0f, 0.0f, 0.0f, 0.0f}}};
	struct vector4 vy = {{{0.0f, 1.0f, 0.0f, 0.0f}}};
	test_assert(scalar_equalsf(vector4_dot_product(&vx, &vy), 0.0f));

	/* Dot product of parallel vectors */
	{
		struct vector4 a = {{{2.0f, 0.0f, 0.0f, 0.0f}}};
		struct vector4 b = {{{3.0f, 0.0f, 0.0f, 0.0f}}};
		test_assert(scalar_equalsf(vector4_dot_product(&a, &b), 6.0f));
	}

	/* General dot product: 1*5 + 2*6 + 3*7 + 4*8 = 5+12+21+32 = 70 */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
		struct vector4 b = {{{5.0f, 6.0f, 7.0f, 8.0f}}};
		test_assert(scalar_equalsf(vector4_dot_product(&a, &b), 70.0f));
	}

	/* Dot product with zero vector */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
		struct vector4 z = {{{0.0f, 0.0f, 0.0f, 0.0f}}};
		test_assert(scalar_equalsf(vector4_dot_product(&a, &z), 0.0f));
	}

	return NULL;
}

static char *test_vector4_cross_product(void)
{
	struct vector4 result;

	/* Cross product of x and y unit vectors should be z */
	{
		struct vector4 vx = {{{1.0f, 0.0f, 0.0f, 0.0f}}};
		struct vector4 vy = {{{0.0f, 1.0f, 0.0f, 0.0f}}};

		vector4_cross_product(&result, &vx, &vy);

		test_assert(scalar_equalsf(result.x, 0.0f));
		test_assert(scalar_equalsf(result.y, 0.0f));
		test_assert(scalar_equalsf(result.z, 1.0f));
		test_assert(scalar_equalsf(result.w, 0.0f));
	}

	/* Cross product of y and x unit vectors should be -z */
	{
		struct vector4 vx = {{{1.0f, 0.0f, 0.0f, 0.0f}}};
		struct vector4 vy = {{{0.0f, 1.0f, 0.0f, 0.0f}}};

		vector4_cross_product(&result, &vy, &vx);

		test_assert(scalar_equalsf(result.x, 0.0f));
		test_assert(scalar_equalsf(result.y, 0.0f));
		test_assert(scalar_equalsf(result.z, -1.0f));
		test_assert(scalar_equalsf(result.w, 0.0f));
	}

	/* General cross product: (1,2,3,w) x (4,5,6,w) */
	/* x = 2*6 - 3*5 = 12-15 = -3 */
	/* y = 3*4 - 1*6 = 12-6 = 6 */
	/* z = 1*5 - 2*4 = 5-8 = -3 */
	/* w = w*w - w*w = 0 */
	{
		struct vector4 a = {{{1.0f, 2.0f, 3.0f, 1.0f}}};
		struct vector4 b = {{{4.0f, 5.0f, 6.0f, 1.0f}}};

		vector4_cross_product(&result, &a, &b);

		test_assert(scalar_equalsf(result.x, -3.0f));
		test_assert(scalar_equalsf(result.y, 6.0f));
		test_assert(scalar_equalsf(result.z, -3.0f));
		test_assert(scalar_equalsf(result.w, 0.0f));
	}

	/* Cross product of parallel vectors is zero */
	{
		struct vector4 a = {{{2.0f, 4.0f, 6.0f, 0.0f}}};
		struct vector4 b = {{{1.0f, 2.0f, 3.0f, 0.0f}}};

		vector4_cross_product(&result, &a, &b);

		test_assert(scalar_equalsf(result.x, 0.0f));
		test_assert(scalar_equalsf(result.y, 0.0f));
		test_assert(scalar_equalsf(result.z, 0.0f));
		test_assert(scalar_equalsf(result.w, 0.0f));
	}

	return NULL;
}

static char *test_vector4_equals(void)
{
	struct vector4 a = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
	struct vector4 b = {{{1.0f, 2.0f, 3.0f, 4.0f}}};
	struct vector4 c = {{{1.0f, 2.0f, 3.0f, 5.0f}}};

	test_assert(vector4_equals(&a, &b));
	test_assert(!vector4_equals(&a, &c));

	/* Self-equality */
	test_assert(vector4_equals(&a, &a));

	return NULL;
}

static char *vector4_all_tests(void)
{
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
	run_test(test_vector4_equals);

	return NULL;
}
