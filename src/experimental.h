#ifndef _INC_EXPERIMENTAL
#define _INC_EXPERIMENTAL


HYPAPI struct quaternion *quaternion_set_from_euler_anglesf3_ZYX_EXP(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI struct quaternion * quaternion_set_from_euler_anglesf3_ZYX_EXP2(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI void quaternion_get_euler_anglesf3_ZYX_EXP(const struct quaternion *self, HYP_FLOAT *ax, HYP_FLOAT *ay, HYP_FLOAT *az);
HYPAPI struct quaternion *quaternion_rotate_by_quaternion_EXP(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_rotate_by_axis_angle_EXP(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct quaternion *quaternion_rotate_by_euler_angles_EXP(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI HYP_FLOAT quaternion_difference_EXP(const struct quaternion *q1, const struct quaternion *q2);
HYPAPI HYP_FLOAT quaternion_angle_between_EXP(const struct quaternion *self, const struct quaternion *qT);
HYPAPI void quaternion_axis_between_EXP(const struct quaternion *self, const struct quaternion *qT, struct quaternion *qR);
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_rh_EXP(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_projection_ortho3d_rh_EXP(struct matrix4 *self,
						HYP_FLOAT xmin, HYP_FLOAT xmax,
						HYP_FLOAT ymin, HYP_FLOAT ymax,
						HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_view_lookat_rh_EXP(struct matrix4 *self, const struct vector3 *eye, const struct vector3 *target, const struct vector3 *up);
HYPAPI struct vector3 *matrix4_multiplyv3_EXP(const struct matrix4 *m, const struct vector3 *vT, struct vector3 *vR);
HYPAPI struct quaternion quaternion_cross_product_EXP(const struct quaternion *self, const struct quaternion *vT);
HYPAPI struct matrix4 *matrix4_set_from_quaternion_EXP(struct matrix4 *self, const struct quaternion *qT);
HYPAPI struct matrix4 *matrix4_set_from_axisv3_angle_EXP(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_set_from_axisf3_angle_EXP(struct matrix4 *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, const HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_set_from_euler_anglesf3_EXP(struct matrix4 *self, const HYP_FLOAT x, const HYP_FLOAT y, const HYP_FLOAT z);
HYPAPI struct vector3 *matrix4_get_translation_EXP(const struct matrix4 *self, struct vector3 *vT);
HYPAPI HYP_FLOAT matrix4_determinant_EXP(const struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_invert_EXP(struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationv3_EXP(struct matrix4 *self, const struct vector3 *vR);
HYPAPI struct matrix4 *matrix4_transformation_compose_EXP(struct matrix4 *self, const struct vector3 *scale, const struct quaternion *orientation, const struct vector3 *position);
HYPAPI struct matrix4 *matrix4_transformation_decompose_EXP(const struct matrix4 *self, struct vector3 *scale, struct quaternion *orientation, struct vector3 *position);


#endif /* _INC_EXPERIMENTAL */
