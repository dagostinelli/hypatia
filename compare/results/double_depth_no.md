# hypatia comparison: double precision, HYP_DEPTH_MINUS_ONE_TO_ONE

Tolerance 1e-10 (difference relative to max(1, |reference|)); 2000 random inputs per row.


## vectors

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector2_normalize | glm::normalize | 2000 | 0 | 1.11022302e-16 |  |  |
| vector3_normalize | glm::normalize | 2000 | 0 | 1.11022302e-16 |  |  |
| vector3_normalize | Eigen normalized | 2000 | 0 | 0 |  |  |
| vector4_normalize | glm::normalize | 2000 | 0 | 1.11022302e-16 |  |  |
| vector3_magnitude | glm::length | 2000 | 0 | 0 |  |  |
| vector2_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector3_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector4_distance | glm::distance | 2000 | 0 | 2.3710079e-16 |  |  |
| vector4_dot_product | glm::dot | 2000 | 0 | 7.10542736e-15 |  |  |
| vector3_cross_product | glm::cross | 2000 | 0 | 0 |  |  |
| vector2_cross_product | a.x b.y - a.y b.x | 2000 | 0 | 0 |  |  |
| vector3_find_normal_axis_between | glm::normalize(glm::cross) | 2000 | 0 | 1.11022302e-16 |  |  |
| vector3_angle_between | glm::angle(normalize, normalize) | 2000 | 0 | 3.66373598e-15 |  | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 1.57009246e-16 |  | a and 3a: parallel |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 7.45117943e-19 |  | 1e-4 rad apart |
| vector2_angle_between | glm::angle(normalize, normalize) | 2000 | 0 | 1.56943685e-12 |  | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector2_angle_between | 0 | 2000 | 0 | 1.66533454e-16 |  | a and 3a: parallel |
| vector3_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector4_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector3_clamp | glm::clamp | 2000 | 0 | 0 |  |  |
| vector3_min, vector3_max | glm::min, glm::max | 2000 | 0 | 0 |  |  |
| vector2_project | glm::proj | 2000 | 0 | 0 |  |  |
| vector3_project | glm::proj | 2000 | 0 | 0 |  |  |
| vector2_reflect | glm::reflect(v, normalize(n)) | 2000 | 0 | 3.55271368e-15 |  |  |
| vector3_reflect | glm::reflect(v, normalize(n)) | 2000 | 0 | 5.32907052e-15 |  |  |
| vector3_refract | glm::refract(normalize(v), normalize(n), eta) | 2000 | 0 | 2.33146835e-15 |  |  |
| vector4_project | glm::proj | 2000 | 0 | 6.00412778e-16 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 3.55271368e-15 |  |  |
| vector3_rotate_by_quaternion | Eigen q * v | 2000 | 0 | 4.21884749e-15 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 6.99440506e-15 |  | q not unit length |
| vector3_reflect_by_quaternion | glm q * (v, 0) * q (as documented) | 2000 | 0 | 1.33226763e-15 |  |  |
| vector3_multiplym4 | glm (M * (v, 1)).xyz | 2000 | 0 | 0 |  |  |
| vector2_multiplym2 | glm M * v | 2000 | 0 | 0 |  |  |
| vector2_multiplym3 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  | README: sets self to mT * self |
| matrix3_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix2_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv4 | glm M * v | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv3 | glm (M * (v, 1)).xyz | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv2 | glm (M * (v, 0, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix3_multiplyv2 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix2_multiplyv2 | glm M * v | 2000 | 0 | 0 |  |  |
| matrix4_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix3_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix4_determinant | glm::determinant | 2000 | 0 | 8.52651283e-16 |  |  |
| matrix4_determinant | Eigen long double | 2000 | 0 | 4.36317649e-14 |  | relative to |det|; entries up to 5 |
| matrix4_determinant | LAPACK dgetrf | 2000 | 0 | 4.4713608e-14 |  | LAPACK in double; single precision rounding of the matrix products |
| matrix3_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix2_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix4_inverse | glm::inverse | 2000 | 0 | 1.25810391e-15 |  |  |
| matrix4_inverse | Eigen inverse | 2000 | 0 | 4.77633082e-15 |  |  |
| matrix4_inverse | LAPACK dgetri | 2000 | 0 | 1.45708996e-14 |  |  |
| matrix3_inverse | glm::inverse | 2000 | 0 | 2.2137218e-18 |  |  |
| matrix2_inverse | glm::inverse | 2000 | 0 | 2.2169008e-18 |  |  |

## inverse accuracy (largest error over the random matrices)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_inverse | long double | 2000 | 0 | 3.81643243e-16 |  | condition ~1e0; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 4.5160922e-16 |  | condition ~1e0; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 4.38684217e-16 |  | condition ~1e0; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 4.87493451e-16 |  | condition ~1e0; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 6.41137848e-13 |  | condition ~1e3; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 6.78053191e-13 |  | condition ~1e3; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 7.78810845e-13 |  | condition ~1e3; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 4.73656663e-14 |  | condition ~1e3; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 7.11487501e-08 |  | condition ~1e6; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 7.67847716e-08 |  | condition ~1e6; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 1.07096327e-07 |  | condition ~1e6; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 4.8600825e-11 |  | condition ~1e6; error relative to the largest element |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_reciprocal_condition | 1 / (norm(A) norm(A^-1)), infinity norm, in long double | 2000 | 0 | 7.33828197e-13 |  | relative error up to 1e-2 (single) or 1e-5 (double); single precision is limited by the float inverse: see the accuracy table |
| matrix4_normal_matrix | glm::inverseTranspose(mat3(M)) | 2000 | 0 | 4.19426496e-16 |  |  |

## matrix builders

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_make_transformation_translationv3 | glm::translate(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_scalingv3 | glm::scale(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_rotationf_x | glm::rotate(I, a, axis) | 2000 | 0 | 1.11022302e-16 |  |  |
| matrix4_make_transformation_rotationf_y | glm::rotate(I, a, axis) | 2000 | 0 | 1.11022302e-16 |  |  |
| matrix4_make_transformation_rotationf_z | glm::rotate(I, a, axis) | 2000 | 0 | 1.11022302e-16 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 6.66133815e-16 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 6.66133815e-16 |  | axis not unit length |
| matrix4_set_from_quaternion | glm::mat4_cast | 2000 | 0 | 7.77156117e-16 |  |  |
| matrix4_set_from_quaternion | glm::mat4_cast(normalize(q)) | 2000 | 0 | 9.99200722e-16 |  | q not unit length |
| matrix4_make_transformation_rotationq | Eigen toRotationMatrix | 2000 | 0 | 6.66133815e-16 |  |  |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleZYX(z, y, x) = Rz Ry Rx | 2000 | 0 | 2.22044605e-16 |  | X first, then Y, then Z |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleXYZ(x, y, z) | 2000 | **2000** | 1.99767129 | -3.80623354, -1.56766422, -0.596501201 -> (0.00259120887, -0.952436486, 0.304726147, 0, -0.00175946364, -0.304731041, -0.95243682, 0, 0.999995095, 0.00193180816, -0.00246539914, 0, 0, 0, 0, 1) | for reference: GLM's XYZ is Rx Ry Rz |
| matrix4_make_transformation_rotationv3(v) | glm::eulerAngleZYX(v.z, v.y, v.x) | 2000 | 0 | 2.22044605e-16 |  |  |
| matrix4_translatev3(M, v) | glm T(v) * M | 2000 | 0 | 0 |  | applies the translation after M |
| matrix4_translatev3(M, v) | glm::translate(M, v) = M * T(v) | 2000 | **2000** | 20.4179313 | (-1.50429664, -1.67785625, -0.341943719, -9.87828185, -1.5751103, 0.448978888, 0.718001933, 9.20759502, -1.11804379, 1.62498308, -0.551451969, -6.14885385, 0, 0, 0, 1), (-9.59372018, 1.94795442, 0.825712118) -> (-1.50429664, -1.67785625, -0.341943719, -19.472002, -1.5751103, 0.448978888, 0.718001933, 11.1555494, -1.11804379, 1.62498308, -0.551451969, -5.32314173, 0, 0, 0, 1) | for reference: GLM applies it before M |
| matrix4_rotatev3(M, axis, a) | glm R * M | 2000 | 0 | 4.88498131e-15 |  |  |
| matrix4_scalev3(M, v) | glm S * M | 2000 | 0 | 0 |  |  |
| matrix4_transformation_compose(s, q, t) | glm T * R * S | 2000 | 0 | 1.88737914e-15 |  |  |
| matrix4_transformation_decompose | glm::decompose | 2000 | 0 | 3.33066907e-16 |  | scale, rotation (q or -q), translation |
| matrix3_make_transformation_rotationf_z | glm::rotate(mat3(1), a) | 2000 | 0 | 0 |  |  |
| matrix3_make_transformation_translationv2 | glm::translate(mat3(1), v) | 2000 | 0 | 0 |  |  |
| matrix3_make_transformation_scalingv2 | glm::scale(mat3(1), v) | 2000 | 0 | 0 |  |  |
| matrix3_rotate(M, a) | glm R * M | 2000 | 0 | 0 |  |  |
| matrix3_translatev2(M, v) | glm T * M | 2000 | 0 | 0 |  |  |
| matrix2_make_transformation_rotationf_z | mat2 of glm::rotate(mat3(1), a) | 2000 | 0 | 0 |  |  |
| matrix2_rotate(M, a) | glm R * M | 2000 | 0 | 0 |  |  |

## projections

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_projection_perspective_fovy_rh | glm::perspectiveRH_NO | 2000 | 0 | 2.43399185e-16 |  |  |
| matrix4_projection_perspective_fovy_lh | glm::perspectiveLH_NO | 2000 | 0 | 2.65858272e-16 |  |  |
| matrix4_projection_ortho3d_rh | glm::orthoRH_NO | 2000 | 0 | 0 |  |  |
| matrix4_projection_ortho3d_lh | glm::orthoLH_NO | 2000 | 0 | 0 |  |  |
| matrix4_view_lookat_rh | glm::lookAtRH | 2000 | 0 | 3.14843324e-15 |  |  |
| matrix4_view_lookat_lh | glm::lookAtLH | 2000 | 0 | 3.45017979e-15 |  |  |
| matrix4_projection_frustum_rh | glm::frustumRH_NO | 2000 | 0 | 0 |  |  |
| matrix4_projection_frustum_lh | glm::frustumLH_NO | 2000 | 0 | 0 |  |  |
| matrix4_projection_perspective_fovy_infinite_rh | glm::infinitePerspectiveRH_NO | 2000 | 0 | 3.79125952e-16 |  |  |
| matrix4_projection_perspective_fovy_infinite_lh | glm::infinitePerspectiveLH_NO | 2000 | 0 | 2.70253615e-16 |  |  |
| vector3_project_to_window | glm::project_NO(p, identity, M, viewport) | 2000 | 0 | 0 |  |  |
| vector3_unproject_from_window | glm::unProject_NO(w, identity, M, viewport) | 2000 | 0 | 1.31561428e-14 |  |  |

## quaternions

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| quaternion_multiply(a, b) | glm a * b | 2000 | 0 | 8.8817842e-16 |  |  |
| quaternion_multiply(a, b) | Eigen a * b | 2000 | 0 | 1.77635684e-15 |  |  |
| quaternion_multiplyv3(q, v) | glm q * (v, 0) | 2000 | 0 | 1.77635684e-15 |  |  |
| quaternion_conjugate | glm::conjugate | 2000 | 0 | 0 |  |  |
| quaternion_inverse | glm::inverse | 2000 | 0 | 3.47755592e-16 |  | not unit length |
| quaternion_inverse | Eigen inverse | 2000 | 0 | 3.62668964e-16 |  | not unit length |
| quaternion_normalize | glm::normalize | 2000 | 0 | 2.22044605e-16 |  |  |
| quaternion_dot_product, norm, magnitude | glm::dot, dot(q, q), glm::length | 2000 | 0 | 9.29431235e-16 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis | 2000 | 0 | 1.11022302e-16 |  |  |
| quaternion_set_from_axis_anglev3 | Eigen AngleAxis | 2000 | 0 | 2.22044605e-16 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis(a, normalize(axis)) | 2000 | 0 | 3.33066907e-16 |  | axis not unit length |
| quaternion_get_axis_anglev3 | glm::axis, glm::angle | 2000 | 0 | 2.22044605e-16 |  | as a rotation: rebuilt with angleAxis, q or -q |
| quaternion_get_axis_anglev3 | glm::angle | 2000 | **998** | 0.927084802 | (x -0.0436436654, y 0.14598386, z 0.147237073, w -0.977294831) -> 0.427004576 vs 5.85618073 | the angle itself (hypatia [0, pi], GLM [0, 2 pi]) |
| quaternion_get_axis_anglev3 | Eigen AngleAxis(q) | 2000 | 0 | 2.22044605e-16 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | glm::quat(vec3(x, y, z)) | 2000 | 0 | 3.33066907e-16 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | matrix4_set_from_euler_anglesf3(x, y, z) | 2000 | 0 | 6.66133815e-16 |  | the matrix of the quaternion |
| quaternion_get_euler_anglesf3 | glm::eulerAngles | 2000 | 0 | 2.78755894e-15 |  | angles; |y| < 1.5 |
| quaternion_get_euler_anglesf3 | the angles given to quaternion_set_from_euler_anglesf3 | 2000 | 0 | 3.48667675e-18 |  | round trip; |y| < 1.5 |
| quaternion_set_from_matrix4 | glm::quat_cast | 2000 | 0 | 4.4408921e-16 |  | q or -q |
| quaternion_set_from_matrix4 | Eigen Quaternion(Matrix3) | 2000 | 0 | 2.22044605e-16 |  | q or -q |
| quaternion_lerp | glm::lerp | 2000 | 0 | 0 |  |  |
| quaternion_nlerp | glm::normalize(glm::lerp) | 2000 | 0 | 2.22044605e-16 |  |  |
| quaternion_slerp | glm::slerp | 2000 | 0 | 3.33066907e-16 |  | components |
| quaternion_slerp | Eigen slerp | 2000 | 0 | 3.33066907e-16 |  | components |
| quaternion_slerp | glm::slerp | 2000 | 0 | 4.4408921e-16 |  | nearly the same: 1e-3 rad apart |
| quaternion_set_look_rotation_rh | glm::quatLookAtRH(normalize(d), up) | 2000 | 0 | 6.10622664e-16 |  |  |
| quaternion_set_look_rotation_lh | glm::quatLookAtLH(normalize(d), up) | 2000 | 0 | 4.4408921e-16 |  |  |
| quaternion_get_rotation_tov3 | glm::rotation(normalize(a), normalize(b)) | 2000 | 0 | 4.89608354e-14 |  | near opposite GLM is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | 0 | 3.60822483e-14 |  | near opposite Eigen is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | 0 | 4.4408921e-16 |  | nearly opposite: 1e-3 rad from opposite; rotating a must give b |
| quaternion_angle_between | Eigen angularDistance | 2000 | 0 | 4.26154016e-16 |  |  |
| quaternion_angle_between | long double | 2000 | 0 | 7.20723394e-19 |  | 1e-4 rad apart |
| quaternion_difference | min(|a - b|^2, |a + b|^2) | 2000 | 0 | 2.94977147e-16 |  |  |
| quaternion_rotate_by_quaternion(a, b) | glm::normalize(a * b) | 2000 | 0 | 2.22044605e-16 |  |  |
| quaternion_rotate_by_axis_angle(q, axis, a) | glm::rotate(q, a, axis) = q * angleAxis | 2000 | 0 | 3.33066907e-16 |  |  |
| quaternion_rotate_by_euler_angles(q, x, y, z) | q * glm::quat(vec3(x, y, z)) | 2000 | 0 | 4.4408921e-16 |  | as a rotation |

## accuracy against long double (largest error over the inputs)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector3_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | random |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 4.4408921e-15 |  | random |
| vector2_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | random |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 3.1638418e-13 |  | random |
| vector3_angle_between | long double | 2000 | 0 | 1.76074433e-16 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 4.51878335e-13 |  | 1e-3 rad apart |
| vector2_angle_between | long double | 2000 | 0 | 1.66533454e-16 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 4.85545716e-12 |  | 1e-3 rad apart |
| vector3_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 4.52082816e-13 |  | 1e-3 rad from opposite |
| vector2_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 8.80628903e-12 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 4.65330643e-16 |  | random |
| glm::rotation | long double | 2000 | 0 | 1.18635507e-14 |  | random |
| Eigen FromTwoVectors | long double | 2000 | 0 | 6.93954374e-15 |  | random |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 3.96194149e-16 |  | 1e-3 rad from opposite |
| glm::rotation | long double | 2000 | 0 | 3.85882902e-13 |  | 1e-3 rad from opposite |
| Eigen FromTwoVectors | long double | 2000 | 0 | 2.24504935e-13 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 2.3820319e-16 |  | exactly opposite (to = -2 from) |
| glm::rotation | long double | 2000 | 0 | 2 |  | exactly opposite (to = -2 from) |
| Eigen FromTwoVectors | long double | 2000 | 0 | 2.98023224e-08 |  | exactly opposite (to = -2 from) |
| matrix4_determinant | long double | 2000 | 0 | 5.01250388e-16 |  | condition up to 1e6; relative to the largest element^4 |
| glm::determinant | long double | 2000 | 0 | 5.21067343e-16 |  | condition up to 1e6; relative to the largest element^4 |
| Eigen determinant | long double | 2000 | 0 | 5.89519861e-16 |  | condition up to 1e6; relative to the largest element^4 |
| matrix4_reciprocal_condition | long double | 2000 | 0 | 1.41191369e-08 |  | condition up to 1e6; |log10(result / exact)| |
| 1 / (norm(A) norm(glm::inverse(A))) | long double | 2000 | 0 | 1.45959803e-08 |  | condition up to 1e6; |log10(result / exact)| |
| LAPACK dgecon estimate (double) | long double | 2000 | 0 | 0.351693847 |  | condition up to 1e6; |log10(result / exact)| |

## Edge cases

| function | input | hypatia | others |
|---|---|---|---|
| vector3_normalize | zero (0, 0, 0) | (0, 0, 0) | GLM (-nan, -nan, -nan); Eigen (0, 0, 0) |
| vector3_normalize | tiny (1e-30 / 1e-200) (1e-200, 1e-200, 0) | (0.707106781, 0.707106781, 0) | GLM (inf, inf, -nan); Eigen (1e-200, 1e-200, 0) |
| vector3_normalize | huge (1e30 / 1e200) (1e+200, 1e+200, 0) | (0.707106781, 0.707106781, 0) | GLM (0, 0, 0); Eigen (0, 0, 0) |
| vector3_normalize | one component inf (inf, 1, 0) | (1, 0, 0) | GLM (-nan, 0, 0); Eigen (-nan, 0, 0) |
| vector3_normalize | one component NaN (-nan, 1, 0) | (-nan, 1, 0) | GLM (-nan, -nan, -nan); Eigen (-nan, 1, 0) |
| vector3_angle_between | a, 3a: parallel | 0 | GLM angle(normalize, normalize) 0 |
| vector3_angle_between | a, zero | 0 | GLM -nan |
| vector2_angle_between | a, 7a: parallel | 0 | GLM angle(normalize, normalize) 0 |
| vector3_project | onto zero | (0, 0, 0) | GLM proj (-nan, -nan, -nan) |
| quaternion_normalize | zero | (x 0, y 0, z 0, w 0) | GLM (x 0, y 0, z 0, w 1); Eigen (x 0, y 0, z 0, w 0) |
| quaternion_inverse | zero | (x 0, y 0, z 0, w 0) | GLM (x -nan, y -nan, z -nan, w -nan); Eigen (x 0, y 0, z 0, w 0) |
| quaternion_slerp | q, -q, 0.5 (same rotation) | (x 0, y 0, z 0.479425539, w 0.877582562) | GLM slerp (x 0, y 0, z 0.479425539, w 0.877582562); GLM mix (x 0, y 0, z 0, w 0); Eigen (x 0, y 0, z 0.479425539, w 0.877582562) |
| quaternion_slerp | q, q, 0.5 | (x 0, y 0, z 0.479425539, w 0.877582562) | GLM (x 0, y 0, z 0.479425539, w 0.877582562); Eigen (x 0, y 0, z 0.479425539, w 0.877582562) |
| quaternion_slerp | t = 1.5 (beyond end) | (x 0, y 0, z 0.998531341, w -0.054177135) | GLM (x 0, y 0, z 0.998531341, w -0.054177135); Eigen (x 0, y 0, z 0.998531341, w -0.054177135) |
| quaternion_get_axis_anglev3 | identity | (0, 0, 0), 0 | GLM axis (0, 0, 1), angle 0 |
| quaternion_get_axis_anglev3 | -identity | (-0, -0, -0), 0 | GLM axis (0, 0, 1), angle 6.28318531 |
| quaternion_get_axis_anglev3 | -(1 rad about Z) | (0, 0, 1), 1 | GLM axis (-0, -0, -1), angle 5.28318531 |
| quaternion_get_rotation_tov3 | a, -a: opposite | (x 0, y 0.832050294, z -0.554700196, w 0) (turns a to (-1, -2, -3)) | GLM (x -0.894427191, y 0.447213595, z 0, w 6.123234e-17) (turns a to (-1, -2, -3)); Eigen (x 0.119522861, y -0.844013232, z 0.522834534, w 0) |
| quaternion_get_rotation_tov3 | a, zero | (x 0, y 0, z 0, w 1) | GLM (x -nan, y -nan, z -nan, w -nan); Eigen (x 0, y 0, z 0, w 0.707106781) |
| matrix4_inverse | singular (1 .. 16) | NULL | GLM (-nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan, -nan); determinant: hypatia 0, GLM 0 |
| matrix4_inverse | nearly singular | NULL | LAPACK rcond (double) see table; GLM determinant 0 |
| matrix4_transformation_decompose | zero scale on x | 0 (fails) | GLM false, scale (0.267261242, 0.534522484, 0.801783726) |
| matrix4_view_lookat_rh | eye == target | (0, 0, 0, -0, 0, 0, 0, -0, -0, -0, -0, 0, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, nan, nan, nan, -nan, 0, 0, 0, 1) |
| matrix4_view_lookat_rh | looking along up | (0, 0, 0, -0, 0, 0, 0, -0, -0, -1, -0, 2, 0, 0, 0, 1) | GLM (-nan, -nan, -nan, nan, -nan, -nan, -nan, nan, -0, -1, -0, 2, 0, 0, 0, 1) |
| matrix4_projection_perspective_fovy_rh | zNear == zFar | (1.83048772, 0, 0, 0, 0, 1.83048772, 0, 0, 0, 0, inf, inf, 0, 0, -1, 0) | GLM (1.83048772, 0, 0, 0, 0, 1.83048772, 0, 0, 0, 0, -inf, -inf, 0, 0, -1, 0) |
| vector3_rotate_by_quaternion | X by 2 * (quarter turn about Z) | (2.22044605e-16, 1, 0) | GLM q * v (-3, 4, 0); Eigen q * v (-3, 4, 0) |
| vector3_rotate_by_quaternion | X by the zero quaternion | (1, 0, 0) | GLM q * v (1, 0, 0); Eigen q * v (1, 0, 0) |
| matrix4_set_from_axisv3_angle | quarter turn about (0, 0, 10), applied to X | (6.123234e-17, 1, 0) | GLM rotate (6.123234e-17, 1, 0); Eigen AngleAxis matrix (6.123234e-17, 10, 0) |
| quaternion_set_from_axis_anglev3 | quarter turn about (0, 0, 10), applied to X | (x 0, y 0, z 0.707106781, w 0.707106781) length 1, X -> (2.22044605e-16, 1, 0) | GLM angleAxis (x 0, y 0, z 7.07106781, w 0.707106781) length 7.1063352, X -> (-99, 10, 0); Eigen AngleAxis (x 0, y 0, z 7.07106781, w 0.707106781) length 7.1063352, X -> (-99, 10, 0) |
| matrix4_set_from_axisv3_angle | quarter turn about the zero axis, applied to X | (1, 0, 0) | GLM rotate (-nan, -nan, -nan); Eigen AngleAxis matrix (6.123234e-17, 0, 0) |
| quaternion_set_from_axis_anglev3 | quarter turn about the zero axis, applied to X | (x 0, y 0, z 0, w 1) length 1, X -> (1, 0, 0) | GLM angleAxis (x 0, y 0, z 0, w 0.707106781) length 0.707106781, X -> (1, 0, 0); Eigen AngleAxis (x 0, y 0, z 0, w 0.707106781) length 0.707106781, X -> (1, 0, 0) |

## Precision against long double

Largest / mean error in units of the double epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over 20000 inputs.  Lower is better; in bold, the smallest mean of each row and the means within 2% of it.

| function | inputs | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|---|
| vector3_normalize | random | **1.22 / 0.321** | 1.41 / 0.369 | **1.22 / 0.321** |  |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 1.01 / 0.129 | 1.35 / 0.183 | **1.01 / 0.121** |  |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | **1.19 / 0.247** | 1.33 / 0.305 | **1.19 / 0.247** |  |
| vector3_magnitude | random | **0.919 / 0.217** | **0.919 / 0.217** | **0.919 / 0.217** |  |
| vector3_dot_product | random | **0.972 / 0.16** | **0.972 / 0.16** | **0.972 / 0.16** |  |
| vector3_dot_product | nearly perpendicular | **0.55 / 0.0914** | **0.55 / 0.0914** | **0.55 / 0.0914** |  |
| vector3_cross_product | random | **0.806 / 0.199** | **0.806 / 0.199** | **0.806 / 0.199** |  |
| vector3_cross_product | nearly parallel (1e-4 rad) | **0.471 / 0.111** | **0.471 / 0.111** | **0.471 / 0.111** |  |
| vector3_project | random | **1.5 / 0.208** | **1.5 / 0.208** |  |  |
| vector3_rotate_by_quaternion | unit q | **3.55 / 0.623** | 4.06 / 0.685 | 3.37 / 0.684 |  |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | **3.4 / 0.66** | 2.27e+10 / 6.05e+09 | 2.27e+10 / 6.05e+09 |  |
| matrix4_multiply | random | **1.32 / 0.367** | **1.32 / 0.367** | **1.32 / 0.367** |  |
| matrix4_inverse | random entries (error / condition number) | **0.6 / 0.0624** | 0.568 / 0.0647 | 0.735 / 0.0642 |  |
| matrix4_inverse | rotation, scale and translation (error / condition number) | **0.196 / 0.0109** | 0.237 / 0.0117 | 0.237 / 0.0115 |  |
| matrix4_inverse | condition ~1e4 (error / condition number) | **14.1 / 0.946** | 13.1 / 1.01 | **15.2 / 0.94** |  |
| matrix3_inverse | random entries (error / condition number) | **0.6 / 0.0757** | 0.6 / 0.0788 | 0.726 / 0.0789 |  |
| matrix4_determinant | random entries | **2.74 / 0.261** | **2.76 / 0.263** | **3.03 / 0.262** |  |
| matrix4_normal_matrix | rotation, scale and translation | **1.89 / 0.384** | **1.89 / 0.382** |  |  |
| matrix4_set_from_axisv3_angle | random | 2.61 / 0.499 | 4.78 / 0.602 | **2.06 / 0.443** |  |
| matrix4_set_from_axisv3_angle | angle 1e-4 | **0.368 / 0.229** | **0.368 / 0.229** | **0.368 / 0.229** |  |
| matrix4_set_from_quaternion | unit q | **2.68 / 0.574** | 2.2 / 0.592 | 2.2 / 0.592 |  |
| matrix4_set_from_euler_anglesf3 | random | **1.33 / 0.392** | **1.33 / 0.391** |  |  |
| matrix4_projection_perspective_fovy_rh | random | **1.4 / 0.362** | **1.4 / 0.368** |  |  |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | **1.69 / 0.411** | 2.19 / 0.449 |  |  |
| quaternion_multiply | random | **1.02 / 0.289** | 1.43 / 0.3 | **1.06 / 0.289** |  |
| quaternion_normalize | random length | **1.19 / 0.281** | 1.45 / 0.354 | **1.25 / 0.279** |  |
| quaternion_inverse | random length | **1.34 / 0.388** | **1.39 / 0.386** | **1.49 / 0.387** |  |
| quaternion_set_from_axis_anglev3 | random | 1.3 / 0.303 | **0.915 / 0.239** | **0.915 / 0.239** |  |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | **0.033 / 0.033** | **0.033 / 0.033** | **0.033 / 0.033** |  |
| quaternion_get_axis_anglev3 | random (axis * angle) | **1.49 / 0.384** | 23.4 / 0.643 | **1.49 / 0.384** |  |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | **1.05 / 0.351** | 1.18e+07 / 1.18e+07 | **1.05 / 0.351** |  |
| quaternion_set_from_matrix4 | random rotation | 1.59 / 0.355 | **1.38 / 0.325** | **1.58 / 0.328** |  |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) | 1.5 / 0.33 | **1.53 / 0.302** | **1.53 / 0.302** |  |
| quaternion_slerp | random | 1.75 / 0.469 | 1.96 / 0.488 | **1.73 / 0.459** |  |
| quaternion_slerp | 1e-3 rad apart | 1.83 / 0.519 | 2.04 / 0.531 | **1.89 / 0.497** |  |
| quaternion_slerp | 1e-6 rad apart | **1.84 / 0.489** | 2.29 / 0.534 | 1.84 / 0.499 |  |
| quaternion_get_rotation_tov3 | random (landing error) | **2.17 / 0.537** | 113 / 0.941 | 88.5 / 0.861 |  |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | **2.44 / 0.351** | 4.09e+03 / 837 | 4.09e+03 / 638 |  |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 1.39 / 0.277 | **0.63 / 0.161** | **0.63 / 0.158** |  |
| quaternion_angle_between | random | 1.71 / 0.333 |  | **11.8 / 0.257** |  |
| quaternion_angle_between | 1e-4 rad apart | **4.51e+03 / 208** |  | 1.13e+04 / 1.78e+03 |  |
| vector3_reflect | random (relative to the length of v) | **2.88 / 0.459** | 4.44 / 0.65 |  |  |
| vector3_refract | random, eta 0.5 .. 2 | **51.2 / 0.389** | 69.8 / 0.428 |  |  |
| quaternion_set_look_rotation_rh | random | 4.34 / 0.432 | **11.8 / 0.413** |  |  |
| vector3_project_to_window | random camera, a point in the view | **1.09e+03 / 1.33** | **1.09e+03 / 1.33** |  |  |
| vector3_unproject_from_window | random camera, window depth 0 .. 0.9 | **32.2 / 1.05** | 34.3 / 1.17 |  |  |

### Oracle check

Each reference against a second computation of it: the largest disagreement over 20000 inputs, in the units of the table above.

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 0.000424 |
| vector rotation: quaternion product against the rotation matrix | 0.00184 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 0.0013 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 0.00155 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.394 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.33 |
| angle between quaternions 1e-4 rad apart: from a - b and a + b against from a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 0.00317 |
