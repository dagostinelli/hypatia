#include "hypatia.h"
#include "internal.h"


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
HYPAPI quaternion * quaternion_set_from_euler_anglesf3_EXP(quaternion *self, float ax, float ay, float az)
{
	vector3 vx;
	vector3 vy;
	vector3 vz;
	quaternion qx;
	quaternion qy;
	quaternion qz;
	
	vector3_setf3(&vx, 1, 0, 0);
	vector3_setf3(&vy, 0, 1, 0);
	vector3_setf3(&vz, 0, 0, 1);
	
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
HYPAPI quaternion * quaternion_rotate_by_axis_angle_EXP(quaternion *self, const vector3 *axis, float angle)
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
HYPAPI float quaternion_difference_EXP(const quaternion *q1, const quaternion *q2)
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
HYPAPI quaternion * quaternion_rotate_by_euler_angles_EXP(quaternion *self, float ax, float ay, float az)
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
HYPAPI float quaternion_angle_between_EXP(const quaternion *self, const quaternion *qT)
{
	float c; /* cosine */
	
	c = quaternion_dot_product(self, qT) / ( quaternion_norm(self) * quaternion_norm(qT) );
	
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
HYPAPI matrix4* matrix4_projection_perspective_fovy_rh_EXP(matrix4 *self, float fovy, float aspect, float zNear, float zFar)
{
	float h;
	float w;
	float p;
	float q;
	
	h = HYP_COTF(fovy) / 2.0f;
	w = h * aspect;
	
	p = zFar/(zNear-zFar);
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
HYPAPI matrix4* matrix4_projection_ortho3d_rh_EXP(matrix4 *self, float xmin, float xmax,float ymin, float ymax,float zNear, float zFar)
{
	float width;
	float height;
	
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
HYPAPI vector3 * matrix4_multiplyv3_EXP(const matrix4 *m, const vector3 *vT, vector3 *vR)
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
HYPAPI matrix4 * matrix4_set_from_axisf3_angle_EXP(matrix4 *self, float x, float y, float z, const float angle)
{
	float c = HYP_COS(angle);
	float s = HYP_SIN(angle);
	
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
HYPAPI matrix4 * matrix4_set_from_axisv3_angle_EXP(matrix4 *self, const vector3 *axis, float angle)
{
	return matrix4_set_from_axisf3_angle_EXP(self, axis->x, axis->y, axis->z, angle);
}


