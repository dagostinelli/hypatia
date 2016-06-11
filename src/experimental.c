#include "hypatia.h"
#include "internal.h"


#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define DEC(x) PRIMITIVE_CAT(DEC_, x)
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
#define A(x) CAT(self->r,  DEC(x))
#define B(x) CAT(inverse.r, DEC(x))
#define A4(x1, x2, x3, x4) (A(x1) * A(x2) * A(x3) * A(x4))
#define A3(x1, x2, x3) (A(x1) * A(x2) * A(x3))


/**
 * @ingroup experimental
 * @brief this is an opinionated method (opinionated about what axis is yaw, pitch, roll and what is left/right/up/down
 *
 * @param self the quaternion
 * @param ax the x axis
 * @param ay the y axis
 * @param az the z axis
 *
 */
HYPAPI quaternion * quaternion_set_from_euler_anglesf3_EXP(quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az)
{
	vector3 vx;
	vector3 vy;
	vector3 vz;
	quaternion qx;
	quaternion qy;
	quaternion qz;

	vector3_setf3(&vx, 1.0f, 0.0f, 0.0f);
	vector3_setf3(&vy, 0.0f, 1.0f, 0.0f);
	vector3_setf3(&vz, 0.0f, 0.0f, 1.0f);

	quaternion_set_from_axis_anglev3(&qx, &vx, ax);
	quaternion_set_from_axis_anglev3(&qy, &vy, ay);
	quaternion_set_from_axis_anglev3(&qz, &vz, az);

	/* self = qx * qy * qz */
	quaternion_multiply(&qx, &qy);
	quaternion_multiply(&qx, &qz);
	quaternion_set(self, &qx);

	quaternion_normalize(self);

	return self;
}




/**
 * @ingroup experimental
 * @brief rotate a quaternion by a quaternion (basically, multiply and then normalize)
 *
 * @param self the quaternion being rotated
 * @param qT the other quaternion
 *
 */
HYPAPI quaternion * quaternion_rotate_by_quaternion_EXP(quaternion *self, const quaternion *qT)
{
	/* self = self * qT */
	quaternion_multiply(self, qT);
	quaternion_normalize(self);

	return self;
}


/**
 * @ingroup experimental
 * @brief rotate a quaternion by the indicated axis and angle
 * first it makes a quaternion from the axis/angle
 * then it rotates the quaternion by that axis/angle
 *
 * @param self the quaternion being rotated
 * @param axis the starting point
 * @param angle the transformed point
 *
 */
HYPAPI quaternion * quaternion_rotate_by_axis_angle_EXP(quaternion *self, const vector3 *axis, HYP_FLOAT angle)
{
	quaternion qT;

	quaternion_set_from_axis_anglev3(&qT, axis, angle);
	quaternion_rotate_by_quaternion_EXP(self, &qT);

	return self;
}



/**
 * @ingroup experimental
 * @brief returns a score that seeks to describe the difference between two quaternions
 *
 */
HYPAPI HYP_FLOAT quaternion_difference_EXP(const quaternion *q1, const quaternion *q2)
{
	quaternion diff;

	diff.x = q2->x - q1->x;
	diff.y = q2->y - q1->y;
	diff.z = q2->z - q1->z;
	diff.w = q2->w - q1->w;
	return quaternion_norm(&diff);
}


/**
 * @ingroup experimental
 * @brief this is an opinionated method (opinionated about what axis is yaw, pitch, roll and what is left/right/up/down
 * applies the rotations in this order: x, y, z (yaw, pitch, roll)
 * rotates the quaternion by these angles
 *
 * @param self the quaternion being rotated
 * @param ax yaw
 * @param ay pitch
 * @param az roll
 *
 */
HYPAPI quaternion * quaternion_rotate_by_euler_angles_EXP(quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az)
{
	quaternion qT;

	/* make a quaternion from the eulers */
	quaternion_set_from_euler_anglesf3_EXP(&qT, ax, ay, az);

	/* rotate the quaternion by it */
	quaternion_rotate_by_quaternion_EXP(self, &qT);

	return self;
}


/**
 * @ingroup experimental
 * @brief This code is suspect.  Computes the cross-product on the vector portion and then something that resembles a negated dot product on the real portion
 *
 */
HYPAPI quaternion quaternion_cross_product_EXP(const quaternion *self, const quaternion *vT)
{
	/*
	 * The code is suspect (missing w element in this whole thing)
	 * It is computing a cross-product on the vector portion and a negative dot product on the real portion
	 */
	quaternion r;

	r.x = (self->y * vT->z) - (self->z * vT->y);
	r.y = (self->z * vT->x) - (self->x * vT->z);
	r.z = (self->x * vT->y) - (self->y * vT->x);
	r.w = -((self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z));

	return r;
}


/**
 * @ingroup experimental
 * @brief Computes the angle between two quaternions
 *
 *  * \f$angle= 2 * acos((self \cdot qT) / (||self|| * ||qT||))\f$
 *
 */
HYPAPI HYP_FLOAT quaternion_angle_between_EXP(const quaternion *self, const quaternion *qT)
{
	HYP_FLOAT c; /* cosine */

	c = quaternion_dot_product(self, qT) / (quaternion_norm(self) * quaternion_norm(qT));

	return 2.0f * HYP_ACOS(c);
}


/**
 * @ingroup experimental
 * @brief This code is suspect. Treats two quaternions sort of like 2 vector4's and then computes the cross-product between them.
 *
 */
HYPAPI void quaternion_axis_between_EXP(const quaternion *self, const quaternion *qT, quaternion *qR)
{
	quaternion axis;

	axis = quaternion_cross_product_EXP(self, qT);
	quaternion_set(qR, &axis);
	quaternion_normalize(qR);
}


/**
 * @ingroup experimental
 * @brief creates an perspective projection matrix using the RH system with a FOV about the Y-axis.  It's opinionated about what that means.
 */
HYPAPI matrix4 *matrix4_projection_perspective_fovy_rh_EXP(matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT h;
	HYP_FLOAT w;
	HYP_FLOAT p;
	HYP_FLOAT q;

	h = HYP_COT(fovy) / 2.0f;
	w = h * aspect;

	p = zFar / (zNear - zFar);
	q = zNear * p;

	matrix4_zero(self);

	self->r00 = w;
	self->r11 = h;
	self->r22 = p;
	self->r23 = -1.0f; /* this is what makes this RH */
	self->r32 = q;

	return self;
}


/**
 * @ingroup experimental
 * @brief make an orthographic projection matrix with right handed coordinates
 */
HYPAPI matrix4 *matrix4_projection_ortho3d_rh_EXP(matrix4 *self,
						HYP_FLOAT xmin, HYP_FLOAT xmax,
						HYP_FLOAT ymin, HYP_FLOAT ymax,
						HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT width;
	HYP_FLOAT height;

	matrix4_zero(self);

	width = xmax - xmin;
	height = ymax - ymin;

	self->c00 = 2.0f / width;
	self->c11 = 2.0f / height;
	self->c22 = 1.0f / (zFar - zNear);
	self->c23 = zNear / (zNear - zFar);
	self->c33 = 1.0f;

	return self;
}


/**
 * @ingroup experimental
 * @brief Multiply a matrix by a vector, returns a vector
 *
 * @param m The matrix being multiplied
 * @param vT The vector being multiplied
 * @param vR The result
 */
HYPAPI vector3 *matrix4_multiplyv3_EXP(const matrix4 *m, const vector3 *vT, vector3 *vR)
{
	vR->x = vT->x * m->c00 + vT->y * m->c01 + vT->z * m->c02 + m->c03;
	vR->y = vT->x * m->c10 + vT->y * m->c11 + vT->z * m->c12 + m->c13;
	vR->z = vT->x * m->c20 + vT->y * m->c21 + vT->z * m->c22 + m->c23;

	return vR;
}


/**
 * @ingroup experimental
 * @brief Opinionated function about what the axis means.  Sets the axis and angle (used as a rotation matrix)
 *
 * @param self The matrix
 * @param x The x part of the axis
 * @param y The y part of the axis
 * @param z The z part of the axis
 * @param angle the angle in radians
 *
 */
HYPAPI matrix4 *matrix4_set_from_axisf3_angle_EXP(matrix4 *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, const HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	self->c00 = (x * x) * (1.0f - c) + c;
	self->c01 = (x * y) * (1.0f - c) + (z * s);
	self->c02 = (x * z) * (1.0f - c) - (y * s);
	self->c03 = 0.0f;

	self->c10 = (y * x) * (1.0f - c) - (z * s);
	self->c11 = (y * y) * (1.0f - c) + c;
	self->c12 = (y * z) * (1.0f - c) + (x * s);
	self->c13 = 0.0f;

	self->c20 = (z * x) * (1.0f - c) + (y * s);
	self->c21 = (z * y) * (1.0f - c) - (x * s);
	self->c22 = (z * z) * (1.0f - c) + c;
	self->c23 = 0.0f;

	self->c30 = 0.0f;
	self->c31 = 0.0f;
	self->c32 = 0.0f;
	self->c33 = 1.0f;

	return self;
}


/**
 * @ingroup experimental
 * @brief Opinionated function about what the axis means.  Sets the axis and angle (used as a rotation matrix)
 *
 * @param self The matrix
 * @param axis The axis
 * @param angle the angle in radians
 *
 */
HYPAPI matrix4 *matrix4_set_from_axisv3_angle_EXP(matrix4 *self, const vector3 *axis, HYP_FLOAT angle)
{
	return matrix4_set_from_axisf3_angle_EXP(self, axis->x, axis->y, axis->z, angle);
}


HYPAPI matrix4 *matrix4_set_from_euler_anglesf3_EXP(matrix4 *self, const HYP_FLOAT x, const HYP_FLOAT y, const HYP_FLOAT z)
{
	HYP_FLOAT A = HYP_COS(x);
	HYP_FLOAT B = HYP_SIN(x);
	HYP_FLOAT C = HYP_COS(y);
	HYP_FLOAT D = HYP_SIN(y);
	HYP_FLOAT E = HYP_COS(z);
	HYP_FLOAT F = HYP_SIN(z);

	HYP_FLOAT AD = A * D;
	HYP_FLOAT BD = B * D;

	self->c00 = C * E;
	self->c01 = -C * F;
	self->c02 = D;
	self->c03 = 0.0f;

	self->c10 = BD * E + A * F;
	self->c11 = -BD * F + A * E;
	self->c12 = -B * C;
	self->c13 = 0.0f;

	self->c20 = -AD * E + B * F;
	self->c21 = AD * F + B * E;
	self->c22 = A * C;
	self->c23 = 0.0f;

	self->c30 = 0.0f;
	self->c31 = 0.0f;
	self->c32 = 0.0f;
	self->c33 = 1.0f;

	return self;
}


HYPAPI vector3 *matrix4_get_translation_EXP(const matrix4 *self, vector3 *vT)
{
	vT->x = self->c30;
	vT->y = self->c31;
	vT->z = self->c32;

	return vT;
}


HYPAPI matrix4 *matrix4_invert_EXP(matrix4 *self)
{
	matrix4 inverse;
	HYP_FLOAT determinant;
	int i;

	determinant = matrix4_determinant_EXP(self);

	if (determinant == 0.0)
	{
		return 0;
	}

	determinant = 1.0 / determinant;

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

	for (i = 0; i < 16; i++)
	{
		self->m[i] = inverse.m[i] * determinant;
	}

	return self;
}


HYPAPI HYP_FLOAT matrix4_determinant_EXP(const matrix4 *self)
{
	HYP_FLOAT determinant;

	determinant =
	  A4(11, 22, 33, 44) + A4(11, 23, 34, 42) + A4(11, 24, 32, 43)
	+ A4(12, 21, 34, 43) + A4(12, 23, 31, 44) + A4(12, 24, 33, 41)
	+ A4(13, 21, 32, 44) + A4(13, 22, 34, 41) + A4(13, 24, 31, 42)
	+ A4(14, 21, 33, 42) + A4(14, 22, 31, 43) + A4(14, 23, 32, 41)
	- A4(11, 22, 34, 43) - A4(11, 23, 32, 44) - A4(11, 24, 33, 42)
	- A4(12, 21, 33, 44) - A4(12, 23, 34, 41) - A4(12, 24, 31, 43)
	- A4(13, 21, 34, 42) - A4(13, 22, 31, 44) - A4(13, 24, 32, 41)
	- A4(14, 21, 32, 43) - A4(14, 22, 33, 41) - A4(14, 23, 31, 42)
	;

	return determinant;
}


/**
 * @ingroup experimental
 * @brief creates a look at matrix using the RH system.
 */
HYPAPI matrix4 *matrix4_view_lookat_rh_EXP(matrix4 *self, const vector3 *eye, const vector3 *target, const vector3 *up)
{
	vector3 yaxis;
	vector3 zaxis;
	vector3 xaxis;

	zaxis.x = target->x - eye->x;
	zaxis.y = target->y - eye->y;
	zaxis.z = target->z - eye->z;
	vector3_normalize(&zaxis);

	/* xaxis = zaxis x up */
	vector3_cross_product(&xaxis, &zaxis, up);
	vector3_normalize(&xaxis);

	/* yaxis = xaxis x zaxis */
	vector3_cross_product(&yaxis, &xaxis, &zaxis);

	matrix4_identity(self);

	self->c00 = xaxis.x;
	self->c10 = xaxis.y;
	self->c20 = xaxis.z;

	self->c01 = yaxis.x;
	self->c11 = yaxis.y;
	self->c21 = yaxis.z;

	self->c02 = -zaxis.x;
	self->c12 = -zaxis.y;
	self->c22 = -zaxis.z;

	self->c30 = -vector3_dot_product(&xaxis, eye);
	self->c31 = -vector3_dot_product(&yaxis, eye);
	self->c32 = -vector3_dot_product(&zaxis, eye);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary rotation matrix and then multiplies self by that.  Opinionated function about what rotation means.
 *
 * @param self The transformation matrix being rotated
 * @param vR the euler angles
 *
 */
HYPAPI matrix4 *matrix4_make_transformation_rotationv3_EXP(matrix4 *self, const vector3 *vR)
{
	matrix4 scratchMatrix;

	matrix4_identity(self);

	return matrix4_multiply(self,
		matrix4_set_from_euler_anglesf3_EXP(&scratchMatrix, vR->x, vR->y, vR->z));
}


HYPAPI matrix4 *matrix4_transformation_compose_EXP(matrix4 *self, const vector3 *scale, const quaternion *orientation, const vector3 *position)
{
	matrix4 scaleM, rotateM;

	matrix4_identity(self);
	matrix4_multiply(self, matrix4_make_transformation_scalingv3(&scaleM, scale));
	matrix4_multiply(self, matrix4_make_transformation_rotationq(&rotateM, orientation));

	self->c30 = position->x;
	self->c31 = position->y;
	self->c32 = position->z;

	return self;
}


/*
HYPAPI matrix4 *matrix4_transformation_decompose_EXP(const matrix4 *self, vector3 *scale, quaternion *orientation, vector3 *position)
{
	HYP_FLOAT sx, sy, sz, inverse_sx, inverse_sy, inverse_sz;
	HYP_FLOAT determinant;
	vector3 scratchVector;

	sx = vector3_magnitude(vector3_setf3(&scratchVector, self->c00, self->c10, self->c20));
	sy = vector3_magnitude(vector3_setf3(&scratchVector, self->c01, self->c11, self->c21));
	sz = vector3_magnitude(vector3_setf3(&scratchVector, self->c02, self->c12, self->c22));

	determinant = matrix4_determinant_EXP(self);

	if (determinant < 0.0)
	{
		sx = -sx;
	}

	position->x = self->c30;
	position->y = self->c31;
	position->z = self->c32;

	inverse_sx = 1.0 / sx;
	inverse_sy = 1.0 / sy;
	inverse_sz = 1.0 / sz;

	self->c00 *= inverse_sx;
	self->c10 *= inverse_sx;
	self->c20 *= inverse_sx;

	self->c01 *= inverse_sy;
	self->c11 *= inverse_sy;
	self->c21 *= inverse_sy;

	self->c02 *= inverse_sz;
	self->c12 *= inverse_sz;
	self->c22 *= inverse_sz;

	scale->x = sx;
	scale->y = sy;
	scale->z = sz;
}
*/
