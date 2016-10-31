#ifndef _INC_VECTOR2
#define _INC_VECTOR2


struct vector2
{
	union
		{
		HYP_FLOAT v[2];
		struct
			{
			HYP_FLOAT x, y;
			};
		};
};


HYPAPI int vector2_equals(const struct vector2 *self, const struct vector2 *vT);

HYPAPI struct vector2 *vector2_zero(struct vector2 *self);
HYPAPI struct vector2 *vector2_set(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_setf2(struct vector2 *self, HYP_FLOAT xT, HYP_FLOAT yT);
HYPAPI struct vector2 *vector2_negate(struct vector2 *self);
HYPAPI struct vector2 *vector2_add(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_addf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_subtract(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_subtractf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_multiply(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_multiplyf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_divide(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_dividef(struct vector2 *self, HYP_FLOAT fT);

HYPAPI struct vector2 *vector2_normalize(struct vector2 *self);
HYPAPI HYP_FLOAT vector2_magnitude(const struct vector2 *self);
HYPAPI HYP_FLOAT vector2_distance(const struct vector2 *v1, const struct vector2 *v2);

HYPAPI HYP_FLOAT vector2_dot_product(const struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_cross_product(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2);

HYPAPI HYP_FLOAT vector2_angle_between(const struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_find_normal_axis_between(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2);

HYPAPI struct vector2 *vector2_multiplym3(struct vector2 *self, const struct matrix3 *mT);

/* the length is the same as "magnitude" */
#define vector2_length(v) vector2_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define vec2 struct vector2

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_VECTOR2 */
