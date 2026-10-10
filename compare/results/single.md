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
| vector2_reflect | glm::reflect(v, normalize(n)) | 2000 | 0 | 2.86102295e-06 |  |  |
| vector3_reflect | glm::reflect(v, normalize(n)) | 2000 | 0 | 2.86102295e-06 |  |  |
| vector3_refract | glm::refract(normalize(v), normalize(n), eta) | 2000 | 0 | 1.25169754e-06 |  |  |
| vector3_reflect | cglm glm_vec3_reflect(v, normalized n) | 2000 | 0 | 2.86102295e-06 |  |  |
| vector3_refract | cglm glm_vec3_refract(normalized v, normalized n, eta) | 2000 | **2000** | 1.21085265 | (1.51273596, -6.12728214, -1.8801657) through (9.04939461, 3.0933702, 2.41114211), 1.00732732 -> (0.286812454, -0.91831404, -0.272833318) vs (-0.924040198, -1.33222198, -0.595455766) | cglm 0.9.4 computes k = 1 + eta^2 - (eta n.v)^2; Snell's law gives 1 - eta^2 + (eta n.v)^2 |
| vector4_project | glm::proj | 2000 | 0 | 3.46519317e-07 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 2.32458115e-06 |  |  |
| vector3_rotate_by_quaternion | Eigen q * v | 2000 | 0 | 2.47359276e-06 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 3.42726707e-06 |  | q not unit length |
| vector3_rotate_by_quaternion | cglm glm_quat_rotatev | 2000 | 0 | 2.08616257e-06 |  |  |
| vector3_reflect_by_quaternion | glm q * (v, 0) * q (as documented) | 2000 | 0 | 7.76451469e-07 |  |  |
| vector3_multiplym4 | glm (M * (v, 1)).xyz | 2000 | 0 | 0 |  |  |
| vector2_multiplym2 | glm M * v | 2000 | 0 | 0 |  |  |
| vector2_multiplym3 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  | README: sets self to mT * self |
| matrix3_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix2_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix4_multiply(A, B) | cglm glm_mat4_mul(B, A) | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv4 | glm M * v | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv3 | glm (M * (v, 1)).xyz | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv2 | glm (M * (v, 0, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix3_multiplyv2 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix2_multiplyv2 | glm M * v | 2000 | 0 | 0 |  |  |
| matrix4_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix3_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix4_determinant | glm::determinant | 2000 | 0 | 4.86102334e-07 |  |  |
| matrix4_determinant | Eigen long double | 2000 | 0 | 1.39230626e-05 |  | relative to |det|; entries up to 5 |
| matrix4_determinant | LAPACK dgetrf | 2000 | **3** | 5.71764533e-05 | (-3.96927905, 2.24306917, 3.2305336, 0.581109405, 3.87497973, 3.76452136, 4.55389071, 3.56331873, 2.81962442, -2.83297372, 4.1208601, -2.34307408, -2.39781475, -2.97633862, -4.69459963, -2.51150775) -> -0.663635254 vs -0.66369243 | LAPACK in double; single precision rounding of the matrix products |
| matrix3_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix2_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix4_inverse | glm::inverse | 2000 | 0 | 4.84309092e-07 |  |  |
| matrix4_inverse | Eigen inverse | 2000 | 0 | 8.39835007e-07 |  |  |
| matrix4_inverse | LAPACK dgetri | 2000 | 0 | 7.77049453e-07 |  |  |
| matrix4_inverse | cglm glm_mat4_inv | 2000 | 0 | 8.07685083e-06 |  |  |
| matrix3_inverse | glm::inverse | 2000 | 0 | 1.19059653e-09 |  |  |
| matrix2_inverse | glm::inverse | 2000 | 0 | 1.19208408e-09 |  |  |

## inverse accuracy (largest error over the random matrices)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_inverse | long double | 2000 | 0 | 2.35509236e-07 |  | condition ~1e0; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 2.52329656e-07 |  | condition ~1e0; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 2.24256218e-07 |  | condition ~1e0; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 4.81819894e-08 |  | condition ~1e0; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | 2.52329656e-07 |  | condition ~1e0; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 0.000377807293 |  | condition ~1e3; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 0.000561638077 |  | condition ~1e3; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 0.000530452673 |  | condition ~1e3; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.78404844e-08 |  | condition ~1e3; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | 0.000561638077 |  | condition ~1e3; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 280.367665 |  | condition ~1e6; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | inf |  | condition ~1e6; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.76172896e-08 |  | condition ~1e6; error relative to the largest element |
| cglm glm_mat4_inv | long double | 2000 | 0 | 281.229935 |  | condition ~1e6; error relative to the largest element |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_reciprocal_condition | 1 / (norm(A) norm(A^-1)), infinity norm, in long double | 2000 | **570** | 0.125037113 | (0.0581810623, -0.453742653, 0.322835892, -0.329227418, -0.0546527132, 0.413402349, -0.290837169, 0.29594636, -0.0430154279, 0.304685295, -0.208947122, 0.211976603, -0.0164392982, 0.150414273, -0.112632006, 0.115474865) -> 1.22408046e-05 vs 9.06477064e-07 | relative error up to 1e-2 (single) or 1e-5 (double); single precision is limited by the float inverse: see the accuracy table |
| matrix4_normal_matrix | glm::inverseTranspose(mat3(M)) | 2000 | 0 | 2.32271959e-07 |  |  |

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
| matrix4_set_from_quaternion | glm::mat4_cast | 2000 | 0 | 3.57627869e-07 |  |  |
| matrix4_set_from_quaternion | glm::mat4_cast(normalize(q)) | 2000 | 0 | 6.55651093e-07 |  | q not unit length |
| matrix4_make_transformation_rotationq | Eigen toRotationMatrix | 2000 | 0 | 4.17232513e-07 |  |  |
| matrix4_make_transformation_rotationq | cglm glm_quat_mat4 | 2000 | 0 | 3.57627869e-07 |  |  |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleZYX(z, y, x) = Rz Ry Rx | 2000 | 0 | 1.1920929e-07 |  | X first, then Y, then Z |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleXYZ(x, y, z) | 2000 | **2000** | 1.99871397 | 0.232920364, -1.53463268, -3.36952257 -> (-0.0352206379, 0.00484339893, 0.999367833, 0, 0.00816980843, -0.999953389, 0.00513416529, 0, 0.999346137, 0.00834547263, 0.0351794288, 0, 0, 0, 0, 1) | for reference: GLM's XYZ is Rx Ry Rz |
| matrix4_set_from_euler_anglesf3(x, y, z) | cglm glm_euler_xyz | 2000 | **2000** | 1.9993434 | -4.54207706, -1.53809547, -1.38597202 -> (0.00600848999, -0.347621262, -0.937615752, 0, -0.0321381837, 0.937081218, -0.347629011, 0, 0.999465346, 0.0322219916, -0.00554147176, 0, 0, 0, 0, 1) | for reference: cglm's xyz is GLM's XYZ |
| matrix4_set_from_euler_anglesf3(x, y, z) | cglm glm_euler_zyx | 2000 | 0 | 1.1920929e-07 |  |  |
| matrix4_make_transformation_rotationv3(v) | glm::eulerAngleZYX(v.z, v.y, v.x) | 2000 | 0 | 1.1920929e-07 |  |  |
| matrix4_translatev3(M, v) | glm T(v) * M | 2000 | 0 | 0 |  | applies the translation after M |
| matrix4_translatev3(M, v) | glm::translate(M, v) = M * T(v) | 2000 | **2000** | 19.4097328 | (-0.00976022799, 1.37613583, -1.44391334, 1.16952491, -0.890396833, -0.881139338, -1.92541027, 0.869939983, -2.14213991, 0.359982222, 0.806890547, -9.85259247, 0, 0, 0, 1), (-8.5738945, -1.8014307, -8.76050282) -> (-0.00976022799, 1.37613583, -1.44391334, -7.40436935, -0.890396833, -0.881139338, -1.92541027, -0.931490719, -2.14213991, 0.359982222, 0.806890547, -18.6130943, 0, 0, 0, 1) | for reference: GLM applies it before M |
| matrix4_rotatev3(M, axis, a) | glm R * M | 2000 | 0 | 2.50339508e-06 |  |  |
| matrix4_scalev3(M, v) | glm S * M | 2000 | 0 | 0 |  |  |
| matrix4_transformation_compose(s, q, t) | glm T * R * S | 2000 | 0 | 1.11468964e-06 |  |  |
| matrix4_transformation_decompose | glm::decompose | 2000 | 0 | 1.78813934e-07 |  | scale, rotation (q or -q), translation |
| matrix4_transformation_decompose | cglm glm_decompose | 2000 | 0 | 0 |  | scale and translation |
| matrix3_make_transformation_rotationf_z | glm::rotate(mat3(1), a) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix3_make_transformation_translationv2 | glm::translate(mat3(1), v) | 2000 | 0 | 0 |  |  |
| matrix3_make_transformation_scalingv2 | glm::scale(mat3(1), v) | 2000 | 0 | 0 |  |  |
| matrix3_rotate(M, a) | glm R * M | 2000 | 0 | 2.88174369e-07 |  |  |
| matrix3_translatev2(M, v) | glm T * M | 2000 | 0 | 0 |  |  |
| matrix2_make_transformation_rotationf_z | mat2 of glm::rotate(mat3(1), a) | 2000 | 0 | 5.96046448e-08 |  |  |
| matrix2_rotate(M, a) | glm R * M | 2000 | 0 | 4.76837158e-07 |  |  |

## projections

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_projection_perspective_fovy_rh | glm::perspectiveRH_ZO | 2000 | 0 | 1.62205515e-07 |  |  |
| matrix4_projection_perspective_fovy_lh | glm::perspectiveLH_ZO | 2000 | 0 | 2.29702114e-07 |  |  |
| matrix4_projection_ortho3d_rh | glm::orthoRH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_projection_ortho3d_lh | glm::orthoLH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_view_lookat_rh | glm::lookAtRH | 2000 | 0 | 2.38418579e-06 |  |  |
| matrix4_view_lookat_lh | glm::lookAtLH | 2000 | 0 | 2.38418579e-06 |  |  |
| matrix4_projection_frustum_rh | glm::frustumRH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_projection_frustum_lh | glm::frustumLH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_projection_perspective_fovy_infinite_rh | glm::infinitePerspectiveRH_ZO | 2000 | 0 | 2.02500291e-07 |  |  |
| matrix4_projection_perspective_fovy_infinite_lh | glm::infinitePerspectiveLH_ZO | 2000 | 0 | 2.27740864e-07 |  |  |
| vector3_project_to_window | glm::project_ZO(p, identity, M, viewport) | 2000 | 0 | 0 |  |  |
| vector3_unproject_from_window | glm::unProject_ZO(w, identity, M, viewport) | 2000 | 0 | 1.85966492e-05 |  |  |
| matrix4_projection_perspective_fovy_rh | cglm glm_perspective_rh_zo | 2000 | 0 | 2.24846e-07 |  |  |
| matrix4_projection_ortho3d_lh | cglm glm_ortho_lh_zo | 2000 | 0 | 1.18953561e-07 |  |  |
| matrix4_view_lookat_rh | cglm glm_lookat_rh | 2000 | 0 | 2.86102295e-06 |  |  |

## quaternions

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| quaternion_multiply(a, b) | glm a * b | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_multiply(a, b) | Eigen a * b | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_multiply(a, b) | cglm glm_quat_mul(a, b) | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_multiplyv3(q, v) | glm q * (v, 0) | 2000 | 0 | 9.53674316e-07 |  |  |
| quaternion_conjugate | glm::conjugate | 2000 | 0 | 0 |  |  |
| quaternion_inverse | glm::inverse | 2000 | 0 | 1.93413459e-07 |  | not unit length |
| quaternion_inverse | Eigen inverse | 2000 | 0 | 1.52117468e-07 |  | not unit length |
| quaternion_inverse | cglm glm_quat_inv | 2000 | 0 | 1.16530247e-07 |  | not unit length |
| quaternion_normalize | glm::normalize | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_dot_product, norm, magnitude | glm::dot, dot(q, q), glm::length | 2000 | 0 | 4.76837158e-07 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_set_from_axis_anglev3 | Eigen AngleAxis | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis(a, normalize(axis)) | 2000 | 0 | 1.1920929e-07 |  | axis not unit length |
| quaternion_get_axis_anglev3 | glm::axis, glm::angle | 2000 | 0 | 8.94069672e-08 |  | as a rotation: rebuilt with angleAxis, q or -q |
| quaternion_get_axis_anglev3 | glm::angle | 2000 | **995** | 0.940164921 | (x 0.132013857, y -0.116670899, z -0.00948799402, w -0.984312057) -> 0.354729623 vs 5.92845583 | the angle itself (hypatia [0, pi], GLM [0, 2 pi]) |
| quaternion_get_axis_anglev3 | Eigen AngleAxis(q) | 2000 | 0 | 1.49011612e-07 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | glm::quat(vec3(x, y, z)) | 2000 | 0 | 1.78813934e-07 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | matrix4_set_from_euler_anglesf3(x, y, z) | 2000 | 0 | 3.27825546e-07 |  | the matrix of the quaternion |
| quaternion_get_euler_anglesf3 | glm::eulerAngles | 2000 | 0 | 1.60932541e-06 |  | angles; |y| < 1.5 |
| quaternion_get_euler_anglesf3 | the angles given to quaternion_set_from_euler_anglesf3 | 2000 | 0 | 1.47758566e-07 |  | round trip; |y| < 1.5 |
| quaternion_set_from_matrix4 | glm::quat_cast | 2000 | 0 | 2.98023224e-07 |  | q or -q |
| quaternion_set_from_matrix4 | Eigen Quaternion(Matrix3) | 2000 | 0 | 1.78813934e-07 |  | q or -q |
| quaternion_set_from_matrix4 | cglm glm_mat4_quat | 2000 | 0 | 1.78813934e-07 |  | q or -q |
| quaternion_lerp | glm::lerp | 2000 | 0 | 0 |  |  |
| quaternion_nlerp | glm::normalize(glm::lerp) | 2000 | 0 | 1.78813934e-07 |  |  |
| quaternion_slerp | glm::slerp | 2000 | 0 | 1.78813934e-07 |  | components |
| quaternion_slerp | Eigen slerp | 2000 | 0 | 1.78813934e-07 |  | components |
| quaternion_slerp | cglm glm_quat_slerp | 2000 | 0 | 1.90734863e-06 |  | as a rotation (cglm does not take the shortest arc) |
| quaternion_slerp | glm::slerp | 2000 | 0 | 2.38418579e-07 |  | nearly the same: 1e-3 rad apart |
| quaternion_set_look_rotation_rh | glm::quatLookAtRH(normalize(d), up) | 2000 | 0 | 8.19563866e-07 |  |  |
| quaternion_set_look_rotation_lh | glm::quatLookAtLH(normalize(d), up) | 2000 | 0 | 7.74860382e-07 |  |  |
| quaternion_set_look_rotation_rh | cglm glm_quat_for(d, up) | 2000 | 0 | 2.38418579e-07 |  |  |
| quaternion_get_rotation_tov3 | glm::rotation(normalize(a), normalize(b)) | 2000 | **1** | 2.89678574e-05 | (-6.00857782, -8.03168106, 9.40004635), (4.47876978, 5.64326906, -6.85126352) -> (x 0.658038497, y 0.310479343, z 0.685905814, w 0.0110089118) | near opposite GLM is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | **1** | 4.74452972e-05 | (-9.33650017, 3.04972005, 7.517313), (7.88128662, -2.65144897, -6.78153849) -> (x -0.178539068, y -0.968780637, z 0.171281904, w 0.0158266686) | near opposite Eigen is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | cglm glm_quat_from_vecs(normalized) | 2000 | 0 | 8.18027183e-06 |  |  |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | 0 | 2.38418579e-07 |  | nearly opposite: 1e-3 rad from opposite; rotating a must give b |
| quaternion_angle_between | Eigen angularDistance | 2000 | 0 | 2.92469353e-07 |  |  |
| quaternion_angle_between | long double | 2000 | 0 | 4.90460674e-07 |  | 1e-4 rad apart |
| quaternion_difference | min(|a - b|^2, |a + b|^2) | 2000 | 0 | 1.47746453e-07 |  |  |
| quaternion_rotate_by_quaternion(a, b) | glm::normalize(a * b) | 2000 | 0 | 1.1920929e-07 |  |  |
| quaternion_rotate_by_axis_angle(q, axis, a) | glm::rotate(q, a, axis) = q * angleAxis | 2000 | 0 | 1.78813934e-07 |  |  |
| quaternion_rotate_by_euler_angles(q, x, y, z) | q * glm::quat(vec3(x, y, z)) | 2000 | 0 | 1.78813934e-07 |  | as a rotation |

## accuracy against long double (largest error over the inputs)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector3_angle_between | long double | 2000 | 0 | 1.89507592e-07 |  | random |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 4.22191721e-06 |  | random |
| vector2_angle_between | long double | 2000 | 0 | 1.73003413e-07 |  | random |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 9.25425642e-05 |  | random |
| vector3_angle_between | long double | 2000 | 0 | 8.30523786e-08 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 0.000227966772 |  | 1e-3 rad apart |
| vector2_angle_between | long double | 2000 | 0 | 9.16416173e-08 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 0.000301439011 |  | 1e-3 rad apart |
| vector3_angle_between | long double | 2000 | 0 | 1.72836296e-07 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 0.000227938152 |  | 1e-3 rad from opposite |
| vector2_angle_between | long double | 2000 | 0 | 1.79498387e-07 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 0.000251916778 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.22291392e-07 |  | random |
| glm::rotation | long double | 2000 | 0 | 4.69529901e-06 |  | random |
| Eigen FromTwoVectors | long double | 2000 | 0 | 4.54144719e-06 |  | random |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 2.5767913e-06 |  | random |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.6892559e-07 |  | 1e-3 rad from opposite |
| glm::rotation | long double | 2000 | 0 | 0.000176808982 |  | 1e-3 rad from opposite |
| Eigen FromTwoVectors | long double | 2000 | 0 | 0.00802592924 |  | 1e-3 rad from opposite |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 0.00433419419 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 1.5219254e-07 |  | exactly opposite (to = -2 from) |
| glm::rotation | long double | 2000 | 0 | 2 |  | exactly opposite (to = -2 from) |
| Eigen FromTwoVectors | long double | 2000 | 0 | 0.000690533953 |  | exactly opposite (to = -2 from) |
| cglm glm_quat_from_vecs | long double | 2000 | 0 | 1.72604002e-07 |  | exactly opposite (to = -2 from) |
| matrix4_determinant | long double | 2000 | 0 | 4.25795349e-07 |  | condition up to 1e6; relative to the largest element^4 |
| glm::determinant | long double | 2000 | 0 | 3.97763296e-07 |  | condition up to 1e6; relative to the largest element^4 |
| Eigen determinant | long double | 2000 | 0 | 2.48935524e-07 |  | condition up to 1e6; relative to the largest element^4 |
| matrix4_reciprocal_condition | long double | 2000 | 0 | inf |  | condition up to 1e6; |log10(result / exact)| |
| 1 / (norm(A) norm(glm::inverse(A))) | long double | 2000 | 0 | 1.89685956 |  | condition up to 1e6; |log10(result / exact)| |
| LAPACK dgecon estimate (double) | long double | 2000 | 0 | 0.494460555 |  | condition up to 1e6; |log10(result / exact)| |

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
| matrix4_transformation_decompose | zero scale on x | 0 (fails) | GLM false, scale (-5.41779469e-33, 4.59163468e-41, -1.19141486e-14) |
| matrix4_view_lookat_rh | eye == target | (0, 0, 0, -0, 0, 0, 0, -0, -0, -0, -0, 0, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, nan, nan, nan, -nan, 0, 0, 0, 1) |
| matrix4_view_lookat_rh | looking along up | (0, 0, 0, -0, 0, 0, 0, -0, -0, -1, -0, 2, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, -0, -1, -0, 2, 0, 0, 0, 1) |
| matrix4_projection_perspective_fovy_rh | zNear == zFar | (1.83048773, 0, 0, 0, 0, 1.83048773, 0, 0, 0, 0, inf, inf, 0, 0, -1, 0) | GLM (1.83048773, 0, 0, 0, 0, 1.83048773, 0, 0, 0, 0, inf, -inf, 0, 0, -1, 0) |
| vector3_rotate_by_quaternion | X by 2 * (quarter turn about Z) | (0, 1, 0) | GLM q * v (-3.00000048, 4.00000048, 0); Eigen q * v (-3.00000048, 4.00000048, 0); cglm glm_quat_rotatev (0, 1.00000012, 0) |
| vector3_rotate_by_quaternion | X by the zero quaternion | (1, 0, 0) | GLM q * v (1, 0, 0); Eigen q * v (1, 0, 0); cglm glm_quat_rotatev (1, 0, 0) |
| matrix4_set_from_axisv3_angle | quarter turn about (0, 0, 10), applied to X | (-4.37113883e-08, 1, 0) | GLM rotate (-4.37113883e-08, 1, 0); Eigen AngleAxis matrix (-4.37113883e-08, 10, 0); cglm glm_rotate_make (-4.37113883e-08, 1, 0) |
| quaternion_set_from_axis_anglev3 | quarter turn about (0, 0, 10), applied to X | (x 0, y 0, z 0.707106829, w 0.707106829) length 1, X -> (0, 1, 0) | GLM angleAxis (x 0, y 0, z 7.07106781, w 0.707106769) length 7.10633516, X -> (-99, 10, 0); Eigen AngleAxis (x 0, y 0, z 7.07106781, w 0.707106769) length 7.10633516, X -> (-99, 10, 0); cglm glm_quatv (x 0, y 0, z 0.707106769, w 0.707106769) length 0.99999994, X -> (0, 1.00000012, 0) |
| matrix4_set_from_axisv3_angle | quarter turn about the zero axis, applied to X | (1, 0, 0) | GLM rotate (-nan, -nan, -nan); Eigen AngleAxis matrix (-4.37113883e-08, 0, 0); cglm glm_rotate_make (-4.37113883e-08, 0, 0) |
| quaternion_set_from_axis_anglev3 | quarter turn about the zero axis, applied to X | (x 0, y 0, z 0, w 1) length 1, X -> (1, 0, 0) | GLM angleAxis (x 0, y 0, z 0, w 0.707106769) length 0.707106769, X -> (1, 0, 0); Eigen AngleAxis (x 0, y 0, z 0, w 0.707106769) length 0.707106769, X -> (1, 0, 0); cglm glm_quatv (x 0, y 0, z 0, w 0.707106769) length 0.707106769, X -> (1, 0, 0) |
| matrix4_reciprocal_condition | condition 710781.223: (0.36144647, 0.317905307, -0.221066371, 0.445509374, -0.10202913, -0.0833959728, 0.0631959513, -0.118042424, 0.333209842, 0.278727174, -0.205455512, 0.393426239, 0.170287475, 0.141520008, -0.104979962, 0.200118214) | 0 | exact 1.40690267e-06, determinant 3.82577813e-12, LAPACK estimate 1.40690267e-06 |
| matrix4_reciprocal_condition | condition 643841.654: (0.261274606, -0.125835329, -0.116278775, -0.568873763, 0.180968046, -0.0797992647, -0.0835543796, -0.384923786, 0.161843747, -0.0823969468, -0.0699409693, -0.358106583, 0.193427905, -0.0824080482, -0.0907986462, -0.407583833) | 0 | exact 1.55317692e-06, determinant 6.97421393e-12, LAPACK estimate 1.55317692e-06 |

## Precision against long double

Largest / mean error in units of the float epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over 20000 inputs.  Lower is better; in bold, the smallest mean of each row and the means within 2% of it.

| function | inputs | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|---|
| vector3_normalize | random | **1.12 / 0.32** | 1.35 / 0.37 | **1.11 / 0.319** | 1.35 / 0.37 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | **1.07 / 0.0855** | 1.23 / 0.15 | **1.07 / 0.0857** | 8.39e+06 / 1.77e+05 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | **0.00312 / 0.000279** | 0.5 / 0.076 | **0.00312 / 0.000279** | 0.5 / 0.076 |
| vector3_magnitude | random | **1.02 / 0.218** | **1.02 / 0.218** | **0.933 / 0.219** | **1.02 / 0.218** |
| vector3_dot_product | random | **1.04 / 0.16** | **1.04 / 0.16** | **1.03 / 0.16** | **1.04 / 0.16** |
| vector3_dot_product | nearly perpendicular | **0.568 / 0.0914** | **0.568 / 0.0914** | **0.584 / 0.0913** | **0.568 / 0.0914** |
| vector3_cross_product | random | **0.764 / 0.197** | **0.764 / 0.197** | **0.764 / 0.197** | **0.764 / 0.197** |
| vector3_cross_product | nearly parallel (1e-4 rad) | **0.46 / 0.111** | **0.46 / 0.111** | **0.46 / 0.111** | **0.46 / 0.111** |
| vector3_project | random | **1.7 / 0.212** | **1.7 / 0.212** |  |  |
| vector3_rotate_by_quaternion | unit q | **3.21 / 0.621** | 3.51 / 0.698 | 3.51 / 0.684 | 3.46 / 0.719 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | **3.09 / 0.645** | 41.3 / 11.3 | 40.9 / 11.3 | 3.46 / 0.718 |
| matrix4_multiply | random | **1.31 / 0.366** | **1.31 / 0.366** | **1.31 / 0.366** | **1.31 / 0.366** |
| matrix4_inverse | random entries (error / condition number) | **0.517 / 0.0627** | 0.439 / 0.0645 | 0.704 / 0.0646 | 0.439 / 0.0645 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | **0.199 / 0.0109** | 0.206 / 0.0116 | 0.202 / 0.0116 | 0.206 / 0.0119 |
| matrix4_inverse | condition ~1e4 (error / condition number) | **13.3 / 0.961** | 14.1 / 1.03 | **14.5 / 0.963** | 14.1 / 1.03 |
| matrix3_inverse | random entries (error / condition number) | **0.493 / 0.0756** | 0.488 / 0.0794 | 0.536 / 0.0791 |  |
| matrix4_determinant | random entries | **3.73 / 0.257** | **2.7 / 0.261** | **2.89 / 0.258** | **2.7 / 0.259** |
| matrix4_normal_matrix | rotation, scale and translation | **2.01 / 0.38** | **1.82 / 0.381** |  |  |
| matrix4_set_from_axisv3_angle | random | 2.52 / 0.5 | 4.61 / 0.6 | **2.1 / 0.441** | 4.61 / 0.6 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** |
| matrix4_set_from_quaternion | unit q | **2.9 / 0.582** | 2.34 / 0.597 | 2.34 / 0.597 | **2.9 / 0.582** |
| matrix4_set_from_euler_anglesf3 | random | **1.09 / 0.391** | **1.1 / 0.39** |  | **1.12 / 0.39** |
| matrix4_projection_perspective_fovy_rh | random | **1.3 / 0.351** | 1.22 / 0.359 |  | 1.36 / 0.379 |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | **2.09 / 0.421** | 2.09 / 0.46 |  | 2.09 / 0.46 |
| quaternion_multiply | random | **1.07 / 0.288** | 1.14 / 0.3 | **0.929 / 0.289** | 1.22 / 0.301 |
| quaternion_normalize | random length | **1.22 / 0.28** | 1.37 / 0.352 | **1.34 / 0.28** | **1.34 / 0.28** |
| quaternion_inverse | random length | **1.58 / 0.386** | **1.4 / 0.386** | **1.51 / 0.387** | 1.59 / 0.432 |
| quaternion_set_from_axis_anglev3 | random | 1.35 / 0.3 | **0.954 / 0.237** | **0.954 / 0.237** | 1.78 / 0.306 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** |
| quaternion_get_axis_anglev3 | random (axis * angle) | **1.33 / 0.384** | 29.7 / 0.714 | 1.44 / 0.396 | 12.1 / 0.711 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | **0.534 / 0.0884** | 2.28e+07 / 1.19e+07 | **0.534 / 0.0888** | 1.01 / 0.374 |
| quaternion_set_from_matrix4 | random rotation | 1.87 / 0.354 | **1.44 / 0.329** | **1.5 / 0.333** | 1.66 / 0.34 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) | 1.44 / 0.352 | **1.4 / 0.304** | **1.4 / 0.305** | 1.67 / 0.33 |
| quaternion_slerp | random | **1.78 / 0.48** | 1.78 / 0.497 | **1.71 / 0.471** | 66.7 / 0.617 |
| quaternion_slerp | 1e-3 rad apart | 1.6 / 0.474 | 2.21 / 0.578 | **1.68 / 0.425** | 5.67e+03 / 54.5 |
| quaternion_slerp | 1e-6 rad apart | 1.5 / 0.377 | **1.69 / 0.362** | **1.23 / 0.357** | 7.08 / 1.93 |
| quaternion_get_rotation_tov3 | random (landing error) | **2.33 / 0.534** | 157 / 0.933 | 98.9 / 0.858 | 157 / 0.876 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | **2.26 / 0.35** | 4.4e+03 / 937 | 1.8e+04 / 1.66e+04 | 8.39e+03 / 8.39e+03 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 1.43 / 0.257 | 8.74 / 8.38 | **0.619 / 0.158** | 8.74 / 8.39 |
| quaternion_angle_between | random | 1.69 / 0.33 |  | **4.21 / 0.273** |  |
| quaternion_angle_between | 1e-4 rad apart | **4.23e+03 / 206** |  | 1.1e+04 / 1.92e+03 |  |
| vector3_reflect | random (relative to the length of v) | **3.28 / 0.464** | 4.31 / 0.662 |  | 4.31 / 0.662 |
| vector3_refract | random, eta 0.5 .. 2 | **285 / 0.411** | 285 / 0.448 |  | 2.5e+07 / 9.23e+06 |
| quaternion_set_look_rotation_rh | random | 8.5 / 0.433 | **8.5 / 0.418** |  | 8.5 / 0.432 |
| vector3_project_to_window | random camera, a point in the view | **390 / 2.21** | **390 / 2.21** |  |  |
| vector3_unproject_from_window | random camera, window depth 0 .. 0.9 | **40 / 1.92** | 65.3 / 2.21 |  |  |

### Oracle check

Each reference against a second computation of it: the largest disagreement over 20000 inputs, in the units of the table above.

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 4.18e-13 |
| vector rotation: quaternion product against the rotation matrix | 3.33e-12 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 2.96e-12 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 2.35e-12 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.339 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.32 |
| angle between quaternions 1e-4 rad apart: from a - b and a + b against from a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 5.52e-12 |
