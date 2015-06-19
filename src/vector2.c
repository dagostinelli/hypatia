#include "hypatia.h"
#include "internal.h"


/**
 * @file vector2.c
 */


HYPAPI vector2 * vector2_set(vector2 *self, const vector2 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	return self;
}


HYPAPI vector2 * vector2_setf2(vector2 *self, float xT, float yT) 
{
	self->x = xT;
	self->y = yT;
	return self;
}


HYPAPI vector2 * vector2_zero(vector2 *self)
{
	return vector2_setf2(self, 0, 0);
}


HYPAPI int vector2_equals(const vector2 *self, const vector2 *vT)
{
	return scalar_equals(self->x, vT->x) && scalar_equals(self->y, vT->y);
}


HYPAPI vector2 * vector2_negate(vector2 *self)
{
	self->v[0] = -self->v[0]; 
	self->v[1] = -self->v[1];
	return self;
}


HYPAPI vector2 * vector2_add(vector2 *self, const vector2 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	return self;
}


HYPAPI vector2 * vector2_addf(vector2 *self, float fT)
{
	self->v[0] += fT;
	self->v[1] += fT;
	return self;
}


HYPAPI vector2 * vector2_subtract(vector2 *self, const vector2 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	return self;
}


HYPAPI vector2 * vector2_subtractf(vector2 *self, float fT)
{
	self->v[0] -= fT;
	self->v[1] -= fT;
	return self;
}


HYPAPI vector2 * vector2_multiply(vector2 *self, const vector2 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	return self;
}


HYPAPI vector2 * vector2_multiplyf(vector2 *self, float fT)
{
	self->v[0] *= fT;
	self->v[1] *= fT;
	return self;
}


HYPAPI vector2 * vector2_divide(vector2 *self, const vector2 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	return self;
}


HYPAPI vector2 * vector2_dividef(vector2 *self, float fT)
{
	self->v[0] /= fT;
	self->v[1] /= fT;
	return self;
}


HYPAPI float vector2_magnitude(const vector2 *self)
{
	return sqrt((self->x*self->x) + (self->y*self->y));
}


HYPAPI vector2 * vector2_normalize(vector2 *self)
{
	float mag = vector2_magnitude(self);

	self->x = self->x / mag;
	self->y = self->y / mag;
	return self;
}


HYPAPI float vector2_dot_product(const vector2 *self, const vector2 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y);
}


HYPAPI vector2 * vector2_cross_product(vector2 *vR, const vector2 *vT1, const vector2 *vT2)
{
	vR->x = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	vR->y = (vT1->y * vT2->x) - (vT1->x * vT2->y);
	return vR;
}


HYPAPI float vector2_angle_between(const vector2 *self, const vector2 *vT)
{ 
	return vector2_dot_product(self, vT) / ( vector2_magnitude(self) * vector2_magnitude(vT) ); 
}


HYPAPI vector2 *vector2_find_normal_axis_between(vector2 *vR, const vector2 *vT1, const vector2 *vT2)
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
HYPAPI float vector2_distance(const vector2 *v1, const vector2 *v2)
{
	return sqrt((v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y));
}


HYPAPI void _vector2_print(const vector2 *self)
{
	printf("x:%10f, y:%10f\r\n", self->x, self->y);
}


/**
 * @ingroup vector2
 * @brief Randomly fills the vector with values. Good for testing.
 *
 */
HYPAPI vector2 * _vector2_set_random(vector2 *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	return self;
}
