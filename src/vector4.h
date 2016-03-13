#ifndef _INC_VECTOR4
#define _INC_VECTOR4


struct tagvector4
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


HYPAPI int vector4_equals(const vector4 *self, const vector4 *vT);

HYPAPI vector4 * vector4_zero(vector4 *self);
HYPAPI vector4 * vector4_set(vector4 *self, const vector4 *vT);
HYPAPI vector4 * vector4_setf4(vector4 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT, HYP_FLOAT wT);
HYPAPI vector4 * vector4_negate(vector4 *self);
HYPAPI vector4 * vector4_add(vector4 *self, const vector4 *vT);
HYPAPI vector4 * vector4_addf(vector4 *self, HYP_FLOAT fT);
HYPAPI vector4 * vector4_subtract(vector4 *self, const vector4 *vT);
HYPAPI vector4 * vector4_subtractf(vector4 *self, HYP_FLOAT fT);
HYPAPI vector4 * vector4_multiply(vector4 *self, const vector4 *vT);
HYPAPI vector4 * vector4_multiplyf(vector4 *self, HYP_FLOAT fT);
HYPAPI vector4 * vector4_divide(vector4 *self, const vector4 *vT);
HYPAPI vector4 * vector4_dividef(vector4 *self, HYP_FLOAT fT);

HYPAPI vector4 * vector4_normalize(vector4 *self);
HYPAPI HYP_FLOAT vector4_magnitude(const vector4 *self);
HYPAPI HYP_FLOAT vector4_distance(const vector4 *v1, const vector4 *v2);

HYPAPI HYP_FLOAT vector4_dot_product(const vector4 *self, const vector4 *vT);
HYPAPI vector4 * vector4_cross_product(vector4 *vR, const vector4 *vT1, const vector4 *vT2);

/* the length is the same as "magnitude" */
#define vector4_length(v) vector4_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define vec4 vector4

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_VECTOR4 */
