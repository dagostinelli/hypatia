/* SPDX-License-Identifier: MIT */

static char *test_vector3_set(void)
{
	struct vector3 v1 = {{{3.0f, 4.0f, 5.0f}}};
	struct vector3 v2;

	vector3_set(&v2, &v1);
	test_assert(scalar_equalsf(v2.x, 3.0f));
	test_assert(scalar_equalsf(v2.y, 4.0f));
	test_assert(scalar_equalsf(v2.z, 5.0f));

	return NULL;
}

static char *test_vector3_setf3(void)
{
	struct vector3 v;

	vector3_setf3(&v, 1.0f, 2.0f, 3.0f);
	test_assert(scalar_equalsf(v.x, 1.0f));
	test_assert(scalar_equalsf(v.y, 2.0f));
	test_assert(scalar_equalsf(v.z, 3.0f));

	return NULL;
}

static char *test_vector3_zero(void)
{
	struct vector3 v = {{{1.0f, 2.0f, 3.0f}}};

	vector3_zero(&v);
	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));

	return NULL;
}

static char *test_vector3_equals(void)
{
	struct vector3 v1 = {{{1.0f, 2.0f, 3.0f}}};
	struct vector3 v2 = {{{1.0f, 2.0f, 3.0f}}};
	struct vector3 v3 = {{{1.0f, 2.0f, 4.0f}}};

	test_assert(vector3_equals(&v1, &v2));
	test_assert(!vector3_equals(&v1, &v3));

	return NULL;
}

static char *test_vector3_negate(void)
{
	struct vector3 v = {{{3.0f, -4.0f, 5.0f}}};

	vector3_negate(&v);
	test_assert(scalar_equalsf(v.x, -3.0f));
	test_assert(scalar_equalsf(v.y, 4.0f));
	test_assert(scalar_equalsf(v.z, -5.0f));

	return NULL;
}

static char *test_vector3_add(void)
{
	struct vector3 v1 = {{{1.0f, 2.0f, 3.0f}}};
	struct vector3 v2 = {{{4.0f, 5.0f, 6.0f}}};

	vector3_add(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 5.0f));
	test_assert(scalar_equalsf(v1.y, 7.0f));
	test_assert(scalar_equalsf(v1.z, 9.0f));

	return NULL;
}

static char *test_vector3_addf(void)
{
	struct vector3 v = {{{1.0f, 2.0f, 3.0f}}};

	vector3_addf(&v, 5.0f);
	test_assert(scalar_equalsf(v.x, 6.0f));
	test_assert(scalar_equalsf(v.y, 7.0f));
	test_assert(scalar_equalsf(v.z, 8.0f));

	return NULL;
}

static char *test_vector3_subtract(void)
{
	struct vector3 v1 = {{{5.0f, 7.0f, 9.0f}}};
	struct vector3 v2 = {{{2.0f, 3.0f, 4.0f}}};

	vector3_subtract(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 3.0f));
	test_assert(scalar_equalsf(v1.y, 4.0f));
	test_assert(scalar_equalsf(v1.z, 5.0f));

	return NULL;
}

static char *test_vector3_subtractf(void)
{
	struct vector3 v = {{{5.0f, 7.0f, 9.0f}}};

	vector3_subtractf(&v, 2.0f);
	test_assert(scalar_equalsf(v.x, 3.0f));
	test_assert(scalar_equalsf(v.y, 5.0f));
	test_assert(scalar_equalsf(v.z, 7.0f));

	return NULL;
}

static char *test_vector3_multiply(void)
{
	struct vector3 v1 = {{{2.0f, 3.0f, 4.0f}}};
	struct vector3 v2 = {{{5.0f, 6.0f, 7.0f}}};

	vector3_multiply(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 10.0f));
	test_assert(scalar_equalsf(v1.y, 18.0f));
	test_assert(scalar_equalsf(v1.z, 28.0f));

	return NULL;
}

static char *test_vector3_multiplyf(void)
{
	struct vector3 v = {{{2.0f, 3.0f, 4.0f}}};

	vector3_multiplyf(&v, 3.0f);
	test_assert(scalar_equalsf(v.x, 6.0f));
	test_assert(scalar_equalsf(v.y, 9.0f));
	test_assert(scalar_equalsf(v.z, 12.0f));

	return NULL;
}

static char *test_vector3_divide(void)
{
	struct vector3 v1 = {{{10.0f, 18.0f, 28.0f}}};
	struct vector3 v2 = {{{5.0f, 6.0f, 7.0f}}};

	vector3_divide(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, 2.0f));
	test_assert(scalar_equalsf(v1.y, 3.0f));
	test_assert(scalar_equalsf(v1.z, 4.0f));

	return NULL;
}

static char *test_vector3_dividef(void)
{
	struct vector3 v = {{{6.0f, 9.0f, 12.0f}}};

	vector3_dividef(&v, 3.0f);
	test_assert(scalar_equalsf(v.x, 2.0f));
	test_assert(scalar_equalsf(v.y, 3.0f));
	test_assert(scalar_equalsf(v.z, 4.0f));

	return NULL;
}

static char *test_vector3_magnitude(void)
{
	/* 3-4-5 right triangle extended: sqrt(1+4+4) = 3 */
	struct vector3 v = {{{1.0f, 2.0f, 2.0f}}};

	test_assert(scalar_equalsf(vector3_magnitude(&v), 3.0f));

	return NULL;
}

static char *test_vector3_normalize(void)
{
	struct vector3 v = {{{1.0f, 2.0f, 2.0f}}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, 1.0f / 3.0f));
	test_assert(scalar_equalsf(v.y, 2.0f / 3.0f));
	test_assert(scalar_equalsf(v.z, 2.0f / 3.0f));
	test_assert(scalar_equalsf(vector3_magnitude(&v), 1.0f));

	return NULL;
}

static char *test_vector3_normalize_zero(void)
{
	struct vector3 v = {{{0.0f, 0.0f, 0.0f}}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));

	return NULL;
}

static char *test_vector3_distance(void)
{
	struct vector3 v1 = {{{1.0f, 2.0f, 3.0f}}};
	struct vector3 v2 = {{{4.0f, 6.0f, 3.0f}}};

	/* sqrt(9 + 16 + 0) = 5 */
	test_assert(scalar_equalsf(vector3_distance(&v1, &v2), 5.0f));

	return NULL;
}

static char *test_vector3_dot_product(void)
{
	struct vector3 v1 = {{{1.0f, 2.0f, 3.0f}}};
	struct vector3 v2 = {{{4.0f, 5.0f, 6.0f}}};

	/* 1*4 + 2*5 + 3*6 = 32 */
	test_assert(scalar_equalsf(vector3_dot_product(&v1, &v2), 32.0f));

	return NULL;
}

static char *test_vector3_dot_product_perpendicular(void)
{
	struct vector3 v1 = {{{1.0f, 0.0f, 0.0f}}};
	struct vector3 v2 = {{{0.0f, 1.0f, 0.0f}}};

	test_assert(scalar_equalsf(vector3_dot_product(&v1, &v2), 0.0f));

	return NULL;
}

static char *test_vector3_cross_product(void)
{
	struct vector3 a;
	struct vector3 b;
	struct vector3 r;

	vector3_setf3(&a, 3.0f, -3.0f, 1.0f);
	vector3_setf3(&b, 4.0f, 9.0f, 2.0f);

	vector3_cross_product(&r, &a, &b);

	test_assert(scalar_equalsf(r.x, -15.0f));
	test_assert(scalar_equalsf(r.y, -2.0f));
	test_assert(scalar_equalsf(r.z, 39.0f));

	/* tests lack of commutative property */
	vector3_cross_product(&r, &b, &a);

	test_assert(!scalar_equalsf(r.x, -15.0f));
	test_assert(!scalar_equalsf(r.y, -2.0f));
	test_assert(!scalar_equalsf(r.z, 39.0f));

	return NULL;
}

static char *test_vector3_angle_between_perpendicular(void)
{
	struct vector3 v1 = {{{1.0f, 0.0f, 0.0f}}};
	struct vector3 v2 = {{{0.0f, 1.0f, 0.0f}}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_PI_HALF));
	return NULL;
}

static char *test_vector3_angle_between_same(void)
{
	struct vector3 v1 = {{{1.0f, 0.0f, 0.0f}}};
	struct vector3 v2 = {{{1.0f, 0.0f, 0.0f}}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), 0.0f));
	return NULL;
}

static char *test_vector3_angle_between_opposite(void)
{
	struct vector3 v1 = {{{1.0f, 0.0f, 0.0f}}};
	struct vector3 v2 = {{{-1.0f, 0.0f, 0.0f}}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_PI));
	return NULL;
}

static char *test_vector3_multiplym4_identity(void)
{
	struct vector3 v = {{{1.0f, 2.0f, 3.0f}}};
	struct matrix4 m;

	matrix4_identity(&m);
	vector3_multiplym4(&v, &m);
	test_assert(scalar_equalsf(v.x, 1.0f));
	test_assert(scalar_equalsf(v.y, 2.0f));
	test_assert(scalar_equalsf(v.z, 3.0f));

	return NULL;
}

static char *test_vector3_multiplym4_scaling(void)
{
	struct vector3 v = {{{1.0f, 2.0f, 3.0f}}};
	struct vector3 scale = {{{2.0f, 3.0f, 4.0f}}};
	struct matrix4 m;

	matrix4_make_transformation_scalingv3(&m, &scale);
	vector3_multiplym4(&v, &m);
	test_assert(scalar_equalsf(v.x, 2.0f));
	test_assert(scalar_equalsf(v.y, 6.0f));
	test_assert(scalar_equalsf(v.z, 12.0f));

	return NULL;
}

static char *test_vector3_find_normal_axis_between(void)
{
	struct vector3 v1 = {{{1.0f, 0.0f, 0.0f}}};
	struct vector3 v2 = {{{0.0f, 1.0f, 0.0f}}};
	struct vector3 vR;

	vector3_find_normal_axis_between(&vR, &v1, &v2);
	/* cross product of x and y axes is z axis, already normalized */
	test_assert(scalar_equalsf(vR.x, 0.0f));
	test_assert(scalar_equalsf(vR.y, 0.0f));
	test_assert(scalar_equalsf(vR.z, 1.0f));
	test_assert(scalar_equalsf(vector3_magnitude(&vR), 1.0f));

	return NULL;
}

static char *test_vector3_rotate_by_quaternion(void)
{
	struct vector3 v = {{{1.0f, 0.0f, 0.0f}}};
	struct vector3 axis = {{{0.0f, 0.0f, 1.0f}}};
	struct quaternion q;

	/* rotate (1,0,0) by 90 degrees around z axis -> (0,1,0) */
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI_HALF);
	vector3_rotate_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 1.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));

	return NULL;
}

static char *test_vector3_rotate_by_quaternion_180(void)
{
	struct vector3 v = {{{1.0f, 0.0f, 0.0f}}};
	struct vector3 axis = {{{0.0f, 0.0f, 1.0f}}};
	struct quaternion q;

	/* rotate (1,0,0) by 180 degrees around z axis -> (-1,0,0) */
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI);
	vector3_rotate_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, -1.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));

	return NULL;
}

static char *test_vector3_reflect_by_quaternion(void)
{
	struct vector3 v;
	struct quaternion q;

	/* pure z quaternion reflects through the xy plane
	 * (0,0,1) should flip to (0,0,-1)
	 */
	q.w = 0.0f; q.x = 0.0f; q.y = 0.0f; q.z = 1.0f;
	v.x = 0.0f; v.y = 0.0f; v.z = 1.0f;
	vector3_reflect_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, -1.0f));

	/* vector in the plane is preserved */
	q.w = 0.0f; q.x = 0.0f; q.y = 0.0f; q.z = 1.0f;
	v.x = 1.0f; v.y = 0.0f; v.z = 0.0f;
	vector3_reflect_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, 1.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, 0.0f));

	/* non-unit vector: (0,0,3) reflected through xy plane should be (0,0,-3)
	 * but normalize forces unit length, so we get (0,0,-1) instead
	 * this test documents the bug
	 */
	q.w = 0.0f; q.x = 0.0f; q.y = 0.0f; q.z = 1.0f;
	v.x = 0.0f; v.y = 0.0f; v.z = 3.0f;
	vector3_reflect_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, 0.0f));
	test_assert(scalar_equalsf(v.y, 0.0f));
	test_assert(scalar_equalsf(v.z, -3.0f));

	return NULL;
}

static char *vector3_all_tests(void)
{
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

	return NULL;
}
