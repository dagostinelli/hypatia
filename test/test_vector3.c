/* SPDX-License-Identifier: MIT */

static char *test_vector3_set(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}}};
	struct vector3 v2;

	vector3_set(&v2, &v1);
	test_assert(scalar_equalsf(v2.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v2.y, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v2.z, HYP_FLOAT_C(5.0)));

	return NULL;
}

static char *test_vector3_setf3(void)
{
	struct vector3 v;

	vector3_setf3(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(3.0)));

	return NULL;
}

static char *test_vector3_zero(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};

	vector3_zero(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static char *test_vector3_equals(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};
	struct vector3 v3 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(4.0)}}};

	test_assert(vector3_equals(&v1, &v2));
	test_assert(!vector3_equals(&v1, &v3));

	return NULL;
}

static char *test_vector3_negate(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(3.0), -HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0)}}};

	vector3_negate(&v);
	test_assert(scalar_equalsf(v.x, -HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v.z, -HYP_FLOAT_C(5.0)));

	return NULL;
}

static char *test_vector3_add(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}}};

	vector3_add(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(7.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(9.0)));

	return NULL;
}

static char *test_vector3_addf(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};

	vector3_addf(&v, HYP_FLOAT_C(5.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(7.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(8.0)));

	return NULL;
}

static char *test_vector3_subtract(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(9.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}}};

	vector3_subtract(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(4.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(5.0)));

	return NULL;
}

static char *test_vector3_subtractf(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(5.0), HYP_FLOAT_C(7.0), HYP_FLOAT_C(9.0)}}};

	vector3_subtractf(&v, HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(5.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(7.0)));

	return NULL;
}

static char *test_vector3_multiply(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0)}}};

	vector3_multiply(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(10.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(18.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(28.0)));

	return NULL;
}

static char *test_vector3_multiplyf(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}}};

	vector3_multiplyf(&v, HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(9.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(12.0)));

	return NULL;
}

static char *test_vector3_divide(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(10.0), HYP_FLOAT_C(18.0), HYP_FLOAT_C(28.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0)}}};

	vector3_divide(&v1, &v2);
	test_assert(scalar_equalsf(v1.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v1.y, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v1.z, HYP_FLOAT_C(4.0)));

	return NULL;
}

static char *test_vector3_dividef(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(6.0), HYP_FLOAT_C(9.0), HYP_FLOAT_C(12.0)}}};

	vector3_dividef(&v, HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(4.0)));

	return NULL;
}

static char *test_vector3_magnitude(void)
{
	/* 3-4-5 right triangle extended: sqrt(1+4+4) = 3 */
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0)}}};

	test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(3.0)));

	return NULL;
}

static char *test_vector3_normalize(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0)}}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0) / HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0) / HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(2.0) / HYP_FLOAT_C(3.0)));
	test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(1.0)));

	return NULL;
}

static char *test_vector3_normalize_zero(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static char *test_vector3_distance(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(4.0), HYP_FLOAT_C(6.0), HYP_FLOAT_C(3.0)}}};

	/* sqrt(9 + 16 + 0) = 5 */
	test_assert(scalar_equalsf(vector3_distance(&v1, &v2), HYP_FLOAT_C(5.0)));

	return NULL;
}

static char *test_vector3_dot_product(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(4.0), HYP_FLOAT_C(5.0), HYP_FLOAT_C(6.0)}}};

	/* 1*4 + 2*5 + 3*6 = 32 */
	test_assert(scalar_equalsf(vector3_dot_product(&v1, &v2), HYP_FLOAT_C(32.0)));

	return NULL;
}

static char *test_vector3_dot_product_perpendicular(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}}};

	test_assert(scalar_equalsf(vector3_dot_product(&v1, &v2), HYP_FLOAT_C(0.0)));

	return NULL;
}

static char *test_vector3_cross_product(void)
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

static char *test_vector3_angle_between_perpendicular(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_PI_HALF));
	return NULL;
}

static char *test_vector3_angle_between_same(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_FLOAT_C(0.0)));
	return NULL;
}

static char *test_vector3_angle_between_opposite(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 v2 = {{{-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};

	test_assert(scalar_equalsf(vector3_angle_between(&v1, &v2), HYP_PI));
	return NULL;
}

static char *test_vector3_multiplym4_identity(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};
	struct matrix4 m;

	matrix4_identity(&m);
	vector3_multiplym4(&v, &m);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(3.0)));

	return NULL;
}

static char *test_vector3_multiplym4_scaling(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)}}};
	struct vector3 scale = {{{HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0)}}};
	struct matrix4 m;

	matrix4_make_transformation_scalingv3(&m, &scale);
	vector3_multiplym4(&v, &m);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(2.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(6.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(12.0)));

	return NULL;
}

static char *test_vector3_find_normal_axis_between(void)
{
	struct vector3 v1 = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 v2 = {{{HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 vR;

	vector3_find_normal_axis_between(&vR, &v1, &v2);
	/* cross product of x and y axes is z axis, already normalized */
	test_assert(scalar_equalsf(vR.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vR.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(vR.z, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(vector3_magnitude(&vR), HYP_FLOAT_C(1.0)));

	return NULL;
}

static char *test_vector3_rotate_by_quaternion(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 axis = {{{HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}}};
	struct quaternion q;

	/* rotate (1,0,0) by 90 degrees around z axis -> (0,1,0) */
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI_HALF);
	vector3_rotate_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static char *test_vector3_rotate_by_quaternion_180(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)}}};
	struct vector3 axis = {{{HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)}}};
	struct quaternion q;

	/* rotate (1,0,0) by 180 degrees around z axis -> (-1,0,0) */
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI);
	vector3_rotate_by_quaternion(&v, &q);
	test_assert(scalar_equalsf(v.x, -HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

	return NULL;
}

static char *test_vector3_reflect_by_quaternion(void)
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

static char *test_vector3_normalize_large(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1e15), HYP_FLOAT_C(1e15), HYP_FLOAT_C(1e15)}}};

	vector3_normalize(&v);
	test_assert(scalar_equalsf(vector3_magnitude(&v), HYP_FLOAT_C(1.0)));

	return NULL;
}

static char *test_vector3_normalize_small(void)
{
	struct vector3 v = {{{HYP_FLOAT_C(1e-15), HYP_FLOAT_C(1e-15), HYP_FLOAT_C(1e-15)}}};

	/* NOTE: vector3_normalize treats very small magnitudes as zero
	 * (via scalar_equalsf), so the vector is returned unchanged.
	 * Verify it does not crash and the vector remains as-is.
	 */
	vector3_normalize(&v);
	test_assert(scalar_equalsf(v.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.y, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(v.z, HYP_FLOAT_C(0.0)));

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
	run_test(test_vector3_normalize_large);
	run_test(test_vector3_normalize_small);

	return NULL;
}
