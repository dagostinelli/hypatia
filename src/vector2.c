#include "hypatia.h"
#include "internal.h"


/**
 * @file vector2.c
 */


static struct vector2 _vector2_zero = { { {0.0f, 0.0f} } };
static struct vector2 _vector2_one = { { {1.0f, 1.0f} } };
static struct vector2 _vector2_unit_x = { { {1.0f, 0.0f} } };
static struct vector2 _vector2_unit_y = { { {0.0f, 1.0f} } };
static struct vector2 _vector2_unit_x_negative = { { {-1.0f, 0.0f} } };
static struct vector2 _vector2_unit_y_negative = { { {0.0f, -1.0f} } };


HYPAPI const struct vector2 *vector2_get_reference_vector2(int id)
{
	switch (id)
	{
		case HYP_REF_VECTOR2_ZERO:
			return &_vector2_zero;
		case HYP_REF_VECTOR2_ONE:
			return &_vector2_one;
		case HYP_REF_VECTOR2_UNIT_X:
			return &_vector2_unit_x;
		case HYP_REF_VECTOR2_UNIT_Y:
			return &_vector2_unit_y;
		case HYP_REF_VECTOR2_UNIT_X_NEGATIVE:
			return &_vector2_unit_x_negative;
		case HYP_REF_VECTOR2_UNIT_Y_NEGATIVE:
			return &_vector2_unit_y_negative;
		default:
			/* undefined case */
			return &_vector2_zero;
	}
}


HYPAPI struct vector2 *vector2_set(struct vector2 *self, const struct vector2 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	return self;
}


HYPAPI struct vector2 *vector2_setf2(struct vector2 *self, HYP_FLOAT xT, HYP_FLOAT yT)
{
	self->x = xT;
	self->y = yT;
	return self;
}


HYPAPI struct vector2 *vector2_zero(struct vector2 *self)
{
	return vector2_setf2(self, 0.0f, 0.0f);
}


HYPAPI int vector2_equals(const struct vector2 *self, const struct vector2 *vT)
{
	return scalar_equals(self->x, vT->x) && scalar_equals(self->y, vT->y);
}


HYPAPI struct vector2 *vector2_negate(struct vector2 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_add(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_addf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] += fT;
	self->v[1] += fT;
	return self;
}


HYPAPI struct vector2 *vector2_subtract(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_subtractf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] -= fT;
	self->v[1] -= fT;
	return self;
}


HYPAPI struct vector2 *vector2_multiply(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_multiplyf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] *= fT;
	self->v[1] *= fT;
	return self;
}


HYPAPI struct vector2 *vector2_divide(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_dividef(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] /= fT;
	self->v[1] /= fT;
	return self;
}


HYPAPI HYP_FLOAT vector2_magnitude(const struct vector2 *self)
{
	return HYP_SQRT((self->x*self->x) + (self->y*self->y));
}


HYPAPI struct vector2 *vector2_normalize(struct vector2 *self)
{
	HYP_FLOAT mag = vector2_magnitude(self);

	self->x = self->x / mag;
	self->y = self->y / mag;
	return self;
}


HYPAPI HYP_FLOAT vector2_dot_product(const struct vector2 *self, const struct vector2 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y);
}


HYPAPI struct vector2 *vector2_cross_product(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2)
{
	vR->x = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	vR->y = (vT1->y * vT2->x) - (vT1->x * vT2->y);
	return vR;
}


HYPAPI HYP_FLOAT vector2_angle_between(const struct vector2 *self, const struct vector2 *vT)
{
	return vector2_dot_product(self, vT) / (vector2_magnitude(self) * vector2_magnitude(vT));
}


HYPAPI struct vector2 *vector2_find_normal_axis_between(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2)
{
	return
		vector2_normalize(
			vector2_cross_product(vR, vT1, vT2)
		);
}


/**
 * @brief Calculates the distance between two points
 *
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$
 *
 * https://en.wikipedia.org/wiki/Distance
 */
HYPAPI HYP_FLOAT vector2_distance(const struct vector2 *v1, const struct vector2 *v2)
{
	return HYP_SQRT((v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y));
}


/**
 * @brief Multiply a vector by a matrix, returns a vector
 *
 * @param self The vector being multiplied
 * @param mT The matrix used to do the multiplication
 */
HYPAPI struct vector2 *vector2_multiplym3(struct vector2 *self, const struct matrix3 *mT)
{
	struct vector2 vR;

	vector2_zero(&vR);

	vR.x = self->x * mT->c00 + self->y * mT->c01 + mT->c20;
	vR.y = self->x * mT->c10 + self->y * mT->c11 + mT->c21;

	vector2_set(self, &vR);

	return self;
}


HYPAPI void _vector2_print(const struct vector2 *self)
{
	printf("x:%10f, y:%10f\r\n", self->x, self->y);
}


/**
 * @ingroup vector2
 * @brief Randomly fills the vector with values. Good for testing.
 *
 */
HYPAPI struct vector2 *_vector2_set_random(struct vector2 *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	return self;
}
