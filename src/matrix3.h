#ifndef _INC_MATRIX3
#define _INC_MATRIX3


struct matrix3
{
	union
	{
		HYP_FLOAT m[9]; /* row-major numbering */
		struct
		{
			/* reference the matrix [row][column] */
			HYP_FLOAT m33[3][3];
		};
		struct
		{
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i03, i06;
			HYP_FLOAT i01, i04, i07;
			HYP_FLOAT i02, i05, i08;
		};
		struct
		{
			/* col-row */
			HYP_FLOAT c00, c10, c20;
			HYP_FLOAT c01, c11, c21;
			HYP_FLOAT c02, c12, c22;
		};
		struct
		{
			/* row-col */
			HYP_FLOAT r00, r01, r02;
			HYP_FLOAT r10, r11, r12;
			HYP_FLOAT r20, r21, r22;
		};
	};
};


HYPAPI int matrix3_equals(const struct matrix3 *self, const struct matrix3 *mT);

HYPAPI struct matrix3 *matrix3_zero(struct matrix3 *self);
HYPAPI struct matrix3 *matrix3_identity(struct matrix3 *self);
HYPAPI struct matrix3 *matrix3_set(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_add(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_subtract(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_multiply(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_multiplyf(struct matrix3 *self, HYP_FLOAT scalar);
HYPAPI struct matrix3 *matrix3_transpose(struct matrix3 *self);

HYPAPI struct matrix3 *matrix3_make_transformation_translationv2(struct matrix3 *self, const struct vector2 *translation);
HYPAPI struct matrix3 *matrix3_make_transformation_scalingv2(struct matrix3 *self, const struct vector2 *scale);
HYPAPI struct matrix3 *matrix3_make_transformation_rotationf_z(struct matrix3 *self, HYP_FLOAT angle);

HYPAPI struct matrix3 *matrix3_translatev2(struct matrix3 *self, const struct vector2 *translation);
HYPAPI struct matrix3 *matrix3_rotate(struct matrix3 *self, HYP_FLOAT angle);
HYPAPI struct matrix3 *matrix3_scalev2(struct matrix3 *self, const struct vector2 *scale);

HYPAPI struct matrix3 *_matrix3_transpose_rowcolumn(struct matrix3 *self);
HYPAPI struct matrix3 *_matrix3_transpose_columnrow(struct matrix3 *self);


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define mat3 struct matrix3

#define mat3_equals matrix3_equals
#define mat3_zero matrix3_zero
#define mat3_identity matrix3_identity
#define mat3_set matrix3_setm3
#define mat3_add matrix3_add
#define mat3_sub matrix3_subtract
#define mat3_mul matrix3_multiply
#define mat3_transpose matrix3_transpose

#define mat3_translatev2 matrix3_translatev2
#define mat3_rotate matrix3_rotate
#define mat3_scalev2 matrix3_scalev2


#define m3 struct matrix3

#define m3_equals matrix3_equals
#define m3_zero matrix3_zero
#define m3_identity matrix3_identity
#define m3_set matrix3_set
#define m3_add matrix3_add
#define m3_sub matrix3_subtract
#define m3_mul matrix3_multiply
#define m3_transpose matrix3_transpose

#define m3_translatev2 matrix3_translatev2
#define m3_rotate matrix3_rotate
#define m3_scalev2 matrix3_scalev2

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_MATRIX3 */
