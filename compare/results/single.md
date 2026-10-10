# hypatia comparison: single precision

Tolerance 2e-05 (difference relative to max(1, |reference|)); 2000 random inputs per row.


## vectors

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector2_normalize | glm::normalize | 2000 | 0 | 1.78813934e-07 |  |  |
| vector3_normalize | glm::normalize | 2000 | 0 | 1.78813934e-07 |  |  |
| vector3_normalize | Eigen normalized | 2000 | 0 | 1.1920929e-07 |  |  |
| vector4_normalize | glm::normalize | 2000 | 0 | 1.78813934e-07 |  |  |
| vector3_normalize | cglm glm_vec3_normalize | 2000 | 0 | 2.38418579e-07 |  |  |
| vector4_normalize | cglm glm_vec4_normalize | 2000 | 0 | 1.1920929e-07 |  |  |
| vector3_magnitude | glm::length | 2000 | 0 | 0 |  |  |
| vector2_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector3_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector4_distance | glm::distance | 2000 | 0 | 1.17999099e-07 |  |  |
| vector4_dot_product | glm::dot | 2000 | 0 | 3.81469727e-06 |  |  |
| vector3_cross_product | glm::cross | 2000 | 0 | 0 |  |  |
| vector2_cross_product | a.x b.y - a.y b.x | 2000 | 0 | 0 |  |  |
| vector3_find_normal_axis_between | glm::normalize(glm::cross) | 2000 | 0 | 1.78813934e-07 |  |  |
| vector3_angle_between | glm::angle(normalize, normalize) | 2000 | 0 | 5.70518437e-06 |  | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 8.15305518e-08 |  | a and 3a: parallel |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 1.0081934e-06 |  | 1e-4 rad apart |
| vector2_angle_between | glm::angle(normalize, normalize) | 2000 | **3** | 0.000179022551 | (-3.85436821, 3.45051289), (-5.69850636, 5.1032629) -> 0.000179022551 vs 0 | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector2_angle_between | 0 | 2000 | 0 | 1.1920929e-07 |  | a and 3a: parallel |
| vector3_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector4_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector3_clamp | glm::clamp | 2000 | 0 | 0 |  |  |
| vector3_min, vector3_max | glm::min, glm::max | 2000 | 0 | 0 |  |  |
| vector2_project | glm::proj | 2000 | 0 | 5.96046448e-07 |  |  |
| vector3_project | glm::proj | 2000 | 0 | 5.60561808e-07 |  |  |
| vector4_project | glm::proj | 2000 | 0 | 7.59959221e-07 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 2.38418579e-06 |  |  |
| vector3_rotate_by_quaternion | Eigen q * v | 2000 | 0 | 2.32458115e-06 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 2.38418579e-06 |  | q not unit length |
| vector3_rotate_by_quaternion | cglm glm_quat_rotatev | 2000 | 0 | 2.2649765e-06 |  |  |
| vector3_reflect_by_quaternion | glm q * (v, 0) * q (as documented) | 2000 | 0 | 9.53674316e-07 |  |  |
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
| matrix4_determinant | glm::determinant | 2000 | 0 | 4.57763672e-07 |  |  |
| matrix4_determinant | Eigen long double | 2000 | 0 | 1.50909152e-05 |  | relative to |det|; entries up to 5 |
| matrix4_determinant | LAPACK dgetrf | 2000 | **2** | 2.97964659e-05 | (-0.549197555, -1.03930926, -4.36849403, -3.3059628, -4.25335979, 3.77016473, 3.09588337, -3.4796195, 3.48791218, -0.0984661877, -1.57687294, 4.97995281, 3.30576253, -4.23620415, 4.64047861, 4.892838) -> 1.23892212 vs 1.23895904 | LAPACK in double; single precision rounding of the matrix products |
| matrix3_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix2_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix4_inverse | glm::inverse | 2000 | 0 | 6.95716991e-07 |  |  |
| matrix4_inverse | Eigen inverse | 2000 | 0 | 2.9719177e-07 |  |  |
| matrix4_inverse | LAPACK dgetri | 2000 | 0 | 7.53912432e-07 |  |  |
| matrix4_inverse | cglm glm_mat4_inv | 2000 | 0 | 4.39803414e-07 |  |  |
| matrix3_inverse | glm::inverse | 2000 | 0 | 0 |  |  |
| matrix2_inverse | glm::inverse | 2000 | 0 | 0 |  |  |

## inverse accuracy (largest error over the random matrices)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_inverse | long double | 2000 | 0 | 2.75681075e-07 |  | condition ~1e0; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 2.62285713e-07 |  | condition ~1e0; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 2.38378657e-07 |  | condition ~1e0; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 4.51277059e-08 |  | condition ~1e0; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | 2.55621061e-07 |  | condition ~1e0; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 0.000502360144 |  | condition ~1e3; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 0.000380882882 |  | condition ~1e3; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 0.000339813091 |  | condition ~1e3; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.92015801e-08 |  | condition ~1e3; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | 0.000380882882 |  | condition ~1e3; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 71.7019166 |  | condition ~1e6; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.81750738e-08 |  | condition ~1e6; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_reciprocal_condition | 1 / (norm(A) norm(A^-1)), infinity norm, in long double | 2000 | **602** | 0.164091484 | (0.215862736, -0.279142052, 0.142417699, -0.463687211, -0.172131643, 0.227488145, -0.122099251, 0.376070201, -0.166937441, 0.21884574, -0.115305625, 0.36221388, -0.156877056, 0.19811672, -0.0952156261, 0.330769926) -> 1.57485028e-05 vs 9.04610755e-07 | relative error up to 1e-2 (single) or 1e-5 (double); single precision is limited by the float inverse: see the accuracy table |
| matrix4_normal_matrix | glm::inverseTranspose(mat3(M)) | 2000 | 0 | 2.73340425e-07 |  |  |

## matrix builders

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_make_transformation_translationv3 | glm::translate(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_scalingv3 | glm::scale(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_rotationf_x | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix4_make_transformation_rotationf_y | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix4_make_transformation_rotationf_z | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-07 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 5.96046448e-07 |  | axis not unit length |
| matrix4_set_from_quaternion | glm::mat4_cast | 2000 | 0 | 4.76837158e-07 |  |  |
| matrix4_set_from_quaternion | glm::mat4_cast(normalize(q)) | 2000 | 0 | 8.34465027e-07 |  | q not unit length |
| matrix4_make_transformation_rotationq | Eigen toRotationMatrix | 2000 | 0 | 4.76837158e-07 |  |  |
| matrix4_make_transformation_rotationq | cglm glm_quat_mat4 | 2000 | 0 | 4.17232513e-07 |  |  |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleZYX(z, y, x) = Rz Ry Rx | 2000 | 0 | 1.1920929e-07 |  | X first, then Y, then Z |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleXYZ(x, y, z) | 2000 | **2000** | 1.99991775 | 0.156445712, 1.57098365, 2.97232413 -> (0.000184644348, -0.319985539, -0.947422385, 0, -3.15564357e-05, -0.947422385, 0.319985539, 0, -1, -2.91862489e-05, -0.000185033801, 0, 0, 0, 0, 1) | for reference: GLM's XYZ is Rx Ry Rz |
| matrix4_set_from_euler_anglesf3(x, y, z) | cglm glm_euler_xyz | 2000 | **2000** | 1.99988419 | 2.47305441, -4.72119761, 5.60432434 -> (0.00685556186, -0.0103413165, -0.999922991, 0, -0.00553092454, -0.999931633, 0.0103034377, 0, -0.999961197, 0.00545986323, -0.00691229012, 0, 0, 0, 0, 1) | for reference: cglm's xyz is GLM's XYZ |
| matrix4_set_from_euler_anglesf3(x, y, z) | cglm glm_euler_zyx | 2000 | 0 | 1.1920929e-07 |  |  |
| matrix4_make_transformation_rotationv3(v) | glm::eulerAngleZYX(v.z, v.y, v.x) | 2000 | 0 | 1.1920929e-07 |  |  |
| matrix4_translatev3(M, v) | glm T(v) * M | 2000 | 0 | 0 |  | applies the translation after M |
| matrix4_translatev3(M, v) | glm::translate(M, v) = M * T(v) | 2000 | **2000** | 19.4892239 | (0.578296661, 0.806612968, 0.128700927, -6.24655342, 0.275829107, -1.53047609, -0.598654091, -9.84695721, -0.0627700537, 0.705938041, -1.44493926, 0.51745683, 0, 0, 0, 1), (-4.21842146, -8.84185028, 2.52386999) -> (0.578296661, 0.806612968, 0.128700927, -10.4649754, 0.275829107, -1.53047609, -0.598654091, -18.6888084, -0.0627700537, 0.705938041, -1.44493926, 3.04132676, 0, 0, 0, 1) | for reference: GLM applies it before M |
| matrix4_rotatev3(M, axis, a) | glm R * M | 2000 | 0 | 2.86102295e-06 |  |  |
| matrix4_scalev3(M, v) | glm S * M | 2000 | 0 | 0 |  |  |
| matrix4_transformation_compose(s, q, t) | glm T * R * S | 2000 | 0 | 9.14978693e-07 |  |  |
| matrix4_transformation_decompose | glm::decompose | 2000 | 0 | 2.38418579e-07 |  | scale, rotation (q or -q), translation |
| matrix4_transformation_decompose | cglm glm_decompose | 2000 | 0 | 1.76097805e-07 |  | scale and translation |
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
| matrix4_view_lookat_rh | glm::lookAtRH | 2000 | 0 | 2.4562047e-06 |  |  |
| matrix4_view_lookat_lh | glm::lookAtLH | 2000 | 0 | 2.14379895e-06 |  |  |
| matrix4_projection_perspective_fovy_rh | cglm glm_perspective_rh_zo | 2000 | 0 | 2.01097247e-07 |  |  |
| matrix4_projection_ortho3d_lh | cglm glm_ortho_lh_zo | 2000 | 0 | 1.18742874e-07 |  |  |
| matrix4_view_lookat_rh | cglm glm_lookat_rh | 2000 | 0 | 2.86102295e-06 |  |  |

## quaternions

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| quaternion_multiply(a, b) | glm a * b | 2000 | 0 | 5.96046448e-07 |  |  |
| quaternion_multiply(a, b) | Eigen a * b | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_multiply(a, b) | cglm glm_quat_mul(a, b) | 2000 | 0 | 0 |  |  |
| quaternion_multiplyv3(q, v) | glm q * (v, 0) | 2000 | 0 | 9.53674316e-07 |  |  |
| quaternion_conjugate | glm::conjugate | 2000 | 0 | 0 |  |  |
| quaternion_inverse | glm::inverse | 2000 | 0 | 2.38418579e-07 |  | not unit length |
| quaternion_inverse | Eigen inverse | 2000 | 0 | 3.04922183e-07 |  | not unit length |
| quaternion_inverse | cglm glm_quat_inv | 2000 | 0 | 2.48380389e-07 |  | not unit length |
| quaternion_normalize | glm::normalize | 2000 | 0 | 1.78813934e-07 |  |  |
| quaternion_dot_product, norm, magnitude | glm::dot, dot(q, q), glm::length | 2000 | 0 | 9.53674316e-07 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_set_from_axis_anglev3 | Eigen AngleAxis | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis(a, normalize(axis)) | 2000 | 0 | 1.78813934e-07 |  | axis not unit length |
| quaternion_get_axis_anglev3 | glm::axis, glm::angle | 2000 | 0 | 1.1920929e-07 |  | as a rotation: rebuilt with angleAxis, q or -q |
| quaternion_get_axis_anglev3 | glm::angle | 2000 | **1025** | 0.954176075 | (x -0.0768249929, y 0.0827454254, z 0.0779737309, w -0.990540802) -> 0.275304675 vs 6.00788069 | the angle itself (hypatia [0, pi], GLM [0, 2 pi]) |
| quaternion_get_axis_anglev3 | Eigen AngleAxis(q) | 2000 | 0 | 1.49011612e-07 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | glm::quat(vec3(x, y, z)) | 2000 | 0 | 1.78813934e-07 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | matrix4_set_from_euler_anglesf3(x, y, z) | 2000 | 0 | 4.76837158e-07 |  | the matrix of the quaternion |
| quaternion_get_euler_anglesf3 | glm::eulerAngles | 2000 | 0 | 1.84774399e-06 |  | angles; |y| < 1.5 |
| quaternion_get_euler_anglesf3 | the angles given to quaternion_set_from_euler_anglesf3 | 2000 | 0 | 1.75833702e-07 |  | round trip; |y| < 1.5 |
| quaternion_set_from_matrix4 | glm::quat_cast | 2000 | 0 | 2.98023224e-07 |  | q or -q |
| quaternion_set_from_matrix4 | Eigen Quaternion(Matrix3) | 2000 | 0 | 1.78813934e-07 |  | q or -q |
| quaternion_set_from_matrix4 | cglm glm_mat4_quat | 2000 | 0 | 1.1920929e-07 |  | q or -q |
| quaternion_lerp | glm::lerp | 2000 | 0 | 0 |  |  |
| quaternion_nlerp | glm::normalize(glm::lerp) | 2000 | 0 | 1.78813934e-07 |  |  |
| quaternion_slerp | glm::slerp | 2000 | 0 | 1.78813934e-07 |  | components |
| quaternion_slerp | Eigen slerp | 2000 | 0 | 1.78813934e-07 |  | components |
| quaternion_slerp | cglm glm_quat_slerp | 2000 | 0 | 3.57627869e-07 |  | as a rotation (cglm does not take the shortest arc) |
| quaternion_slerp | glm::slerp | 2000 | 0 | 1.78813934e-07 |  | nearly the same: 1e-3 rad apart |
| quaternion_get_rotation_tov3 | glm::rotation(normalize(a), normalize(b)) | 2000 | 0 | 1.80006027e-05 |  | near opposite GLM is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | **1** | 6.28829002e-05 | (-2.95046353, -3.41663909, -3.21371937), (7.57799435, 8.63873196, 7.83170176) -> (x 0.608427167, y -0.755048931, z 0.24413693, w 0.0107107358) | near opposite Eigen is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | cglm glm_quat_from_vecs(normalized) | 2000 | 0 | 2.12341547e-06 |  |  |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | 0 | 2.98023224e-07 |  | nearly opposite: 1e-3 rad from opposite; rotating a must give b |
| quaternion_angle_between | Eigen angularDistance | 2000 | 0 | 3.16650764e-07 |  |  |
| quaternion_angle_between | long double | 2000 | 0 | 1.89282328e-06 |  | 1e-4 rad apart |
| quaternion_difference | min(|a - b|^2, |a + b|^2) | 2000 | 0 | 1.86263409e-07 |  |  |
| quaternion_rotate_by_quaternion(a, b) | glm::normalize(a * b) | 2000 | 0 | 1.78813934e-07 |  |  |
| quaternion_rotate_by_axis_angle(q, axis, a) | glm::rotate(q, a, axis) = q * angleAxis | 2000 | 0 | 2.38418579e-07 |  |  |
| quaternion_rotate_by_euler_angles(q, x, y, z) | q * glm::quat(vec3(x, y, z)) | 2000 | 0 | 2.38418579e-07 |  | as a rotation |

## accuracy against long double (largest error over the inputs)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector3_angle_between | long double | 2000 | 0 | 2.05864501e-07 |  | random |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 2.64067818e-06 |  | random |
| vector2_angle_between | long double | 2000 | 0 | 1.8198699e-07 |  | random |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 6.56509064e-05 |  | random |
| vector3_angle_between | long double | 2000 | 0 | 8.85778177e-08 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 0.000309465107 |  | 1e-3 rad apart |
| vector2_angle_between | long double | 2000 | 0 | 1.04146304e-07 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 0.000344820948 |  | 1e-3 rad apart |
| vector3_angle_between | long double | 2000 | 0 | 1.88439709e-07 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 0.000309468749 |  | 1e-3 rad from opposite |
| vector2_angle_between | long double | 2000 | 0 | 1.89226673e-07 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 0.000383716721 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.70084451e-07 |  | random |
| glm::rotation | long double | 2000 | 0 | 3.38437249e-06 |  | random |
| Eigen FromTwoVectors | long double | 2000 | 0 | 3.07199738e-06 |  | random |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 4.11568947e-06 |  | random |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.96378113e-07 |  | 1e-3 rad from opposite |
| glm::rotation | long double | 2000 | 0 | 0.000178389325 |  | 1e-3 rad from opposite |
| Eigen FromTwoVectors | long double | 2000 | 0 | 0.0082763187 |  | 1e-3 rad from opposite |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 0.00414754604 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.50216154e-07 |  | exactly opposite (to = -2 from) |
| glm::rotation | long double | 2000 | 0 | 2 |  | exactly opposite (to = -2 from) |
| Eigen FromTwoVectors | long double | 2000 | 0 | 0.000690533985 |  | exactly opposite (to = -2 from) |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 1.58593601e-07 |  | exactly opposite (to = -2 from) |
| matrix4_determinant | long double | 2000 | 0 | 4.76803908e-07 |  | condition up to 1e6; relative to the largest element^4 |
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
| vector3_angle_between | a, 3a: parallel | 0 | GLM angle(normalize, normalize) 0 |
| vector3_angle_between | a, zero | 0 | GLM -nan |
| vector2_angle_between | a, 7a: parallel | 1.4901163e-08 | GLM angle(normalize, normalize) 0 |
| vector3_project | onto zero | (0, 0, 0) | GLM proj (-nan, -nan, -nan) |
| quaternion_normalize | zero | (x 0, y 0, z 0, w 0) | GLM (x 0, y 0, z 0, w 1); Eigen (x 0, y 0, z 0, w 0) |
| quaternion_inverse | zero | (x 0, y 0, z 0, w 0) | GLM (x -nan, y -nan, z -nan, w -nan); Eigen (x 0, y 0, z 0, w 0) |
| quaternion_slerp | q, -q, 0.5 (same rotation) | (x 0, y 0, z 0.47942555, w 0.87758255) | GLM slerp (x 0, y 0, z 0.47942555, w 0.87758255); GLM mix (x -0, y -0, z -0, w -0); Eigen (x 0, y 0, z 0.47942555, w 0.87758255) |
| quaternion_slerp | q, q, 0.5 | (x 0, y 0, z 0.47942555, w 0.87758255) | GLM (x 0, y 0, z 0.47942555, w 0.87758255); Eigen (x 0, y 0, z 0.47942555, w 0.87758255) |
| quaternion_slerp | t = 1.5 (beyond end) | (x 0, y 0, z 0.998531222, w -0.0541771948) | GLM (x 0, y 0, z 0.998531222, w -0.0541771539); Eigen (x 0, y 0, z 0.998531222, w -0.0541771948) |
| quaternion_get_axis_anglev3 | identity | (0, 0, 0), 0 | GLM axis (0, 0, 1), angle 0 |
| quaternion_get_axis_anglev3 | -identity | (-0, -0, -0), 0 | GLM axis (0, 0, 1), angle 6.28318548 |
| quaternion_get_axis_anglev3 | -(1 rad about Z) | (0, 0, 1), 1 | GLM axis (-0, -0, -0.99999994), angle 5.28318548 |
| quaternion_get_rotation_tov3 | a, -a: opposite | (x 0, y 0.832050323, z -0.554700196, w 0) (turns a to (-1.00000012, -2, -3)) | GLM (x 0, y 0, z 0, w 0.000244140625) (turns a to (1, 2, 3)); Eigen (x 0.119522862, y -0.844013155, z 0.52283448, w 0.000244140625) |
| quaternion_get_rotation_tov3 | a, zero | (x 0, y 0, z 0, w 1) | GLM (x -nan, y -nan, z -nan, w -nan); Eigen (x 0, y 0, z 0, w 0.707106769) |
| matrix4_inverse | singular (1 .. 16) | NULL | GLM (-nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan); determinant: hypatia 0, GLM 0 |
| matrix4_inverse | nearly singular | NULL | LAPACK rcond (double) see table; GLM determinant 0 |
| matrix4_transformation_decompose | zero scale on x | 0 (fails) | GLM false, scale (4.59149455e-41, -1.98410356e+30, 4.59149455e-41) |
| matrix4_view_lookat_rh | eye == target | (0, 0, 0, -0, 0, 0, 0, -0, -0, -0, -0, 0, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, nan, nan, nan, -nan, 0, 0, 0, 1) |
| matrix4_view_lookat_rh | looking along up | (0, 0, 0, -0, 0, 0, 0, -0, -0, -1, -0, 2, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, -0, -1, -0, 2, 0, 0, 0, 1) |
| matrix4_projection_perspective_fovy_rh | zNear == zFar | (1.83048773, 0, 0, 0, 0, 1.83048773, 0, 0, 0, 0, inf, inf, 0, 0, -1, 0) | GLM (1.83048773, 0, 0, 0, 0, 1.83048773, 0, 0, 0, 0, inf, -inf, 0, 0, -1, 0) |
| matrix4_reciprocal_condition | condition 382683.294: (0.478296936, -0.224881455, 0.141815647, 0.329972059, -0.401035637, 0.192583129, -0.11625839, -0.276705831, 0.282752484, -0.13159588, 0.0851294845, 0.194945484, 0.298147202, -0.154780522, 0.0782729313, 0.205980733) | 0 | exact 2.61312688e-06, determinant 1.63351264e-11, LAPACK estimate 2.61312688e-06 |
| matrix4_reciprocal_condition | condition 764080.973: (0.0106293159, 0.245246425, -0.44132784, -0.0994636789, -0.00998749211, -0.294274837, 0.537353218, 0.131597131, 0.00865735393, 0.271072865, -0.496834368, -0.123884082, -0.00323972898, -0.0414853878, 0.0698970109, 0.00989153329) | 0 | exact 1.30876181e-06, determinant 3.22128679e-12, LAPACK estimate 1.30876181e-06 |
