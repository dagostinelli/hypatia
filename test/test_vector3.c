
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

	return 0;
}

static char *vector3_all_tests(void)
{
	run_test(test_vector3_cross_product);

	return 0;
}
