/* SPDX-License-Identifier: MIT */

/* master (2.1.0-dev) names: the functions that were experimental there, and
 * matrix4_make_transformation_rotationq for matrix4_set_from_quaternion
 */
#define matrix4_set_from_axisv3_angle matrix4_set_from_axisv3_angle_EXP
#define matrix4_set_from_euler_anglesf3 matrix4_set_from_euler_anglesf3_EXP
#define matrix4_projection_perspective_fovy_rh matrix4_projection_perspective_fovy_rh_EXP
#define matrix4_view_lookat_rh matrix4_view_lookat_rh_EXP
#define matrix4_transformation_compose matrix4_transformation_compose_EXP
#define quaternion_angle_between quaternion_angle_between_EXP
#define matrix4_set_from_quaternion matrix4_make_transformation_rotationq
