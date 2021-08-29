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
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10 + self->c02 * mT->c20 + self->c03 * mT->c30 + self->c03 * mT->c40 + self->c03 * mT->c50;
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10 + self->c12 * mT->c20 + self->c13 * mT->c30 + self->c13 * mT->c40 + self->c13 * mT->c50;
	r.r02 = self->c20 * mT->c00 + self->c21 * mT->c10 + self->c22 * mT->c20 + self->c23 * mT->c30 + self->c23 * mT->c40 + self->c23 * mT->c50;
	r.r03 = self->c30 * mT->c00 + self->c31 * mT->c10 + self->c32 * mT->c20 + self->c33 * mT->c30 + self->c33 * mT->c40 + self->c33 * mT->c50;
	r.r04 = self->c40 * mT->c00 + self->c41 * mT->c10 + self->c42 * mT->c20 + self->c43 * mT->c30 + self->c43 * mT->c40 + self->c43 * mT->c50;
	r.r05 = self->c50 * mT->c00 + self->c51 * mT->c10 + self->c52 * mT->c20 + self->c53 * mT->c30 + self->c53 * mT->c40 + self->c53 * mT->c50;

	/* second row */
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11 + self->c02 * mT->c21 + self->c03 * mT->c31 + self->c03 * mT->c41 + self->c03 * mT->c51;
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11 + self->c12 * mT->c21 + self->c13 * mT->c31 + self->c13 * mT->c41 + self->c13 * mT->c51;
	r.r12 = self->c20 * mT->c01 + self->c21 * mT->c11 + self->c22 * mT->c21 + self->c23 * mT->c31 + self->c23 * mT->c41 + self->c23 * mT->c51;
	r.r13 = self->c30 * mT->c01 + self->c31 * mT->c11 + self->c32 * mT->c21 + self->c33 * mT->c31 + self->c33 * mT->c41 + self->c33 * mT->c51;
	r.r14 = self->c40 * mT->c01 + self->c41 * mT->c11 + self->c42 * mT->c21 + self->c43 * mT->c31 + self->c43 * mT->c41 + self->c43 * mT->c51;
	r.r15 = self->c50 * mT->c01 + self->c51 * mT->c11 + self->c52 * mT->c21 + self->c53 * mT->c31 + self->c53 * mT->c41 + self->c53 * mT->c51;

	/* third row */
	r.r20 = self->c00 * mT->c02 + self->c01 * mT->c12 + self->c02 * mT->c22 + self->c03 * mT->c32 + self->c03 * mT->c42 + self->c03 * mT->c52;
	r.r21 = self->c10 * mT->c02 + self->c11 * mT->c12 + self->c12 * mT->c22 + self->c13 * mT->c32 + self->c13 * mT->c42 + self->c13 * mT->c52;
	r.r22 = self->c20 * mT->c02 + self->c21 * mT->c12 + self->c22 * mT->c22 + self->c23 * mT->c32 + self->c23 * mT->c42 + self->c23 * mT->c52;
	r.r23 = self->c30 * mT->c02 + self->c31 * mT->c12 + self->c32 * mT->c22 + self->c33 * mT->c32 + self->c33 * mT->c42 + self->c33 * mT->c52;
	r.r24 = self->c40 * mT->c02 + self->c41 * mT->c12 + self->c42 * mT->c22 + self->c43 * mT->c32 + self->c43 * mT->c42 + self->c43 * mT->c52;
	r.r25 = self->c50 * mT->c02 + self->c51 * mT->c12 + self->c52 * mT->c22 + self->c53 * mT->c32 + self->c53 * mT->c42 + self->c53 * mT->c52;

	/* fourth row */
	r.r30 = self->c00 * mT->c03 + self->c01 * mT->c13 + self->c02 * mT->c23 + self->c03 * mT->c33 + self->c03 * mT->c43 + self->c03 * mT->c53;
	r.r31 = self->c10 * mT->c03 + self->c11 * mT->c13 + self->c12 * mT->c23 + self->c13 * mT->c33 + self->c13 * mT->c43 + self->c13 * mT->c53;
	r.r32 = self->c20 * mT->c03 + self->c21 * mT->c13 + self->c22 * mT->c23 + self->c23 * mT->c33 + self->c23 * mT->c43 + self->c23 * mT->c53;
	r.r33 = self->c30 * mT->c03 + self->c31 * mT->c13 + self->c32 * mT->c23 + self->c33 * mT->c33 + self->c33 * mT->c43 + self->c33 * mT->c53;
	r.r34 = self->c40 * mT->c03 + self->c41 * mT->c13 + self->c42 * mT->c23 + self->c43 * mT->c33 + self->c43 * mT->c43 + self->c43 * mT->c53;
	r.r35 = self->c50 * mT->c03 + self->c51 * mT->c13 + self->c52 * mT->c23 + self->c53 * mT->c33 + self->c53 * mT->c43 + self->c53 * mT->c53;

	/* fifth row */
	r.r40 = self->c00 * mT->c04 + self->c01 * mT->c14 + self->c02 * mT->c24 + self->c03 * mT->c34 + self->c03 * mT->c44 + self->c03 * mT->c54;
	r.r41 = self->c10 * mT->c04 + self->c11 * mT->c14 + self->c12 * mT->c24 + self->c13 * mT->c34 + self->c13 * mT->c44 + self->c13 * mT->c54;
	r.r42 = self->c20 * mT->c04 + self->c21 * mT->c14 + self->c22 * mT->c24 + self->c23 * mT->c34 + self->c23 * mT->c44 + self->c23 * mT->c54;
	r.r43 = self->c30 * mT->c04 + self->c31 * mT->c14 + self->c32 * mT->c24 + self->c33 * mT->c34 + self->c33 * mT->c44 + self->c33 * mT->c54;
	r.r44 = self->c40 * mT->c04 + self->c41 * mT->c14 + self->c42 * mT->c24 + self->c43 * mT->c34 + self->c43 * mT->c44 + self->c43 * mT->c54;
	r.r45 = self->c50 * mT->c04 + self->c51 * mT->c14 + self->c52 * mT->c24 + self->c53 * mT->c34 + self->c53 * mT->c44 + self->c53 * mT->c54;

	/* sixth row */
	r.r50 = self->c00 * mT->c05 + self->c01 * mT->c15 + self->c02 * mT->c25 + self->c03 * mT->c35 + self->c03 * mT->c45 + self->c03 * mT->c55;
	r.r51 = self->c10 * mT->c05 + self->c11 * mT->c15 + self->c12 * mT->c25 + self->c13 * mT->c35 + self->c13 * mT->c45 + self->c13 * mT->c55;
	r.r52 = self->c20 * mT->c05 + self->c21 * mT->c15 + self->c22 * mT->c25 + self->c23 * mT->c35 + self->c23 * mT->c45 + self->c23 * mT->c55;
	r.r53 = self->c30 * mT->c05 + self->c31 * mT->c15 + self->c32 * mT->c25 + self->c33 * mT->c35 + self->c33 * mT->c45 + self->c33 * mT->c55;
	r.r54 = self->c40 * mT->c05 + self->c41 * mT->c15 + self->c42 * mT->c25 + self->c43 * mT->c35 + self->c43 * mT->c45 + self->c43 * mT->c55;
	r.r55 = self->c50 * mT->c05 + self->c51 * mT->c15 + self->c52 * mT->c25 + self->c53 * mT->c35 + self->c53 * mT->c45 + self->c53 * mT->c55;

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

#endif /* HYPATIA_IMPLEMENTATION */

#endif /* _HYPATIA_MACROS_H_ */
