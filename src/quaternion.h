#ifndef _INC_QUATERNION
#define _INC_QUATERNION


struct tagquaternion
{
	union
		{
		HYP_FLOAT q[4];
		struct
			{
			HYP_FLOAT x, y, z, w;
			};
		struct
			{
			HYP_FLOAT i, j, k, a;
			};
		};
};


HYPAPI int quaternion_equals(const quaternion *self, const quaternion *vT);

HYPAPI quaternion * quaternion_identity(quaternion *self);
HYPAPI quaternion * quaternion_setf4(quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT w);
HYPAPI quaternion * quaternion_set(quaternion *self, const quaternion *qT);
HYPAPI quaternion * quaternion_add(quaternion *self, const quaternion *qT);
HYPAPI quaternion * quaternion_multiply(quaternion *self, const quaternion *qT);
HYPAPI quaternion * quaternion_multiplyv3(quaternion *self, const vector3 *vT);
HYPAPI quaternion * quaternion_multiplyf(quaternion *self, HYP_FLOAT f);
HYPAPI quaternion * quaternion_subtract(quaternion *self, const quaternion *qT);
HYPAPI quaternion * quaternion_negate(quaternion *self);
HYPAPI quaternion * quaternion_conjugate(quaternion *self);
HYPAPI quaternion * quaternion_inverse(quaternion *self);

HYPAPI short quaternion_is_unit(quaternion *self);
HYPAPI short quaternion_is_pure(quaternion *self);
HYPAPI HYP_FLOAT quaternion_norm(const quaternion *self);
HYPAPI HYP_FLOAT quaternion_magnitude(const quaternion *self);
HYPAPI quaternion * quaternion_normalize(quaternion *self);
HYPAPI HYP_FLOAT quaternion_dot_product(const quaternion *self, const quaternion *qT);

HYPAPI quaternion * quaternion_lerp(const quaternion *start, const quaternion *end, HYP_FLOAT percent, quaternion *qR);
HYPAPI quaternion * quaternion_nlerp(const quaternion *start, const quaternion *end, HYP_FLOAT percent, quaternion *qR);
HYPAPI quaternion* quaternion_slerp(const quaternion *start, const quaternion *end, HYP_FLOAT percent, quaternion *qR);

HYPAPI void quaternion_get_axis_anglev3(const quaternion *self, vector3 *vR, HYP_FLOAT *angle);

HYPAPI quaternion * quaternion_set_from_axis_anglev3(quaternion *self, const vector3 *axis, HYP_FLOAT angle);
HYPAPI quaternion * quaternion_set_from_axis_anglef3(quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT angle);

HYPAPI quaternion * quaternion_get_rotation_tov3(const vector3 *from, const vector3 *to, quaternion *qR);


#endif /* _INC_QUATERNION */
