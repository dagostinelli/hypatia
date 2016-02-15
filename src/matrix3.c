#include "hypatia.h"
#include "internal.h"


/**
 * @file matrix3.c
 */


/**
 * @ingroup matrix3
 * @brief Initializes the matrix with 0.0 in every element.
 */
HYPAPI matrix3 * matrix3_zero(matrix3 *self)
{
	memset(self, 0, sizeof(matrix3));
	return self;
}


/**
 * @ingroup matrix3
 * @brief Initializes the matrix as an identity matrix.
 */
HYPAPI matrix3 * matrix3_identity(matrix3 *m)
{
	m->c00 = 1.0f, m->c10 = 0.0f, m->c20 = 0.0f;
	m->c01 = 0.0f, m->c11 = 1.0f, m->c21 = 0.0f;
	m->c02 = 0.0f, m->c12 = 0.0f, m->c22 = 1.0f;
	
	return m;
}


/**
 * @ingroup matrix3
 * @brief Initializes the matrix by copying mT
 *
 * @param self The matrix to initialize
 * @param mT The matrix to copy
 */
HYPAPI matrix3 * matrix3_set(matrix3 *self, const matrix3 *mT)
{
	unsigned char i;
	
	for(i = 0; i < 9; i++)
	{
		self->m[i] = mT->m[i];
	}
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief Compares every element of the matrix.  Uses HYP_EPSILON for precision.
 * returns 1 if equal, 0 if different
 */
HYPAPI int matrix3_equals(const matrix3 *self, const matrix3 *mT)
{
	unsigned char i;
	
	for(i = 0; i < 9; i++)
	{
		if(scalar_equalsf(self->m[i], mT->m[i]) == 0)
		{
			return 0;
		}
	}
	
	return 1;
}


/**
 * @ingroup matrix3
 * @brief "add row and column to row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to add
 */
HYPAPI matrix3 * matrix3_add(matrix3 *self, const matrix3 *mT)
{
	/* "add row and column to row and column" */
	unsigned char i;
	
	for(i = 0; i < 9; i++)
	{
		self->m[i] += mT->m[i];
	}
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief "subtract row and column from row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to subtract from self (self = self - mT)
 */
HYPAPI matrix3 * matrix3_subtract(matrix3 *self, const matrix3 *mT)
{
	/* "subtract row and column from row and column" */
	unsigned char i;
	
	for(i = 0; i < 9; i++)
	{
		self->m[i] -= mT->m[i];
	}
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief Multiply a matrix by a scalar, returns the matrix changed
 *
 * @param self The matrix being changed
 * @param scalar The scalar factor being multiplied in
 */
HYPAPI matrix3 * matrix3_multiplyf(matrix3 *self, float scalar)
{
	unsigned char i;
	
	for(i = 0; i < 9; i++)
	{
		self->m[i] *= scalar;
	}
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief Multiply a matrix by a matrix, returns the matrix changed
 *
 * @param self the matrix being changed
 * @param mT The matrix being multiplied into self
 * 
 * self = self * mT
 */
HYPAPI matrix3 * matrix3_multiply(matrix3 *self, const matrix3 *mT)
{
	/* mT is the multiplicand */
	
	matrix3 r;
	matrix3_identity(&r);
	
	/* first row */
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10 + self->c02 * mT->c20;
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10 + self->c12 * mT->c20;
	r.r02 = self->c20 * mT->c00 + self->c21 * mT->c10 + self->c22 * mT->c20;
	
	/* second row */
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11 + self->c02 * mT->c21;
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11 + self->c12 * mT->c21;
	r.r12 = self->c20 * mT->c01 + self->c21 * mT->c11 + self->c22 * mT->c21;
	
	/* third row */
	r.r20 = self->c00 * mT->c02 + self->c01 * mT->c12 + self->c02 * mT->c22;
	r.r21 = self->c10 * mT->c02 + self->c11 * mT->c12 + self->c12 * mT->c22;
	r.r22 = self->c20 * mT->c02 + self->c21 * mT->c12 + self->c22 * mT->c22;
		
	matrix3_set(self, &r); /* overwrite/save it */
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief Transpose the matrix
 *
 * @param self The matrix being changed
 */
HYPAPI matrix3 * matrix3_transpose(matrix3 *self)
{
	return _matrix3_transpose_columnrow(self);
}


/**
 * @ingroup matrix3 
 * @brief Swaps the row and column
 *
 */
HYPAPI matrix3 * _matrix3_transpose_rowcolumn(matrix3 *self)
{
	float tmp;
	_SWAP(self->r01, self->r10);
	_SWAP(self->r02, self->r20);
	_SWAP(self->r12, self->r21);
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief Swaps the columns and row
 *
 */
HYPAPI matrix3 * _matrix3_transpose_columnrow(matrix3 *self)
{
	float tmp;
	_SWAP(self->c01, self->c10);
	_SWAP(self->c02, self->c20);
	_SWAP(self->c12, self->c21);
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief Prints out the matrix using column and row notation
 *
 */
HYPAPI void _matrix3_print_with_columnrow_indexer(matrix3 *self)
{
	printf("%10f, %10f, %10f\r\n", self->c00, self->c10, self->c20);
	printf("%10f, %10f, %10f\r\n", self->c01, self->c11, self->c21);
	printf("%10f, %10f, %10f\r\n", self->c02, self->c12, self->c22);
}


/**
 * @ingroup matrix3
 * @brief Prints out the matrix using row and column notation
 *
 */
HYPAPI void _matrix3_print_with_rowcolumn_indexer(matrix3 *self)
{
	printf("%10f, %10f, %10f\r\n", self->r00, self->r01, self->r02);
	printf("%10f, %10f, %10f\r\n", self->r10, self->r11, self->r12);
	printf("%10f, %10f, %10f\r\n", self->r20, self->r21, self->r22);
}


/**
 * @ingroup matrix3
 * @brief Randomly fills the matrix with values. Good for testing.
 *
 */
HYPAPI matrix3 * _matrix3_set_random(matrix3 *self)
{
	unsigned char i;
	for(i = 0; i < 9; i++)
	{
		self->m[i] = HYP_RANDOM_FLOAT;
	}
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief converts the quaternion to a 4x4 rotation matrix (column major, right hand rule)
 *
 */
HYPAPI matrix3 * matrix3_make_transformation_rotationq(matrix3 *self, const quaternion *qT)
{
	matrix3 *m;
	const quaternion *q;
	
	q = qT;
	m = self;
	
	matrix3_identity(m);
	
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
 * @ingroup matrix3
 * @brief creates a translation matrix.  It's opinionated about what that means.
 *
 */
HYPAPI matrix3 * matrix3_make_transformation_translationv2(matrix3 *self, const vector2 *translation)
{
	matrix3_identity(self);
	
	self->r02 = translation->x;
	self->r12 = translation->y;
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief creates a scaling matrix.  It's opinionated about what that means.
 *
 */
HYPAPI matrix3 * matrix3_make_transformation_scalingv2(matrix3 *self, const vector2 *scale)
{
	matrix3_identity(self);
	
	self->r00 = scale->x;
	self->r11 = scale->y;
	
	return self;
}


/**
 * @ingroup matrix3
 * @brief creates a rotation matrix about the z.  It's opinionated about what that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI matrix3 * matrix3_make_transformation_rotationf_z(matrix3 *m, float angle)
{
	float c = HYP_COS(angle);
	float s = HYP_SIN(angle);
	
	matrix3_identity(m);
	
	m->r00 = c;
	m->r01 = s;
	m->r10 = -s;
	m->r12 = c;
	
	return m;
}


/**
 * @ingroup matrix3
 * @brief Creates a temporary translation matrix and then multiplies self by that.  Opinionated function about what translation means.
 *
 * @param self The transformation matrix being translated
 * @param translation the translation vector
 *
 */
HYPAPI matrix3 * matrix3_translatev2(matrix3 *self, const vector2 * translation)
{
	matrix3 translationMatrix;
	
	return matrix3_multiply(self, 
		matrix3_make_transformation_translationv2(&translationMatrix, translation));
}


/**
 * @ingroup matrix3
 * @brief Creates a temporary rotation matrix and then multiplies self by that.  Opinionated function about what rotation means.  It always rotates about the z which it assumes is coming out of the screen.
 *
 * @param self The transformation matrix being rotated
 * @param angle the angle of rotation in radians
 *
 */
HYPAPI matrix3 * matrix3_rotate(matrix3 *self, float angle)
{
	matrix3 rotationMatrix;
	quaternion q;
	
	return matrix3_multiply(self, 
		matrix3_make_transformation_rotationq(&rotationMatrix, 
			quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, angle)));
}


/**
 * @ingroup matrix3
 * @brief Creates a temporary rotation matrix and then multiplies self by that.  Opinionated function about what rotation means.
 *
 * @param self The transformation matrix being rotated
 * @param axis the axis to rotate the matrix around
 * @param angle the angle of rotation in radians
 *
 */
HYPAPI matrix3 * matrix3_rotatev3(matrix3 *self, const vector3 *axis, float angle)
{
	matrix3 rotationMatrix;
	quaternion q;
	
	return matrix3_multiply(self, 
		matrix3_make_transformation_rotationq(&rotationMatrix, 
			quaternion_set_from_axis_anglev3(&q, axis, angle)));
}


/**
 * @ingroup matrix3
 * @brief Creates a temporary scaling matrix and then multiplies self by that.  Opinionated function about what scaling means.
 *
 * @param self The transformation matrix being scaled
 * @param scale the scaling vector
 *
 */
HYPAPI matrix3 * matrix3_scalev2(matrix3 *self, const vector2 *scale)
{
	matrix3 scalingMatrix;
	
	return matrix3_multiply(self, 
		matrix3_make_transformation_scalingv2(&scalingMatrix, scale));
}
