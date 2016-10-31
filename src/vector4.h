#ifndef _INC_VECTOR4
#define _INC_VECTOR4


struct vector4
{
	union
	{
		HYP_FLOAT v[4];
		struct
		{
			HYP_FLOAT x, y, z, w;
		};
	};
};


HYPAPI int vector4_equals(const struct vector4 *self, const struct vector4 *vT);

HYPAPI struct vector4 *vector4_zero(struct vector4 *self);
HYPAPI struct vector4 *vector4_set(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_setf4(struct vector4 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT, HYP_FLOAT wT);
HYPAPI struct vector4 *vector4_negate(struct vector4 *self);
HYPAPI struct vector4 *vector4_add(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_addf(struct vector4 *self, HYP_FLOAT fT);
HYPAPI struct vector4 *vector4_subtract(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_subtractf(struct vector4 *self, HYP_FLOAT fT);
HYPAPI struct vector4 *vector4_multiply(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_multiplyf(struct vector4 *self, HYP_FLOAT fT);
HYPAPI struct vector4 *vector4_divide(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_dividef(struct vector4 *self, HYP_FLOAT fT);

HYPAPI struct vector4 *vector4_normalize(struct vector4 *self);
HYPAPI HYP_FLOAT vector4_magnitude(const struct vector4 *self);
HYPAPI HYP_FLOAT vector4_distance(const struct vector4 *v1, const struct vector4 *v2);

HYPAPI HYP_FLOAT vector4_dot_product(const struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_cross_product(struct vector4 *vR, const struct vector4 *vT1, const struct vector4 *vT2);

/* the length is the same as "magnitude" */
#define vector4_length(v) vector4_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define vec4 struct vector4

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_VECTOR4 */
