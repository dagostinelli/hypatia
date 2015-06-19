#ifndef _INC_VECTOR3
#define _INC_VECTOR3


struct tagvector3
{
	union vector3_value
		{
		float v[3];
		struct vector3_xyz
			{
			float x, y, z;
			};
		struct vector3_ypr
			{
			float yaw, pitch, roll;
			};
		};
};


HYPAPI int vector3_equals(const vector3 *self, const vector3 *vT);

HYPAPI vector3 * vector3_zero(vector3 *self);
HYPAPI vector3 * vector3_set(vector3 *self, const vector3 *vT);
HYPAPI vector3 * vector3_setf3(vector3 *self, float xT, float yT, float zT);
HYPAPI vector3 * vector3_negate(vector3 *self);
HYPAPI vector3 * vector3_add(vector3 *self, const vector3 *vT);
HYPAPI vector3 * vector3_addf(vector3 *self, float fT);
HYPAPI vector3 * vector3_subtract(vector3 *self, const vector3 *vT);
HYPAPI vector3 * vector3_subtractf(vector3 *self, float fT);
HYPAPI vector3 * vector3_multiply(vector3 *self, const vector3 *vT);
HYPAPI vector3 * vector3_multiplyf(vector3 *self, float fT);
HYPAPI vector3 * vector3_divide(vector3 *self, const vector3 *vT);
HYPAPI vector3 * vector3_dividef(vector3 *self, float fT);

HYPAPI vector3 * vector3_normalize(vector3 *self);
HYPAPI float vector3_magnitude(const vector3 *self);
HYPAPI float vector3_distance(const vector3 *v1, const vector3 *v2);

HYPAPI float vector3_dot_product(const vector3 *self, const vector3 *vT);
HYPAPI vector3 * vector3_cross_product(vector3 *vR, const vector3 *vT1, const vector3 *vT2);

HYPAPI float vector3_angle_between(const vector3 *self, const vector3 *vT);
HYPAPI vector3 * vector3_find_normal_axis_between(vector3 *vR, const vector3 *vT1, const vector3 *vT2);

HYPAPI vector3 * vector3_multiplym4(vector3 *self, const matrix4 *mT);
HYPAPI vector3 * vector3_rotate_by_quaternion(vector3 *self, const quaternion *qT);
HYPAPI vector3 * vector3_reflect_by_quaternion(vector3 *self, const quaternion *qT);

/* the length is the same as "magnitude" */
#define vector3_length(v) vector3_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define vec3 vector3

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_VECTOR3 */
