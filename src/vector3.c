#include "hypatia.h"
#include "internal.h"


/**
 * @file vector3.c
 */


static struct vector3 _vector3_zero = { { {0.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_one = { { {1.0f, 1.0f, 1.0f} } };
static struct vector3 _vector3_unit_x = { { {1.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_unit_y = { { {0.0f, 1.0f, 0.0f} } };
static struct vector3 _vector3_unit_z = { { {0.0f, 0.0f, 1.0f} } };
static struct vector3 _vector3_unit_x_negative = { { {-1.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_unit_y_negative = { { {0.0f, -1.0f,  0.0f} } };
static struct vector3 _vector3_unit_z_negative = { { {0.0f,  0.0f, -1.0f} } };


HYPAPI const struct vector3 *vector3_get_reference_vector3(int id)
{
	switch (id)
	{
		case HYP_REF_VECTOR3_ZERO:
			return &_vector3_zero;
		case HYP_REF_VECTOR3_ONE:
			return &_vector3_one;
		case HYP_REF_VECTOR3_UNIT_X:
			return &_vector3_unit_x;
		case HYP_REF_VECTOR3_UNIT_Y:
			return &_vector3_unit_y;
		case HYP_REF_VECTOR3_UNIT_Z:
			return &_vector3_unit_z;
		case HYP_REF_VECTOR3_UNIT_X_NEGATIVE:
			return &_vector3_unit_x_negative;
		case HYP_REF_VECTOR3_UNIT_Y_NEGATIVE:
			return &_vector3_unit_y_negative;
		case HYP_REF_VECTOR3_UNIT_Z_NEGATIVE:
			return &_vector3_unit_z_negative;
		default:
			/* undefined case */
			return &_vector3_zero;
	}
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with specific values
 */
HYPAPI struct vector3 *vector3_setf3(struct vector3 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT)
{
	self->x = xT;
	self->y = yT;
	self->z = zT;
	return self;
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with values from another vector
 */
HYPAPI struct vector3 *vector3_set(struct vector3 *self, const struct vector3 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	self->z = vT->z;
	return self;
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with zeros
 */
HYPAPI struct vector3 *vector3_zero(struct vector3 *self)
{
	return vector3_setf3(self, 0.0f, 0.0f, 0.0f);
}


/**
 * @ingroup vector3
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */
HYPAPI int vector3_equals(const struct vector3 *self, const struct vector3 *vT)
{
	return  HYP_ABS(self->x - vT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - vT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - vT->z) < HYP_EPSILON;
}


/**
 * @ingroup vector3
 * @brief switches the sign on each component of the vector
 */
HYPAPI struct vector3 *vector3_negate(struct vector3 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	self->v[2] = -self->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief adds vectors using component-wise addition
 */
HYPAPI struct vector3 *vector3_add(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	self->v[2] += vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief add to each component of the vector using a scalar
 */
HYPAPI struct vector3 *vector3_addf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] += f;
	self->v[1] += f;
	self->v[2] += f;
	return self;
}


/**
 * @ingroup vector3
 * @brief subtract two vectors using component-wise subtraction
 */
HYPAPI struct vector3 *vector3_subtract(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	self->v[2] -= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief subtract each vector's component by a scalar
 */
HYPAPI struct vector3 *vector3_subtractf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] -= f;
	self->v[1] -= f;
	self->v[2] -= f;
	return self;
}


/**
 * @ingroup vector3
 * @brief multiplies two vectors using component-wise multiplication
 */
HYPAPI struct vector3 *vector3_multiply(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	self->v[2] *= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief multiplies each component of the vector by a scalar
 */
HYPAPI struct vector3 *vector3_multiplyf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] *= f;
	self->v[1] *= f;
	self->v[2] *= f;
	return self;
}


/**
 * @ingroup vector3
 * @brief divides one vector into another using component-wise division
 *
 */
HYPAPI struct vector3 *vector3_divide(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	self->v[2] /= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief calculates the magnitude of the vector
 */
HYPAPI HYP_FLOAT vector3_magnitude(const struct vector3 *self)
{
	return HYP_SQRT((self->x*self->x) + (self->y*self->y) + (self->z*self->z));
}


/**
 * @ingroup vector3
 * @brief normalizes the vector by dividing each component by the magnitude
 */
HYPAPI struct vector3 *vector3_normalize(struct vector3 *self)
{
	HYP_FLOAT mag;

	mag = vector3_magnitude(self);

	if (scalar_equalsf(mag, 0.0f))
	{
		/* can't normalize a zero
		 * avoid divide by zero
		 */
		return self;
	}

	self->x = self->x / mag;
	self->y = self->y / mag;
	self->z = self->z / mag;

	return self;
}


/**
 * @ingroup vector3
 * @brief computes the dot product of two vectors
 */
HYPAPI HYP_FLOAT vector3_dot_product(const struct vector3 *self, const struct vector3 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z);
}


/**
 * @ingroup vector3
 * @brief computes the cross-product between two vectors
 */
HYPAPI struct vector3 *vector3_cross_product(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2)
{
	vR->x = (vT1->y * vT2->z) - (vT1->z * vT2->y);
	vR->y = (vT1->z * vT2->x) - (vT1->x * vT2->z);
	vR->z = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	return vR;
}

/**
 * @ingroup vector3
 * @brief finds the angle between two vectors. make sure to do this on a
 * normalized vector only
 *
 */
HYPAPI HYP_FLOAT vector3_angle_between(const struct vector3 *vT1, const struct vector3 *vT2)
{
	HYP_FLOAT c; /* cosine */

	c = vector3_dot_product(vT1, vT2) / (vector3_magnitude(vT1) * vector3_magnitude(vT2));

	return 2.0f * HYP_ACOS(c);
}


/**
 * @ingroup vector3
 * @brief finds the vector describing the normal between two vectors
 *
 */
HYPAPI struct vector3 *vector3_find_normal_axis_between(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2)
{
	vector3_cross_product(vR, vT1, vT2);
	vector3_normalize(vR);
	return vR;
}


/**
 * @brief Calculates the distance between two points
 *
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2+(z_2-z_1)^2}\f$
 *
 * https://en.wikipedia.org/wiki/Distance
 */
HYPAPI HYP_FLOAT vector3_distance(const struct vector3 *v1, const struct vector3 *v2)
{
	return HYP_SQRT((v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y) + (v2->z - v1->z) * (v2->z - v1->z));
}


/**
 * @brief Multiply a vector by a matrix, returns a vector
 *
 * @param self The vector being multiplied
 * @param mT The matrix used to do the multiplication
 */
HYPAPI struct vector3 *vector3_multiplym4(struct vector3 *self, const struct matrix4 *mT)
{
	struct vector3 vR;

	vector3_zero(&vR);

	vR.x = self->x * mT->r00 + self->y * mT->r01 + self->z * mT->r02 + mT->r03;
	vR.y = self->x * mT->r10 + self->y * mT->r11 + self->z * mT->r12 + mT->r13;
	vR.z = self->x * mT->r20 + self->y * mT->r21 + self->z * mT->r22 + mT->r23;

	vector3_set(self, &vR);

	return self;
}


HYPAPI void _vector3_print(const struct vector3 *self)
{
	printf("x:%10f, y:%10f, z:%10f\r\n", self->x, self->y, self->z);
}


/**
 * @ingroup vector3
 * @brief Rotate a point by the quaternion.  Returns the rotated point.
 *
 * \f$self= qT * self * conjugate(qT)\f$
 *
 * @param self the starting point
 * @param qT the quaternion
 *
 */
HYPAPI struct vector3 *vector3_rotate_by_quaternion(struct vector3 *self, const struct quaternion *qT)
{
	struct quaternion qconj;
	struct quaternion q;

	/* make the conjugate */
	quaternion_set(&qconj, qT);
	quaternion_conjugate(&qconj);

	quaternion_set(&q, qT);
	quaternion_multiplyv3(&q, self);
	quaternion_multiply(&q, &qconj);

	self->x = q.x;
	self->y = q.y;
	self->z = q.z;

	return self;
}


/**
 * @ingroup vector3
 * @brief Reflect a point by the quaternion.  Returns the reflected point.
 * (through the origin)
 *
 * \f$self= qT * self * qT\f$
 *
 * @param qT the quaternion
 * @param self the starting point that is rotated by qT
 *
 */
HYPAPI struct vector3 *vector3_reflect_by_quaternion(struct vector3 *self, const struct quaternion *qT)
{
	struct quaternion q;

	quaternion_set(&q, qT);
	quaternion_multiplyv3(&q, self);
	quaternion_multiply(&q, qT);

	/* this seems to be necessary */
	quaternion_normalize(&q);

	self->x = q.x;
	self->y = q.y;
	self->z = q.z;

	return self;
}


/**
 * @ingroup vector3
 * @brief Randomly fills the vector with values. Good for testing.
 *
 */
HYPAPI struct vector3 *_vector3_set_random(struct vector3 *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	self->z = HYP_RANDOM_FLOAT;
	return self;
}
