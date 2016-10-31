#include "hypatia.h"
#include "internal.h"


/**
 * @file vector4.c
 */


static struct vector4 _vector4_zero = { { {0.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_one = { { {1.0f, 1.0f, 1.0f, 1.0f} } };
static struct vector4 _vector4_unit_x = { { {1.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_y = { { {0.0f, 1.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_z = { { {0.0f, 0.0f, 1.0f, 0.0f} } };
static struct vector4 _vector4_unit_x_negative = { { {-1.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_y_negative = { { {0.0f, -1.0f,  0.0f, 0.0f} } };
static struct vector4 _vector4_unit_z_negative = { { {0.0f,  0.0f, -1.0f, 0.0f} } };


HYPAPI const struct vector4 *vector4_get_reference_vector4(int id)
{
	switch (id)
	{
		case HYP_REF_VECTOR4_ZERO:
			return &_vector4_zero;
		case HYP_REF_VECTOR4_ONE:
			return &_vector4_one;
		case HYP_REF_VECTOR4_UNIT_X:
			return &_vector4_unit_x;
		case HYP_REF_VECTOR4_UNIT_Y:
			return &_vector4_unit_y;
		case HYP_REF_VECTOR4_UNIT_Z:
			return &_vector4_unit_z;
		case HYP_REF_VECTOR4_UNIT_X_NEGATIVE:
			return &_vector4_unit_x_negative;
		case HYP_REF_VECTOR4_UNIT_Y_NEGATIVE:
			return &_vector4_unit_y_negative;
		case HYP_REF_VECTOR4_UNIT_Z_NEGATIVE:
			return &_vector4_unit_z_negative;
		default:
			/* undefined case */
			return &_vector4_zero;
	}
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with specific values
 */
HYPAPI struct vector4 *vector4_setf4(struct vector4 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT, HYP_FLOAT wT)
{
	self->x = xT;
	self->y = yT;
	self->z = zT;
	self->w = wT;
	return self;
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with values from another vector
 */
HYPAPI struct vector4 *vector4_set(struct vector4 *self, const struct vector4 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	self->z = vT->z;
	return self;
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with zeros
 */
HYPAPI struct vector4 *vector4_zero(struct vector4 *self)
{
	return vector4_setf4(self, 0.0f, 0.0f, 0.0f, 0.0f);
}


/**
 * @ingroup vector4
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */
HYPAPI int vector4_equals(const struct vector4 *self, const struct vector4 *vT)
{
	return  HYP_ABS(self->x - vT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - vT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - vT->z) < HYP_EPSILON &&
		HYP_ABS(self->w - vT->w) < HYP_EPSILON;
}


/**
 * @ingroup vector4
 * @brief switches the sign on each component of the vector
 */
HYPAPI struct vector4 *vector4_negate(struct vector4 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	self->v[2] = -self->v[2];
	self->v[3] = -self->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief adds vectors using component-wise addition
 */
HYPAPI struct vector4 *vector4_add(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	self->v[2] += vT->v[2];
	self->v[3] += vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief add to each component of the vector using a scalar
 */
HYPAPI struct vector4 *vector4_addf(struct vector4 *self, HYP_FLOAT f)
{
	self->v[0] += f;
	self->v[1] += f;
	self->v[2] += f;
	self->v[3] += f;
	return self;
}


/**
 * @ingroup vector4
 * @brief subtract two vectors using component-wise subtraction
 */
HYPAPI struct vector4 *vector4_subtract(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	self->v[2] -= vT->v[2];
	self->v[3] -= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief subtract each vector's component by a scalar
 */
HYPAPI struct vector4 *vector4_subtractf(struct vector4 *self, HYP_FLOAT f)
{
	self->x -= f;
	self->y -= f;
	self->z -= f;
	self->w -= f;
	return self;
}


/**
 * @ingroup vector4
 * @brief multiplies two vectors using component-wise multiplication
 */
HYPAPI struct vector4 *vector4_multiply(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	self->v[2] *= vT->v[2];
	self->v[3] *= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief multiplies each component of the vector by a scalar
 */
HYPAPI struct vector4 *vector4_multiplyf(struct vector4 *self, HYP_FLOAT f)
{
	self->v[0] *= f;
	self->v[1] *= f;
	self->v[2] *= f;
	self->v[3] *= f;
	return self;
}


/**
 * @ingroup vector4
 * @brief divides one vector into another using component-wise division
 *
 */
HYPAPI struct vector4 *vector4_divide(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	self->v[2] /= vT->v[2];
	self->v[3] /= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief calculates the magnitude of the vector
 */
HYPAPI HYP_FLOAT vector4_magnitude(const struct vector4 *self)
{
	return HYP_SQRT((self->x*self->x) + (self->y*self->y) + (self->z*self->z) + (self->w*self->w));
}


/**
 * @ingroup vector4
 * @brief normalizes the vector by dividing each component by the magnitude
 */
HYPAPI struct vector4 *vector4_normalize(struct vector4 *self)
{
	HYP_FLOAT mag;

	mag = vector4_magnitude(self);

	if (scalar_equalsf(mag, 0.0))
	{
		/* can't normalize a zero
		 * avoid divide by zero
		 */
		return self;
	}

	self->x = self->x / mag;
	self->y = self->y / mag;
	self->z = self->z / mag;
	self->w = self->w / mag;

	return self;
}


/**
 * @ingroup vector4
 * @brief computes the dot product of two vectors
 */
HYPAPI HYP_FLOAT vector4_dot_product(const struct vector4 *self, const struct vector4 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z) + (self->w * vT->w);
}


/**
 * @ingroup vector4
 * @brief computes the cross-product between two vectors
 */
HYPAPI struct vector4 *vector4_cross_product(struct vector4 *vR, const struct vector4 *vT1, const struct vector4 *vT2)
{
	vR->x = (vT1->y * vT2->z) - (vT1->z * vT2->y);
	vR->y = (vT1->z * vT2->x) - (vT1->x * vT2->z);
	vR->z = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	vR->w = (vT1->w * vT2->w) - (vT1->w * vT2->w);
	return vR;
}


/**
 * @brief Calculates the distance between two points
 *
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2+(z_2-z_1)^2}\f$
 *
 * https://en.wikipedia.org/wiki/Distance
 */
HYPAPI HYP_FLOAT vector4_distance(const struct vector4 *v1, const struct vector4 *v2)
{
	return HYP_SQRT((v2->x - v1->x) * (v2->x - v1->x)
		    + (v2->y - v1->y) * (v2->y - v1->y)
		    + (v2->z - v1->z) * (v2->z - v1->z)
		    + (v2->w - v1->w) * (v2->w - v1->w));
}


HYPAPI void _vector4_print(const struct vector4 *self)
{
	printf("x:%10f, y:%10f, z:%10f, w:%10f\r\n", self->x, self->y, self->z, self->w);
}


/**
 * @ingroup vector4
 * @brief Randomly fills the vector with values. Good for testing.
 *
 */
HYPAPI struct vector4 *_vector4_set_random(struct vector4 *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	self->z = HYP_RANDOM_FLOAT;
	self->w = HYP_RANDOM_FLOAT;
	return self;
}
