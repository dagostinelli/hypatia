#include "hypatia.h"
#include "internal.h"


/**
 * @file matrix4.c
 */


/**
 * @ingroup matrix4
 * @brief Initializes the matrix with 0.0 in every element.
 */
HYPAPI struct matrix4 *matrix4_zero(struct matrix4 *self)
{
	memset(self, 0, sizeof(struct matrix4));
	return self;
}


/**
 * @ingroup matrix4
 * @brief Initializes the matrix as an identity matrix.
 */
HYPAPI struct matrix4 *matrix4_identity(struct matrix4 *m)
{
	m->c00 = 1.0f, m->c10 = 0.0f, m->c20 = 0.0f, m->c30 = 0.0f;
	m->c01 = 0.0f, m->c11 = 1.0f, m->c21 = 0.0f, m->c31 = 0.0f;
	m->c02 = 0.0f, m->c12 = 0.0f, m->c22 = 1.0f, m->c32 = 0.0f;
	m->c03 = 0.0f, m->c13 = 0.0f, m->c23 = 0.0f, m->c33 = 1.0f;

	return m;
}


/**
 * @ingroup matrix4
 * @brief Initializes the matrix by copying mT
 *
 * @param self The matrix to initialize
 * @param mT The matrix to copy
 */
HYPAPI struct matrix4 *matrix4_set(struct matrix4 *self, const struct matrix4 *mT)
{
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		self->m[i] = mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Compares every element of the matrix.  Uses HYP_EPSILON for precision.
 * returns 1 if equal, 0 if different
 */
HYPAPI int matrix4_equals(const struct matrix4 *self, const struct matrix4 *mT)
{
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		if (scalar_equalsf(self->m[i], mT->m[i]) == 0)
		{
			return 0;
		}
	}

	return 1;
}


/**
 * @ingroup matrix4
 * @brief "add row and column to row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to add
 */
HYPAPI struct matrix4 *matrix4_add(struct matrix4 *self, const struct matrix4 *mT)
{
	/* "add row and column to row and column" */
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		self->m[i] += mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief "subtract row and column from row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to subtract from self (self = self - mT)
 */
HYPAPI struct matrix4 *matrix4_subtract(struct matrix4 *self, const struct matrix4 *mT)
{
	/* "subtract row and column from row and column" */
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		self->m[i] -= mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Multiply a matrix by a scalar, returns the matrix changed
 *
 * @param self The matrix being changed
 * @param scalar The scalar factor being multiplied in
 */
HYPAPI struct matrix4 *matrix4_multiplyf(struct matrix4 *self, HYP_FLOAT scalar)
{
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		self->m[i] *= scalar;
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Multiply a matrix by a matrix, returns the matrix changed
 *
 * @param self the matrix being changed
 * @param mT The matrix being multiplied into self
 *
 * self = self * mT
 */
HYPAPI struct matrix4 *matrix4_multiply(struct matrix4 *self, const struct matrix4 *mT)
{
	/* mT is the multiplicand */

	struct matrix4 r;

	matrix4_identity(&r);

	/* first row */
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10 + self->c02 * mT->c20 + self->c03 * mT->c30;
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10 + self->c12 * mT->c20 + self->c13 * mT->c30;
	r.r02 = self->c20 * mT->c00 + self->c21 * mT->c10 + self->c22 * mT->c20 + self->c23 * mT->c30;
	r.r03 = self->c30 * mT->c00 + self->c31 * mT->c10 + self->c32 * mT->c20 + self->c33 * mT->c30;

	/* second row */
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11 + self->c02 * mT->c21 + self->c03 * mT->c31;
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11 + self->c12 * mT->c21 + self->c13 * mT->c31;
	r.r12 = self->c20 * mT->c01 + self->c21 * mT->c11 + self->c22 * mT->c21 + self->c23 * mT->c31;
	r.r13 = self->c30 * mT->c01 + self->c31 * mT->c11 + self->c32 * mT->c21 + self->c33 * mT->c31;

	/* third row */
	r.r20 = self->c00 * mT->c02 + self->c01 * mT->c12 + self->c02 * mT->c22 + self->c03 * mT->c32;
	r.r21 = self->c10 * mT->c02 + self->c11 * mT->c12 + self->c12 * mT->c22 + self->c13 * mT->c32;
	r.r22 = self->c20 * mT->c02 + self->c21 * mT->c12 + self->c22 * mT->c22 + self->c23 * mT->c32;
	r.r23 = self->c30 * mT->c02 + self->c31 * mT->c12 + self->c32 * mT->c22 + self->c33 * mT->c32;

	/* fourth row */
	r.r30 = self->c00 * mT->c03 + self->c01 * mT->c13 + self->c02 * mT->c23 + self->c03 * mT->c33;
	r.r31 = self->c10 * mT->c03 + self->c11 * mT->c13 + self->c12 * mT->c23 + self->c13 * mT->c33;
	r.r32 = self->c20 * mT->c03 + self->c21 * mT->c13 + self->c22 * mT->c23 + self->c23 * mT->c33;
	r.c33 = self->c30 * mT->c03 + self->c31 * mT->c13 + self->c32 * mT->c23 + self->c33 * mT->c33;

	matrix4_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup matrix4
 * @brief Transpose the matrix
 *
 * @param self The matrix being changed
 */
HYPAPI struct matrix4 *matrix4_transpose(struct matrix4 *self)
{
	return _matrix4_transpose_columnrow(self);
}


/**
 * @ingroup matrix4
 * @brief Swaps the row and column
 *
 */
HYPAPI struct matrix4 *_matrix4_transpose_rowcolumn(struct matrix4 *self)
{
	HYP_FLOAT tmp;

	_SWAP(self->r01, self->r10);
	_SWAP(self->r02, self->r20);
	_SWAP(self->r03, self->r30);
	_SWAP(self->r12, self->r21);
	_SWAP(self->r13, self->r31);
	_SWAP(self->r23, self->r32);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Swaps the columns and row
 *
 */
HYPAPI struct matrix4 *_matrix4_transpose_columnrow(struct matrix4 *self)
{
	HYP_FLOAT tmp;

	_SWAP(self->c01, self->c10);
	_SWAP(self->c02, self->c20);
	_SWAP(self->c03, self->c30);
	_SWAP(self->c12, self->c21);
	_SWAP(self->c13, self->c31);
	_SWAP(self->c23, self->c32);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Prints out the matrix using column and row notation
 *
 */
HYPAPI void _matrix4_print_with_columnrow_indexer(struct matrix4 *self)
{
	printf("%10f, %10f, %10f, %10f\r\n", self->c00, self->c10, self->c20, self->c30);
	printf("%10f, %10f, %10f, %10f\r\n", self->c01, self->c11, self->c21, self->c31);
	printf("%10f, %10f, %10f, %10f\r\n", self->c02, self->c12, self->c22, self->c32);
	printf("%10f, %10f, %10f, %10f\r\n", self->c03, self->c13, self->c23, self->c33);
}


/**
 * @ingroup matrix4
 * @brief Prints out the matrix using row and column notation
 *
 */
HYPAPI void _matrix4_print_with_rowcolumn_indexer(struct matrix4 *self)
{
	printf("%10f, %10f, %10f, %10f\r\n", self->r00, self->r01, self->r02, self->r03);
	printf("%10f, %10f, %10f, %10f\r\n", self->r10, self->r11, self->r12, self->r13);
	printf("%10f, %10f, %10f, %10f\r\n", self->r20, self->r21, self->r22, self->r23);
	printf("%10f, %10f, %10f, %10f\r\n", self->r30, self->r31, self->r32, self->r33);
}


/**
 * @ingroup matrix4
 * @brief Randomly fills the matrix with values. Good for testing.
 *
 */
HYPAPI struct matrix4 *_matrix4_set_random(struct matrix4 *self)
{
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		self->m[i] = HYP_RANDOM_FLOAT;
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief converts the quaternion to a 4x4 rotation matrix (column major,
 * right hand rule)
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationq(struct matrix4 *self, const struct quaternion *qT)
{
	struct matrix4 *m;
	const struct quaternion *q;

	q = qT;
	m = self;

	matrix4_identity(m);

	m->m[0] = 1.0f - 2.0f * (q->y * q->y + q->z * q->z);
	m->m[4] = 2.0f * (q->x * q->y - q->z * q->w);
	m->m[8] = 2.0f * (q->x * q->z + q->y * q->w);
	m->m[1] = 2.0f * (q->x * q->y + q->z * q->w);
	m->m[5] = 1.0f - 2.0f * (q->x * q->x + q->z * q->z);
	m->m[9] = 2.0f * (q->y * q->z - q->x * q->w);
	m->m[2] = 2.0f * (q->x * q->z - q->y * q->w);
	m->m[6] = 2.0f * (q->y * q->z + q->x * q->w);
	m->m[10] = 1.0f - 2.0f * (q->x * q->x + q->y * q->y);

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a translation matrix.  It's opinionated about what that means.
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_translationv3(struct matrix4 *self, const struct vector3 * translation)
{
	matrix4_identity(self);

	self->c30 = translation->x;
	self->c31 = translation->y;
	self->c32 = translation->z;

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a scaling matrix.  It's opinionated about what that means.
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_scalingv3(struct matrix4 *self, const struct vector3 *scale)
{
	matrix4_identity(self);

	self->c00 = scale->x;
	self->c11 = scale->y;
	self->c22 = scale->z;

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a rotation matrix about the x.  It's opinionated about
 * what that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_x(struct matrix4 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix4_identity(m);

	m->r11 = c;
	m->r12 = s;
	m->r21 = -s;
	m->r22 = c;

	return m;
}


/**
 * @ingroup matrix4
 * @brief creates a rotation matrix about the y.  It's opinionated about
 * what that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_y(struct matrix4 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix4_identity(m);

	/* assuming col-major */
	m->r00 = c;
	m->r02 = -s;
	m->r20 = s;
	m->r22 = c;

	return m;
}


/**
 * @ingroup matrix4
 * @brief creates a rotation matrix about the z.  It's opinionated about
 * what that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_z(struct matrix4 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix4_identity(m);

	/* assuming col-major */
	m->r00 = c;
	m->r01 = s;
	m->r10 = -s;
	m->r11 = c;

	return m;
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary translation matrix and then multiplies self
 * by that.  Opinionated function about what translation means.
 *
 * @param self The transformation matrix being translated
 * @param translation the translation vector
 *
 */
HYPAPI struct matrix4 *matrix4_translatev3(struct matrix4 *self, const struct vector3 * translation)
{
	struct matrix4 translationMatrix;

	return matrix4_multiply(self,
		matrix4_make_transformation_translationv3(&translationMatrix, translation));
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary rotation matrix and then multiplies self by that.
 * Opinionated function about what rotation means.
 *
 * @param self The transformation matrix being rotated
 * @param axis the axis to rotate the matrix around
 * @param angle the angle of rotation in radians
 *
 */
HYPAPI struct matrix4 *matrix4_rotatev3(struct matrix4 *self, const struct vector3 * axis, HYP_FLOAT angle)
{
	struct matrix4 rotationMatrix;
	struct quaternion q;

	return matrix4_multiply(self,
				matrix4_make_transformation_rotationq(&rotationMatrix,
								      quaternion_set_from_axis_anglev3(&q, axis, angle)));
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary scaling matrix and then multiplies self by that.
 * Opinionated function about what scaling means.
 *
 * @param self The transformation matrix being scaled
 * @param scale the scaling vector
 *
 */
HYPAPI struct matrix4 *matrix4_scalev3(struct matrix4 *self, const struct vector3 *scale)
{
	struct matrix4 scalingMatrix;

	return matrix4_multiply(self,
		matrix4_make_transformation_scalingv3(&scalingMatrix, scale));
}
