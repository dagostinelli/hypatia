# hypatia comparison: single precision

Tolerance 2e-05 (difference relative to max(1, |reference|)); 2000 random inputs per row.


## vectors

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector2_normalize | glm::normalize | 2000 | 0 | 5.96046448e-08 |  |  |
| vector3_normalize | glm::normalize | 2000 | 0 | 5.96046448e-08 |  |  |
| vector3_normalize | Eigen normalized | 2000 | 0 | 1.1920929e-07 |  |  |
| vector4_normalize | glm::normalize | 2000 | 0 | 5.96046448e-08 |  |  |
| vector3_normalize | cglm glm_vec3_normalize | 2000 | 0 | 5.96046448e-08 |  |  |
| vector4_normalize | cglm glm_vec4_normalize | 2000 | 0 | 1.1920929e-07 |  |  |
| vector3_magnitude | glm::length | 2000 | 0 | 0 |  |  |
| vector2_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector3_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector4_distance | glm::distance | 2000 | 0 | 1.17999099e-07 |  |  |
| vector4_dot_product | glm::dot | 2000 | 0 | 3.81469727e-06 |  |  |
| vector3_cross_product | glm::cross | 2000 | 0 | 0 |  |  |
| vector2_cross_product | a.x b.y - a.y b.x | 2000 | 0 | 0 |  |  |
| vector3_find_normal_axis_between | glm::normalize(glm::cross) | 2000 | 0 | 5.96046448e-08 |  |  |
| vector3_angle_between | glm::angle(normalize, normalize) | 2000 | 0 | 5.70518437e-06 |  | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 9.33354323e-08 |  | a and 3a: parallel |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 3.62091501e-07 |  | 1e-4 rad apart |
| vector2_angle_between | glm::angle(normalize, normalize) | 2000 | **3** | 0.000179052353 | (-3.85436821, 3.45051289), (-5.69850636, 5.1032629) -> 0.000179052353 vs 0 | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector2_angle_between | 0 | 2000 | 0 | 8.94069814e-08 |  | a and 3a: parallel |
| vector3_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector4_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector3_clamp | glm::clamp | 2000 | 0 | 0 |  |  |
| vector3_min, vector3_max | glm::min, glm::max | 2000 | 0 | 0 |  |  |
| vector2_project | glm::proj | 2000 | 0 | 0 |  |  |
| vector3_project | glm::proj | 2000 | 0 | 0 |  |  |
| vector4_project | glm::proj | 2000 | 0 | 3.32072712e-07 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 2.86102295e-06 |  |  |
| vector3_rotate_by_quaternion | Eigen q * v | 2000 | 0 | 2.14576721e-06 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 3.69548798e-06 |  | q not unit length |
| vector3_rotate_by_quaternion | cglm glm_quat_rotatev | 2000 | 0 | 1.99899885e-06 |  |  |
| vector3_reflect_by_quaternion | glm q * (v, 0) * q (as documented) | 2000 | 0 | 7.15255737e-07 |  |  |
| vector3_multiplym4 | glm (M * (v, 1)).xyz | 2000 | 0 | 1.74343586e-06 |  |  |
| vector2_multiplym2 | glm M * v | 2000 | 0 | 0 |  |  |
| vector2_multiplym3 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  | README: sets self to mT * self |
| matrix3_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix2_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix4_multiply(A, B) | cglm glm_mat4_mul(B, A) | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv4 | glm M * v | 2000 | 0 | 1.90734863e-06 |  |  |
| matrix4_multiplyv3 | glm (M * (v, 1)).xyz | 2000 | 0 | 1.66141107e-06 |  |  |
| matrix4_multiplyv2 | glm (M * (v, 0, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix3_multiplyv2 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix2_multiplyv2 | glm M * v | 2000 | 0 | 0 |  |  |
| matrix4_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix3_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix4_determinant | glm::determinant | 2000 | 0 | 1.86854767e-07 |  |  |
| matrix4_determinant | Eigen long double | 2000 | 0 | 1.64765527e-05 |  | relative to |det|; entries up to 5 |
| matrix4_determinant | LAPACK dgetrf | 2000 | **1** | 4.17483338e-05 | (-1.81076717, 2.4319706, 4.24093676, 3.28758144, 4.94790077, 0.684572637, -4.35234404, 4.85153627, -4.1794486, 4.53491402, -3.78691697, -4.54426765, -0.342680603, 0.671074271, 3.93790483, 3.37306118) -> -1.56942749 vs -1.56949301 | LAPACK in double; single precision rounding of the matrix products |
| matrix3_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix2_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix4_inverse | glm::inverse | 2000 | 0 | 2.16795796e-06 |  |  |
| matrix4_inverse | Eigen inverse | 2000 | 0 | 4.21486895e-07 |  |  |
| matrix4_inverse | LAPACK dgetri | 2000 | 0 | 6.9850871e-07 |  |  |
| matrix4_inverse | cglm glm_mat4_inv | 2000 | 0 | 5.39910127e-07 |  |  |
| matrix3_inverse | glm::inverse | 2000 | 0 | 1.18514628e-09 |  |  |
| matrix2_inverse | glm::inverse | 2000 | 0 | 1.19004409e-09 |  |  |

## inverse accuracy (largest error over the random matrices)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_inverse | long double | 2000 | 0 | 2.37888535e-07 |  | condition ~1e0; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 2.62285713e-07 |  | condition ~1e0; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 2.38378657e-07 |  | condition ~1e0; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 4.51277059e-08 |  | condition ~1e0; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | 2.55621061e-07 |  | condition ~1e0; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 0.00053138556 |  | condition ~1e3; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 0.000380882882 |  | condition ~1e3; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 0.000339813091 |  | condition ~1e3; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.92015801e-08 |  | condition ~1e3; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | 0.000380882882 |  | condition ~1e3; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 68.9002521 |  | condition ~1e6; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.81750738e-08 |  | condition ~1e6; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_reciprocal_condition | 1 / (norm(A) norm(A^-1)), infinity norm, in long double | 2000 | **604** | 0.167138848 | (0.215862736, -0.279142052, 0.142417699, -0.463687211, -0.172131643, 0.227488145, -0.122099251, 0.376070201, -0.166937441, 0.21884574, -0.115305625, 0.36221388, -0.156877056, 0.19811672, -0.0952156261, 0.330769926) -> 1.60241707e-05 vs 9.04610755e-07 | relative error up to 1e-2 (single) or 1e-5 (double); single precision is limited by the float inverse: see the accuracy table |
| matrix4_normal_matrix | glm::inverseTranspose(mat3(M)) | 2000 | 0 | 2.47775636e-07 |  |  |

## matrix builders

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_make_transformation_translationv3 | glm::translate(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_scalingv3 | glm::scale(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_rotationf_x | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix4_make_transformation_rotationf_y | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix4_make_transformation_rotationf_z | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 3.57627869e-07 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 3.57627869e-07 |  | axis not unit length |
| matrix4_set_from_quaternion | glm::mat4_cast | 2000 | 0 | 4.17232513e-07 |  |  |
| matrix4_set_from_quaternion | glm::mat4_cast(normalize(q)) | 2000 | 0 | 7.15255737e-07 |  | q not unit length |
| matrix4_make_transformation_rotationq | Eigen toRotationMatrix | 2000 | 0 | 3.57627869e-07 |  |  |
| matrix4_make_transformation_rotationq | cglm glm_quat_mat4 | 2000 | 0 | 3.57627869e-07 |  |  |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleZYX(z, y, x) = Rz Ry Rx | 2000 | 0 | 1.1920929e-07 |  | X first, then Y, then Z |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleXYZ(x, y, z) | 2000 | **2000** | 1.99991775 | 0.156445712, 1.57098365, 2.97232413 -> (0.000184644348, -0.319985539, -0.947422385, 0, -3.15564357e-05, -0.947422385, 0.319985539, 0, -1, -2.91862489e-05, -0.000185033801, 0, 0, 0, 0, 1) | for reference: GLM's XYZ is Rx Ry Rz |
| matrix4_set_from_euler_anglesf3(x, y, z) | cglm glm_euler_xyz | 2000 | **2000** | 1.99988419 | 2.47305441, -4.72119761, 5.60432434 -> (0.00685556186, -0.0103413165, -0.999922991, 0, -0.00553092454, -0.999931633, 0.0103034377, 0, -0.999961197, 0.00545986323, -0.00691229012, 0, 0, 0, 0, 1) | for reference: cglm's xyz is GLM's XYZ |
| matrix4_set_from_euler_anglesf3(x, y, z) | cglm glm_euler_zyx | 2000 | 0 | 1.1920929e-07 |  |  |
| matrix4_make_transformation_rotationv3(v) | glm::eulerAngleZYX(v.z, v.y, v.x) | 2000 | 0 | 1.1920929e-07 |  |  |
| matrix4_translatev3(M, v) | glm T(v) * M | 2000 | 0 | 0 |  | applies the translation after M |
| matrix4_translatev3(M, v) | glm::translate(M, v) = M * T(v) | 2000 | **2000** | 19.2035532 | (-0.340906203, -0.75268662, 1.76634157, 2.0039258, -0.745925307, 0.198491365, -1.16968167, -0.716735125, 0.137070894, -0.791822433, -1.97225213, 9.73465729, 0, 0, 0, 1), (9.00541782, -6.97982979, 8.73570538) -> (-0.340906203, -0.75268662, 1.76634157, 11.0093441, -0.745925307, 0.198491365, -1.16968167, -7.69656467, 0.137070894, -0.791822433, -1.97225213, 18.4703636, 0, 0, 0, 1) | for reference: GLM applies it before M |
| matrix4_rotatev3(M, axis, a) | glm R * M | 2000 | 0 | 2.9937479e-06 |  |  |
| matrix4_scalev3(M, v) | glm S * M | 2000 | 0 | 0 |  |  |
| matrix4_transformation_compose(s, q, t) | glm T * R * S | 2000 | 0 | 1.07288361e-06 |  |  |
| matrix4_transformation_decompose | glm::decompose | 2000 | 0 | 1.60816666e-07 |  | scale, rotation (q or -q), translation |
| matrix4_transformation_decompose | cglm glm_decompose | 2000 | 0 | 0 |  | scale and translation |
| matrix3_make_transformation_rotationf_z | glm::rotate(mat3(1), a) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix3_make_transformation_translationv2 | glm::translate(mat3(1), v) | 2000 | 0 | 0 |  |  |
| matrix3_make_transformation_scalingv2 | glm::scale(mat3(1), v) | 2000 | 0 | 0 |  |  |
| matrix3_rotate(M, a) | glm R * M | 2000 | 0 | 2.38418579e-07 |  |  |
| matrix3_translatev2(M, v) | glm T * M | 2000 | 0 | 0 |  |  |
| matrix2_make_transformation_rotationf_z | mat2 of glm::rotate(mat3(1), a) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix2_rotate(M, a) | glm R * M | 2000 | 0 | 2.38418579e-07 |  |  |

## projections

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_projection_perspective_fovy_rh | glm::perspectiveRH_ZO | 2000 | 0 | 1.78813934e-07 |  |  |
| matrix4_projection_perspective_fovy_lh | glm::perspectiveLH_ZO | 2000 | 0 | 2.33356082e-07 |  |  |
| matrix4_projection_ortho3d_rh | glm::orthoRH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_projection_ortho3d_lh | glm::orthoLH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_view_lookat_rh | glm::lookAtRH | 2000 | 0 | 2.29477882e-06 |  |  |
| matrix4_view_lookat_lh | glm::lookAtLH | 2000 | 0 | 1.88099774e-06 |  |  |
| matrix4_projection_perspective_fovy_rh | cglm glm_perspective_rh_zo | 2000 | 0 | 2.01097247e-07 |  |  |
| matrix4_projection_ortho3d_lh | cglm glm_ortho_lh_zo | 2000 | 0 | 1.18742874e-07 |  |  |
| matrix4_view_lookat_rh | cglm glm_lookat_rh | 2000 | 0 | 2.39250422e-06 |  |  |

## quaternions

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| quaternion_multiply(a, b) | glm a * b | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_multiply(a, b) | Eigen a * b | 2000 | 0 | 9.53674316e-07 |  |  |
| quaternion_multiply(a, b) | cglm glm_quat_mul(a, b) | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_multiplyv3(q, v) | glm q * (v, 0) | 2000 | 0 | 8.7506749e-07 |  |  |
| quaternion_conjugate | glm::conjugate | 2000 | 0 | 0 |  |  |
| quaternion_inverse | glm::inverse | 2000 | 0 | 2.22741681e-07 |  | not unit length |
| quaternion_inverse | Eigen inverse | 2000 | 0 | 1.52211867e-07 |  | not unit length |
| quaternion_inverse | cglm glm_quat_inv | 2000 | 0 | 1.18502802e-07 |  | not unit length |
| quaternion_normalize | glm::normalize | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_dot_product, norm, magnitude | glm::dot, dot(q, q), glm::length | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_set_from_axis_anglev3 | Eigen AngleAxis | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis(a, normalize(axis)) | 2000 | 0 | 1.78813934e-07 |  | axis not unit length |
| quaternion_get_axis_anglev3 | glm::axis, glm::angle | 2000 | 0 | 8.94069672e-08 |  | as a rotation: rebuilt with angleAxis, q or -q |
| quaternion_get_axis_anglev3 | glm::angle | 2000 | **1019** | 0.966547635 | (x -0.0835443959, y -0.0442205779, z -0.0370204411, w -0.994833827) -> 0.203383744 vs 6.07980156 | the angle itself (hypatia [0, pi], GLM [0, 2 pi]) |
| quaternion_get_axis_anglev3 | Eigen AngleAxis(q) | 2000 | 0 | 1.78813934e-07 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | glm::quat(vec3(x, y, z)) | 2000 | 0 | 1.78813934e-07 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | matrix4_set_from_euler_anglesf3(x, y, z) | 2000 | 0 | 2.98023224e-07 |  | the matrix of the quaternion |
| quaternion_get_euler_anglesf3 | glm::eulerAngles | 2000 | 0 | 1.31130219e-06 |  | angles; |y| < 1.5 |
| quaternion_get_euler_anglesf3 | the angles given to quaternion_set_from_euler_anglesf3 | 2000 | 0 | 1.65089689e-07 |  | round trip; |y| < 1.5 |
| quaternion_set_from_matrix4 | glm::quat_cast | 2000 | 0 | 2.38418579e-07 |  | q or -q |
| quaternion_set_from_matrix4 | Eigen Quaternion(Matrix3) | 2000 | 0 | 1.78813934e-07 |  | q or -q |
| quaternion_set_from_matrix4 | cglm glm_mat4_quat | 2000 | 0 | 1.78813934e-07 |  | q or -q |
| quaternion_lerp | glm::lerp | 2000 | 0 | 0 |  |  |
| quaternion_nlerp | glm::normalize(glm::lerp) | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_slerp | glm::slerp | 2000 | 0 | 2.38418579e-07 |  | components |
| quaternion_slerp | Eigen slerp | 2000 | 0 | 1.78813934e-07 |  | components |
| quaternion_slerp | cglm glm_quat_slerp | 2000 | 0 | 3.57627869e-07 |  | as a rotation (cglm does not take the shortest arc) |
| quaternion_slerp | glm::slerp | 2000 | 0 | 1.78813934e-07 |  | nearly the same: 1e-3 rad apart |
| quaternion_get_rotation_tov3 | glm::rotation(normalize(a), normalize(b)) | 2000 | 0 | 1.80602074e-05 |  | near opposite GLM is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | **1** | 6.25252724e-05 | (-2.95046353, -3.41663909, -3.21371937), (7.57799435, 8.63873196, 7.83170176) -> (x 0.60842818, y -0.755048573, z 0.244135618, w 0.0107107181) | near opposite Eigen is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | cglm glm_quat_from_vecs(normalized) | 2000 | 0 | 2.58348882e-06 |  |  |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | 0 | 2.38418579e-07 |  | nearly opposite: 1e-3 rad from opposite; rotating a must give b |
| quaternion_angle_between | Eigen angularDistance | 2000 | 0 | 2.46921405e-07 |  |  |
| quaternion_angle_between | long double | 2000 | 0 | 4.66353676e-07 |  | 1e-4 rad apart |
| quaternion_difference | min(|a - b|^2, |a + b|^2) | 2000 | 0 | 1.42744742e-07 |  |  |
| quaternion_rotate_by_quaternion(a, b) | glm::normalize(a * b) | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_rotate_by_axis_angle(q, axis, a) | glm::rotate(q, a, axis) = q * angleAxis | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_rotate_by_euler_angles(q, x, y, z) | q * glm::quat(vec3(x, y, z)) | 2000 | 0 | 2.38418579e-07 |  | as a rotation |

## accuracy against long double (largest error over the inputs)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector3_angle_between | long double | 2000 | 0 | 1.86728848e-07 |  | random |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 2.64067818e-06 |  | random |
| vector2_angle_between | long double | 2000 | 0 | 1.7093108e-07 |  | random |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 6.56509064e-05 |  | random |
| vector3_angle_between | long double | 2000 | 0 | 7.38533512e-08 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 0.000227996337 |  | 1e-3 rad apart |
| vector2_angle_between | long double | 2000 | 0 | 9.80270618e-08 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 0.000344820948 |  | 1e-3 rad apart |
| vector3_angle_between | long double | 2000 | 0 | 9.4568724e-08 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 0.000309468141 |  | 1e-3 rad from opposite |
| vector2_angle_between | long double | 2000 | 0 | 1.8786052e-07 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 0.000383716721 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.47130031e-07 |  | random |
| glm::rotation | long double | 2000 | 0 | 3.38437249e-06 |  | random |
| Eigen FromTwoVectors | long double | 2000 | 0 | 3.07199738e-06 |  | random |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 1.97147013e-06 |  | random |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.38565978e-07 |  | 1e-3 rad from opposite |
| glm::rotation | long double | 2000 | 0 | 0.000324068289 |  | 1e-3 rad from opposite |
| Eigen FromTwoVectors | long double | 2000 | 0 | 0.0085646417 |  | 1e-3 rad from opposite |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 0.00428000932 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 1.28774502e-07 |  | exactly opposite (to = -2 from) |
| glm::rotation | long double | 2000 | 0 | 2 |  | exactly opposite (to = -2 from) |
| Eigen FromTwoVectors | long double | 2000 | 0 | 0.000690533985 |  | exactly opposite (to = -2 from) |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 1.50833854e-07 |  | exactly opposite (to = -2 from) |
| matrix4_determinant | long double | 2000 | 0 | 3.51880553e-07 |  | condition up to 1e6; relative to the largest element^4 |
| glm::determinant | long double | 2000 | 0 | 2.47403167e-07 |  | condition up to 1e6; relative to the largest element^4 |
| Eigen determinant | long double | 2000 | 0 | 1.82214152e-07 |  | condition up to 1e6; relative to the largest element^4 |
| matrix4_reciprocal_condition | long double | 2000 | 0 | inf |  | condition up to 1e6; |log10(result / exact)| |
| 1 / (norm(A) norm(glm::inverse(A))) | long double | 2000 | 0 | 2.7594501 |  | condition up to 1e6; |log10(result / exact)| |
| LAPACK dgecon estimate (double) | long double | 2000 | 0 | 0.572857143 |  | condition up to 1e6; |log10(result / exact)| |

## Edge cases

| function | input | hypatia | others |
|---|---|---|---|
| vector3_normalize | zero (0, 0, 0) | (0, 0, 0) | GLM (-nan, -nan, -nan); Eigen (0, 0, 0); cglm (0, 0, 0) |
| vector3_normalize | tiny (1e-30 / 1e-200) (1e-30, 1e-30, 0) | (0.707106769, 0.707106769, 0) | GLM (inf, inf, -nan); Eigen (1e-30, 1e-30, 0); cglm (0, 0, 0) |
| vector3_normalize | huge (1e30 / 1e200) (1.00000002e+30, 1.00000002e+30, 0) | (0.707106769, 0.707106769, 0) | GLM (0, 0, 0); Eigen (0, 0, 0); cglm (0, 0, 0) |
| vector3_normalize | one component inf (inf, 1, 0) | (1, 0, 0) | GLM (-nan, 0, 0); Eigen (-nan, 0, 0); cglm (-nan, 0, 0) |
| vector3_normalize | one component NaN (-nan, 1, 0) | (-nan, 1, 0) | GLM (-nan, -nan, -nan); Eigen (-nan, 1, 0); cglm (-nan, -nan, -nan) |
| vector3_angle_between | a, 3a: parallel | 3.33200099e-08 | GLM angle(normalize, normalize) 0 |
| vector3_angle_between | a, zero | 0 | GLM -nan |
| vector2_angle_between | a, 7a: parallel | 1.4901163e-08 | GLM angle(normalize, normalize) 0 |
| vector3_project | onto zero | (0, 0, 0) | GLM proj (-nan, -nan, -nan) |
| quaternion_normalize | zero | (x 0, y 0, z 0, w 0) | GLM (x 0, y 0, z 0, w 1); Eigen (x 0, y 0, z 0, w 0) |
| quaternion_inverse | zero | (x 0, y 0, z 0, w 0) | GLM (x -nan, y -nan, z -nan, w -nan); Eigen (x 0, y 0, z 0, w 0) |
| quaternion_slerp | q, -q, 0.5 (same rotation) | (x 0, y 0, z 0.47942555, w 0.87758255) | GLM slerp (x 0, y 0, z 0.47942555, w 0.87758255); GLM mix (x -0, y -0, z -0, w -0); Eigen (x 0, y 0, z 0.47942555, w 0.87758255) |
| quaternion_slerp | q, q, 0.5 | (x 0, y 0, z 0.47942555, w 0.87758255) | GLM (x 0, y 0, z 0.47942555, w 0.87758255); Eigen (x 0, y 0, z 0.47942555, w 0.87758255) |
| quaternion_slerp | t = 1.5 (beyond end) | (x 0, y 0, z 0.998531222, w -0.0541771352) | GLM (x 0, y 0, z 0.998531282, w -0.0541771129); Eigen (x 0, y 0, z 0.998531222, w -0.0541771352) |
| quaternion_get_axis_anglev3 | identity | (0, 0, 0), 0 | GLM axis (0, 0, 1), angle 0 |
| quaternion_get_axis_anglev3 | -identity | (-0, -0, -0), 0 | GLM axis (0, 0, 1), angle 6.28318548 |
| quaternion_get_axis_anglev3 | -(1 rad about Z) | (0, 0, 1), 1 | GLM axis (-0, -0, -0.99999994), angle 5.28318548 |
| quaternion_get_rotation_tov3 | a, -a: opposite | (x 0, y 0.832050264, z -0.554700196, w 0) (turns a to (-1, -2.00000024, -2.99999976)) | GLM (x 0, y 0, z 0, w 0.000244140625) (turns a to (1, 2, 3)); Eigen (x 0.119522862, y -0.844013155, z 0.52283448, w 0.000244140625) |
| quaternion_get_rotation_tov3 | a, zero | (x 0, y 0, z 0, w 1) | GLM (x -nan, y -nan, z -nan, w -nan); Eigen (x 0, y 0, z 0, w 0.707106769) |
| matrix4_inverse | singular (1 .. 16) | NULL | GLM (-nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan); determinant: hypatia 0, GLM 0 |
| matrix4_inverse | nearly singular | NULL | LAPACK rcond (double) see table; GLM determinant 0 |
| matrix4_transformation_decompose | zero scale on x | 0 (fails) | GLM false, scale (4.5748191e-41, 4.02067871e+34, 4.59121429e-41) |
| matrix4_view_lookat_rh | eye == target | (0, 0, 0, -0, 0, 0, 0, -0, -0, -0, -0, 0, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, nan, nan, nan, -nan, 0, 0, 0, 1) |
| matrix4_view_lookat_rh | looking along up | (0, 0, 0, -0, 0, 0, 0, -0, -0, -1, -0, 2, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, -0, -1, -0, 2, 0, 0, 0, 1) |
| matrix4_projection_perspective_fovy_rh | zNear == zFar | (1.83048773, 0, 0, 0, 0, 1.83048773, 0, 0, 0, 0, inf, inf, 0, 0, -1, 0) | GLM (1.83048773, 0, 0, 0, 0, 1.83048773, 0, 0, 0, 0, inf, -inf, 0, 0, -1, 0) |
| vector3_rotate_by_quaternion | X by 2 * (quarter turn about Z) | (0, 1, 0) | GLM q * v (-3.00000048, 4.00000048, 0); Eigen q * v (-3.00000048, 4.00000048, 0); cglm glm_quat_rotatev (0, 1.00000012, 0) |
| vector3_rotate_by_quaternion | X by the zero quaternion | (1, 0, 0) | GLM q * v (1, 0, 0); Eigen q * v (1, 0, 0); cglm glm_quat_rotatev (1, 0, 0) |
| matrix4_set_from_axisv3_angle | quarter turn about (0, 0, 10), applied to X | (-4.37113883e-08, 1, 0) | GLM rotate (-4.37113883e-08, 1, 0); Eigen AngleAxis matrix (-4.37113883e-08, 10, 0); cglm glm_rotate_make (-4.37113883e-08, 1, 0) |
| quaternion_set_from_axis_anglev3 | quarter turn about (0, 0, 10), applied to X | (x 0, y 0, z 0.707106829, w 0.707106829) length 1, X -> (0, 1, 0) | GLM angleAxis (x 0, y 0, z 7.07106781, w 0.707106769) length 7.10633516, X -> (-99, 10, 0); Eigen AngleAxis (x 0, y 0, z 7.07106781, w 0.707106769) length 7.10633516, X -> (-99, 10, 0); cglm glm_quatv (x 0, y 0, z 0.707106769, w 0.707106769) length 0.99999994, X -> (0, 1.00000012, 0) |
| matrix4_set_from_axisv3_angle | quarter turn about the zero axis, applied to X | (1, 0, 0) | GLM rotate (-nan, -nan, -nan); Eigen AngleAxis matrix (-4.37113883e-08, 0, 0); cglm glm_rotate_make (-4.37113883e-08, 0, 0) |
| quaternion_set_from_axis_anglev3 | quarter turn about the zero axis, applied to X | (x 0, y 0, z 0, w 1) length 1, X -> (1, 0, 0) | GLM angleAxis (x 0, y 0, z 0, w 0.707106769) length 0.707106769, X -> (1, 0, 0); Eigen AngleAxis (x 0, y 0, z 0, w 0.707106769) length 0.707106769, X -> (1, 0, 0); cglm glm_quatv (x 0, y 0, z 0, w 0.707106769) length 0.707106769, X -> (1, 0, 0) |
| matrix4_reciprocal_condition | condition 176238.375: (0.304295689, -0.300753355, 0.260543406, -0.163530409, 0.311520368, -0.292385697, 0.270641655, -0.158564284, -0.260384202, 0.243341565, -0.226126298, 0.13239032, -0.283766091, 0.287824482, -0.241424471, 0.156322032) | 0 | exact 5.67413311e-06, determinant 4.70936909e-11, LAPACK estimate 5.67413311e-06 |
| matrix4_reciprocal_condition | condition 992848.2: (0.0129342815, -0.0449734144, 0.0682325438, 0.0586642474, 0.0917068496, -0.3745386, 0.545777857, 0.497912407, 0.0525199063, -0.230726078, 0.331227779, 0.309261352, 0.0145041309, -0.084460184, 0.114912726, 0.116065487) | 0 | exact 1.00720332e-06, determinant 2.40509248e-12, LAPACK estimate 1.00720332e-06 |
| matrix4_reciprocal_condition | condition 382683.294: (0.478296936, -0.224881455, 0.141815647, 0.329972059, -0.401035637, 0.192583129, -0.11625839, -0.276705831, 0.282752484, -0.13159588, 0.0851294845, 0.194945484, 0.298147202, -0.154780522, 0.0782729313, 0.205980733) | 0 | exact 2.61312688e-06, determinant 1.63351264e-11, LAPACK estimate 2.61312688e-06 |

## Precision against long double

Largest / mean error in units of the float epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over 20000 inputs.  Lower is better; in bold, the smallest mean of each row and the means within 2% of it.

| function | inputs | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|---|
| vector3_normalize | random | **1.16 / 0.32** | 1.28 / 0.369 | **1.14 / 0.321** | 1.28 / 0.369 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | **1.06 / 0.0858** | 1.12 / 0.15 | **1.06 / 0.0857** | 8.39e+06 / 1.81e+05 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | **0.00292 / 0.000282** | 0.5 / 0.075 | **0.00292 / 0.000282** | 0.5 / 0.075 |
| vector3_magnitude | random | **0.935 / 0.217** | **0.935 / 0.217** | **1.01 / 0.218** | **0.935 / 0.217** |
| vector3_dot_product | random | **1.04 / 0.161** | **1.04 / 0.161** | **1.01 / 0.159** | **1.04 / 0.161** |
| vector3_dot_product | nearly perpendicular | **0.536 / 0.0913** | **0.536 / 0.0913** | **0.53 / 0.0913** | **0.536 / 0.0913** |
| vector3_cross_product | random | **0.902 / 0.198** | **0.902 / 0.198** | **0.902 / 0.198** | **0.902 / 0.198** |
| vector3_cross_product | nearly parallel (1e-4 rad) | **0.421 / 0.111** | **0.421 / 0.111** | **0.421 / 0.111** | **0.421 / 0.111** |
| vector3_project | random | **1.48 / 0.207** | **1.48 / 0.207** |  |  |
| vector3_rotate_by_quaternion | unit q | **3.27 / 0.619** | 3.67 / 0.697 | 3.37 / 0.684 | 3.27 / 0.71 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | **3.37 / 0.647** | 40.3 / 11.4 | 40.3 / 11.4 | 3.67 / 0.718 |
| matrix4_multiply | random | **1.2 / 0.366** | **1.2 / 0.366** | **1.2 / 0.366** | **1.2 / 0.366** |
| matrix4_inverse | random entries (error / condition number) | **0.581 / 0.0622** | 0.478 / 0.0646 | 0.581 / 0.0637 | 0.478 / 0.0643 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | **0.208 / 0.0108** | 0.223 / 0.0116 | 0.208 / 0.0115 | 0.249 / 0.0118 |
| matrix4_inverse | condition ~1e4 (error / condition number) | **12.8 / 0.942** | 17.5 / 1.02 | **11.1 / 0.957** | 17.5 / 1.02 |
| matrix3_inverse | random entries (error / condition number) | **0.54 / 0.0747** | 0.587 / 0.0783 | 0.51 / 0.0783 |  |
| matrix4_determinant | random entries | **2.27 / 0.256** | 2.52 / 0.263 | **4.03 / 0.26** | **2.22 / 0.261** |
| matrix4_normal_matrix | rotation, scale and translation | **2.1 / 0.383** | **1.92 / 0.381** |  |  |
| matrix4_set_from_axisv3_angle | random | 2.6 / 0.497 | 4.37 / 0.595 | **2.25 / 0.441** | 4.37 / 0.595 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** |
| matrix4_set_from_quaternion | unit q | **2.68 / 0.582** | 2.08 / 0.595 | 2.08 / 0.595 | **2.68 / 0.583** |
| matrix4_set_from_euler_anglesf3 | random | **1.18 / 0.39** | **1.19 / 0.388** |  | **1.18 / 0.388** |
| matrix4_projection_perspective_fovy_rh | random | **1.27 / 0.351** | 1.2 / 0.36 |  | 1.29 / 0.378 |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | **2.01 / 0.417** | 2.24 / 0.46 |  | 2.24 / 0.46 |
| quaternion_multiply | random | **1.02 / 0.29** | 1.22 / 0.301 | **1.02 / 0.29** | 1.17 / 0.303 |
| quaternion_normalize | random length | **1.3 / 0.279** | 1.47 / 0.357 | **1.29 / 0.279** | **1.29 / 0.279** |
| quaternion_inverse | random length | **1.34 / 0.388** | **1.36 / 0.386** | **1.47 / 0.388** | 1.47 / 0.43 |
| quaternion_set_from_axis_anglev3 | random | 1.36 / 0.3 | **0.913 / 0.239** | **0.913 / 0.239** | 1.83 / 0.304 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** |
| quaternion_get_axis_anglev3 | random (axis * angle) | **1.4 / 0.385** | 21.5 / 0.711 | 1.38 / 0.399 | 11.5 / 0.709 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | **0.534 / 0.0895** | 2.29e+07 / 1.19e+07 | **0.534 / 0.0893** | 1 / 0.377 |
| quaternion_set_from_matrix4 | random rotation | 2 / 0.351 | **1.41 / 0.323** | **1.48 / 0.326** | 1.45 / 0.335 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) | 1.45 / 0.353 | **1.45 / 0.306** | **1.45 / 0.306** | 1.59 / 0.331 |
| quaternion_slerp | random | 1.82 / 0.48 | 1.85 / 0.497 | **1.86 / 0.468** | 155 / 0.62 |
| quaternion_slerp | 1e-3 rad apart | 1.66 / 0.474 | 1.77 / 0.572 | **1.8 / 0.425** | 5.84e+03 / 57.4 |
| quaternion_slerp | 1e-6 rad apart | 1.4 / 0.376 | **1.31 / 0.363** | **1.21 / 0.359** | 6.8 / 1.94 |
| quaternion_get_rotation_tov3 | random (landing error) | **2.04 / 0.538** | 143 / 0.966 | 198 / 0.865 | 143 / 0.908 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | **2.33 / 0.345** | 4.41e+03 / 928 | 1.8e+04 / 1.66e+04 | 8.39e+03 / 8.39e+03 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 1.45 / 0.26 | 8.74 / 8.38 | **0.628 / 0.158** | 8.74 / 8.39 |
| quaternion_angle_between | random | 2.1 / 0.332 |  | **5.74 / 0.271** |  |
| quaternion_angle_between | 1e-4 rad apart | **4.66e+03 / 203** |  | 1.19e+04 / 1.91e+03 |  |

### Oracle check

Each reference against a second computation of it: the largest disagreement over 20000 inputs, in the units of the table above.

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 4.19e-13 |
| vector rotation: quaternion product against the rotation matrix | 3.16e-12 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 2.36e-12 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 2.41e-12 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.338 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.372 |
| angle between quaternions 1e-4 rad apart: |a - b|, |a + b| against a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 5.15e-12 |
