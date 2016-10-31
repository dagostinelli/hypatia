#include "hypatia.h"
#include "internal.h"

/**
 * @file quaternion.c
 */


/**
 * @ingroup quaternion
 * @brief Initializes the vector portion of the quaternion with 0.0 and the
 * scalar portion with 1.0.
 * The resulting quaternion has a norm of 1.0
 *
 * While Hypatia as well as other math libraries call this the 'identity'
 * quaternion, this is not an accurate term.  Since the principle of a
 * quaternion with no rotation works similar to an identity matrix, we let
 * the term pass.
 *
 * @snippet test_quaternion.c quaternion identity example
 */
HYPAPI struct quaternion *quaternion_identity(struct quaternion *self)
{
	self->x = 0.0f;
	self->y = 0.0f;
	self->z = 0.0f;
	self->w = 1.0f;

	return self;
}


/**
 * @ingroup quaternion
 * @brief initializes the quaternion with the passed in x, y, z, w
 */
HYPAPI struct quaternion *quaternion_setf4(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT w)
{
	self->x = x;
	self->y = y;
	self->z = z;
	self->w = w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief initializes the quaternion by copying the data from qT.  This is
 * effectively a copy function.
 */
HYPAPI struct quaternion *quaternion_set(struct quaternion * self, const struct quaternion *qT)
{
	return quaternion_setf4(self, qT->x, qT->y, qT->z, qT->w);
}


/**
 * @ingroup quaternion
 * @brief Sets the values in the quaternion, in place, based on the axis and
 * angle.
 *
 * @param self the quaternion that will become initialized with the values of
 * the axis and angle
 * @param x the x axis
 * @param y the y axis
 * @param z the z axis
 * @param angle the angle is in radians
 *
 * q = cos(a/2) + i ( x * sin(a/2)) + j (y * sin(a/2)) + k ( z * sin(a/2))
 *
 */
HYPAPI struct quaternion *quaternion_set_from_axis_anglef3(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT angle)
{
	HYP_FLOAT s = HYP_SIN(angle / 2.0f);
	HYP_FLOAT c = HYP_COS(angle / 2.0f);

	self->x = x * s;
	self->y = y * s;
	self->z = z * s;
	self->w = c;

	/* reduce rounding errors caused by sin/cos */
	return quaternion_normalize(self);
}


/**
 * @ingroup quaternion
 * @brief Sets the values in the quaternion, in place, based on the axis and
 * angle.
 *
 * @param self the quaternion that will become initialized with the values of
 * the axis and angle
 * @param axis the reference axis
 * @param angle the angle is in radians
 *
 */
HYPAPI struct quaternion *quaternion_set_from_axis_anglev3(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	return quaternion_set_from_axis_anglef3(self, axis->x, axis->y, axis->z, angle);
}


/**
 * @ingroup quaternion
 * @brief Checks for mathematical equality within EPSILON.
 *
 */
HYPAPI int quaternion_equals(const struct quaternion *self, const struct quaternion *qT)
{
	return HYP_ABS(self->x - qT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - qT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - qT->z) < HYP_EPSILON &&
		HYP_ABS(self->w - qT->w) < HYP_EPSILON;
}


/**
 * @ingroup quaternion
 * @brief Calculates the norm of the quaternion
 *
 * \f$\|q\| = x^2+y^2+z^2+w^2\f$.
 *
 */
HYPAPI HYP_FLOAT quaternion_norm(const struct quaternion *self)
{
	return (self->x * self->x) + (self->y * self->y) + (self->z * self->z) + (self->w * self->w);
}


/**
 * @ingroup quaternion
 * @brief Calculates the magnitude of the quaternion
 *
 * The magnitude is the sqrt of the norm
 *
 * \f$\sqrt{\|q\|}\f$
 */
HYPAPI HYP_FLOAT quaternion_magnitude(const struct quaternion *self)
{
	return HYP_SQRT(quaternion_norm(self));
}


/**
 * @ingroup quaternion
 * @brief Turns the quaternion into its conjugate form
 *
 * \f$q'=-x, -y, -z, w\f$
 *
 * @snippet test_quaternion.c quaternion conjugate example
 */
HYPAPI struct quaternion *quaternion_conjugate(struct quaternion *self)
{
	self->x = -self->x;
	self->y = -self->y;
	self->z = -self->z;
	/* doesn't switch the sign of w (that would be negate) */

	return self;
}


/**
 * @ingroup quaternion
 * @brief negates all parts of the quaternion
 *
 */
HYPAPI struct quaternion *quaternion_negate(struct quaternion *self)
{
	self->x = -self->x;
	self->y = -self->y;
	self->z = -self->z;
	self->w = -self->w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief Calculates the inverse of the quaternion
 *
 * \f$q^{-1} = (q')/\|q\|\f$
 *
 * inverse = conjugate(q) / norm(q)
 *
 * The inverse can be checked by multiplying the inverse against the original
 * quaternion. The result should be the identity.
 *
 * @snippet test_quaternion.c quaternion inverse example
 */
HYPAPI struct quaternion *quaternion_inverse(struct quaternion *self)
{
	HYP_FLOAT norm;

	norm = quaternion_norm(self);

	if (scalar_equalsf(norm, 0.0f))
	{
		/* avoid divide by zero */
		return self;
	}

	quaternion_conjugate(self);

	if (scalar_equalsf(norm, 1.0f))
	{
		/* we're done */
		return self;
	}

	self->x /= norm;
	self->y /= norm;
	self->z /= norm;
	self->w /= norm;

	return self;
}


/**
 * @ingroup quaternion
 * @brief Constrains the values to sane values.  If this is done enough times
 * during the course of program execution, it has the effect of keeping the
 * values on the manifold.
 *
 */
HYPAPI struct quaternion *quaternion_normalize(struct quaternion *self)
{
	HYP_FLOAT mag;

	mag = quaternion_magnitude(self);

	if (scalar_equalsf(mag, 0.0f))
	{
		/* can't normalize a zero
		 * avoid divide by zero
		 */
		return self;
	}

	self->x /= mag;
	self->y /= mag;
	self->z /= mag;
	self->w /= mag;

	return self;
}


/**
 * @ingroup quaternion
 * @brief if the norm is 1.0, then the quaternion is said to be a 'unit
 * quaternion'
 */
HYPAPI short quaternion_is_unit(struct quaternion *self)
{
	return scalar_equalsf(1.0f, quaternion_norm(self));
}


/**
 * @ingroup quaternion
 * @brief if the scalar is 0.0 (w == 0.0), then the quaternion is said to be a
 * 'pure quaternion'
 */
HYPAPI short quaternion_is_pure(struct quaternion *self)
{
	return scalar_equalsf(self->w, 0.0f);
}


/**
 * @ingroup quaternion
 * @brief Normalized LERP.  This calls lerp and then normalizes the quaternion
 * (pulls it back on the manifold)
 *
 * @param percent This refers to how far along we want to go toward end.  1.0
 * means go to the end. 0.0 means start.  1.1 is nonsense. Negative is
 * nonsense.
 * @param start The quaternion representing the starting orientation
 * @param end The quaternion representing the final or ending orientation
 * @param qR The resulting new orientation.
 *
 */
HYPAPI struct quaternion *quaternion_nlerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR)
{
	quaternion_lerp(start, end, percent, qR);
	quaternion_normalize(qR);
	return qR;
}


/**
 * @ingroup quaternion
 * @brief Linear interpolates between two quaternions.  It will cause the
 * resulting quaternion to move in a straight line "through the 3-sphere"
 *
 * @param percent This refers to how far along we want to go toward end. 1.0
 * means go to the end. 0.0 means start. 1.1 is nonsense. Negative is nonsense.
 * @param start The quaternion representing the starting orientation
 * @param end The quaternion representing the final or ending orientation
 * @param qR The resulting new orientation.
 *
 */
HYPAPI struct quaternion *quaternion_lerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR)
{
	HYP_FLOAT f1, f2;

	/* if percent is 0, return start */
	if (scalar_equalsf(percent, 0.0f))
	{
		quaternion_set(qR, start);
		return qR;
	}

	/* if percent is 1 return end */
	if (scalar_equalsf(percent, 1.0f))
	{
		quaternion_set(qR, end);
		return qR;
	}

	f1 = 1.0f - percent;
	f2 = percent;

	/* this expanded form avoids calling quaternion_multiply and
	 * quaternion_add
	 */
	qR->w = f1 * start->w + f2 * end->w;
	qR->x = f1 * start->x + f2 * end->x;
	qR->y = f1 * start->y + f2 * end->y;
	qR->z = f1 * start->z + f2 * end->z;

	return qR;
}


/**
 * @ingroup quaternion
 * @brief This computes the SLERP between two quaternions.  It computes that
 * absolute final position interopolated between start and end.
 * This function computes shortest arc.
 * If the start and end result in a negative dot product (reversed direction)
 * then the SLERP will reverse direction.
 *
 * @param percent This refers to how far along we want to go toward end.
 * 1.0 means go to the end. 0.0 means start.  1.1 is nonsense. Negative is
 * nonsense.
 * @param start The quaternion representing the starting orientation
 * @param end The quaternion representing the final or ending orientation
 * @param qR The resulting new orientation.
 *
 */
HYPAPI struct quaternion *quaternion_slerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR)
{
	HYP_FLOAT dot;
	HYP_FLOAT f1, f2;
	HYP_FLOAT theta;
	HYP_FLOAT s;
	struct quaternion qneg;

	/* if percent is 0, return start */
	if (scalar_equalsf(percent, 0.0f))
	{
		quaternion_set(qR, start);
		return qR;
	}

	/* if percent is 1 return end */
	if (scalar_equalsf(percent, 1.0f))
	{
		quaternion_set(qR, end);
		return qR;
	}

	/* how parallel are the quaternions (also the dot is the cosine) */
	dot = quaternion_dot_product(start, end);

	/* if they are close to parallel, use LERP
	 *	- This avoids div/0
	 *	- At small angles, the slerp and lerp are the same
	 */

	if (scalar_equalsf(dot, 1.0f))
	{
		quaternion_lerp(start, end, percent, qR);
		return qR;
	}

	/* if dot is negative, they are "pointing" away from one another,
	 * use the shortest arc instead (reverse end and start)
	 * This has the effect of changing the direction of travel around
	 * the sphere beginning with "end" and going the other way around
	 * the sphere
	 */
	if (dot < 0.0f)
	{
		quaternion_set(&qneg, end);
		/*quaternion_conjugate(&qneg);*/
		quaternion_negate(&qneg);
		quaternion_slerp(start, &qneg, percent, qR);
		quaternion_negate(qR);
		/*quaternion_conjugate(qR);*/
		return qR;
	}

	/* keep the dot product in the range that acos can handle */
	/* (shouldn't get here) */
	HYP_CLAMP(dot, -1.0f, 1.0f);

	/* the angle between start and end in radians */
	theta = HYP_ACOS(dot);
	/* cache */
	s = HYP_SIN(theta);
	/* compute negative */
	f1 = HYP_SIN((1.0 - percent) * theta) / s;
	/* compute positive */
	f2 = HYP_SIN(percent * theta) / s;

	/* this expanded form avoids calling quaternion_multiply
	 * and quaternion_add
	 */
	qR->w = f1 * start->w + f2 * end->w;
	qR->x = f1 * start->x + f2 * end->x;
	qR->y = f1 * start->y + f2 * end->y;
	qR->z = f1 * start->z + f2 * end->z;

	return qR;
}


/**
 * @ingroup quaternion
 * @brief Used by SLERP. This treats the whole quaternion as a 4D vector and
 * computes a regular dot product on it
 *
 */
HYPAPI HYP_FLOAT quaternion_dot_product(const struct quaternion *self, const struct quaternion *qT)
{
	return (self->x * qT->x) + (self->y * qT->y) + (self->z * qT->z) + (self->w * qT->w);
}


/**
 * @ingroup quaternion
 * @brief in place adds each element of the quaternion by the other quaternion.
 * This has a "scaling" effect.
 * To add quaternions, add each element one by one like a vector.
 *
 */
HYPAPI struct quaternion *quaternion_add(struct quaternion *self, const struct quaternion *qT)
{
	self->x += qT->x;
	self->y += qT->y;
	self->z += qT->z;
	self->w += qT->w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief in place subtracts each element of the quaternion by the scalar
 * value. This has a "scaling" effect.
 * to subtract quaternions, subtract each element one by one like a vector
 *
 */
HYPAPI struct quaternion *quaternion_subtract(struct quaternion *self, const struct quaternion *qT)
{
	self->x -= qT->x;
	self->y -= qT->y;
	self->z -= qT->z;
	self->w -= qT->w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief in place multiplies each element of the quaternion by the scalar
 * value. This has a "scaling" effect.
 * to multiply by a scalar, apply it to element one by one like a vector
 *
 */
HYPAPI struct quaternion *quaternion_multiplyf(struct quaternion *self, HYP_FLOAT f)
{
	self->x *= f;
	self->y *= f;
	self->z *= f;
	self->w *= f;

	return self;
}


/**
 * @ingroup quaternion
 * @brief in place multiplies the quaternion by a quaternion
 */
HYPAPI struct quaternion *quaternion_multiply(struct quaternion *self, const struct quaternion *qT)
{
	/* qT is the multiplicand */

	struct quaternion r;

	r.x = self->w * qT->x + self->x * qT->w + self->y * qT->z - self->z * qT->y;
	r.y = self->w * qT->y - self->x * qT->z + self->y * qT->w + self->z * qT->x;
	r.z = self->w * qT->z + self->x * qT->y - self->y * qT->x + self->z * qT->w;
	r.w = self->w * qT->w - self->x * qT->x - self->y * qT->y - self->z * qT->z;

	quaternion_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup quaternion
 * @brief multiplies the quaternion by the vector.
 */
HYPAPI struct quaternion *quaternion_multiplyv3(struct quaternion *self, const struct vector3 *vT)
{
	/* vT is the multiplicand */

	struct quaternion r;

	r.x = self->w*vT->x + self->y*vT->z - self->z*vT->y;
	r.y = self->w*vT->y - self->x*vT->z + self->z*vT->x;
	r.z = self->w*vT->z + self->x*vT->y - self->y*vT->x;
	r.w = self->x*vT->x - self->y*vT->y - self->z*vT->z;

	quaternion_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup quaternion
 * @brief Retrieves the axis and angle from the quaternion, assumes normalized
 * quaternion
 *
 * @param self the quaternion
 * @param vR the axis that will be filled
 * @param angle the angle in radians; will be filled with the angle value
 *
 */
HYPAPI void quaternion_get_axis_anglev3(const struct quaternion *self, struct vector3 *vR, HYP_FLOAT *angle)
{
	/* scale is not same as magnitude */
	HYP_FLOAT scale = HYP_SQRT(1.0f - self->w * self->w);

	vR->x = self->x / scale;
	vR->y = self->y / scale;
	vR->z = self->z / scale;
	*angle = 2.0f * HYP_ACOS(self->w);
}


/**
 * @ingroup quaternion
 * @brief initializes the quaternion with random values, then normalizes it
 */
HYPAPI struct quaternion *_quaternion_set_random(struct quaternion *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	self->z = HYP_RANDOM_FLOAT;
	self->w = HYP_RANDOM_FLOAT;

	quaternion_normalize(self);

	return self;
}


/**
 * @ingroup quaternion
 * @brief prints out the elements of the quaternion to stdout
 */
HYPAPI void _quaternion_print(const struct quaternion *self)
{
	printf("x:%10f, y:%10f, z:%10f, w:%10f\r\n", self->x, self->y, self->z, self->w);
}


/**
 * @ingroup quaternion
 * @brief Given two vectors, find a quaternion that will get you from one to
 * the other
 *
 * @param from the starting vector
 * @param to the ending vector
 * @param qR the resulting quaternion that gets you from the starting vector
 * to the ending vector
 */
HYPAPI struct quaternion *quaternion_get_rotation_tov3(const struct vector3 *from, const struct vector3 *to, struct quaternion *qR)
{
	/* this code avoids sqrt and cos and sin and would be nice to
	 * avoid division
	 */
	struct vector3 w;
	HYP_FLOAT dot;
	HYP_FLOAT norm;

	vector3_cross_product(&w, from, to);
	dot = vector3_dot_product(from, to);

	qR->x = w.x;
	qR->y = w.y;
	qR->z = w.z;
	qR->w = dot;

	norm = quaternion_norm(qR);
	qR->w += norm;

	/* normalization with avoidance of div/0 and reusing the norm */
	/* (already calculated above) */
	if (!scalar_equalsf(norm, 0.0f))
	{
		qR->x /= norm;
		qR->y /= norm;
		qR->z /= norm;
		qR->w /= norm;
	}

	return qR;
}
