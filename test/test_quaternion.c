

/** [quaternion identity example] */
static char * test_quaternion_identity()
{
	quaternion q;	
	quaternion_identity(&q);
	test_assert(q.x == 0.0f);
	test_assert(q.y == 0.0f);
	test_assert(q.z == 0.0f);
	test_assert(q.w == 1.0f);
	test_assert(1.0f == quaternion_norm(&q));
	/* test_assert(0.0f == quaternion_magnitude(&q)); */
	test_assert(quaternion_is_unit(&q));
	test_assert(!quaternion_is_pure(&q));
	
	return 0;
}
/** [quaternion identity example] */


/** [quaternion conjugate example] */
static char * test_quaternion_conjugate()
{
	quaternion qA;
	quaternion qB;
	
	quaternion_set_from_axis_anglef3(&qA, 1.0f, 1.0f, 1.0f, HYP_TAU / 4.0f);
	quaternion_set_from_axis_anglef3(&qB, -1.0f, -1.0f, -1.0f, HYP_TAU / 4.0f);
	quaternion_conjugate(&qB);
	test_assert(quaternion_equals(&qA, &qB));
		
	return 0;
}
/** [quaternion conjugate example] */


/** [quaternion inverse example] */
static char * test_quaternion_inverse()
{
	quaternion qA;
	quaternion qInverse;
	quaternion qIdentity;
	
	quaternion_set_from_axis_anglef3(&qA, 1.0f, 1.0f, 1.0f, HYP_TAU / 4.0f);
	quaternion_set(&qInverse, &qA);
	quaternion_inverse(&qInverse);
	quaternion_multiply(&qA, &qInverse);
	quaternion_normalize(&qA);
	quaternion_identity(&qIdentity);
	test_assert(quaternion_equals(&qA, &qIdentity));
		
	return 0;
}
/** [quaternion inverse example] */


static char * test_quaternion_axis_anglev3()
{
	quaternion q, q1;
	float c;
	float s;
	float angle = HYP_TAU / 4.0f;
	vector3 axis;
	
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, angle);

	vector3_set(&axis, HYP_VECTOR3_UNIT_X);

	c = HYP_COS(angle / 2.0f);
	s = HYP_SIN(angle / 2.0f);

	q1.x = axis.x * s;
	q1.y = axis.y * s;
	q1.z = axis.z * s;
	q1.w = c;

	quaternion_normalize(&q1);

	test_assert(quaternion_equals(&q, &q1));
	
	return 0;
}


static char * test_quaternion_get_set_axis_anglev3()
{
	quaternion q;
	float angle, angle1;
	vector3 axis, axis1;
	
	vector3_set(&axis, HYP_VECTOR3_UNIT_X);
	angle = HYP_TAU / 4.0f;
	
	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	quaternion_get_axis_anglev3(&q, &axis1, &angle1);

	test_assert(vector3_equals(&axis, &axis1));
	test_assert(scalar_equalsf(angle,angle1));
	
	return 0;
}


static char * test_quaternion_multiply()
{
	quaternion qA, qB;

	quaternion_set_from_axis_anglev3(&qA, HYP_VECTOR3_UNIT_X, HYP_TAU / 8.0f);
	quaternion_set_from_axis_anglev3(&qB, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);

	/* qA squared */
	quaternion_multiply(&qA, &qA);

	test_assert(quaternion_equals(&qA, &qB));

	return 0;
}


static char * test_quaternion_multiply_identity()
{
	quaternion qA, qB, q;

	qA.x = 1;
	qA.y = 2;
	qA.z = 3;
	qA.w = 4;

	quaternion_identity(&qB);

	quaternion_set(&q, &qA);
	quaternion_multiply(&q, &qB);

	test_assert(quaternion_equals(&q, &qA));

	return 0;
}


static char * test_vector3_rotate_by_quaternion_xy_quarter_turn()
{
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z));

	return 0;
}


static char * test_vector3_rotate_by_quaternion_xz_quarter_turn()
{
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return 0;
}


static char * test_vector3_rotate_by_quaternion_yx_quarter_turn()
{
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Z_NEGATIVE));

	return 0;
}


static char * test_vector3_rotate_by_quaternion_yz_quarter_turn()
{
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Y, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Z), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X));

	return 0;
}


static char * test_vector3_rotate_by_quaternion_zx_quarter_turn()
{
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_X), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return 0;
}


static char * test_vector3_rotate_by_quaternion_zy_quarter_turn()
{
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / 4.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_X_NEGATIVE));

	return 0;
}


static char * test_vector3_rotate_by_quaternion_xy_half_turn()
{
	quaternion q;
	vector3 r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_X, HYP_TAU / 2.0f);
	vector3_rotate_by_quaternion(vector3_set(&r, HYP_VECTOR3_UNIT_Y), &q);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y_NEGATIVE));

	return 0;
}


static char * test_quaternion_slerp()
{
	quaternion q, q1, q2, q3;
	float angle;

	angle = HYP_TAU / 4.0f;

	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_X, angle);
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_X, angle * 1.1f);
	quaternion_set_from_axis_anglev3(&q3, HYP_VECTOR3_UNIT_X, angle * 1.2f);

	/* half-way */
	quaternion_slerp(&q1, &q3, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* none */
	quaternion_slerp(&q1, &q3, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q1));

	/* all the way */
	quaternion_slerp(&q1, &q3, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q3));


	/* swap order half-way */
	quaternion_slerp(&q3, &q1, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* swap order none */
	quaternion_slerp(&q3, &q1, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q3));

	/* swap order all the way */
	quaternion_slerp(&q3, &q1, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q1));


	/* go reverse around the sphere */
	quaternion_set_from_axis_anglev3(&q1, HYP_VECTOR3_UNIT_X, angle);
	quaternion_set_from_axis_anglev3(&q2, HYP_VECTOR3_UNIT_X, angle * 0.9f);
	quaternion_set_from_axis_anglev3(&q3, HYP_VECTOR3_UNIT_X, angle * 0.8f);

	/* go reverse half-way */
	quaternion_slerp(&q1, &q3, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* go reverse none */
	quaternion_slerp(&q1, &q3, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q1));

	/* go reverse all the way */
	quaternion_slerp(&q1, &q3, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q3));


	/* swap order reverse half-way */
	quaternion_slerp(&q3, &q1, 0.5f, &q);
	test_assert(quaternion_equals(&q, &q2));

	/* swap order reverse none */
	quaternion_slerp(&q3, &q1, 0.0f, &q);
	test_assert(quaternion_equals(&q, &q3));

	/* swap order reverse all the way */
	quaternion_slerp(&q3, &q1, 1.0f, &q);
	test_assert(quaternion_equals(&q, &q1));

	return 0;
}


/*static char * test_quaternion_tofromrotationmatrix()
{
	quaternion q, q1, q2;
	matrix4 m, m1;
	
	q.x = 0.3f;
	q.y = 0.16f;
	q.z = 0.75f;
	q.w = 0.40f;
	quaternion_normalize(&q);
	
	quaternion_to_rotation_matrix4(&q, &m);
	quaternion_from_rotation_matrix4(&q1, &m);
	quaternion_normalize(&q1);
	quaternion_to_rotation_matrix4(&q1, &m1);
	quaternion_from_rotation_matrix4(&q2, &m1);
	quaternion_normalize(&q2);
	
	printf("%lf %lf %lf %lf %lf %lf\r\n",
		quaternion_angle_between(&q, &q1), 
		quaternion_angle_between(&q1, &q),
		quaternion_angle_between(&q1, &q2),
		quaternion_angle_between(&q2, &q1),
		quaternion_angle_between(&q, &q2),
		quaternion_angle_between(&q2, &q)
		);
			
	_quaternion_print(&q);
	_quaternion_print(&q1);
	_quaternion_print(&q2);
	
	return 0;
	
	test_assert(matrix4_equalsm4(&m, &m1) == 1);
	test_assert(quaternion_equals(&q, &q2) == 1);
	test_assert(quaternion_equals(&q, &q1) == 1);
	
	return 0;
}*/


static char * quaternion_all_tests()
{
	run_test(test_quaternion_identity);
	run_test(test_quaternion_conjugate);
	run_test(test_quaternion_inverse);
	run_test(test_quaternion_axis_anglev3);
	run_test(test_quaternion_multiply);
	run_test(test_quaternion_multiply_identity);	
	run_test(test_vector3_rotate_by_quaternion_xy_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_xz_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_yx_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_yz_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_zx_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_zy_quarter_turn);
	run_test(test_vector3_rotate_by_quaternion_xy_half_turn);
	run_test(test_quaternion_get_set_axis_anglev3);
	run_test(test_quaternion_slerp);	
	/*run_test(test_quaternion_tofromrotationmatrix);*/
	
	return 0;
}
