#ifndef _INC_QUATERNION
#define _INC_QUATERNION


struct quaternion
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


HYPAPI int quaternion_equals(const struct quaternion *self, const struct quaternion *vT);

HYPAPI struct quaternion *quaternion_identity(struct quaternion *self);
HYPAPI struct quaternion *quaternion_setf4(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT w);
HYPAPI struct quaternion *quaternion_set(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_add(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_multiply(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_multiplyv3(struct quaternion *self, const struct vector3 *vT);
HYPAPI struct quaternion *quaternion_multiplyf(struct quaternion *self, HYP_FLOAT f);
HYPAPI struct quaternion *quaternion_subtract(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_negate(struct quaternion *self);
HYPAPI struct quaternion *quaternion_conjugate(struct quaternion *self);
HYPAPI struct quaternion *quaternion_inverse(struct quaternion *self);

HYPAPI short quaternion_is_unit(struct quaternion *self);
HYPAPI short quaternion_is_pure(struct quaternion *self);
HYPAPI HYP_FLOAT quaternion_norm(const struct quaternion *self);
HYPAPI HYP_FLOAT quaternion_magnitude(const struct quaternion *self);
HYPAPI struct quaternion *quaternion_normalize(struct quaternion *self);
HYPAPI HYP_FLOAT quaternion_dot_product(const struct quaternion *self, const struct quaternion *qT);

HYPAPI struct quaternion *quaternion_lerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR);
HYPAPI struct quaternion *quaternion_nlerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR);
HYPAPI struct quaternion *quaternion_slerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR);

HYPAPI void quaternion_get_axis_anglev3(const struct quaternion *self, struct vector3 *vR, HYP_FLOAT *angle);

HYPAPI struct quaternion *quaternion_set_from_axis_anglev3(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct quaternion *quaternion_set_from_axis_anglef3(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT angle);

HYPAPI struct quaternion *quaternion_get_rotation_tov3(const struct vector3 *from, const struct vector3 *to, struct quaternion *qR);

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define quat struct quaternion

#define quat_equals quaternion_equals
#define quat_identity quaternion_identity
#define quat_lerp quaternion_lerp
#define quat_nlerp quaternion_nlerp
#define quat_slerp quaternion_slerp

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_QUATERNION */
