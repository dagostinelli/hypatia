/* SPDX-License-Identifier: MIT */

#ifndef _HYPATIA_MACROS_H_
#define _HYPATIA_MACROS_H_

#include "hypatia.h"

#define MATRIXN(dimension) \
\
struct matrix ## dimension {\
	union {\
		HYP_FLOAT m[(dimension * dimension)]; /* row-major numbering */\
		struct {\
			/* reference the matrix [row][column] */\
			HYP_FLOAT m ## dimension ## dimension[dimension][dimension];\
		};\
		struct {\
			/* indexed (column-major numbering) */\
			HYP_FLOAT i00, i04, i08, i12;\
			HYP_FLOAT i01, i05, i09, i13;\
			HYP_FLOAT i02, i06, i10, i14;\
			HYP_FLOAT i03, i07, i11, i15;\
		};\
		struct {\
			/* col-row */\
			HYP_FLOAT c00, c10, c20, c30;\
			HYP_FLOAT c01, c11, c21, c31;\
			HYP_FLOAT c02, c12, c22, c32;\
			HYP_FLOAT c03, c13, c23, c33;\
		};\
		struct {\
			/* row-col */\
			HYP_FLOAT r00, r01, r02, r03;\
			HYP_FLOAT r10, r11, r12, r13;\
			HYP_FLOAT r20, r21, r22, r23;\
			HYP_FLOAT r30, r31, r32, r33;\
		};\
	};\
};\
\
HYPAPI int matrix ## dimension ## _equals(const struct matrix ## dimension *self, const struct matrix ## dimension *mT);\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _zero(struct matrix ## dimension *self);\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _identity(struct matrix ## dimension *self);\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _set(struct matrix ## dimension *self, const struct matrix ## dimension *mT);\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _add(struct matrix ## dimension *self, const struct matrix ## dimension *mT);\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _subtract(struct matrix ## dimension *self, const struct matrix ## dimension *mT);\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _multiply(struct matrix ## dimension *self, const struct matrix ## dimension *mT);\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _multiplyf(struct matrix ## dimension *self, HYP_FLOAT scalar);\
HYPAPI struct vector4 *matrix ## dimension ## _multiplyv4(const struct matrix ## dimension *self, const struct vector4 *vT, struct vector4 *vR);\
HYPAPI struct vector3 *matrix ## dimension ## _multiplyv3(const struct matrix ## dimension *self, const struct vector3 *vT, struct vector3 *vR);\
HYPAPI struct vector2 *matrix ## dimension ## _multiplyv2(const struct matrix ## dimension *self, const struct vector2 *vT, struct vector2 *vR);\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _transpose(struct matrix ## dimension *self);\
HYPAPI HYP_FLOAT matrix ## dimension ## _determinant(const struct matrix ## dimension *self);\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _invert(struct matrix ## dimension *self);\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _inverse(const struct matrix ## dimension *self, struct matrix ## dimension *mR);\
\
HYPAPI struct matrix ## dimension *_matrix ## dimension ## _transpose_rowcolumn(struct matrix ## dimension *self);\
HYPAPI struct matrix ## dimension *_matrix ## dimension ## _transpose_columnrow(struct matrix ## dimension *self);\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _zero(struct matrix ## dimension *self)\
{\
	HYP_MEMSET(self, 0, sizeof(struct matrix ## dimension));\
	return self;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _identity(struct matrix ## dimension *m)\
{\
	m->c00 = 1.0f, m->c10 = 0.0f, m->c20 = 0.0f, m->c30 = 0.0f;\
	m->c01 = 0.0f, m->c11 = 1.0f, m->c21 = 0.0f, m->c31 = 0.0f;\
	m->c02 = 0.0f, m->c12 = 0.0f, m->c22 = 1.0f, m->c32 = 0.0f;\
	m->c03 = 0.0f, m->c13 = 0.0f, m->c23 = 0.0f, m->c33 = 1.0f;\
\
	return m;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _set(struct matrix ## dimension *self, const struct matrix ## dimension *mT)\
{\
	uint8_t i;\
\
	for (i = 0; i < (dimension * dimension); i++) {\
		self->m[i] = mT->m[i];\
	}\
\
	return self;\
}\
\
\
HYPAPI int matrix ## dimension ## _equals(const struct matrix ## dimension *self, const struct matrix ## dimension *mT)\
{\
	uint8_t i;\
\
	for (i = 0; i < (dimension * dimension); i++) {\
		if (scalar_equalsf(self->m[i], mT->m[i]) == 0) {\
			return 0;\
		}\
	}\
\
	return 1;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _add(struct matrix ## dimension *self, const struct matrix ## dimension *mT)\
{\
	/* "add row and column to row and column" */\
	uint8_t i;\
\
	for (i = 0; i < (dimension * dimension); i++) {\
		self->m[i] += mT->m[i];\
	}\
\
	return self;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _subtract(struct matrix ## dimension *self, const struct matrix ## dimension *mT)\
{\
	/* "subtract row and column from row and column" */\
	uint8_t i;\
\
	for (i = 0; i < (dimension * dimension); i++) {\
		self->m[i] -= mT->m[i];\
	}\
\
	return self;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _multiplyf(struct matrix ## dimension *self, HYP_FLOAT scalar)\
{\
	uint8_t i;\
\
	for (i = 0; i < (dimension * dimension); i++) {\
		self->m[i] *= scalar;\
	}\
\
	return self;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _multiply(struct matrix ## dimension *self, const struct matrix ## dimension *mT)\
{\
	/* mT is the multiplicand */\
\
	struct matrix ## dimension r;\
\
	matrix ## dimension ## _identity(&r);\
\
	/* first row */\
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10 + self->c02 * mT->c20 + self->c03 * mT->c30;\
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10 + self->c12 * mT->c20 + self->c13 * mT->c30;\
	r.r02 = self->c20 * mT->c00 + self->c21 * mT->c10 + self->c22 * mT->c20 + self->c23 * mT->c30;\
	r.r03 = self->c30 * mT->c00 + self->c31 * mT->c10 + self->c32 * mT->c20 + self->c33 * mT->c30;\
\
	/* second row */\
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11 + self->c02 * mT->c21 + self->c03 * mT->c31;\
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11 + self->c12 * mT->c21 + self->c13 * mT->c31;\
	r.r12 = self->c20 * mT->c01 + self->c21 * mT->c11 + self->c22 * mT->c21 + self->c23 * mT->c31;\
	r.r13 = self->c30 * mT->c01 + self->c31 * mT->c11 + self->c32 * mT->c21 + self->c33 * mT->c31;\
\
	/* third row */\
	r.r20 = self->c00 * mT->c02 + self->c01 * mT->c12 + self->c02 * mT->c22 + self->c03 * mT->c32;\
	r.r21 = self->c10 * mT->c02 + self->c11 * mT->c12 + self->c12 * mT->c22 + self->c13 * mT->c32;\
	r.r22 = self->c20 * mT->c02 + self->c21 * mT->c12 + self->c22 * mT->c22 + self->c23 * mT->c32;\
	r.r23 = self->c30 * mT->c02 + self->c31 * mT->c12 + self->c32 * mT->c22 + self->c33 * mT->c32;\
\
	/* fourth row */\
	r.r30 = self->c00 * mT->c03 + self->c01 * mT->c13 + self->c02 * mT->c23 + self->c03 * mT->c33;\
	r.r31 = self->c10 * mT->c03 + self->c11 * mT->c13 + self->c12 * mT->c23 + self->c13 * mT->c33;\
	r.r32 = self->c20 * mT->c03 + self->c21 * mT->c13 + self->c22 * mT->c23 + self->c23 * mT->c33;\
	r.c33 = self->c30 * mT->c03 + self->c31 * mT->c13 + self->c32 * mT->c23 + self->c33 * mT->c33;\
\
	matrix ## dimension ## _set(self, &r); /* overwrite/save it */\
\
	return self;\
}\
\
\
HYPAPI struct vector4 *matrix ## dimension ## _multiplyv4(const struct matrix ## dimension *self, const struct vector4 *vT, struct vector4 *vR)\
{\
	vR->x = vT->x * self->r00 + vT->y * self->r01 + vT->z * self->r02 + vT->w * self->r03;\
	vR->y = vT->x * self->r10 + vT->y * self->r11 + vT->z * self->r12 + vT->w * self->r13;\
	vR->z = vT->x * self->r20 + vT->y * self->r21 + vT->z * self->r22 + vT->w * self->r23;\
	vR->w = vT->x * self->r30 + vT->y * self->r31 + vT->z * self->r32 + vT->w * self->r33;\
\
	return vR;\
}\
\
\
HYPAPI struct vector3 *matrix ## dimension ## _multiplyv3(const struct matrix ## dimension *self, const struct vector3 *vT, struct vector3 *vR)\
{\
	vR->x = vT->x * self->r00 + vT->y * self->r01 + vT->z * self->r02 + self->r03;\
	vR->y = vT->x * self->r10 + vT->y * self->r11 + vT->z * self->r12 + self->r13;\
	vR->z = vT->x * self->r20 + vT->y * self->r21 + vT->z * self->r22 + self->r23;\
\
	return vR;\
}\
\
\
HYPAPI struct vector2 *matrix ## dimension ## _multiplyv2(const struct matrix ## dimension *self, const struct vector2 *vT, struct vector2 *vR)\
{\
	vR->x = vT->x * self->r00 + vT->y * self->r01 + self->r02 + self->r03;\
	vR->y = vT->x * self->r10 + vT->y * self->r11 + self->r12 + self->r13;\
\
	return vR;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _transpose(struct matrix ## dimension *self)\
{\
	return _matrix ## dimension ## _transpose_columnrow(self);\
}\
\
\
HYPAPI struct matrix ## dimension *_matrix ## dimension ## _transpose_rowcolumn(struct matrix ## dimension *self)\
{\
	HYP_SWAP(&self->r01, &self->r10);\
	HYP_SWAP(&self->r02, &self->r20);\
	HYP_SWAP(&self->r03, &self->r30);\
	HYP_SWAP(&self->r12, &self->r21);\
	HYP_SWAP(&self->r13, &self->r31);\
	HYP_SWAP(&self->r23, &self->r32);\
\
	return self;\
}\
\
\
HYPAPI struct matrix ## dimension *_matrix ## dimension ## _transpose_columnrow(struct matrix ## dimension *self)\
{\
	HYP_SWAP(&self->c01, &self->c10);\
	HYP_SWAP(&self->c02, &self->c20);\
	HYP_SWAP(&self->c03, &self->c30);\
	HYP_SWAP(&self->c12, &self->c21);\
	HYP_SWAP(&self->c13, &self->c31);\
	HYP_SWAP(&self->c23, &self->c32);\
\
	return self;\
}\
\
\
HYPAPI HYP_FLOAT matrix ## dimension ## _determinant(const struct matrix ## dimension *self)\
{\
	HYP_FLOAT determinant;\
\
	determinant =\
	  A4(11, 22, 33, 44) + A4(11, 23, 34, 42) + A4(11, 24, 32, 43)\
	+ A4(12, 21, 34, 43) + A4(12, 23, 31, 44) + A4(12, 24, 33, 41)\
	+ A4(13, 21, 32, 44) + A4(13, 22, 34, 41) + A4(13, 24, 31, 42)\
	+ A4(14, 21, 33, 42) + A4(14, 22, 31, 43) + A4(14, 23, 32, 41)\
	- A4(11, 22, 34, 43) - A4(11, 23, 32, 44) - A4(11, 24, 33, 42)\
	- A4(12, 21, 33, 44) - A4(12, 23, 34, 41) - A4(12, 24, 31, 43)\
	- A4(13, 21, 34, 42) - A4(13, 22, 31, 44) - A4(13, 24, 32, 41)\
	- A4(14, 21, 32, 43) - A4(14, 22, 33, 41) - A4(14, 23, 31, 42)\
	;\
\
	return determinant;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _invert(struct matrix ## dimension *self)\
{\
	struct matrix ## dimension inverse;\
	uint8_t i;\
\
	if (matrix ## dimension ## _inverse(self, &inverse)) {\
		for (i = 0; i < (dimension * dimension); i++) {\
			self->m[i] = inverse.m[i];\
		}\
	}\
\
	return self;\
}\
\
\
HYPAPI struct matrix ## dimension *matrix ## dimension ## _inverse(const struct matrix ## dimension *self, struct matrix ## dimension *mR)\
{\
	struct matrix ## dimension inverse;\
	HYP_FLOAT determinant;\
	uint8_t i;\
\
	determinant = matrix ## dimension ## _determinant(self);\
\
	/* calculated early for a quick exit if no determinant exists */\
	if (scalar_equalsf(determinant, 0.0f)) {\
		return NULL;\
	}\
\
	determinant = 1.0f / determinant;\
\
	matrix ## dimension ## _identity(&inverse);\
\
	B(11) = A3(22, 33, 44) + A3(23, 34, 42) + A3(24, 32, 43) - A3(22, 34, 43) - A3(23, 32, 44) - A3(24, 33, 42);\
	B(12) = A3(12, 34, 43) + A3(13, 32, 44) + A3(14, 33, 42) - A3(12, 33, 44) - A3(13, 34, 42) - A3(14, 32, 43);\
	B(13) = A3(12, 23, 44) + A3(13, 24, 42) + A3(14, 22, 43) - A3(12, 24, 43) - A3(13, 22, 44) - A3(14, 23, 42);\
	B(14) = A3(12, 24, 33) + A3(13, 22, 34) + A3(14, 23, 32) - A3(12, 23, 34) - A3(13, 24, 32) - A3(14, 22, 33);\
	B(21) = A3(21, 34, 43) + A3(23, 31, 44) + A3(24, 33, 41) - A3(21, 33, 44) - A3(23, 34, 41) - A3(24, 31, 43);\
	B(22) = A3(11, 33, 44) + A3(13, 34, 41) + A3(14, 31, 43) - A3(11, 34, 43) - A3(13, 31, 44) - A3(14, 33, 41);\
	B(23) = A3(11, 24, 43) + A3(13, 21, 44) + A3(14, 23, 41) - A3(11, 23, 44) - A3(13, 24, 41) - A3(14, 21, 43);\
	B(24) = A3(11, 23, 34) + A3(13, 24, 31) + A3(14, 21, 33) - A3(11, 24, 33) - A3(13, 21, 34) - A3(14, 23, 31);\
	B(31) = A3(21, 32, 44) + A3(22, 34, 41) + A3(24, 31, 42) - A3(21, 34, 42) - A3(22, 31, 44) - A3(24, 32, 41);\
	B(32) = A3(11, 34, 42) + A3(12, 31, 44) + A3(14, 32, 41) - A3(11, 32, 44) - A3(12, 34, 41) - A3(14, 31, 42);\
	B(33) = A3(11, 22, 44) + A3(12, 24, 41) + A3(14, 21, 42) - A3(11, 24, 42) - A3(12, 21, 44) - A3(14, 22, 41);\
	B(34) = A3(11, 24, 32) + A3(12, 21, 34) + A3(14, 22, 31) - A3(11, 22, 34) - A3(12, 24, 31) - A3(14, 21, 32);\
	B(41) = A3(21, 33, 42) + A3(22, 31, 43) + A3(23, 32, 41) - A3(21, 32, 43) - A3(22, 33, 41) - A3(23, 31, 42);\
	B(42) = A3(11, 32, 43) + A3(12, 33, 41) + A3(13, 31, 42) - A3(11, 33, 42) - A3(12, 31, 43) - A3(13, 32, 41);\
	B(43) = A3(11, 23, 42) + A3(12, 21, 43) + A3(13, 22, 41) - A3(11, 22, 43) - A3(12, 23, 41) - A3(13, 21, 42);\
	B(44) = A3(11, 22, 33) + A3(12, 23, 31) + A3(13, 21, 32) - A3(11, 23, 32) - A3(12, 21, 33) - A3(13, 22, 31);\
\
	/* divide the determinant */\
	for (i = 0; i < (dimension * dimension); i++) {\
		mR->m[i] = inverse.m[i] * determinant;\
	}\
\
	return mR;\
}\

#endif /* _HYPATIA_MACROS_H_ */
