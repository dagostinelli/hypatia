#ifndef _INC_MATRIX4
#define _INC_MATRIX4


struct matrix4
{
	union
	{
		HYP_FLOAT m[16]; /* row-major numbering */
		struct
		{
			/* reference the matrix [row][column] */
			HYP_FLOAT m44[4][4];
		};
		struct
		{
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i04, i08, i12;
			HYP_FLOAT i01, i05, i09, i13;
			HYP_FLOAT i02, i06, i10, i14;
			HYP_FLOAT i03, i07, i11, i15;
		};
		struct
		{
			/* col-row */
			HYP_FLOAT c00, c10, c20, c30;
			HYP_FLOAT c01, c11, c21, c31;
			HYP_FLOAT c02, c12, c22, c32;
			HYP_FLOAT c03, c13, c23, c33;
		};
		struct
		{
			/* row-col */
			HYP_FLOAT r00, r01, r02, r03;
			HYP_FLOAT r10, r11, r12, r13;
			HYP_FLOAT r20, r21, r22, r23;
			HYP_FLOAT r30, r31, r32, r33;
		};
	};
};


HYPAPI int matrix4_equals(const struct matrix4 *self, const struct matrix4 *mT);

HYPAPI struct matrix4 *matrix4_zero(struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_identity(struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_set(struct matrix4 *self, const struct matrix4 *mT);
HYPAPI struct matrix4 *matrix4_add(struct matrix4 *self, const struct matrix4 *mT);
HYPAPI struct matrix4 *matrix4_subtract(struct matrix4 *self, const struct matrix4 *mT);
HYPAPI struct matrix4 *matrix4_multiply(struct matrix4 *self, const struct matrix4 *mT);
HYPAPI struct matrix4 *matrix4_multiplyf(struct matrix4 *self, HYP_FLOAT scalar);
HYPAPI struct matrix4 *matrix4_transpose(struct matrix4 *self);

HYPAPI struct matrix4 *matrix4_make_transformation_translationv3(struct matrix4 *self, const struct vector3 *translation);
HYPAPI struct matrix4 *matrix4_make_transformation_scalingv3(struct matrix4 *self, const struct vector3 *scale);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationq(struct matrix4 *self, const struct quaternion *qT);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_x(struct matrix4 *self, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_y(struct matrix4 *self, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_z(struct matrix4 *self, HYP_FLOAT angle);

HYPAPI struct matrix4 *matrix4_translatev3(struct matrix4 *self, const struct vector3 *translation);
HYPAPI struct matrix4 *matrix4_rotatev3(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_scalev3(struct matrix4 *self, const struct vector3 *scale);

HYPAPI struct matrix4 *_matrix4_transpose_rowcolumn(struct matrix4 *self);
HYPAPI struct matrix4 *_matrix4_transpose_columnrow(struct matrix4 *self);


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define mat4 struct matrix4

#define mat4_equals matrix4_equals
#define mat4_zero matrix4_zero
#define mat4_identity matrix4_identity
#define mat4_set matrix4_setm4
#define mat4_add matrix4_add
#define mat4_sub matrix4_subtract
#define mat4_mul matrix4_multiply
#define mat4_transpose matrix4_transpose

#define mat4_translatev3 matrix3_translatev3
#define mat4_rotatev3 matrix3_rotatev3
#define mat4_scalev3 matrix3_scalev3


#define m4 struct matrix4

#define m4_equals matrix4_equals
#define m4_zero matrix4_zero
#define m4_identity matrix4_identity
#define m4_set matrix4_set
#define m4_add matrix4_add
#define m4_sub matrix4_subtract
#define m4_mul matrix4_multiply
#define m4_transpose matrix4_transpose

#define m4_translatev3 matrix3_translatev3
#define m4_rotatev3 matrix3_rotatev3
#define m4_scalev3 matrix3_scalev3

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_MATRIX4 */
