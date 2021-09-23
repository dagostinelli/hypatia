/* SPDX-License-Identifier: MIT */

#ifndef _HYPATIA_MAT6_H_
#define _HYPATIA_MAT6_H_

#include "hypatia.h"

/* forward declarations */
struct matrix6;

struct matrix6 {
	union {
		HYP_FLOAT m[36]; /* row-major numbering */
		struct {
			/* reference the matrix [row][column] */
			HYP_FLOAT m66[6][6];
		};
		struct {
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i06, i12, i18, i24, i30;
			HYP_FLOAT i01, i07, i13, i19, i25, i31;
			HYP_FLOAT i02, i08, i14, i20, i26, i32;
			HYP_FLOAT i03, i09, i15, i21, i27, i33;
			HYP_FLOAT i04, i10, i16, i22, i28, i34;
			HYP_FLOAT i05, i11, i17, i23, i29, i35;
		};
		struct {
			/* col-row */
			HYP_FLOAT c00, c10, c20, c30, c40, c50;
			HYP_FLOAT c01, c11, c21, c31, c41, c51;
			HYP_FLOAT c02, c12, c22, c32, c42, c52;
			HYP_FLOAT c03, c13, c23, c33, c43, c53;
			HYP_FLOAT c04, c14, c24, c34, c44, c54;
			HYP_FLOAT c05, c15, c25, c35, c45, c55;
		};
		struct {
			/* row-col */
			HYP_FLOAT r00, r01, r02, r03, r04, r05;
			HYP_FLOAT r10, r11, r12, r13, r14, r15;
			HYP_FLOAT r20, r21, r22, r23, r24, r25;
			HYP_FLOAT r30, r31, r32, r33, r34, r35;
			HYP_FLOAT r40, r41, r42, r43, r44, r45;
			HYP_FLOAT r50, r51, r52, r53, r54, r55;
		};
	};
};

HYPAPI int matrix6_equals(const struct matrix6 *self, const struct matrix6 *mT);

HYPAPI struct matrix6 *matrix6_zero(struct matrix6 *self);
HYPAPI struct matrix6 *matrix6_identity(struct matrix6 *self);
HYPAPI struct matrix6 *matrix6_set(struct matrix6 *self, const struct matrix6 *mT);
HYPAPI struct matrix6 *matrix6_add(struct matrix6 *self, const struct matrix6 *mT);
HYPAPI struct matrix6 *matrix6_subtract(struct matrix6 *self, const struct matrix6 *mT);

HYPAPI struct matrix6 *matrix6_multiply(struct matrix6 *self, const struct matrix6 *mT);
HYPAPI struct matrix6 *matrix6_multiplyf(struct matrix6 *self, HYP_FLOAT scalar);
HYPAPI struct vector4 *matrix6_multiplyv4(const struct matrix6 *self, const struct vector4 *vT, struct vector4 *vR);
HYPAPI struct vector3 *matrix6_multiplyv3(const struct matrix6 *self, const struct vector3 *vT, struct vector3 *vR);
HYPAPI struct vector2 *matrix6_multiplyv2(const struct matrix6 *self, const struct vector2 *vT, struct vector2 *vR);

HYPAPI struct matrix6 *matrix6_transpose(struct matrix6 *self);
HYPAPI HYP_FLOAT matrix6_determinant(const struct matrix6 *self);
HYPAPI struct matrix6 *matrix6_invert(struct matrix6 *self);
HYPAPI struct matrix6 *matrix6_inverse(const struct matrix6 *self, struct matrix6 *mR);

HYPAPI struct matrix6 *_matrix6_transpose_rowcolumn(struct matrix6 *self);
HYPAPI struct matrix6 *_matrix6_transpose_columnrow(struct matrix6 *self);

HYPAPI void _matrix6_print_with_columnrow_indexer(struct matrix6 *self);
HYPAPI void _matrix6_print_with_rowcolumn_indexer(struct matrix6 *self);
HYPAPI struct matrix6 *_matrix6_set_random(struct matrix6 *self);

/* BETA aliases */
#define mat6 struct matrix6
#define mat6_equals matrix6_equals
#define mat6_zero matrix6_zero
#define mat6_identity matrix6_identity
#define mat6_set matrix6_set
#define mat6_add matrix6_add
#define mat6_sub matrix6_subtract
#define mat6_mul matrix6_multiply
#define mat6_transpose matrix6_transpose

#ifdef HYPATIA_IMPLEMENTATION

HYPAPI struct matrix6 *matrix6_zero(struct matrix6 *self)
{
	HYP_MEMSET(self, 0, sizeof(struct matrix6));
	return self;
}


HYPAPI struct matrix6 *matrix6_identity(struct matrix6 *m)
{
	m->c00 = 1.0f, m->c10 = 0.0f, m->c20 = 0.0f, m->c30 = 0.0f, m->c40 = 0.0f, m->c50 = 0.0f;
	m->c01 = 0.0f, m->c11 = 1.0f, m->c21 = 0.0f, m->c31 = 0.0f, m->c41 = 0.0f, m->c51 = 0.0f;
	m->c02 = 0.0f, m->c12 = 0.0f, m->c22 = 1.0f, m->c32 = 0.0f, m->c42 = 0.0f, m->c52 = 0.0f;
	m->c03 = 0.0f, m->c13 = 0.0f, m->c23 = 0.0f, m->c33 = 1.0f, m->c43 = 0.0f, m->c53 = 0.0f;
	m->c04 = 0.0f, m->c14 = 0.0f, m->c24 = 0.0f, m->c34 = 0.0f, m->c44 = 1.0f, m->c54 = 0.0f;
	m->c05 = 0.0f, m->c15 = 0.0f, m->c25 = 0.0f, m->c35 = 0.0f, m->c45 = 0.0f, m->c55 = 1.0f;
	return m;
}


HYPAPI struct matrix6 *matrix6_set(struct matrix6 *self, const struct matrix6 *mT)
{
	uint8_t i;

	for (i = 0; i < (36); i++) {
		self->m[i] = mT->m[i];
	}

	return self;
}


HYPAPI int matrix6_equals(const struct matrix6 *self, const struct matrix6 *mT)
{
	uint8_t i;

	for (i = 0; i < (36); i++) {
		if (scalar_equalsf(self->m[i], mT->m[i]) == 0) {
			return 0;
		}
	}

	return 1;
}


HYPAPI struct matrix6 *matrix6_add(struct matrix6 *self, const struct matrix6 *mT)
{
	/* "add row and column to row and column" */
	uint8_t i;

	for (i = 0; i < (36); i++) {
		self->m[i] += mT->m[i];
	}

	return self;
}


HYPAPI struct matrix6 *matrix6_subtract(struct matrix6 *self, const struct matrix6 *mT)
{
	/* "subtract row and column from row and column" */
	uint8_t i;

	for (i = 0; i < (36); i++) {
		self->m[i] -= mT->m[i];
	}

	return self;
}


HYPAPI struct matrix6 *matrix6_multiplyf(struct matrix6 *self, HYP_FLOAT scalar)
{
	uint8_t i;

	for (i = 0; i < (36); i++) {
		self->m[i] *= scalar;
	}

	return self;
}


HYPAPI struct matrix6 *matrix6_multiply(struct matrix6 *self, const struct matrix6 *mT)
{
	/* mT is the multiplicand */

	struct matrix6 r;

	matrix6_identity(&r);

	/* first row */
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10 + self->c02 * mT->c20 + self->c03 * mT->c30 + self->c04 * mT->c40 + self->c05 * mT->c50;
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10 + self->c12 * mT->c20 + self->c13 * mT->c30 + self->c14 * mT->c40 + self->c15 * mT->c50;
	r.r02 = self->c20 * mT->c00 + self->c21 * mT->c10 + self->c22 * mT->c20 + self->c23 * mT->c30 + self->c24 * mT->c40 + self->c25 * mT->c50;
	r.r03 = self->c30 * mT->c00 + self->c31 * mT->c10 + self->c32 * mT->c20 + self->c33 * mT->c30 + self->c34 * mT->c40 + self->c35 * mT->c50;
	r.r04 = self->c40 * mT->c00 + self->c41 * mT->c10 + self->c42 * mT->c20 + self->c43 * mT->c30 + self->c44 * mT->c40 + self->c45 * mT->c50;
	r.r05 = self->c50 * mT->c00 + self->c51 * mT->c10 + self->c52 * mT->c20 + self->c53 * mT->c30 + self->c54 * mT->c40 + self->c55 * mT->c50;

	/* second row */
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11 + self->c02 * mT->c21 + self->c03 * mT->c31 + self->c04 * mT->c41 + self->c05 * mT->c51;
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11 + self->c12 * mT->c21 + self->c13 * mT->c31 + self->c14 * mT->c41 + self->c15 * mT->c51;
	r.r12 = self->c20 * mT->c01 + self->c21 * mT->c11 + self->c22 * mT->c21 + self->c23 * mT->c31 + self->c24 * mT->c41 + self->c25 * mT->c51;
	r.r13 = self->c30 * mT->c01 + self->c31 * mT->c11 + self->c32 * mT->c21 + self->c33 * mT->c31 + self->c34 * mT->c41 + self->c35 * mT->c51;
	r.r14 = self->c40 * mT->c01 + self->c41 * mT->c11 + self->c42 * mT->c21 + self->c43 * mT->c31 + self->c44 * mT->c41 + self->c45 * mT->c51;
	r.r15 = self->c50 * mT->c01 + self->c51 * mT->c11 + self->c52 * mT->c21 + self->c53 * mT->c31 + self->c54 * mT->c41 + self->c55 * mT->c51;

	/* third row */
	r.r20 = self->c00 * mT->c02 + self->c01 * mT->c12 + self->c02 * mT->c22 + self->c03 * mT->c32 + self->c04 * mT->c42 + self->c05 * mT->c52;
	r.r21 = self->c10 * mT->c02 + self->c11 * mT->c12 + self->c12 * mT->c22 + self->c13 * mT->c32 + self->c14 * mT->c42 + self->c15 * mT->c52;
	r.r22 = self->c20 * mT->c02 + self->c21 * mT->c12 + self->c22 * mT->c22 + self->c23 * mT->c32 + self->c24 * mT->c42 + self->c25 * mT->c52;
	r.r23 = self->c30 * mT->c02 + self->c31 * mT->c12 + self->c32 * mT->c22 + self->c33 * mT->c32 + self->c34 * mT->c42 + self->c35 * mT->c52;
	r.r24 = self->c40 * mT->c02 + self->c41 * mT->c12 + self->c42 * mT->c22 + self->c43 * mT->c32 + self->c44 * mT->c42 + self->c45 * mT->c52;
	r.r25 = self->c50 * mT->c02 + self->c51 * mT->c12 + self->c52 * mT->c22 + self->c53 * mT->c32 + self->c54 * mT->c42 + self->c55 * mT->c52;

	/* fourth row */
	r.r30 = self->c00 * mT->c03 + self->c01 * mT->c13 + self->c02 * mT->c23 + self->c03 * mT->c33 + self->c04 * mT->c43 + self->c05 * mT->c53;
	r.r31 = self->c10 * mT->c03 + self->c11 * mT->c13 + self->c12 * mT->c23 + self->c13 * mT->c33 + self->c14 * mT->c43 + self->c15 * mT->c53;
	r.r32 = self->c20 * mT->c03 + self->c21 * mT->c13 + self->c22 * mT->c23 + self->c23 * mT->c33 + self->c24 * mT->c43 + self->c25 * mT->c53;
	r.r33 = self->c30 * mT->c03 + self->c31 * mT->c13 + self->c32 * mT->c23 + self->c33 * mT->c33 + self->c34 * mT->c43 + self->c35 * mT->c53;
	r.r34 = self->c40 * mT->c03 + self->c41 * mT->c13 + self->c42 * mT->c23 + self->c43 * mT->c33 + self->c44 * mT->c43 + self->c45 * mT->c53;
	r.r35 = self->c50 * mT->c03 + self->c51 * mT->c13 + self->c52 * mT->c23 + self->c53 * mT->c33 + self->c54 * mT->c43 + self->c55 * mT->c53;

	/* fifth row */
	r.r40 = self->c00 * mT->c04 + self->c01 * mT->c14 + self->c02 * mT->c24 + self->c03 * mT->c34 + self->c04 * mT->c44 + self->c05 * mT->c54;
	r.r41 = self->c10 * mT->c04 + self->c11 * mT->c14 + self->c12 * mT->c24 + self->c13 * mT->c34 + self->c14 * mT->c44 + self->c15 * mT->c54;
	r.r42 = self->c20 * mT->c04 + self->c21 * mT->c14 + self->c22 * mT->c24 + self->c23 * mT->c34 + self->c24 * mT->c44 + self->c25 * mT->c54;
	r.r43 = self->c30 * mT->c04 + self->c31 * mT->c14 + self->c32 * mT->c24 + self->c33 * mT->c34 + self->c34 * mT->c44 + self->c35 * mT->c54;
	r.r44 = self->c40 * mT->c04 + self->c41 * mT->c14 + self->c42 * mT->c24 + self->c43 * mT->c34 + self->c44 * mT->c44 + self->c45 * mT->c54;
	r.r45 = self->c50 * mT->c04 + self->c51 * mT->c14 + self->c52 * mT->c24 + self->c53 * mT->c34 + self->c54 * mT->c44 + self->c55 * mT->c54;

	/* sixth row */
	r.r50 = self->c00 * mT->c05 + self->c01 * mT->c15 + self->c02 * mT->c25 + self->c03 * mT->c35 + self->c04 * mT->c45 + self->c05 * mT->c55;
	r.r51 = self->c10 * mT->c05 + self->c11 * mT->c15 + self->c12 * mT->c25 + self->c13 * mT->c35 + self->c14 * mT->c45 + self->c15 * mT->c55;
	r.r52 = self->c20 * mT->c05 + self->c21 * mT->c15 + self->c22 * mT->c25 + self->c23 * mT->c35 + self->c24 * mT->c45 + self->c25 * mT->c55;
	r.r53 = self->c30 * mT->c05 + self->c31 * mT->c15 + self->c32 * mT->c25 + self->c33 * mT->c35 + self->c34 * mT->c45 + self->c35 * mT->c55;
	r.r54 = self->c40 * mT->c05 + self->c41 * mT->c15 + self->c42 * mT->c25 + self->c43 * mT->c35 + self->c44 * mT->c45 + self->c45 * mT->c55;
	r.r55 = self->c50 * mT->c05 + self->c51 * mT->c15 + self->c52 * mT->c25 + self->c53 * mT->c35 + self->c54 * mT->c45 + self->c55 * mT->c55;

	matrix6_set(self, &r); /* overwrite/save it */

	return self;
}


HYPAPI struct vector4 *matrix6_multiplyv4(const struct matrix6 *self, const struct vector4 *vT, struct vector4 *vR)
{
	vR->x = vT->x * self->r00 + vT->y * self->r01 + vT->z * self->r02 + vT->w * self->r03 + self->r04 + self->r05;
	vR->y = vT->x * self->r10 + vT->y * self->r11 + vT->z * self->r12 + vT->w * self->r13 + self->r14 + self->r15;
	vR->z = vT->x * self->r20 + vT->y * self->r21 + vT->z * self->r22 + vT->w * self->r23 + self->r24 + self->r25;
	vR->w = vT->x * self->r30 + vT->y * self->r31 + vT->z * self->r32 + vT->w * self->r33 + self->r34 + self->r35;

	return vR;
}


HYPAPI struct vector3 *matrix6_multiplyv3(const struct matrix6 *self, const struct vector3 *vT, struct vector3 *vR)
{
	vR->x = vT->x * self->r00 + vT->y * self->r01 + vT->z * self->r02 + self->r03 + self->r04 + self->r05;
	vR->y = vT->x * self->r10 + vT->y * self->r11 + vT->z * self->r12 + self->r13 + self->r14 + self->r15;
	vR->z = vT->x * self->r20 + vT->y * self->r21 + vT->z * self->r22 + self->r23 + self->r24 + self->r25;

	return vR;
}


HYPAPI struct vector2 *matrix6_multiplyv2(const struct matrix6 *self, const struct vector2 *vT, struct vector2 *vR)
{
	vR->x = vT->x * self->r00 + vT->y * self->r01 + self->r02 + self->r03 + self->r04 + self->r05;
	vR->y = vT->x * self->r10 + vT->y * self->r11 + self->r12 + self->r13 + self->r14 + self->r14;

	return vR;
}


HYPAPI struct matrix6 *matrix6_transpose(struct matrix6 *self)
{
	return _matrix6_transpose_columnrow(self);
}


HYPAPI struct matrix6 *_matrix6_transpose_rowcolumn(struct matrix6 *self)
{
	HYP_SWAP(&self->r01, &self->r10);
	HYP_SWAP(&self->r02, &self->r20);
	HYP_SWAP(&self->r03, &self->r30);
	HYP_SWAP(&self->r04, &self->r40);
	HYP_SWAP(&self->r05, &self->r50);

	HYP_SWAP(&self->r12, &self->r21);
	HYP_SWAP(&self->r13, &self->r31);
	HYP_SWAP(&self->r14, &self->r41);
	HYP_SWAP(&self->r15, &self->r51);

	HYP_SWAP(&self->r23, &self->r32);
	HYP_SWAP(&self->r24, &self->r42);
	HYP_SWAP(&self->r25, &self->r52);

	HYP_SWAP(&self->r34, &self->r43);
	HYP_SWAP(&self->r35, &self->r53);

	HYP_SWAP(&self->r45, &self->r54);
	return self;
}


HYPAPI struct matrix6 *_matrix6_transpose_columnrow(struct matrix6 *self)
{
	HYP_SWAP(&self->c01, &self->c10);
	HYP_SWAP(&self->c02, &self->c20);
	HYP_SWAP(&self->c03, &self->c30);
	HYP_SWAP(&self->c04, &self->c40);
	HYP_SWAP(&self->c05, &self->c50);

	HYP_SWAP(&self->c12, &self->c21);
	HYP_SWAP(&self->c13, &self->c31);
	HYP_SWAP(&self->c14, &self->c41);
	HYP_SWAP(&self->c15, &self->c51);

	HYP_SWAP(&self->c23, &self->c32);
	HYP_SWAP(&self->c24, &self->c42);
	HYP_SWAP(&self->c25, &self->c52);

	HYP_SWAP(&self->c34, &self->c43);
	HYP_SWAP(&self->c35, &self->c53);

	HYP_SWAP(&self->c45, &self->c54);

	return self;
}

#ifndef HYP_NO_STDIO
/**
 * @ingroup matrix6
 * @brief Prints out the matrix using column and row notation
 *
 */
HYPAPI void _matrix6_print_with_columnrow_indexer(struct matrix6 *self)
{
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->c00, self->c10, self->c20, self->c30, self->c40, self->c50);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->c01, self->c11, self->c21, self->c31, self->c41, self->c51);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->c02, self->c12, self->c22, self->c32, self->c42, self->c52);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->c03, self->c13, self->c23, self->c33, self->c43, self->c53);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->c04, self->c14, self->c24, self->c34, self->c44, self->c54);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->c05, self->c15, self->c25, self->c35, self->c45, self->c55);
}


/**
 * @ingroup matrix6
 * @brief Prints out the matrix using row and column notation
 *
 */
HYPAPI void _matrix6_print_with_rowcolumn_indexer(struct matrix6 *self)
{
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->r00, self->r01, self->r02, self->r03, self->r04, self->r05);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->r10, self->r11, self->r12, self->r13, self->r14, self->r15);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->r20, self->r21, self->r22, self->r23, self->r24, self->r25);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->r30, self->r31, self->r32, self->r33, self->r34, self->r35);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->r40, self->r41, self->r42, self->r43, self->r44, self->r45);
	printf("%10f, %10f, %10f, %10f, %10f, %10f\r\n", self->r50, self->r51, self->r52, self->r53, self->r54, self->r55);
}
#endif


/**
 * @ingroup matrix6
 * @brief Randomly fills the matrix with values. Good for testing.
 *
 */
HYPAPI struct matrix6 *_matrix6_set_random(struct matrix6 *self)
{
	uint8_t i;

	for (i = 0; i < 36; i++) {
		self->m[i] = HYP_RANDOM_FLOAT;
	}

	return self;
}

#define _HYP_CAT(a, ...) _HYP_PRIMITIVE_CAT(a, __VA_ARGS__)
#define _HYP_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define _HYP_DEC(x) _HYP_PRIMITIVE_CAT(DEC_, x)
#define DEC_11 00
#define DEC_12 01
#define DEC_13 02
#define DEC_14 03
#define DEC_21 10
#define DEC_22 11
#define DEC_23 12
#define DEC_24 13
#define DEC_31 20
#define DEC_32 21
#define DEC_33 22
#define DEC_34 23
#define DEC_41 30
#define DEC_42 31
#define DEC_43 32
#define DEC_44 33
#define A(x) _HYP_CAT(self->r,  _HYP_DEC(x))
#define B(x) _HYP_CAT(inverse.r, _HYP_DEC(x))
#define A4(x1, x2, x3, x4) (A(x1) * A(x2) * A(x3) * A(x4))
#define A3(x1, x2, x3) (A(x1) * A(x2) * A(x3))
#define A2(x1, x2) (A(x1) * A(x2))
#define A6(x1, x2, x3, x4, x5, x6) (A(x1) * A(x2) * A(x3) * A(x4) * A(x5) * A(x6))

HYPAPI HYP_FLOAT matrix6_determinant(const struct matrix6 *self)
{
	HYP_FLOAT determinant;

	/* using the Leibniz formula */
	/* avoids temporary structures */

	determinant =
	  (self->r00 * self->r11 * self->r22 * self->r33 * self->r44 * self->r55)
	- (self->r00 * self->r11 * self->r22 * self->r33 * self->r45 * self->r54)
	- (self->r00 * self->r11 * self->r22 * self->r34 * self->r43 * self->r55)
	+ (self->r00 * self->r11 * self->r22 * self->r34 * self->r45 * self->r53)
	+ (self->r00 * self->r11 * self->r22 * self->r35 * self->r43 * self->r54)
	- (self->r00 * self->r11 * self->r22 * self->r35 * self->r44 * self->r53)
	- (self->r00 * self->r11 * self->r23 * self->r32 * self->r44 * self->r55)
	+ (self->r00 * self->r11 * self->r23 * self->r32 * self->r45 * self->r54)
	+ (self->r00 * self->r11 * self->r23 * self->r34 * self->r42 * self->r55)
	- (self->r00 * self->r11 * self->r23 * self->r34 * self->r45 * self->r52)
	- (self->r00 * self->r11 * self->r23 * self->r35 * self->r42 * self->r54)
	+ (self->r00 * self->r11 * self->r23 * self->r35 * self->r44 * self->r52)
	+ (self->r00 * self->r11 * self->r24 * self->r32 * self->r43 * self->r55)
	- (self->r00 * self->r11 * self->r24 * self->r32 * self->r45 * self->r53)
	- (self->r00 * self->r11 * self->r24 * self->r33 * self->r42 * self->r55)
	+ (self->r00 * self->r11 * self->r24 * self->r33 * self->r45 * self->r52)
	+ (self->r00 * self->r11 * self->r24 * self->r35 * self->r42 * self->r53)
	- (self->r00 * self->r11 * self->r24 * self->r35 * self->r43 * self->r52)
	- (self->r00 * self->r11 * self->r25 * self->r32 * self->r43 * self->r54)
	+ (self->r00 * self->r11 * self->r25 * self->r32 * self->r44 * self->r53)
	+ (self->r00 * self->r11 * self->r25 * self->r33 * self->r42 * self->r54)
	- (self->r00 * self->r11 * self->r25 * self->r33 * self->r44 * self->r52)
	- (self->r00 * self->r11 * self->r25 * self->r34 * self->r42 * self->r53)
	+ (self->r00 * self->r11 * self->r25 * self->r34 * self->r43 * self->r52)
	- (self->r00 * self->r12 * self->r21 * self->r33 * self->r44 * self->r55)
	+ (self->r00 * self->r12 * self->r21 * self->r33 * self->r45 * self->r54)
	+ (self->r00 * self->r12 * self->r21 * self->r34 * self->r43 * self->r55)
	- (self->r00 * self->r12 * self->r21 * self->r34 * self->r45 * self->r53)
	- (self->r00 * self->r12 * self->r21 * self->r35 * self->r43 * self->r54)
	+ (self->r00 * self->r12 * self->r21 * self->r35 * self->r44 * self->r53)
	+ (self->r00 * self->r12 * self->r23 * self->r31 * self->r44 * self->r55)
	- (self->r00 * self->r12 * self->r23 * self->r31 * self->r45 * self->r54)
	- (self->r00 * self->r12 * self->r23 * self->r34 * self->r41 * self->r55)
	+ (self->r00 * self->r12 * self->r23 * self->r34 * self->r45 * self->r51)
	+ (self->r00 * self->r12 * self->r23 * self->r35 * self->r41 * self->r54)
	- (self->r00 * self->r12 * self->r23 * self->r35 * self->r44 * self->r51)
	- (self->r00 * self->r12 * self->r24 * self->r31 * self->r43 * self->r55)
	+ (self->r00 * self->r12 * self->r24 * self->r31 * self->r45 * self->r53)
	+ (self->r00 * self->r12 * self->r24 * self->r33 * self->r41 * self->r55)
	- (self->r00 * self->r12 * self->r24 * self->r33 * self->r45 * self->r51)
	- (self->r00 * self->r12 * self->r24 * self->r35 * self->r41 * self->r53)
	+ (self->r00 * self->r12 * self->r24 * self->r35 * self->r43 * self->r51)
	+ (self->r00 * self->r12 * self->r25 * self->r31 * self->r43 * self->r54)
	- (self->r00 * self->r12 * self->r25 * self->r31 * self->r44 * self->r53)
	- (self->r00 * self->r12 * self->r25 * self->r33 * self->r41 * self->r54)
	+ (self->r00 * self->r12 * self->r25 * self->r33 * self->r44 * self->r51)
	+ (self->r00 * self->r12 * self->r25 * self->r34 * self->r41 * self->r53)
	- (self->r00 * self->r12 * self->r25 * self->r34 * self->r43 * self->r51)
	+ (self->r00 * self->r13 * self->r21 * self->r32 * self->r44 * self->r55)
	- (self->r00 * self->r13 * self->r21 * self->r32 * self->r45 * self->r54)
	- (self->r00 * self->r13 * self->r21 * self->r34 * self->r42 * self->r55)
	+ (self->r00 * self->r13 * self->r21 * self->r34 * self->r45 * self->r52)
	+ (self->r00 * self->r13 * self->r21 * self->r35 * self->r42 * self->r54)
	- (self->r00 * self->r13 * self->r21 * self->r35 * self->r44 * self->r52)
	- (self->r00 * self->r13 * self->r22 * self->r31 * self->r44 * self->r55)
	+ (self->r00 * self->r13 * self->r22 * self->r31 * self->r45 * self->r54)
	+ (self->r00 * self->r13 * self->r22 * self->r34 * self->r41 * self->r55)
	- (self->r00 * self->r13 * self->r22 * self->r34 * self->r45 * self->r51)
	- (self->r00 * self->r13 * self->r22 * self->r35 * self->r41 * self->r54)
	+ (self->r00 * self->r13 * self->r22 * self->r35 * self->r44 * self->r51)
	+ (self->r00 * self->r13 * self->r24 * self->r31 * self->r42 * self->r55)
	- (self->r00 * self->r13 * self->r24 * self->r31 * self->r45 * self->r52)
	- (self->r00 * self->r13 * self->r24 * self->r32 * self->r41 * self->r55)
	+ (self->r00 * self->r13 * self->r24 * self->r32 * self->r45 * self->r51)
	+ (self->r00 * self->r13 * self->r24 * self->r35 * self->r41 * self->r52)
	- (self->r00 * self->r13 * self->r24 * self->r35 * self->r42 * self->r51)
	- (self->r00 * self->r13 * self->r25 * self->r31 * self->r42 * self->r54)
	+ (self->r00 * self->r13 * self->r25 * self->r31 * self->r44 * self->r52)
	+ (self->r00 * self->r13 * self->r25 * self->r32 * self->r41 * self->r54)
	- (self->r00 * self->r13 * self->r25 * self->r32 * self->r44 * self->r51)
	- (self->r00 * self->r13 * self->r25 * self->r34 * self->r41 * self->r52)
	+ (self->r00 * self->r13 * self->r25 * self->r34 * self->r42 * self->r51)
	- (self->r00 * self->r14 * self->r21 * self->r32 * self->r43 * self->r55)
	+ (self->r00 * self->r14 * self->r21 * self->r32 * self->r45 * self->r53)
	+ (self->r00 * self->r14 * self->r21 * self->r33 * self->r42 * self->r55)
	- (self->r00 * self->r14 * self->r21 * self->r33 * self->r45 * self->r52)
	- (self->r00 * self->r14 * self->r21 * self->r35 * self->r42 * self->r53)
	+ (self->r00 * self->r14 * self->r21 * self->r35 * self->r43 * self->r52)
	+ (self->r00 * self->r14 * self->r22 * self->r31 * self->r43 * self->r55)
	- (self->r00 * self->r14 * self->r22 * self->r31 * self->r45 * self->r53)
	- (self->r00 * self->r14 * self->r22 * self->r33 * self->r41 * self->r55)
	+ (self->r00 * self->r14 * self->r22 * self->r33 * self->r45 * self->r51)
	+ (self->r00 * self->r14 * self->r22 * self->r35 * self->r41 * self->r53)
	- (self->r00 * self->r14 * self->r22 * self->r35 * self->r43 * self->r51)
	- (self->r00 * self->r14 * self->r23 * self->r31 * self->r42 * self->r55)
	+ (self->r00 * self->r14 * self->r23 * self->r31 * self->r45 * self->r52)
	+ (self->r00 * self->r14 * self->r23 * self->r32 * self->r41 * self->r55)
	- (self->r00 * self->r14 * self->r23 * self->r32 * self->r45 * self->r51)
	- (self->r00 * self->r14 * self->r23 * self->r35 * self->r41 * self->r52)
	+ (self->r00 * self->r14 * self->r23 * self->r35 * self->r42 * self->r51)
	+ (self->r00 * self->r14 * self->r25 * self->r31 * self->r42 * self->r53)
	- (self->r00 * self->r14 * self->r25 * self->r31 * self->r43 * self->r52)
	- (self->r00 * self->r14 * self->r25 * self->r32 * self->r41 * self->r53)
	+ (self->r00 * self->r14 * self->r25 * self->r32 * self->r43 * self->r51)
	+ (self->r00 * self->r14 * self->r25 * self->r33 * self->r41 * self->r52)
	- (self->r00 * self->r14 * self->r25 * self->r33 * self->r42 * self->r51)
	+ (self->r00 * self->r15 * self->r21 * self->r32 * self->r43 * self->r54)
	- (self->r00 * self->r15 * self->r21 * self->r32 * self->r44 * self->r53)
	- (self->r00 * self->r15 * self->r21 * self->r33 * self->r42 * self->r54)
	+ (self->r00 * self->r15 * self->r21 * self->r33 * self->r44 * self->r52)
	+ (self->r00 * self->r15 * self->r21 * self->r34 * self->r42 * self->r53)
	- (self->r00 * self->r15 * self->r21 * self->r34 * self->r43 * self->r52)
	- (self->r00 * self->r15 * self->r22 * self->r31 * self->r43 * self->r54)
	+ (self->r00 * self->r15 * self->r22 * self->r31 * self->r44 * self->r53)
	+ (self->r00 * self->r15 * self->r22 * self->r33 * self->r41 * self->r54)
	- (self->r00 * self->r15 * self->r22 * self->r33 * self->r44 * self->r51)
	- (self->r00 * self->r15 * self->r22 * self->r34 * self->r41 * self->r53)
	+ (self->r00 * self->r15 * self->r22 * self->r34 * self->r43 * self->r51)
	+ (self->r00 * self->r15 * self->r23 * self->r31 * self->r42 * self->r54)
	- (self->r00 * self->r15 * self->r23 * self->r31 * self->r44 * self->r52)
	- (self->r00 * self->r15 * self->r23 * self->r32 * self->r41 * self->r54)
	+ (self->r00 * self->r15 * self->r23 * self->r32 * self->r44 * self->r51)
	+ (self->r00 * self->r15 * self->r23 * self->r34 * self->r41 * self->r52)
	- (self->r00 * self->r15 * self->r23 * self->r34 * self->r42 * self->r51)
	- (self->r00 * self->r15 * self->r24 * self->r31 * self->r42 * self->r53)
	+ (self->r00 * self->r15 * self->r24 * self->r31 * self->r43 * self->r52)
	+ (self->r00 * self->r15 * self->r24 * self->r32 * self->r41 * self->r53)
	- (self->r00 * self->r15 * self->r24 * self->r32 * self->r43 * self->r51)
	- (self->r00 * self->r15 * self->r24 * self->r33 * self->r41 * self->r52)
	+ (self->r00 * self->r15 * self->r24 * self->r33 * self->r42 * self->r51)
	- (self->r01 * self->r10 * self->r22 * self->r33 * self->r44 * self->r55)
	+ (self->r01 * self->r10 * self->r22 * self->r33 * self->r45 * self->r54)
	+ (self->r01 * self->r10 * self->r22 * self->r34 * self->r43 * self->r55)
	- (self->r01 * self->r10 * self->r22 * self->r34 * self->r45 * self->r53)
	- (self->r01 * self->r10 * self->r22 * self->r35 * self->r43 * self->r54)
	+ (self->r01 * self->r10 * self->r22 * self->r35 * self->r44 * self->r53)
	+ (self->r01 * self->r10 * self->r23 * self->r32 * self->r44 * self->r55)
	- (self->r01 * self->r10 * self->r23 * self->r32 * self->r45 * self->r54)
	- (self->r01 * self->r10 * self->r23 * self->r34 * self->r42 * self->r55)
	+ (self->r01 * self->r10 * self->r23 * self->r34 * self->r45 * self->r52)
	+ (self->r01 * self->r10 * self->r23 * self->r35 * self->r42 * self->r54)
	- (self->r01 * self->r10 * self->r23 * self->r35 * self->r44 * self->r52)
	- (self->r01 * self->r10 * self->r24 * self->r32 * self->r43 * self->r55)
	+ (self->r01 * self->r10 * self->r24 * self->r32 * self->r45 * self->r53)
	+ (self->r01 * self->r10 * self->r24 * self->r33 * self->r42 * self->r55)
	- (self->r01 * self->r10 * self->r24 * self->r33 * self->r45 * self->r52)
	- (self->r01 * self->r10 * self->r24 * self->r35 * self->r42 * self->r53)
	+ (self->r01 * self->r10 * self->r24 * self->r35 * self->r43 * self->r52)
	+ (self->r01 * self->r10 * self->r25 * self->r32 * self->r43 * self->r54)
	- (self->r01 * self->r10 * self->r25 * self->r32 * self->r44 * self->r53)
	- (self->r01 * self->r10 * self->r25 * self->r33 * self->r42 * self->r54)
	+ (self->r01 * self->r10 * self->r25 * self->r33 * self->r44 * self->r52)
	+ (self->r01 * self->r10 * self->r25 * self->r34 * self->r42 * self->r53)
	- (self->r01 * self->r10 * self->r25 * self->r34 * self->r43 * self->r52)
	+ (self->r01 * self->r12 * self->r20 * self->r33 * self->r44 * self->r55)
	- (self->r01 * self->r12 * self->r20 * self->r33 * self->r45 * self->r54)
	- (self->r01 * self->r12 * self->r20 * self->r34 * self->r43 * self->r55)
	+ (self->r01 * self->r12 * self->r20 * self->r34 * self->r45 * self->r53)
	+ (self->r01 * self->r12 * self->r20 * self->r35 * self->r43 * self->r54)
	- (self->r01 * self->r12 * self->r20 * self->r35 * self->r44 * self->r53)
	- (self->r01 * self->r12 * self->r23 * self->r30 * self->r44 * self->r55)
	+ (self->r01 * self->r12 * self->r23 * self->r30 * self->r45 * self->r54)
	+ (self->r01 * self->r12 * self->r23 * self->r34 * self->r40 * self->r55)
	- (self->r01 * self->r12 * self->r23 * self->r34 * self->r45 * self->r50)
	- (self->r01 * self->r12 * self->r23 * self->r35 * self->r40 * self->r54)
	+ (self->r01 * self->r12 * self->r23 * self->r35 * self->r44 * self->r50)
	+ (self->r01 * self->r12 * self->r24 * self->r30 * self->r43 * self->r55)
	- (self->r01 * self->r12 * self->r24 * self->r30 * self->r45 * self->r53)
	- (self->r01 * self->r12 * self->r24 * self->r33 * self->r40 * self->r55)
	+ (self->r01 * self->r12 * self->r24 * self->r33 * self->r45 * self->r50)
	+ (self->r01 * self->r12 * self->r24 * self->r35 * self->r40 * self->r53)
	- (self->r01 * self->r12 * self->r24 * self->r35 * self->r43 * self->r50)
	- (self->r01 * self->r12 * self->r25 * self->r30 * self->r43 * self->r54)
	+ (self->r01 * self->r12 * self->r25 * self->r30 * self->r44 * self->r53)
	+ (self->r01 * self->r12 * self->r25 * self->r33 * self->r40 * self->r54)
	- (self->r01 * self->r12 * self->r25 * self->r33 * self->r44 * self->r50)
	- (self->r01 * self->r12 * self->r25 * self->r34 * self->r40 * self->r53)
	+ (self->r01 * self->r12 * self->r25 * self->r34 * self->r43 * self->r50)
	- (self->r01 * self->r13 * self->r20 * self->r32 * self->r44 * self->r55)
	+ (self->r01 * self->r13 * self->r20 * self->r32 * self->r45 * self->r54)
	+ (self->r01 * self->r13 * self->r20 * self->r34 * self->r42 * self->r55)
	- (self->r01 * self->r13 * self->r20 * self->r34 * self->r45 * self->r52)
	- (self->r01 * self->r13 * self->r20 * self->r35 * self->r42 * self->r54)
	+ (self->r01 * self->r13 * self->r20 * self->r35 * self->r44 * self->r52)
	+ (self->r01 * self->r13 * self->r22 * self->r30 * self->r44 * self->r55)
	- (self->r01 * self->r13 * self->r22 * self->r30 * self->r45 * self->r54)
	- (self->r01 * self->r13 * self->r22 * self->r34 * self->r40 * self->r55)
	+ (self->r01 * self->r13 * self->r22 * self->r34 * self->r45 * self->r50)
	+ (self->r01 * self->r13 * self->r22 * self->r35 * self->r40 * self->r54)
	- (self->r01 * self->r13 * self->r22 * self->r35 * self->r44 * self->r50)
	- (self->r01 * self->r13 * self->r24 * self->r30 * self->r42 * self->r55)
	+ (self->r01 * self->r13 * self->r24 * self->r30 * self->r45 * self->r52)
	+ (self->r01 * self->r13 * self->r24 * self->r32 * self->r40 * self->r55)
	- (self->r01 * self->r13 * self->r24 * self->r32 * self->r45 * self->r50)
	- (self->r01 * self->r13 * self->r24 * self->r35 * self->r40 * self->r52)
	+ (self->r01 * self->r13 * self->r24 * self->r35 * self->r42 * self->r50)
	+ (self->r01 * self->r13 * self->r25 * self->r30 * self->r42 * self->r54)
	- (self->r01 * self->r13 * self->r25 * self->r30 * self->r44 * self->r52)
	- (self->r01 * self->r13 * self->r25 * self->r32 * self->r40 * self->r54)
	+ (self->r01 * self->r13 * self->r25 * self->r32 * self->r44 * self->r50)
	+ (self->r01 * self->r13 * self->r25 * self->r34 * self->r40 * self->r52)
	- (self->r01 * self->r13 * self->r25 * self->r34 * self->r42 * self->r50)
	+ (self->r01 * self->r14 * self->r20 * self->r32 * self->r43 * self->r55)
	- (self->r01 * self->r14 * self->r20 * self->r32 * self->r45 * self->r53)
	- (self->r01 * self->r14 * self->r20 * self->r33 * self->r42 * self->r55)
	+ (self->r01 * self->r14 * self->r20 * self->r33 * self->r45 * self->r52)
	+ (self->r01 * self->r14 * self->r20 * self->r35 * self->r42 * self->r53)
	- (self->r01 * self->r14 * self->r20 * self->r35 * self->r43 * self->r52)
	- (self->r01 * self->r14 * self->r22 * self->r30 * self->r43 * self->r55)
	+ (self->r01 * self->r14 * self->r22 * self->r30 * self->r45 * self->r53)
	+ (self->r01 * self->r14 * self->r22 * self->r33 * self->r40 * self->r55)
	- (self->r01 * self->r14 * self->r22 * self->r33 * self->r45 * self->r50)
	- (self->r01 * self->r14 * self->r22 * self->r35 * self->r40 * self->r53)
	+ (self->r01 * self->r14 * self->r22 * self->r35 * self->r43 * self->r50)
	+ (self->r01 * self->r14 * self->r23 * self->r30 * self->r42 * self->r55)
	- (self->r01 * self->r14 * self->r23 * self->r30 * self->r45 * self->r52)
	- (self->r01 * self->r14 * self->r23 * self->r32 * self->r40 * self->r55)
	+ (self->r01 * self->r14 * self->r23 * self->r32 * self->r45 * self->r50)
	+ (self->r01 * self->r14 * self->r23 * self->r35 * self->r40 * self->r52)
	- (self->r01 * self->r14 * self->r23 * self->r35 * self->r42 * self->r50)
	- (self->r01 * self->r14 * self->r25 * self->r30 * self->r42 * self->r53)
	+ (self->r01 * self->r14 * self->r25 * self->r30 * self->r43 * self->r52)
	+ (self->r01 * self->r14 * self->r25 * self->r32 * self->r40 * self->r53)
	- (self->r01 * self->r14 * self->r25 * self->r32 * self->r43 * self->r50)
	- (self->r01 * self->r14 * self->r25 * self->r33 * self->r40 * self->r52)
	+ (self->r01 * self->r14 * self->r25 * self->r33 * self->r42 * self->r50)
	- (self->r01 * self->r15 * self->r20 * self->r32 * self->r43 * self->r54)
	+ (self->r01 * self->r15 * self->r20 * self->r32 * self->r44 * self->r53)
	+ (self->r01 * self->r15 * self->r20 * self->r33 * self->r42 * self->r54)
	- (self->r01 * self->r15 * self->r20 * self->r33 * self->r44 * self->r52)
	- (self->r01 * self->r15 * self->r20 * self->r34 * self->r42 * self->r53)
	+ (self->r01 * self->r15 * self->r20 * self->r34 * self->r43 * self->r52)
	+ (self->r01 * self->r15 * self->r22 * self->r30 * self->r43 * self->r54)
	- (self->r01 * self->r15 * self->r22 * self->r30 * self->r44 * self->r53)
	- (self->r01 * self->r15 * self->r22 * self->r33 * self->r40 * self->r54)
	+ (self->r01 * self->r15 * self->r22 * self->r33 * self->r44 * self->r50)
	+ (self->r01 * self->r15 * self->r22 * self->r34 * self->r40 * self->r53)
	- (self->r01 * self->r15 * self->r22 * self->r34 * self->r43 * self->r50)
	- (self->r01 * self->r15 * self->r23 * self->r30 * self->r42 * self->r54)
	+ (self->r01 * self->r15 * self->r23 * self->r30 * self->r44 * self->r52)
	+ (self->r01 * self->r15 * self->r23 * self->r32 * self->r40 * self->r54)
	- (self->r01 * self->r15 * self->r23 * self->r32 * self->r44 * self->r50)
	- (self->r01 * self->r15 * self->r23 * self->r34 * self->r40 * self->r52)
	+ (self->r01 * self->r15 * self->r23 * self->r34 * self->r42 * self->r50)
	+ (self->r01 * self->r15 * self->r24 * self->r30 * self->r42 * self->r53)
	- (self->r01 * self->r15 * self->r24 * self->r30 * self->r43 * self->r52)
	- (self->r01 * self->r15 * self->r24 * self->r32 * self->r40 * self->r53)
	+ (self->r01 * self->r15 * self->r24 * self->r32 * self->r43 * self->r50)
	+ (self->r01 * self->r15 * self->r24 * self->r33 * self->r40 * self->r52)
	- (self->r01 * self->r15 * self->r24 * self->r33 * self->r42 * self->r50)
	+ (self->r02 * self->r10 * self->r21 * self->r33 * self->r44 * self->r55)
	- (self->r02 * self->r10 * self->r21 * self->r33 * self->r45 * self->r54)
	- (self->r02 * self->r10 * self->r21 * self->r34 * self->r43 * self->r55)
	+ (self->r02 * self->r10 * self->r21 * self->r34 * self->r45 * self->r53)
	+ (self->r02 * self->r10 * self->r21 * self->r35 * self->r43 * self->r54)
	- (self->r02 * self->r10 * self->r21 * self->r35 * self->r44 * self->r53)
	- (self->r02 * self->r10 * self->r23 * self->r31 * self->r44 * self->r55)
	+ (self->r02 * self->r10 * self->r23 * self->r31 * self->r45 * self->r54)
	+ (self->r02 * self->r10 * self->r23 * self->r34 * self->r41 * self->r55)
	- (self->r02 * self->r10 * self->r23 * self->r34 * self->r45 * self->r51)
	- (self->r02 * self->r10 * self->r23 * self->r35 * self->r41 * self->r54)
	+ (self->r02 * self->r10 * self->r23 * self->r35 * self->r44 * self->r51)
	+ (self->r02 * self->r10 * self->r24 * self->r31 * self->r43 * self->r55)
	- (self->r02 * self->r10 * self->r24 * self->r31 * self->r45 * self->r53)
	- (self->r02 * self->r10 * self->r24 * self->r33 * self->r41 * self->r55)
	+ (self->r02 * self->r10 * self->r24 * self->r33 * self->r45 * self->r51)
	+ (self->r02 * self->r10 * self->r24 * self->r35 * self->r41 * self->r53)
	- (self->r02 * self->r10 * self->r24 * self->r35 * self->r43 * self->r51)
	- (self->r02 * self->r10 * self->r25 * self->r31 * self->r43 * self->r54)
	+ (self->r02 * self->r10 * self->r25 * self->r31 * self->r44 * self->r53)
	+ (self->r02 * self->r10 * self->r25 * self->r33 * self->r41 * self->r54)
	- (self->r02 * self->r10 * self->r25 * self->r33 * self->r44 * self->r51)
	- (self->r02 * self->r10 * self->r25 * self->r34 * self->r41 * self->r53)
	+ (self->r02 * self->r10 * self->r25 * self->r34 * self->r43 * self->r51)
	- (self->r02 * self->r11 * self->r20 * self->r33 * self->r44 * self->r55)
	+ (self->r02 * self->r11 * self->r20 * self->r33 * self->r45 * self->r54)
	+ (self->r02 * self->r11 * self->r20 * self->r34 * self->r43 * self->r55)
	- (self->r02 * self->r11 * self->r20 * self->r34 * self->r45 * self->r53)
	- (self->r02 * self->r11 * self->r20 * self->r35 * self->r43 * self->r54)
	+ (self->r02 * self->r11 * self->r20 * self->r35 * self->r44 * self->r53)
	+ (self->r02 * self->r11 * self->r23 * self->r30 * self->r44 * self->r55)
	- (self->r02 * self->r11 * self->r23 * self->r30 * self->r45 * self->r54)
	- (self->r02 * self->r11 * self->r23 * self->r34 * self->r40 * self->r55)
	+ (self->r02 * self->r11 * self->r23 * self->r34 * self->r45 * self->r50)
	+ (self->r02 * self->r11 * self->r23 * self->r35 * self->r40 * self->r54)
	- (self->r02 * self->r11 * self->r23 * self->r35 * self->r44 * self->r50)
	- (self->r02 * self->r11 * self->r24 * self->r30 * self->r43 * self->r55)
	+ (self->r02 * self->r11 * self->r24 * self->r30 * self->r45 * self->r53)
	+ (self->r02 * self->r11 * self->r24 * self->r33 * self->r40 * self->r55)
	- (self->r02 * self->r11 * self->r24 * self->r33 * self->r45 * self->r50)
	- (self->r02 * self->r11 * self->r24 * self->r35 * self->r40 * self->r53)
	+ (self->r02 * self->r11 * self->r24 * self->r35 * self->r43 * self->r50)
	+ (self->r02 * self->r11 * self->r25 * self->r30 * self->r43 * self->r54)
	- (self->r02 * self->r11 * self->r25 * self->r30 * self->r44 * self->r53)
	- (self->r02 * self->r11 * self->r25 * self->r33 * self->r40 * self->r54)
	+ (self->r02 * self->r11 * self->r25 * self->r33 * self->r44 * self->r50)
	+ (self->r02 * self->r11 * self->r25 * self->r34 * self->r40 * self->r53)
	- (self->r02 * self->r11 * self->r25 * self->r34 * self->r43 * self->r50)
	+ (self->r02 * self->r13 * self->r20 * self->r31 * self->r44 * self->r55)
	- (self->r02 * self->r13 * self->r20 * self->r31 * self->r45 * self->r54)
	- (self->r02 * self->r13 * self->r20 * self->r34 * self->r41 * self->r55)
	+ (self->r02 * self->r13 * self->r20 * self->r34 * self->r45 * self->r51)
	+ (self->r02 * self->r13 * self->r20 * self->r35 * self->r41 * self->r54)
	- (self->r02 * self->r13 * self->r20 * self->r35 * self->r44 * self->r51)
	- (self->r02 * self->r13 * self->r21 * self->r30 * self->r44 * self->r55)
	+ (self->r02 * self->r13 * self->r21 * self->r30 * self->r45 * self->r54)
	+ (self->r02 * self->r13 * self->r21 * self->r34 * self->r40 * self->r55)
	- (self->r02 * self->r13 * self->r21 * self->r34 * self->r45 * self->r50)
	- (self->r02 * self->r13 * self->r21 * self->r35 * self->r40 * self->r54)
	+ (self->r02 * self->r13 * self->r21 * self->r35 * self->r44 * self->r50)
	+ (self->r02 * self->r13 * self->r24 * self->r30 * self->r41 * self->r55)
	- (self->r02 * self->r13 * self->r24 * self->r30 * self->r45 * self->r51)
	- (self->r02 * self->r13 * self->r24 * self->r31 * self->r40 * self->r55)
	+ (self->r02 * self->r13 * self->r24 * self->r31 * self->r45 * self->r50)
	+ (self->r02 * self->r13 * self->r24 * self->r35 * self->r40 * self->r51)
	- (self->r02 * self->r13 * self->r24 * self->r35 * self->r41 * self->r50)
	- (self->r02 * self->r13 * self->r25 * self->r30 * self->r41 * self->r54)
	+ (self->r02 * self->r13 * self->r25 * self->r30 * self->r44 * self->r51)
	+ (self->r02 * self->r13 * self->r25 * self->r31 * self->r40 * self->r54)
	- (self->r02 * self->r13 * self->r25 * self->r31 * self->r44 * self->r50)
	- (self->r02 * self->r13 * self->r25 * self->r34 * self->r40 * self->r51)
	+ (self->r02 * self->r13 * self->r25 * self->r34 * self->r41 * self->r50)
	- (self->r02 * self->r14 * self->r20 * self->r31 * self->r43 * self->r55)
	+ (self->r02 * self->r14 * self->r20 * self->r31 * self->r45 * self->r53)
	+ (self->r02 * self->r14 * self->r20 * self->r33 * self->r41 * self->r55)
	- (self->r02 * self->r14 * self->r20 * self->r33 * self->r45 * self->r51)
	- (self->r02 * self->r14 * self->r20 * self->r35 * self->r41 * self->r53)
	+ (self->r02 * self->r14 * self->r20 * self->r35 * self->r43 * self->r51)
	+ (self->r02 * self->r14 * self->r21 * self->r30 * self->r43 * self->r55)
	- (self->r02 * self->r14 * self->r21 * self->r30 * self->r45 * self->r53)
	- (self->r02 * self->r14 * self->r21 * self->r33 * self->r40 * self->r55)
	+ (self->r02 * self->r14 * self->r21 * self->r33 * self->r45 * self->r50)
	+ (self->r02 * self->r14 * self->r21 * self->r35 * self->r40 * self->r53)
	- (self->r02 * self->r14 * self->r21 * self->r35 * self->r43 * self->r50)
	- (self->r02 * self->r14 * self->r23 * self->r30 * self->r41 * self->r55)
	+ (self->r02 * self->r14 * self->r23 * self->r30 * self->r45 * self->r51)
	+ (self->r02 * self->r14 * self->r23 * self->r31 * self->r40 * self->r55)
	- (self->r02 * self->r14 * self->r23 * self->r31 * self->r45 * self->r50)
	- (self->r02 * self->r14 * self->r23 * self->r35 * self->r40 * self->r51)
	+ (self->r02 * self->r14 * self->r23 * self->r35 * self->r41 * self->r50)
	+ (self->r02 * self->r14 * self->r25 * self->r30 * self->r41 * self->r53)
	- (self->r02 * self->r14 * self->r25 * self->r30 * self->r43 * self->r51)
	- (self->r02 * self->r14 * self->r25 * self->r31 * self->r40 * self->r53)
	+ (self->r02 * self->r14 * self->r25 * self->r31 * self->r43 * self->r50)
	+ (self->r02 * self->r14 * self->r25 * self->r33 * self->r40 * self->r51)
	- (self->r02 * self->r14 * self->r25 * self->r33 * self->r41 * self->r50)
	+ (self->r02 * self->r15 * self->r20 * self->r31 * self->r43 * self->r54)
	- (self->r02 * self->r15 * self->r20 * self->r31 * self->r44 * self->r53)
	- (self->r02 * self->r15 * self->r20 * self->r33 * self->r41 * self->r54)
	+ (self->r02 * self->r15 * self->r20 * self->r33 * self->r44 * self->r51)
	+ (self->r02 * self->r15 * self->r20 * self->r34 * self->r41 * self->r53)
	- (self->r02 * self->r15 * self->r20 * self->r34 * self->r43 * self->r51)
	- (self->r02 * self->r15 * self->r21 * self->r30 * self->r43 * self->r54)
	+ (self->r02 * self->r15 * self->r21 * self->r30 * self->r44 * self->r53)
	+ (self->r02 * self->r15 * self->r21 * self->r33 * self->r40 * self->r54)
	- (self->r02 * self->r15 * self->r21 * self->r33 * self->r44 * self->r50)
	- (self->r02 * self->r15 * self->r21 * self->r34 * self->r40 * self->r53)
	+ (self->r02 * self->r15 * self->r21 * self->r34 * self->r43 * self->r50)
	+ (self->r02 * self->r15 * self->r23 * self->r30 * self->r41 * self->r54)
	- (self->r02 * self->r15 * self->r23 * self->r30 * self->r44 * self->r51)
	- (self->r02 * self->r15 * self->r23 * self->r31 * self->r40 * self->r54)
	+ (self->r02 * self->r15 * self->r23 * self->r31 * self->r44 * self->r50)
	+ (self->r02 * self->r15 * self->r23 * self->r34 * self->r40 * self->r51)
	- (self->r02 * self->r15 * self->r23 * self->r34 * self->r41 * self->r50)
	- (self->r02 * self->r15 * self->r24 * self->r30 * self->r41 * self->r53)
	+ (self->r02 * self->r15 * self->r24 * self->r30 * self->r43 * self->r51)
	+ (self->r02 * self->r15 * self->r24 * self->r31 * self->r40 * self->r53)
	- (self->r02 * self->r15 * self->r24 * self->r31 * self->r43 * self->r50)
	- (self->r02 * self->r15 * self->r24 * self->r33 * self->r40 * self->r51)
	+ (self->r02 * self->r15 * self->r24 * self->r33 * self->r41 * self->r50)
	- (self->r03 * self->r10 * self->r21 * self->r32 * self->r44 * self->r55)
	+ (self->r03 * self->r10 * self->r21 * self->r32 * self->r45 * self->r54)
	+ (self->r03 * self->r10 * self->r21 * self->r34 * self->r42 * self->r55)
	- (self->r03 * self->r10 * self->r21 * self->r34 * self->r45 * self->r52)
	- (self->r03 * self->r10 * self->r21 * self->r35 * self->r42 * self->r54)
	+ (self->r03 * self->r10 * self->r21 * self->r35 * self->r44 * self->r52)
	+ (self->r03 * self->r10 * self->r22 * self->r31 * self->r44 * self->r55)
	- (self->r03 * self->r10 * self->r22 * self->r31 * self->r45 * self->r54)
	- (self->r03 * self->r10 * self->r22 * self->r34 * self->r41 * self->r55)
	+ (self->r03 * self->r10 * self->r22 * self->r34 * self->r45 * self->r51)
	+ (self->r03 * self->r10 * self->r22 * self->r35 * self->r41 * self->r54)
	- (self->r03 * self->r10 * self->r22 * self->r35 * self->r44 * self->r51)
	- (self->r03 * self->r10 * self->r24 * self->r31 * self->r42 * self->r55)
	+ (self->r03 * self->r10 * self->r24 * self->r31 * self->r45 * self->r52)
	+ (self->r03 * self->r10 * self->r24 * self->r32 * self->r41 * self->r55)
	- (self->r03 * self->r10 * self->r24 * self->r32 * self->r45 * self->r51)
	- (self->r03 * self->r10 * self->r24 * self->r35 * self->r41 * self->r52)
	+ (self->r03 * self->r10 * self->r24 * self->r35 * self->r42 * self->r51)
	+ (self->r03 * self->r10 * self->r25 * self->r31 * self->r42 * self->r54)
	- (self->r03 * self->r10 * self->r25 * self->r31 * self->r44 * self->r52)
	- (self->r03 * self->r10 * self->r25 * self->r32 * self->r41 * self->r54)
	+ (self->r03 * self->r10 * self->r25 * self->r32 * self->r44 * self->r51)
	+ (self->r03 * self->r10 * self->r25 * self->r34 * self->r41 * self->r52)
	- (self->r03 * self->r10 * self->r25 * self->r34 * self->r42 * self->r51)
	+ (self->r03 * self->r11 * self->r20 * self->r32 * self->r44 * self->r55)
	- (self->r03 * self->r11 * self->r20 * self->r32 * self->r45 * self->r54)
	- (self->r03 * self->r11 * self->r20 * self->r34 * self->r42 * self->r55)
	+ (self->r03 * self->r11 * self->r20 * self->r34 * self->r45 * self->r52)
	+ (self->r03 * self->r11 * self->r20 * self->r35 * self->r42 * self->r54)
	- (self->r03 * self->r11 * self->r20 * self->r35 * self->r44 * self->r52)
	- (self->r03 * self->r11 * self->r22 * self->r30 * self->r44 * self->r55)
	+ (self->r03 * self->r11 * self->r22 * self->r30 * self->r45 * self->r54)
	+ (self->r03 * self->r11 * self->r22 * self->r34 * self->r40 * self->r55)
	- (self->r03 * self->r11 * self->r22 * self->r34 * self->r45 * self->r50)
	- (self->r03 * self->r11 * self->r22 * self->r35 * self->r40 * self->r54)
	+ (self->r03 * self->r11 * self->r22 * self->r35 * self->r44 * self->r50)
	+ (self->r03 * self->r11 * self->r24 * self->r30 * self->r42 * self->r55)
	- (self->r03 * self->r11 * self->r24 * self->r30 * self->r45 * self->r52)
	- (self->r03 * self->r11 * self->r24 * self->r32 * self->r40 * self->r55)
	+ (self->r03 * self->r11 * self->r24 * self->r32 * self->r45 * self->r50)
	+ (self->r03 * self->r11 * self->r24 * self->r35 * self->r40 * self->r52)
	- (self->r03 * self->r11 * self->r24 * self->r35 * self->r42 * self->r50)
	- (self->r03 * self->r11 * self->r25 * self->r30 * self->r42 * self->r54)
	+ (self->r03 * self->r11 * self->r25 * self->r30 * self->r44 * self->r52)
	+ (self->r03 * self->r11 * self->r25 * self->r32 * self->r40 * self->r54)
	- (self->r03 * self->r11 * self->r25 * self->r32 * self->r44 * self->r50)
	- (self->r03 * self->r11 * self->r25 * self->r34 * self->r40 * self->r52)
	+ (self->r03 * self->r11 * self->r25 * self->r34 * self->r42 * self->r50)
	- (self->r03 * self->r12 * self->r20 * self->r31 * self->r44 * self->r55)
	+ (self->r03 * self->r12 * self->r20 * self->r31 * self->r45 * self->r54)
	+ (self->r03 * self->r12 * self->r20 * self->r34 * self->r41 * self->r55)
	- (self->r03 * self->r12 * self->r20 * self->r34 * self->r45 * self->r51)
	- (self->r03 * self->r12 * self->r20 * self->r35 * self->r41 * self->r54)
	+ (self->r03 * self->r12 * self->r20 * self->r35 * self->r44 * self->r51)
	+ (self->r03 * self->r12 * self->r21 * self->r30 * self->r44 * self->r55)
	- (self->r03 * self->r12 * self->r21 * self->r30 * self->r45 * self->r54)
	- (self->r03 * self->r12 * self->r21 * self->r34 * self->r40 * self->r55)
	+ (self->r03 * self->r12 * self->r21 * self->r34 * self->r45 * self->r50)
	+ (self->r03 * self->r12 * self->r21 * self->r35 * self->r40 * self->r54)
	- (self->r03 * self->r12 * self->r21 * self->r35 * self->r44 * self->r50)
	- (self->r03 * self->r12 * self->r24 * self->r30 * self->r41 * self->r55)
	+ (self->r03 * self->r12 * self->r24 * self->r30 * self->r45 * self->r51)
	+ (self->r03 * self->r12 * self->r24 * self->r31 * self->r40 * self->r55)
	- (self->r03 * self->r12 * self->r24 * self->r31 * self->r45 * self->r50)
	- (self->r03 * self->r12 * self->r24 * self->r35 * self->r40 * self->r51)
	+ (self->r03 * self->r12 * self->r24 * self->r35 * self->r41 * self->r50)
	+ (self->r03 * self->r12 * self->r25 * self->r30 * self->r41 * self->r54)
	- (self->r03 * self->r12 * self->r25 * self->r30 * self->r44 * self->r51)
	- (self->r03 * self->r12 * self->r25 * self->r31 * self->r40 * self->r54)
	+ (self->r03 * self->r12 * self->r25 * self->r31 * self->r44 * self->r50)
	+ (self->r03 * self->r12 * self->r25 * self->r34 * self->r40 * self->r51)
	- (self->r03 * self->r12 * self->r25 * self->r34 * self->r41 * self->r50)
	+ (self->r03 * self->r14 * self->r20 * self->r31 * self->r42 * self->r55)
	- (self->r03 * self->r14 * self->r20 * self->r31 * self->r45 * self->r52)
	- (self->r03 * self->r14 * self->r20 * self->r32 * self->r41 * self->r55)
	+ (self->r03 * self->r14 * self->r20 * self->r32 * self->r45 * self->r51)
	+ (self->r03 * self->r14 * self->r20 * self->r35 * self->r41 * self->r52)
	- (self->r03 * self->r14 * self->r20 * self->r35 * self->r42 * self->r51)
	- (self->r03 * self->r14 * self->r21 * self->r30 * self->r42 * self->r55)
	+ (self->r03 * self->r14 * self->r21 * self->r30 * self->r45 * self->r52)
	+ (self->r03 * self->r14 * self->r21 * self->r32 * self->r40 * self->r55)
	- (self->r03 * self->r14 * self->r21 * self->r32 * self->r45 * self->r50)
	- (self->r03 * self->r14 * self->r21 * self->r35 * self->r40 * self->r52)
	+ (self->r03 * self->r14 * self->r21 * self->r35 * self->r42 * self->r50)
	+ (self->r03 * self->r14 * self->r22 * self->r30 * self->r41 * self->r55)
	- (self->r03 * self->r14 * self->r22 * self->r30 * self->r45 * self->r51)
	- (self->r03 * self->r14 * self->r22 * self->r31 * self->r40 * self->r55)
	+ (self->r03 * self->r14 * self->r22 * self->r31 * self->r45 * self->r50)
	+ (self->r03 * self->r14 * self->r22 * self->r35 * self->r40 * self->r51)
	- (self->r03 * self->r14 * self->r22 * self->r35 * self->r41 * self->r50)
	- (self->r03 * self->r14 * self->r25 * self->r30 * self->r41 * self->r52)
	+ (self->r03 * self->r14 * self->r25 * self->r30 * self->r42 * self->r51)
	+ (self->r03 * self->r14 * self->r25 * self->r31 * self->r40 * self->r52)
	- (self->r03 * self->r14 * self->r25 * self->r31 * self->r42 * self->r50)
	- (self->r03 * self->r14 * self->r25 * self->r32 * self->r40 * self->r51)
	+ (self->r03 * self->r14 * self->r25 * self->r32 * self->r41 * self->r50)
	- (self->r03 * self->r15 * self->r20 * self->r31 * self->r42 * self->r54)
	+ (self->r03 * self->r15 * self->r20 * self->r31 * self->r44 * self->r52)
	+ (self->r03 * self->r15 * self->r20 * self->r32 * self->r41 * self->r54)
	- (self->r03 * self->r15 * self->r20 * self->r32 * self->r44 * self->r51)
	- (self->r03 * self->r15 * self->r20 * self->r34 * self->r41 * self->r52)
	+ (self->r03 * self->r15 * self->r20 * self->r34 * self->r42 * self->r51)
	+ (self->r03 * self->r15 * self->r21 * self->r30 * self->r42 * self->r54)
	- (self->r03 * self->r15 * self->r21 * self->r30 * self->r44 * self->r52)
	- (self->r03 * self->r15 * self->r21 * self->r32 * self->r40 * self->r54)
	+ (self->r03 * self->r15 * self->r21 * self->r32 * self->r44 * self->r50)
	+ (self->r03 * self->r15 * self->r21 * self->r34 * self->r40 * self->r52)
	- (self->r03 * self->r15 * self->r21 * self->r34 * self->r42 * self->r50)
	- (self->r03 * self->r15 * self->r22 * self->r30 * self->r41 * self->r54)
	+ (self->r03 * self->r15 * self->r22 * self->r30 * self->r44 * self->r51)
	+ (self->r03 * self->r15 * self->r22 * self->r31 * self->r40 * self->r54)
	- (self->r03 * self->r15 * self->r22 * self->r31 * self->r44 * self->r50)
	- (self->r03 * self->r15 * self->r22 * self->r34 * self->r40 * self->r51)
	+ (self->r03 * self->r15 * self->r22 * self->r34 * self->r41 * self->r50)
	+ (self->r03 * self->r15 * self->r24 * self->r30 * self->r41 * self->r52)
	- (self->r03 * self->r15 * self->r24 * self->r30 * self->r42 * self->r51)
	- (self->r03 * self->r15 * self->r24 * self->r31 * self->r40 * self->r52)
	+ (self->r03 * self->r15 * self->r24 * self->r31 * self->r42 * self->r50)
	+ (self->r03 * self->r15 * self->r24 * self->r32 * self->r40 * self->r51)
	- (self->r03 * self->r15 * self->r24 * self->r32 * self->r41 * self->r50)
	+ (self->r04 * self->r10 * self->r21 * self->r32 * self->r43 * self->r55)
	- (self->r04 * self->r10 * self->r21 * self->r32 * self->r45 * self->r53)
	- (self->r04 * self->r10 * self->r21 * self->r33 * self->r42 * self->r55)
	+ (self->r04 * self->r10 * self->r21 * self->r33 * self->r45 * self->r52)
	+ (self->r04 * self->r10 * self->r21 * self->r35 * self->r42 * self->r53)
	- (self->r04 * self->r10 * self->r21 * self->r35 * self->r43 * self->r52)
	- (self->r04 * self->r10 * self->r22 * self->r31 * self->r43 * self->r55)
	+ (self->r04 * self->r10 * self->r22 * self->r31 * self->r45 * self->r53)
	+ (self->r04 * self->r10 * self->r22 * self->r33 * self->r41 * self->r55)
	- (self->r04 * self->r10 * self->r22 * self->r33 * self->r45 * self->r51)
	- (self->r04 * self->r10 * self->r22 * self->r35 * self->r41 * self->r53)
	+ (self->r04 * self->r10 * self->r22 * self->r35 * self->r43 * self->r51)
	+ (self->r04 * self->r10 * self->r23 * self->r31 * self->r42 * self->r55)
	- (self->r04 * self->r10 * self->r23 * self->r31 * self->r45 * self->r52)
	- (self->r04 * self->r10 * self->r23 * self->r32 * self->r41 * self->r55)
	+ (self->r04 * self->r10 * self->r23 * self->r32 * self->r45 * self->r51)
	+ (self->r04 * self->r10 * self->r23 * self->r35 * self->r41 * self->r52)
	- (self->r04 * self->r10 * self->r23 * self->r35 * self->r42 * self->r51)
	- (self->r04 * self->r10 * self->r25 * self->r31 * self->r42 * self->r53)
	+ (self->r04 * self->r10 * self->r25 * self->r31 * self->r43 * self->r52)
	+ (self->r04 * self->r10 * self->r25 * self->r32 * self->r41 * self->r53)
	- (self->r04 * self->r10 * self->r25 * self->r32 * self->r43 * self->r51)
	- (self->r04 * self->r10 * self->r25 * self->r33 * self->r41 * self->r52)
	+ (self->r04 * self->r10 * self->r25 * self->r33 * self->r42 * self->r51)
	- (self->r04 * self->r11 * self->r20 * self->r32 * self->r43 * self->r55)
	+ (self->r04 * self->r11 * self->r20 * self->r32 * self->r45 * self->r53)
	+ (self->r04 * self->r11 * self->r20 * self->r33 * self->r42 * self->r55)
	- (self->r04 * self->r11 * self->r20 * self->r33 * self->r45 * self->r52)
	- (self->r04 * self->r11 * self->r20 * self->r35 * self->r42 * self->r53)
	+ (self->r04 * self->r11 * self->r20 * self->r35 * self->r43 * self->r52)
	+ (self->r04 * self->r11 * self->r22 * self->r30 * self->r43 * self->r55)
	- (self->r04 * self->r11 * self->r22 * self->r30 * self->r45 * self->r53)
	- (self->r04 * self->r11 * self->r22 * self->r33 * self->r40 * self->r55)
	+ (self->r04 * self->r11 * self->r22 * self->r33 * self->r45 * self->r50)
	+ (self->r04 * self->r11 * self->r22 * self->r35 * self->r40 * self->r53)
	- (self->r04 * self->r11 * self->r22 * self->r35 * self->r43 * self->r50)
	- (self->r04 * self->r11 * self->r23 * self->r30 * self->r42 * self->r55)
	+ (self->r04 * self->r11 * self->r23 * self->r30 * self->r45 * self->r52)
	+ (self->r04 * self->r11 * self->r23 * self->r32 * self->r40 * self->r55)
	- (self->r04 * self->r11 * self->r23 * self->r32 * self->r45 * self->r50)
	- (self->r04 * self->r11 * self->r23 * self->r35 * self->r40 * self->r52)
	+ (self->r04 * self->r11 * self->r23 * self->r35 * self->r42 * self->r50)
	+ (self->r04 * self->r11 * self->r25 * self->r30 * self->r42 * self->r53)
	- (self->r04 * self->r11 * self->r25 * self->r30 * self->r43 * self->r52)
	- (self->r04 * self->r11 * self->r25 * self->r32 * self->r40 * self->r53)
	+ (self->r04 * self->r11 * self->r25 * self->r32 * self->r43 * self->r50)
	+ (self->r04 * self->r11 * self->r25 * self->r33 * self->r40 * self->r52)
	- (self->r04 * self->r11 * self->r25 * self->r33 * self->r42 * self->r50)
	+ (self->r04 * self->r12 * self->r20 * self->r31 * self->r43 * self->r55)
	- (self->r04 * self->r12 * self->r20 * self->r31 * self->r45 * self->r53)
	- (self->r04 * self->r12 * self->r20 * self->r33 * self->r41 * self->r55)
	+ (self->r04 * self->r12 * self->r20 * self->r33 * self->r45 * self->r51)
	+ (self->r04 * self->r12 * self->r20 * self->r35 * self->r41 * self->r53)
	- (self->r04 * self->r12 * self->r20 * self->r35 * self->r43 * self->r51)
	- (self->r04 * self->r12 * self->r21 * self->r30 * self->r43 * self->r55)
	+ (self->r04 * self->r12 * self->r21 * self->r30 * self->r45 * self->r53)
	+ (self->r04 * self->r12 * self->r21 * self->r33 * self->r40 * self->r55)
	- (self->r04 * self->r12 * self->r21 * self->r33 * self->r45 * self->r50)
	- (self->r04 * self->r12 * self->r21 * self->r35 * self->r40 * self->r53)
	+ (self->r04 * self->r12 * self->r21 * self->r35 * self->r43 * self->r50)
	+ (self->r04 * self->r12 * self->r23 * self->r30 * self->r41 * self->r55)
	- (self->r04 * self->r12 * self->r23 * self->r30 * self->r45 * self->r51)
	- (self->r04 * self->r12 * self->r23 * self->r31 * self->r40 * self->r55)
	+ (self->r04 * self->r12 * self->r23 * self->r31 * self->r45 * self->r50)
	+ (self->r04 * self->r12 * self->r23 * self->r35 * self->r40 * self->r51)
	- (self->r04 * self->r12 * self->r23 * self->r35 * self->r41 * self->r50)
	- (self->r04 * self->r12 * self->r25 * self->r30 * self->r41 * self->r53)
	+ (self->r04 * self->r12 * self->r25 * self->r30 * self->r43 * self->r51)
	+ (self->r04 * self->r12 * self->r25 * self->r31 * self->r40 * self->r53)
	- (self->r04 * self->r12 * self->r25 * self->r31 * self->r43 * self->r50)
	- (self->r04 * self->r12 * self->r25 * self->r33 * self->r40 * self->r51)
	+ (self->r04 * self->r12 * self->r25 * self->r33 * self->r41 * self->r50)
	- (self->r04 * self->r13 * self->r20 * self->r31 * self->r42 * self->r55)
	+ (self->r04 * self->r13 * self->r20 * self->r31 * self->r45 * self->r52)
	+ (self->r04 * self->r13 * self->r20 * self->r32 * self->r41 * self->r55)
	- (self->r04 * self->r13 * self->r20 * self->r32 * self->r45 * self->r51)
	- (self->r04 * self->r13 * self->r20 * self->r35 * self->r41 * self->r52)
	+ (self->r04 * self->r13 * self->r20 * self->r35 * self->r42 * self->r51)
	+ (self->r04 * self->r13 * self->r21 * self->r30 * self->r42 * self->r55)
	- (self->r04 * self->r13 * self->r21 * self->r30 * self->r45 * self->r52)
	- (self->r04 * self->r13 * self->r21 * self->r32 * self->r40 * self->r55)
	+ (self->r04 * self->r13 * self->r21 * self->r32 * self->r45 * self->r50)
	+ (self->r04 * self->r13 * self->r21 * self->r35 * self->r40 * self->r52)
	- (self->r04 * self->r13 * self->r21 * self->r35 * self->r42 * self->r50)
	- (self->r04 * self->r13 * self->r22 * self->r30 * self->r41 * self->r55)
	+ (self->r04 * self->r13 * self->r22 * self->r30 * self->r45 * self->r51)
	+ (self->r04 * self->r13 * self->r22 * self->r31 * self->r40 * self->r55)
	- (self->r04 * self->r13 * self->r22 * self->r31 * self->r45 * self->r50)
	- (self->r04 * self->r13 * self->r22 * self->r35 * self->r40 * self->r51)
	+ (self->r04 * self->r13 * self->r22 * self->r35 * self->r41 * self->r50)
	+ (self->r04 * self->r13 * self->r25 * self->r30 * self->r41 * self->r52)
	- (self->r04 * self->r13 * self->r25 * self->r30 * self->r42 * self->r51)
	- (self->r04 * self->r13 * self->r25 * self->r31 * self->r40 * self->r52)
	+ (self->r04 * self->r13 * self->r25 * self->r31 * self->r42 * self->r50)
	+ (self->r04 * self->r13 * self->r25 * self->r32 * self->r40 * self->r51)
	- (self->r04 * self->r13 * self->r25 * self->r32 * self->r41 * self->r50)
	+ (self->r04 * self->r15 * self->r20 * self->r31 * self->r42 * self->r53)
	- (self->r04 * self->r15 * self->r20 * self->r31 * self->r43 * self->r52)
	- (self->r04 * self->r15 * self->r20 * self->r32 * self->r41 * self->r53)
	+ (self->r04 * self->r15 * self->r20 * self->r32 * self->r43 * self->r51)
	+ (self->r04 * self->r15 * self->r20 * self->r33 * self->r41 * self->r52)
	- (self->r04 * self->r15 * self->r20 * self->r33 * self->r42 * self->r51)
	- (self->r04 * self->r15 * self->r21 * self->r30 * self->r42 * self->r53)
	+ (self->r04 * self->r15 * self->r21 * self->r30 * self->r43 * self->r52)
	+ (self->r04 * self->r15 * self->r21 * self->r32 * self->r40 * self->r53)
	- (self->r04 * self->r15 * self->r21 * self->r32 * self->r43 * self->r50)
	- (self->r04 * self->r15 * self->r21 * self->r33 * self->r40 * self->r52)
	+ (self->r04 * self->r15 * self->r21 * self->r33 * self->r42 * self->r50)
	+ (self->r04 * self->r15 * self->r22 * self->r30 * self->r41 * self->r53)
	- (self->r04 * self->r15 * self->r22 * self->r30 * self->r43 * self->r51)
	- (self->r04 * self->r15 * self->r22 * self->r31 * self->r40 * self->r53)
	+ (self->r04 * self->r15 * self->r22 * self->r31 * self->r43 * self->r50)
	+ (self->r04 * self->r15 * self->r22 * self->r33 * self->r40 * self->r51)
	- (self->r04 * self->r15 * self->r22 * self->r33 * self->r41 * self->r50)
	- (self->r04 * self->r15 * self->r23 * self->r30 * self->r41 * self->r52)
	+ (self->r04 * self->r15 * self->r23 * self->r30 * self->r42 * self->r51)
	+ (self->r04 * self->r15 * self->r23 * self->r31 * self->r40 * self->r52)
	- (self->r04 * self->r15 * self->r23 * self->r31 * self->r42 * self->r50)
	- (self->r04 * self->r15 * self->r23 * self->r32 * self->r40 * self->r51)
	+ (self->r04 * self->r15 * self->r23 * self->r32 * self->r41 * self->r50)
	- (self->r05 * self->r10 * self->r21 * self->r32 * self->r43 * self->r54)
	+ (self->r05 * self->r10 * self->r21 * self->r32 * self->r44 * self->r53)
	+ (self->r05 * self->r10 * self->r21 * self->r33 * self->r42 * self->r54)
	- (self->r05 * self->r10 * self->r21 * self->r33 * self->r44 * self->r52)
	- (self->r05 * self->r10 * self->r21 * self->r34 * self->r42 * self->r53)
	+ (self->r05 * self->r10 * self->r21 * self->r34 * self->r43 * self->r52)
	+ (self->r05 * self->r10 * self->r22 * self->r31 * self->r43 * self->r54)
	- (self->r05 * self->r10 * self->r22 * self->r31 * self->r44 * self->r53)
	- (self->r05 * self->r10 * self->r22 * self->r33 * self->r41 * self->r54)
	+ (self->r05 * self->r10 * self->r22 * self->r33 * self->r44 * self->r51)
	+ (self->r05 * self->r10 * self->r22 * self->r34 * self->r41 * self->r53)
	- (self->r05 * self->r10 * self->r22 * self->r34 * self->r43 * self->r51)
	- (self->r05 * self->r10 * self->r23 * self->r31 * self->r42 * self->r54)
	+ (self->r05 * self->r10 * self->r23 * self->r31 * self->r44 * self->r52)
	+ (self->r05 * self->r10 * self->r23 * self->r32 * self->r41 * self->r54)
	- (self->r05 * self->r10 * self->r23 * self->r32 * self->r44 * self->r51)
	- (self->r05 * self->r10 * self->r23 * self->r34 * self->r41 * self->r52)
	+ (self->r05 * self->r10 * self->r23 * self->r34 * self->r42 * self->r51)
	+ (self->r05 * self->r10 * self->r24 * self->r31 * self->r42 * self->r53)
	- (self->r05 * self->r10 * self->r24 * self->r31 * self->r43 * self->r52)
	- (self->r05 * self->r10 * self->r24 * self->r32 * self->r41 * self->r53)
	+ (self->r05 * self->r10 * self->r24 * self->r32 * self->r43 * self->r51)
	+ (self->r05 * self->r10 * self->r24 * self->r33 * self->r41 * self->r52)
	- (self->r05 * self->r10 * self->r24 * self->r33 * self->r42 * self->r51)
	+ (self->r05 * self->r11 * self->r20 * self->r32 * self->r43 * self->r54)
	- (self->r05 * self->r11 * self->r20 * self->r32 * self->r44 * self->r53)
	- (self->r05 * self->r11 * self->r20 * self->r33 * self->r42 * self->r54)
	+ (self->r05 * self->r11 * self->r20 * self->r33 * self->r44 * self->r52)
	+ (self->r05 * self->r11 * self->r20 * self->r34 * self->r42 * self->r53)
	- (self->r05 * self->r11 * self->r20 * self->r34 * self->r43 * self->r52)
	- (self->r05 * self->r11 * self->r22 * self->r30 * self->r43 * self->r54)
	+ (self->r05 * self->r11 * self->r22 * self->r30 * self->r44 * self->r53)
	+ (self->r05 * self->r11 * self->r22 * self->r33 * self->r40 * self->r54)
	- (self->r05 * self->r11 * self->r22 * self->r33 * self->r44 * self->r50)
	- (self->r05 * self->r11 * self->r22 * self->r34 * self->r40 * self->r53)
	+ (self->r05 * self->r11 * self->r22 * self->r34 * self->r43 * self->r50)
	+ (self->r05 * self->r11 * self->r23 * self->r30 * self->r42 * self->r54)
	- (self->r05 * self->r11 * self->r23 * self->r30 * self->r44 * self->r52)
	- (self->r05 * self->r11 * self->r23 * self->r32 * self->r40 * self->r54)
	+ (self->r05 * self->r11 * self->r23 * self->r32 * self->r44 * self->r50)
	+ (self->r05 * self->r11 * self->r23 * self->r34 * self->r40 * self->r52)
	- (self->r05 * self->r11 * self->r23 * self->r34 * self->r42 * self->r50)
	- (self->r05 * self->r11 * self->r24 * self->r30 * self->r42 * self->r53)
	+ (self->r05 * self->r11 * self->r24 * self->r30 * self->r43 * self->r52)
	+ (self->r05 * self->r11 * self->r24 * self->r32 * self->r40 * self->r53)
	- (self->r05 * self->r11 * self->r24 * self->r32 * self->r43 * self->r50)
	- (self->r05 * self->r11 * self->r24 * self->r33 * self->r40 * self->r52)
	+ (self->r05 * self->r11 * self->r24 * self->r33 * self->r42 * self->r50)
	- (self->r05 * self->r12 * self->r20 * self->r31 * self->r43 * self->r54)
	+ (self->r05 * self->r12 * self->r20 * self->r31 * self->r44 * self->r53)
	+ (self->r05 * self->r12 * self->r20 * self->r33 * self->r41 * self->r54)
	- (self->r05 * self->r12 * self->r20 * self->r33 * self->r44 * self->r51)
	- (self->r05 * self->r12 * self->r20 * self->r34 * self->r41 * self->r53)
	+ (self->r05 * self->r12 * self->r20 * self->r34 * self->r43 * self->r51)
	+ (self->r05 * self->r12 * self->r21 * self->r30 * self->r43 * self->r54)
	- (self->r05 * self->r12 * self->r21 * self->r30 * self->r44 * self->r53)
	- (self->r05 * self->r12 * self->r21 * self->r33 * self->r40 * self->r54)
	+ (self->r05 * self->r12 * self->r21 * self->r33 * self->r44 * self->r50)
	+ (self->r05 * self->r12 * self->r21 * self->r34 * self->r40 * self->r53)
	- (self->r05 * self->r12 * self->r21 * self->r34 * self->r43 * self->r50)
	- (self->r05 * self->r12 * self->r23 * self->r30 * self->r41 * self->r54)
	+ (self->r05 * self->r12 * self->r23 * self->r30 * self->r44 * self->r51)
	+ (self->r05 * self->r12 * self->r23 * self->r31 * self->r40 * self->r54)
	- (self->r05 * self->r12 * self->r23 * self->r31 * self->r44 * self->r50)
	- (self->r05 * self->r12 * self->r23 * self->r34 * self->r40 * self->r51)
	+ (self->r05 * self->r12 * self->r23 * self->r34 * self->r41 * self->r50)
	+ (self->r05 * self->r12 * self->r24 * self->r30 * self->r41 * self->r53)
	- (self->r05 * self->r12 * self->r24 * self->r30 * self->r43 * self->r51)
	- (self->r05 * self->r12 * self->r24 * self->r31 * self->r40 * self->r53)
	+ (self->r05 * self->r12 * self->r24 * self->r31 * self->r43 * self->r50)
	+ (self->r05 * self->r12 * self->r24 * self->r33 * self->r40 * self->r51)
	- (self->r05 * self->r12 * self->r24 * self->r33 * self->r41 * self->r50)
	+ (self->r05 * self->r13 * self->r20 * self->r31 * self->r42 * self->r54)
	- (self->r05 * self->r13 * self->r20 * self->r31 * self->r44 * self->r52)
	- (self->r05 * self->r13 * self->r20 * self->r32 * self->r41 * self->r54)
	+ (self->r05 * self->r13 * self->r20 * self->r32 * self->r44 * self->r51)
	+ (self->r05 * self->r13 * self->r20 * self->r34 * self->r41 * self->r52)
	- (self->r05 * self->r13 * self->r20 * self->r34 * self->r42 * self->r51)
	- (self->r05 * self->r13 * self->r21 * self->r30 * self->r42 * self->r54)
	+ (self->r05 * self->r13 * self->r21 * self->r30 * self->r44 * self->r52)
	+ (self->r05 * self->r13 * self->r21 * self->r32 * self->r40 * self->r54)
	- (self->r05 * self->r13 * self->r21 * self->r32 * self->r44 * self->r50)
	- (self->r05 * self->r13 * self->r21 * self->r34 * self->r40 * self->r52)
	+ (self->r05 * self->r13 * self->r21 * self->r34 * self->r42 * self->r50)
	+ (self->r05 * self->r13 * self->r22 * self->r30 * self->r41 * self->r54)
	- (self->r05 * self->r13 * self->r22 * self->r30 * self->r44 * self->r51)
	- (self->r05 * self->r13 * self->r22 * self->r31 * self->r40 * self->r54)
	+ (self->r05 * self->r13 * self->r22 * self->r31 * self->r44 * self->r50)
	+ (self->r05 * self->r13 * self->r22 * self->r34 * self->r40 * self->r51)
	- (self->r05 * self->r13 * self->r22 * self->r34 * self->r41 * self->r50)
	- (self->r05 * self->r13 * self->r24 * self->r30 * self->r41 * self->r52)
	+ (self->r05 * self->r13 * self->r24 * self->r30 * self->r42 * self->r51)
	+ (self->r05 * self->r13 * self->r24 * self->r31 * self->r40 * self->r52)
	- (self->r05 * self->r13 * self->r24 * self->r31 * self->r42 * self->r50)
	- (self->r05 * self->r13 * self->r24 * self->r32 * self->r40 * self->r51)
	+ (self->r05 * self->r13 * self->r24 * self->r32 * self->r41 * self->r50)
	- (self->r05 * self->r14 * self->r20 * self->r31 * self->r42 * self->r53)
	+ (self->r05 * self->r14 * self->r20 * self->r31 * self->r43 * self->r52)
	+ (self->r05 * self->r14 * self->r20 * self->r32 * self->r41 * self->r53)
	- (self->r05 * self->r14 * self->r20 * self->r32 * self->r43 * self->r51)
	- (self->r05 * self->r14 * self->r20 * self->r33 * self->r41 * self->r52)
	+ (self->r05 * self->r14 * self->r20 * self->r33 * self->r42 * self->r51)
	+ (self->r05 * self->r14 * self->r21 * self->r30 * self->r42 * self->r53)
	- (self->r05 * self->r14 * self->r21 * self->r30 * self->r43 * self->r52)
	- (self->r05 * self->r14 * self->r21 * self->r32 * self->r40 * self->r53)
	+ (self->r05 * self->r14 * self->r21 * self->r32 * self->r43 * self->r50)
	+ (self->r05 * self->r14 * self->r21 * self->r33 * self->r40 * self->r52)
	- (self->r05 * self->r14 * self->r21 * self->r33 * self->r42 * self->r50)
	- (self->r05 * self->r14 * self->r22 * self->r30 * self->r41 * self->r53)
	+ (self->r05 * self->r14 * self->r22 * self->r30 * self->r43 * self->r51)
	+ (self->r05 * self->r14 * self->r22 * self->r31 * self->r40 * self->r53)
	- (self->r05 * self->r14 * self->r22 * self->r31 * self->r43 * self->r50)
	- (self->r05 * self->r14 * self->r22 * self->r33 * self->r40 * self->r51)
	+ (self->r05 * self->r14 * self->r22 * self->r33 * self->r41 * self->r50)
	+ (self->r05 * self->r14 * self->r23 * self->r30 * self->r41 * self->r52)
	- (self->r05 * self->r14 * self->r23 * self->r30 * self->r42 * self->r51)
	- (self->r05 * self->r14 * self->r23 * self->r31 * self->r40 * self->r52)
	+ (self->r05 * self->r14 * self->r23 * self->r31 * self->r42 * self->r50)
	+ (self->r05 * self->r14 * self->r23 * self->r32 * self->r40 * self->r51)
	- (self->r05 * self->r14 * self->r23 * self->r32 * self->r41 * self->r50)
	;

	return determinant;
}


HYPAPI struct matrix6 *matrix6_invert(struct matrix6 *self)
{
	struct matrix6 inverse;
	uint8_t i;

	if (matrix6_inverse(self, &inverse)) {
		for (i = 0; i < (36); i++) {
			self->m[i] = inverse.m[i];
		}
	}

	return self;
}


HYPAPI struct matrix6 *matrix6_inverse(const struct matrix6 *self, struct matrix6 *mR)
{
	struct matrix6 inverse;
	HYP_FLOAT determinant;
	uint8_t i;

	determinant = matrix6_determinant(self);

	/* calculated early for a quick exit if no determinant exists */
	if (scalar_equalsf(determinant, 0.0f)) {
		return NULL;
	}

	determinant = 1.0f / determinant;

	matrix6_identity(&inverse);

	B(11) = A3(22, 33, 44) + A3(23, 34, 42) + A3(24, 32, 43) - A3(22, 34, 43) - A3(23, 32, 44) - A3(24, 33, 42);
	B(12) = A3(12, 34, 43) + A3(13, 32, 44) + A3(14, 33, 42) - A3(12, 33, 44) - A3(13, 34, 42) - A3(14, 32, 43);
	B(13) = A3(12, 23, 44) + A3(13, 24, 42) + A3(14, 22, 43) - A3(12, 24, 43) - A3(13, 22, 44) - A3(14, 23, 42);
	B(14) = A3(12, 24, 33) + A3(13, 22, 34) + A3(14, 23, 32) - A3(12, 23, 34) - A3(13, 24, 32) - A3(14, 22, 33);

	B(21) = A3(21, 34, 43) + A3(23, 31, 44) + A3(24, 33, 41) - A3(21, 33, 44) - A3(23, 34, 41) - A3(24, 31, 43);
	B(22) = A3(11, 33, 44) + A3(13, 34, 41) + A3(14, 31, 43) - A3(11, 34, 43) - A3(13, 31, 44) - A3(14, 33, 41);
	B(23) = A3(11, 24, 43) + A3(13, 21, 44) + A3(14, 23, 41) - A3(11, 23, 44) - A3(13, 24, 41) - A3(14, 21, 43);
	B(24) = A3(11, 23, 34) + A3(13, 24, 31) + A3(14, 21, 33) - A3(11, 24, 33) - A3(13, 21, 34) - A3(14, 23, 31);

	B(31) = A3(21, 32, 44) + A3(22, 34, 41) + A3(24, 31, 42) - A3(21, 34, 42) - A3(22, 31, 44) - A3(24, 32, 41);
	B(32) = A3(11, 34, 42) + A3(12, 31, 44) + A3(14, 32, 41) - A3(11, 32, 44) - A3(12, 34, 41) - A3(14, 31, 42);
	B(33) = A3(11, 22, 44) + A3(12, 24, 41) + A3(14, 21, 42) - A3(11, 24, 42) - A3(12, 21, 44) - A3(14, 22, 41);
	B(34) = A3(11, 24, 32) + A3(12, 21, 34) + A3(14, 22, 31) - A3(11, 22, 34) - A3(12, 24, 31) - A3(14, 21, 32);

	B(41) = A3(21, 33, 42) + A3(22, 31, 43) + A3(23, 32, 41) - A3(21, 32, 43) - A3(22, 33, 41) - A3(23, 31, 42);
	B(42) = A3(11, 32, 43) + A3(12, 33, 41) + A3(13, 31, 42) - A3(11, 33, 42) - A3(12, 31, 43) - A3(13, 32, 41);
	B(43) = A3(11, 23, 42) + A3(12, 21, 43) + A3(13, 22, 41) - A3(11, 22, 43) - A3(12, 23, 41) - A3(13, 21, 42);
	B(44) = A3(11, 22, 33) + A3(12, 23, 31) + A3(13, 21, 32) - A3(11, 23, 32) - A3(12, 21, 33) - A3(13, 22, 31);

	/* divide the determinant */
	for (i = 0; i < (36); i++) {
		mR->m[i] = inverse.m[i] * determinant;
	}

	return mR;
}

#undef _HYP_CAT
#undef _HYP_PRIMITIVE_CAT
#undef _HYP_DEC
#undef DEC_11
#undef DEC_12
#undef DEC_13
#undef DEC_14
#undef DEC_21
#undef DEC_22
#undef DEC_23
#undef DEC_24
#undef DEC_31
#undef DEC_32
#undef DEC_33
#undef DEC_34
#undef DEC_41
#undef DEC_42
#undef DEC_43
#undef DEC_44
#undef A
#undef B
#undef A4
#undef A3
#undef A2
#undef A6

#endif /* HYPATIA_IMPLEMENTATION */

#endif /* _HYPATIA_MACROS_H_ */
