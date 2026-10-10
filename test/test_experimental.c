/* SPDX-License-Identifier: MIT */

static const char *test_matrix4_transformation_decompose_translation(void)
{
	struct vector3 in_translation;
	struct vector3 out_translation;
	struct vector3 out_scale;
	struct quaternion out_rotation;
	struct matrix4 t;

	/* make a translation matrix */
	in_translation.x = HYP_FLOAT_C(1.0);
	in_translation.y = HYP_FLOAT_C(2.0);
	in_translation.z = HYP_FLOAT_C(3.0);
	matrix4_make_transformation_translationv3(&t, &in_translation);

	/* decompose */
	matrix4_transformation_decompose(&t, &out_scale, &out_rotation, &out_translation);

	/* same */
	test_assert(vector3_equals(&in_translation, &out_translation));

	return NULL;
}

static const char *test_matrix4_transformation_decompose_scaling(void)
{
	struct vector3 in_scale;
	struct vector3 out_translation;
	struct vector3 out_scale;
	struct quaternion out_rotation;
	struct matrix4 t;

	/* make a scaling matrix */
	in_scale.x = HYP_FLOAT_C(1.0);
	in_scale.y = HYP_FLOAT_C(2.0);
	in_scale.z = HYP_FLOAT_C(3.0);
	matrix4_make_transformation_scalingv3(&t, &in_scale);

	/* decompose */
	matrix4_transformation_decompose(&t, &out_scale, &out_rotation, &out_translation);

	/* same */
	test_assert(vector3_equals(&in_scale, &out_scale));

	return NULL;
}

/* compose, then decompose: the parts come back (the rotation as q or -q) */
static int decompose_round_trip(const struct vector3 *scale, const struct quaternion *rotation, const struct vector3 *translation)
{
	struct matrix4 m;
	struct vector3 out_scale;
	struct vector3 out_translation;
	struct quaternion out_rotation;

	matrix4_transformation_compose(&m, scale, rotation, translation);

	if (!matrix4_transformation_decompose(&m, &out_scale, &out_rotation, &out_translation)) {
		return 0;
	}

	return vector3_equals(&out_scale, scale) &&
		vector3_equals(&out_translation, translation) &&
		scalar_equalsf(quaternion_angle_between(&out_rotation, rotation), HYP_FLOAT_C(0.0));
}


static const char *test_matrix4_transformation_decompose_rotation(void)
{
	struct vector3 scale;
	struct vector3 translation;
	struct vector3 axis;
	struct quaternion rotation;

	vector3_setf3(&scale, HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	vector3_setf3(&translation, HYP_FLOAT_C(5.0), -HYP_FLOAT_C(6.0), HYP_FLOAT_C(7.0));

	/* a general rotation */
	vector3_normalize(vector3_setf3(&axis, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0)));
	quaternion_set_from_axis_anglev3(&rotation, &axis, HYP_FLOAT_C(1.1));
	test_assert(decompose_round_trip(&scale, &rotation, &translation));

	/* nearly half turns about each axis */
	quaternion_set_from_axis_anglev3(&rotation, HYP_VECTOR3_UNIT_X, HYP_FLOAT_C(3.0));
	test_assert(decompose_round_trip(&scale, &rotation, &translation));
	quaternion_set_from_axis_anglev3(&rotation, HYP_VECTOR3_UNIT_Y, HYP_FLOAT_C(3.0));
	test_assert(decompose_round_trip(&scale, &rotation, &translation));
	quaternion_set_from_axis_anglev3(&rotation, HYP_VECTOR3_UNIT_Z, HYP_FLOAT_C(3.0));
	test_assert(decompose_round_trip(&scale, &rotation, &translation));

	/* a mirror: a negative scale comes back on x */
	vector3_setf3(&scale, -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0));
	quaternion_set_from_axis_anglev3(&rotation, &axis, HYP_FLOAT_C(1.1));
	test_assert(decompose_round_trip(&scale, &rotation, &translation));

	return NULL;
}


static const char *test_matrix4_transformation_decompose_short_and_long(void)
{
	struct matrix4 m;
	struct vector3 scale;
	struct vector3 translation;
	struct vector3 out_scale;
	struct vector3 out_translation;
	struct quaternion rotation;
	struct quaternion out_rotation;
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-20), HYP_FLOAT_C(1e20) };
#elif defined(TEST_LONG_DOUBLE_RANGE)
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-2470), HYP_FLOAT_C(1e2470) };
#else
	HYP_FLOAT lengths[2] = { HYP_FLOAT_C(1e-160), HYP_FLOAT_C(1e160) };
#endif
	int i;

	/* scales whose squares underflow or overflow */
	quaternion_set_from_axis_anglev3(&rotation, HYP_VECTOR3_UNIT_Z, HYP_FLOAT_C(1.1));
	vector3_zero(&translation);

	for (i = 0; i < 2; i++) {
		vector3_setf3(&scale, lengths[i], HYP_FLOAT_C(2.0) * lengths[i], HYP_FLOAT_C(3.0) * lengths[i]);
		matrix4_transformation_compose(&m, &scale, &rotation, &translation);
		test_assert(matrix4_transformation_decompose(&m, &out_scale, &out_rotation, &out_translation));
		test_assert(scalar_equalsf(out_scale.x / scale.x, HYP_FLOAT_C(1.0)));
		test_assert(scalar_equalsf(out_scale.y / scale.y, HYP_FLOAT_C(1.0)));
		test_assert(scalar_equalsf(out_scale.z / scale.z, HYP_FLOAT_C(1.0)));
		test_assert(scalar_equalsf(quaternion_angle_between(&out_rotation, &rotation), HYP_FLOAT_C(0.0)));
	}

	return NULL;
}


static const char *test_matrix4_transformation_decompose_zero_scale(void)
{
	struct matrix4 m;
	struct vector3 scale;
	struct vector3 out_scale;
	struct vector3 out_translation;
	struct quaternion out_rotation;

	/* a zero scale loses the rotation: decompose reports it */
	vector3_setf3(&scale, HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	matrix4_make_transformation_scalingv3(&m, &scale);
	test_assert(!matrix4_transformation_decompose(&m, &out_scale, &out_rotation, &out_translation));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_quarter_turn_z(void)
{
	struct matrix4 m;
	struct quaternion q;
	struct vector3 r;

	/* right hand rule: a quarter turn about Z takes X to Y */
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	matrix4_set_from_quaternion(&m, &q);
	vector3_multiplym4(vector3_set(&r, HYP_VECTOR3_UNIT_X), &m);
	test_assert(vector3_equals(&r, HYP_VECTOR3_UNIT_Y));

	return NULL;
}


/* the matrix must rotate v the same way as the quaternion, and match
 * matrix4_set_from_axisv3_angle for the same axis and angle
 */
static int set_from_quaternion_matches_rotation(HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az, HYP_FLOAT angle)
{
	struct matrix4 m;
	struct matrix4 expected;
	struct quaternion q;
	struct vector3 axis;
	struct vector3 v;
	struct vector3 r;

	vector3_normalize(vector3_setf3(&axis, ax, ay, az));
	quaternion_set_from_axis_anglev3(&q, &axis, angle);
	matrix4_set_from_quaternion(&m, &q);
	matrix4_set_from_axisv3_angle(&expected, &axis, angle);

	vector3_setf3(&v, HYP_FLOAT_C(0.3), -HYP_FLOAT_C(1.2), HYP_FLOAT_C(2.5));
	vector3_set(&r, &v);
	vector3_multiplym4(&r, &m);
	vector3_rotate_by_quaternion(&v, &q);

	return vector3_equals(&r, &v) && matrix4_equals(&m, &expected);
}


static const char *test_matrix4_set_from_quaternion_matches_rotation(void)
{
	test_assert(set_from_quaternion_matches_rotation(HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.7)));
	test_assert(set_from_quaternion_matches_rotation(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(2.0)));
	test_assert(set_from_quaternion_matches_rotation(HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.1)));
	test_assert(set_from_quaternion_matches_rotation(-HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.5), HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0)));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_not_unit(void)
{
	struct matrix4 m;
	struct matrix4 expected;
	struct quaternion q;
	struct quaternion scaled;

	/* only the direction of the quaternion is used */
	quaternion_set_from_axis_anglef3(&q, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.6), HYP_FLOAT_C(0.8), HYP_FLOAT_C(1.3));
	matrix4_set_from_quaternion(&expected, &q);

	quaternion_setf4(&scaled, HYP_FLOAT_C(3.0) * q.x, HYP_FLOAT_C(3.0) * q.y, HYP_FLOAT_C(3.0) * q.z, HYP_FLOAT_C(3.0) * q.w);
	test_assert(matrix4_equals(matrix4_set_from_quaternion(&m, &scaled), &expected));

	quaternion_setf4(&scaled, HYP_FLOAT_C(0.001) * q.x, HYP_FLOAT_C(0.001) * q.y, HYP_FLOAT_C(0.001) * q.z, HYP_FLOAT_C(0.001) * q.w);
	test_assert(matrix4_equals(matrix4_set_from_quaternion(&m, &scaled), &expected));

	return NULL;
}


static const char *test_matrix4_set_from_quaternion_zero(void)
{
	struct matrix4 m;
	struct matrix4 identity;
	struct quaternion q;

	/* a zero quaternion describes no rotation */
	quaternion_setf4(&q, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
	matrix4_identity(&identity);
	test_assert(matrix4_equals(matrix4_set_from_quaternion(&m, &q), &identity));

	return NULL;
}


/* applies m to the point (x, y, z, 1) and divides by w: the normalized device
 * coordinates of a projection
 */
static struct vector3 *project(const struct matrix4 *m, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, struct vector3 *vR)
{
	struct vector4 point;
	struct vector4 clip;

	vector4_setf4(&point, x, y, z, HYP_FLOAT_C(1.0));
	matrix4_multiplyv4(m, &point, &clip);

	return vector3_setf3(vR, clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
}


static const char *test_matrix4_projection_perspective(void)
{
	struct matrix4 m;
	struct vector3 r;
	struct vector3 expected;

	/* 90 degrees vertical field of view, aspect 2, near 1, far 100: at the
	 * near plane (z = -1) the view is 4 wide and 2 high
	 */
	matrix4_projection_perspective_fovy_rh(&m, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));

	/* the near top right corner: depth 0 */
	project(&m, HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));

	/* the far bottom left corner: depth 1 */
	project(&m, -HYP_FLOAT_C(200.0), -HYP_FLOAT_C(100.0), -HYP_FLOAT_C(100.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0))));

	return NULL;
}


static const char *test_matrix4_projection_frustum(void)
{
	struct matrix4 m;
	struct matrix4 perspective;
	struct vector3 r;
	struct vector3 expected;

	/* off center: x from -1 to 3 and y from -2 to 1 at the near plane z = -2;
	 * at the far plane z = -50 the bounds are 25 times as large
	 */
	matrix4_projection_frustum_rh(&m, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(50.0));
	project(&m, HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));
	project(&m, -HYP_FLOAT_C(25.0), -HYP_FLOAT_C(50.0), -HYP_FLOAT_C(50.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0))));

	matrix4_projection_frustum_lh(&m, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(50.0));
	project(&m, HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));
	project(&m, -HYP_FLOAT_C(25.0), -HYP_FLOAT_C(50.0), HYP_FLOAT_C(50.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0))));

	/* centered bounds: the same as the perspective with that field of view
	 * (90 degrees, aspect 2, near 1: 4 wide and 2 high at the near plane)
	 */
	matrix4_projection_frustum_rh(&m, -HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	matrix4_projection_perspective_fovy_rh(&perspective, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	test_assert(matrix4_equals(&m, &perspective));
	matrix4_projection_frustum_lh(&m, -HYP_FLOAT_C(2.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	matrix4_projection_perspective_fovy_lh(&perspective, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	test_assert(matrix4_equals(&m, &perspective));

	return NULL;
}


static const char *test_matrix4_projection_perspective_infinite(void)
{
	struct matrix4 m;
	struct matrix4 far;
	struct vector3 r;
	struct vector3 expected;

	/* 90 degrees, aspect 2, near 1: the near top right corner is at depth 0 */
	matrix4_projection_perspective_fovy_infinite_rh(&m, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0));
	project(&m, HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));

	/* far away the depth approaches 1 and stays below it */
	project(&m, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(1e6), &r);
	test_assert(scalar_equalsf(r.z, HYP_FLOAT_C(1.0)) && r.z < HYP_FLOAT_C(1.0));

	/* the limit of the perspective with a far plane */
	matrix4_projection_perspective_fovy_rh(&far, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1e7));
	test_assert(matrix4_equals(&m, &far));

	matrix4_projection_perspective_fovy_infinite_lh(&m, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0));
	project(&m, HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));
	project(&m, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1e6), &r);
	test_assert(scalar_equalsf(r.z, HYP_FLOAT_C(1.0)) && r.z < HYP_FLOAT_C(1.0));
	matrix4_projection_perspective_fovy_lh(&far, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1e7));
	test_assert(matrix4_equals(&m, &far));

	return NULL;
}


static const char *test_vector3_project_to_window(void)
{
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
	const HYP_FLOAT far_tolerance = HYP_FLOAT_C(1e-4);
#else
	const HYP_FLOAT far_tolerance = HYP_FLOAT_C(1e-12);
#endif
	struct matrix4 m;
	struct matrix4 view;
	struct vector4 viewport;
	struct vector3 eye;
	struct vector3 v;
	struct vector3 point;
	struct vector3 expected;

	/* 90 degrees, aspect 2, near 1, far 100, in an 800 by 400 viewport at (10, 20) */
	matrix4_projection_perspective_fovy_rh(&m, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	vector4_setf4(&viewport, HYP_FLOAT_C(10.0), HYP_FLOAT_C(20.0), HYP_FLOAT_C(800.0), HYP_FLOAT_C(400.0));

	/* the near top right corner, the far bottom left corner and the center */
	vector3_setf3(&v, HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0));
	test_assert(vector3_project_to_window(&v, &m, &viewport) == &v);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(810.0), HYP_FLOAT_C(420.0), HYP_FLOAT_C(0.0))));
	vector3_setf3(&v, -HYP_FLOAT_C(200.0), -HYP_FLOAT_C(100.0), -HYP_FLOAT_C(100.0));
	vector3_project_to_window(&v, &m, &viewport);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(10.0), HYP_FLOAT_C(20.0), HYP_FLOAT_C(1.0))));

	/* back from the window: depth 0 is the near plane, 1 the far plane */
	vector3_setf3(&v, HYP_FLOAT_C(410.0), HYP_FLOAT_C(220.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_unproject_from_window(&v, &m, &viewport) == &v);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(1.0))));
	/* at the far plane w = 1 - 0.99, so the rounding of the matrix is magnified
	 * by far / near = 100: a relative tolerance of 100 times a few epsilons
	 */
	vector3_setf3(&v, HYP_FLOAT_C(10.0), HYP_FLOAT_C(20.0), HYP_FLOAT_C(1.0));
	vector3_unproject_from_window(&v, &m, &viewport);
	vector3_setf3(&expected, -HYP_FLOAT_C(200.0), -HYP_FLOAT_C(100.0), -HYP_FLOAT_C(100.0));
	test_assert(vector3_distance(&v, &expected) < far_tolerance * vector3_magnitude(&expected));

	/* a world point through a camera and back */
	vector3_setf3(&eye, HYP_FLOAT_C(3.0), HYP_FLOAT_C(4.0), HYP_FLOAT_C(10.0));
	matrix4_view_lookat_rh(&view, &eye, HYP_VECTOR3_ZERO, HYP_VECTOR3_UNIT_Y);
	matrix4_multiply(&view, &m); /* m * view */
	vector3_setf3(&point, HYP_FLOAT_C(0.5), -HYP_FLOAT_C(1.5), HYP_FLOAT_C(2.0));
	vector3_set(&v, &point);
	vector3_project_to_window(&v, &view, &viewport);
	test_assert(v.z > HYP_FLOAT_C(0.0) && v.z < HYP_FLOAT_C(1.0));
	vector3_unproject_from_window(&v, &view, &viewport);
	test_assert(vector3_equals(&v, &point));

	/* a point in the plane of the eye has no window position; a matrix
	 * with no inverse cannot be undone: NULL, and v is unchanged
	 */
	vector3_setf3(&v, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0));
	test_assert(vector3_project_to_window(&v, &m, &viewport) == NULL);
	test_assert(vector3_equals(&v, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));
	matrix4_zero(&m);
	test_assert(vector3_unproject_from_window(&v, &m, &viewport) == NULL);
	test_assert(vector3_equals(&v, &expected));

	return NULL;
}


static const char *test_matrix4_projection_ortho3d(void)
{
	struct matrix4 m;
	struct vector3 r;
	struct vector3 expected;

	/* an 800 by 600 box that does not start at the origin, near 1, far 100 */
	matrix4_projection_ortho3d_rh(&m, HYP_FLOAT_C(100.0), HYP_FLOAT_C(900.0), HYP_FLOAT_C(50.0), HYP_FLOAT_C(650.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));

	project(&m, HYP_FLOAT_C(900.0), HYP_FLOAT_C(650.0), -HYP_FLOAT_C(1.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));

	project(&m, HYP_FLOAT_C(100.0), HYP_FLOAT_C(50.0), -HYP_FLOAT_C(100.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0))));

	project(&m, HYP_FLOAT_C(500.0), HYP_FLOAT_C(350.0), -HYP_FLOAT_C(50.5), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.5))));

	return NULL;
}


static const char *test_matrix4_multiplyv3_exp(void)
{
	struct matrix4 m;
	struct matrix4 rotation;
	struct vector3 translation;
	struct vector3 v;
	struct vector3 r;
	struct vector3 expected;

	/* a quarter turn about Z, then a translation by (10, 20, 30) */
	vector3_setf3(&translation, HYP_FLOAT_C(10.0), HYP_FLOAT_C(20.0), HYP_FLOAT_C(30.0));
	matrix4_make_transformation_translationv3(&m, &translation);
	matrix4_multiply(&m, matrix4_make_transformation_rotationf_z(&rotation, HYP_TAU / HYP_FLOAT_C(4.0)));

	vector3_setf3(&v, HYP_FLOAT_C(0.2), HYP_FLOAT_C(0.4), HYP_FLOAT_C(0.9));
	matrix4_multiplyv3(&m, &v, &expected);
	matrix4_multiplyv3_EXP(&m, &v, &r);
	test_assert(vector3_equals(&r, &expected));

	return NULL;
}


static const char *test_matrix4_view_lookat(void)
{
	struct matrix4 m;
	struct vector3 eye;
	struct vector3 target;
	struct vector3 r;
	struct vector3 expected;

	/* the camera at (1, 2, 10) looks down -Z with +Y up */
	vector3_setf3(&eye, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(10.0));
	vector3_setf3(&target, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0));
	matrix4_view_lookat_rh(&m, &eye, &target, HYP_VECTOR3_UNIT_Y);

	/* the eye goes to the origin and the target 10 ahead, down -Z */
	matrix4_multiplyv3(&m, &eye, &r);
	test_assert(vector3_equals(&r, HYP_VECTOR3_ZERO));
	matrix4_multiplyv3(&m, &target, &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), -HYP_FLOAT_C(10.0))));

	/* right of and above the eye stay right and above */
	matrix4_multiplyv3(&m, vector3_setf3(&target, HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), HYP_FLOAT_C(10.0)), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));

	return NULL;
}


static const char *test_matrix4_projection_left_handed(void)
{
	struct matrix4 m;
	struct vector3 eye;
	struct vector3 target;
	struct vector3 r;
	struct vector3 expected;

	/* left-handed: the camera looks down +Z */
	matrix4_projection_perspective_fovy_lh(&m, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	project(&m, HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));
	project(&m, -HYP_FLOAT_C(200.0), -HYP_FLOAT_C(100.0), HYP_FLOAT_C(100.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0))));

	matrix4_projection_ortho3d_lh(&m, HYP_FLOAT_C(100.0), HYP_FLOAT_C(900.0), HYP_FLOAT_C(50.0), HYP_FLOAT_C(650.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	project(&m, HYP_FLOAT_C(900.0), HYP_FLOAT_C(650.0), HYP_FLOAT_C(1.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));
	project(&m, HYP_FLOAT_C(100.0), HYP_FLOAT_C(50.0), HYP_FLOAT_C(100.0), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0))));

	/* the eye at (1, 2, -10) looks at (1, 2, 0): the target is 10 ahead, +Z */
	vector3_setf3(&eye, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), -HYP_FLOAT_C(10.0));
	vector3_setf3(&target, HYP_FLOAT_C(1.0), HYP_FLOAT_C(2.0), HYP_FLOAT_C(0.0));
	matrix4_view_lookat_lh(&m, &eye, &target, HYP_VECTOR3_UNIT_Y);
	matrix4_multiplyv3(&m, &eye, &r);
	test_assert(vector3_equals(&r, HYP_VECTOR3_ZERO));
	matrix4_multiplyv3(&m, &target, &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0))));
	/* right of and above the eye stay right and above */
	matrix4_multiplyv3(&m, vector3_setf3(&target, HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(10.0)), &r);
	test_assert(vector3_equals(&r, vector3_setf3(&expected, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0))));

	return NULL;
}


static const char *test_matrix4_view_projection(void)
{
	struct matrix4 view;
	struct matrix4 viewProjection;
	struct vector3 eye;
	struct vector3 r;

	/* world to normalized device coordinates: the camera at (0, 0, 10) looks
	 * at the origin, which lands in the center of the screen
	 */
	vector3_setf3(&eye, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(10.0));
	matrix4_view_lookat_rh(&view, &eye, HYP_VECTOR3_ZERO, HYP_VECTOR3_UNIT_Y);
	matrix4_projection_perspective_fovy_rh(&viewProjection, HYP_TAU / HYP_FLOAT_C(4.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));

	/* matrix4_multiply(self, mT) is mT * self: projection * view */
	matrix4_multiply(&view, &viewProjection);

	project(&view, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), &r);
	test_assert(scalar_equalsf(r.x, HYP_FLOAT_C(0.0)));
	test_assert(scalar_equalsf(r.y, HYP_FLOAT_C(0.0)));
	test_assert(r.z > HYP_FLOAT_C(0.0) && r.z < HYP_FLOAT_C(1.0));

	/* 10 ahead with a 90 degree view: (10, 10, 0) is the top right corner */
	project(&view, HYP_FLOAT_C(10.0), HYP_FLOAT_C(10.0), HYP_FLOAT_C(0.0), &r);
	test_assert(scalar_equalsf(r.x, HYP_FLOAT_C(1.0)));
	test_assert(scalar_equalsf(r.y, HYP_FLOAT_C(1.0)));

	return NULL;
}


static const char *test_quaternion_angle_between(void)
{
	struct quaternion a;
	struct quaternion b;

	quaternion_identity(&a);
	quaternion_set_from_axis_anglev3(&b, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	test_assert(scalar_equalsf(quaternion_angle_between(&a, &b), HYP_TAU / HYP_FLOAT_C(4.0)));

	/* -b is the same rotation as b */
	quaternion_negate(&b);
	test_assert(scalar_equalsf(quaternion_angle_between(&a, &b), HYP_TAU / HYP_FLOAT_C(4.0)));

	/* the lengths do not matter */
	quaternion_setf4(&a, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(2.0));
	quaternion_multiplyf(&b, HYP_FLOAT_C(3.0));
	test_assert(scalar_equalsf(quaternion_angle_between(&a, &b), HYP_TAU / HYP_FLOAT_C(4.0)));

	/* small angles stay accurate */
	quaternion_identity(&a);
	quaternion_set_from_axis_anglev3(&b, HYP_VECTOR3_UNIT_X, HYP_FLOAT_C(0.001));
	test_assert(scalar_equalsf(quaternion_angle_between(&a, &b), HYP_FLOAT_C(0.001)));

	/* the same rotation */
	test_assert(scalar_equalsf(quaternion_angle_between(&b, &b), HYP_FLOAT_C(0.0)));

	return NULL;
}


static const char *test_quaternion_difference(void)
{
	struct quaternion a;
	struct quaternion b;

	quaternion_set_from_axis_anglev3(&a, HYP_VECTOR3_UNIT_Y, HYP_FLOAT_C(0.5));
	quaternion_set(&b, &a);
	test_assert(scalar_equalsf(quaternion_difference(&a, &b), HYP_FLOAT_C(0.0)));

	/* -a is the same rotation as a */
	quaternion_negate(&b);
	test_assert(scalar_equalsf(quaternion_difference(&a, &b), HYP_FLOAT_C(0.0)));

	/* different rotations score above zero */
	quaternion_identity(&b);
	test_assert(quaternion_difference(&a, &b) > HYP_FLOAT_C(0.01));

	return NULL;
}


static const char *test_matrix4_get_translation(void)
{
	struct matrix4 m;
	struct vector3 t;
	struct vector3 r;

	vector3_setf3(&t, HYP_FLOAT_C(1.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(3.0));
	matrix4_make_transformation_translationv3(&m, &t);
	test_assert(vector3_equals(matrix4_get_translation(&m, &r), &t));

	return NULL;
}


static const char *test_matrix4_make_transformation_rotationv3(void)
{
	struct matrix4 m;
	struct matrix4 expected;
	struct vector3 angles;

	/* the same as matrix4_set_from_euler_anglesf3 */
	vector3_setf3(&angles, HYP_FLOAT_C(0.4), -HYP_FLOAT_C(1.1), HYP_FLOAT_C(2.0));
	matrix4_make_transformation_rotationv3(&m, &angles);
	matrix4_set_from_euler_anglesf3(&expected, angles.x, angles.y, angles.z);
	test_assert(matrix4_equals(&m, &expected));

	return NULL;
}


static const char *test_quaternion_rotate_by_axis_angle(void)
{
	struct quaternion q;
	struct quaternion expected;

	/* a quarter turn about Z, then another quarter turn about Z: a half turn */
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	quaternion_rotate_by_axis_angle(&q, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(4.0));
	quaternion_set_from_axis_anglev3(&expected, HYP_VECTOR3_UNIT_Z, HYP_TAU / HYP_FLOAT_C(2.0));
	test_assert(scalar_equalsf(quaternion_angle_between(&q, &expected), HYP_FLOAT_C(0.0)));

	return NULL;
}


static const char *experimental_all_tests(void)
{
	run_test(test_quaternion_rotate_by_axis_angle);
	run_test(test_matrix4_make_transformation_rotationv3);
	run_test(test_matrix4_get_translation);
	run_test(test_matrix4_set_from_quaternion_quarter_turn_z);
	run_test(test_matrix4_set_from_quaternion_matches_rotation);
	run_test(test_matrix4_set_from_quaternion_not_unit);
	run_test(test_matrix4_set_from_quaternion_zero);
	run_test(test_matrix4_transformation_decompose_translation);
	run_test(test_matrix4_transformation_decompose_scaling);
	run_test(test_matrix4_transformation_decompose_rotation);
	run_test(test_matrix4_transformation_decompose_zero_scale);
	run_test(test_matrix4_transformation_decompose_short_and_long);
	run_test(test_matrix4_projection_perspective);
	run_test(test_matrix4_projection_ortho3d);
	run_test(test_matrix4_multiplyv3_exp);
	run_test(test_matrix4_view_lookat);
	run_test(test_matrix4_view_projection);
	run_test(test_matrix4_projection_left_handed);
	run_test(test_matrix4_projection_frustum);
	run_test(test_matrix4_projection_perspective_infinite);
	run_test(test_vector3_project_to_window);
	run_test(test_quaternion_angle_between);
	run_test(test_quaternion_difference);

	return NULL;
}
