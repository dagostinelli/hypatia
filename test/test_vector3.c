
static char *test_vector3_cross_product()
{
	vector3 a;
	vector3 b;
	vector3 r;

	vector3_setf3(&a, 3, -3, 1);
	vector3_setf3(&b, 4, 9, 2);

	vector3_cross_product(&r, &a, &b);

	test_assert(r.x == -15);
	test_assert(r.y == -2);
	test_assert(r.z == 39);

	/* tests lack of commutative property */
	vector3_cross_product(&r, &b, &a);

	test_assert(r.x != -15);
	test_assert(r.y != -2);
	test_assert(r.z != 39);

	return 0;
}

static char *vector3_all_tests()
{
	run_test(test_vector3_cross_product);

	return 0;
}
