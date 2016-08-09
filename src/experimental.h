#ifndef _INC_EXPERIMENTAL
#define _INC_EXPERIMENTAL


HYPAPI quaternion *quaternion_set_from_euler_anglesf3_ZYX_EXP(quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI quaternion * quaternion_set_from_euler_anglesf3_ZYX_EXP2(quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI void quaternion_get_euler_anglesf3_ZYX_EXP(quaternion *self, HYP_FLOAT *ax, HYP_FLOAT *ay, HYP_FLOAT *az);
HYPAPI quaternion *quaternion_rotate_by_quaternion_EXP(quaternion *self, const quaternion *qT);
HYPAPI quaternion *quaternion_rotate_by_axis_angle_EXP(quaternion *self, const vector3 *axis, HYP_FLOAT angle);
HYPAPI quaternion *quaternion_rotate_by_euler_angles_EXP(quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI HYP_FLOAT quaternion_difference_EXP(const quaternion *q1, const quaternion *q2);
HYPAPI HYP_FLOAT quaternion_angle_between_EXP(const quaternion *self, const quaternion *qT);
HYPAPI void quaternion_axis_between_EXP(const quaternion *self, const quaternion *qT, quaternion *qR);
HYPAPI matrix4 *matrix4_projection_perspective_fovy_rh_EXP(matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI matrix4 *matrix4_projection_ortho3d_rh_EXP(matrix4 *self,
						HYP_FLOAT xmin, HYP_FLOAT xmax,
						HYP_FLOAT ymin, HYP_FLOAT ymax,
						HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI matrix4 *matrix4_view_lookat_rh_EXP(matrix4 *self, const vector3 *eye, const vector3 *target, const vector3 *up);
HYPAPI vector3 *matrix4_multiplyv3_EXP(const matrix4 *m, const vector3 *vT, vector3 *vR);
HYPAPI quaternion quaternion_cross_product_EXP(const quaternion *self, const quaternion *vT);
HYPAPI matrix4 *matrix4_set_from_quaternion_EXP(matrix4 *self, const quaternion *qT);
HYPAPI matrix4 *matrix4_set_from_axisv3_angle_EXP(matrix4 *self, const vector3 *axis, HYP_FLOAT angle);
HYPAPI matrix4 *matrix4_set_from_axisf3_angle_EXP(matrix4 *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, const HYP_FLOAT angle);
HYPAPI matrix4 *matrix4_set_from_euler_anglesf3_EXP(matrix4 *self, const HYP_FLOAT x, const HYP_FLOAT y, const HYP_FLOAT z);
HYPAPI vector3 *matrix4_get_translation_EXP(const matrix4 *self, vector3 *vT);
HYPAPI HYP_FLOAT matrix4_determinant_EXP(const matrix4 *self);
HYPAPI matrix4 *matrix4_invert_EXP(matrix4 *self);
HYPAPI matrix4 *matrix4_make_transformation_rotationv3_EXP(matrix4 *self, const vector3 *vR);
HYPAPI matrix4 *matrix4_transformation_compose_EXP(matrix4 *self, const vector3 *scale, const quaternion *orientation, const vector3 *position);
HYPAPI matrix4 *matrix4_transformation_decompose_EXP(const matrix4 *self, vector3 *scale, quaternion *orientation, vector3 *position);


#endif /* _INC_EXPERIMENTAL */
