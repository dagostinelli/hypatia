#ifndef _INC_VECTOR3
#define _INC_VECTOR3


struct vector3
{
	union
		{
		HYP_FLOAT v[3];
		struct
			{
			HYP_FLOAT x, y, z;
			};
		struct
			{
			HYP_FLOAT yaw, pitch, roll;
			};
		};
};


HYPAPI int vector3_equals(const struct vector3 *self, const struct vector3 *vT);

HYPAPI struct vector3 *vector3_zero(struct vector3 *self);
HYPAPI struct vector3 *vector3_set(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_setf3(struct vector3 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT);
HYPAPI struct vector3 *vector3_negate(struct vector3 *self);
HYPAPI struct vector3 *vector3_add(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_addf(struct vector3 *self, HYP_FLOAT fT);
HYPAPI struct vector3 *vector3_subtract(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_subtractf(struct vector3 *self, HYP_FLOAT fT);
HYPAPI struct vector3 *vector3_multiply(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_multiplyf(struct vector3 *self, HYP_FLOAT fT);
HYPAPI struct vector3 *vector3_divide(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_dividef(struct vector3 *self, HYP_FLOAT fT);

HYPAPI struct vector3 *vector3_normalize(struct vector3 *self);
HYPAPI HYP_FLOAT vector3_magnitude(const struct vector3 *self);
HYPAPI HYP_FLOAT vector3_distance(const struct vector3 *v1, const struct vector3 *v2);

HYPAPI HYP_FLOAT vector3_dot_product(const struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_cross_product(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2);

HYPAPI HYP_FLOAT vector3_angle_between(const struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_find_normal_axis_between(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2);

HYPAPI struct vector3 *vector3_multiplym4(struct vector3 *self, const struct matrix4 *mT);
HYPAPI struct vector3 *vector3_rotate_by_quaternion(struct vector3 *self, const struct quaternion *qT);
HYPAPI struct vector3 *vector3_reflect_by_quaternion(struct vector3 *self, const struct quaternion *qT);

/*the length is the same as "magnitude" */
#define vector3_length(v) vector3_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*BETA aliases */
#define vec3 struct vector3

#endif /*DOXYGEN_SHOULD_SKIP_THIS */

#endif /*_INC_VECTOR3 */
