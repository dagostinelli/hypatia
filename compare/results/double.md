# hypatia comparison: double precision

Tolerance 1e-10 (difference relative to max(1, |reference|)); 2000 random inputs per row.


## vectors

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector2_normalize | glm::normalize | 2000 | 0 | 3.33066907e-16 |  |  |
| vector3_normalize | glm::normalize | 2000 | 0 | 3.33066907e-16 |  |  |
| vector3_normalize | Eigen normalized | 2000 | 0 | 4.4408921e-16 |  |  |
| vector4_normalize | glm::normalize | 2000 | 0 | 2.22044605e-16 |  |  |
| vector3_magnitude | glm::length | 2000 | 0 | 0 |  |  |
| vector2_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector3_distance | glm::distance | 2000 | 0 | 0 |  |  |
| vector4_distance | glm::distance | 2000 | 0 | 2.3710079e-16 |  |  |
| vector4_dot_product | glm::dot | 2000 | 0 | 7.10542736e-15 |  |  |
| vector3_cross_product | glm::cross | 2000 | 0 | 0 |  |  |
| vector2_cross_product | a.x b.y - a.y b.x | 2000 | 0 | 0 |  |  |
| vector3_find_normal_axis_between | glm::normalize(glm::cross) | 2000 | 0 | 3.33066907e-16 |  |  |
| vector3_angle_between | glm::angle(normalize, normalize) | 2000 | 0 | 3.66373598e-15 |  | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 1.62236422e-16 |  | a and 3a: parallel |
| vector3_angle_between | long double atan2(|a x b|, a . b) | 2000 | 0 | 1.82810039e-18 |  | 1e-4 rad apart |
| vector2_angle_between | glm::angle(normalize, normalize) | 2000 | 0 | 1.56943691e-12 |  | GLM's acos loses accuracy near 0 and pi: see the accuracy table |
| vector2_angle_between | 0 | 2000 | 0 | 1.66533454e-16 |  | a and 3a: parallel |
| vector3_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector4_lerp | glm::mix | 2000 | 0 | 0 |  |  |
| vector3_clamp | glm::clamp | 2000 | 0 | 0 |  |  |
| vector3_min, vector3_max | glm::min, glm::max | 2000 | 0 | 0 |  |  |
| vector2_project | glm::proj | 2000 | 0 | 1.11022302e-15 |  |  |
| vector3_project | glm::proj | 2000 | 0 | 1.22124533e-15 |  |  |
| vector4_project | glm::proj | 2000 | 0 | 1.44328993e-15 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 5.77315973e-15 |  |  |
| vector3_rotate_by_quaternion | Eigen q * v | 2000 | 0 | 5.77315973e-15 |  |  |
| vector3_rotate_by_quaternion | glm q * v | 2000 | 0 | 5.32907052e-15 |  | q not unit length |
| vector3_reflect_by_quaternion | glm q * (v, 0) * q (as documented) | 2000 | 0 | 1.77635684e-15 |  |  |
| vector3_multiplym4 | glm (M * (v, 1)).xyz | 2000 | 0 | 2.66453526e-15 |  |  |
| vector2_multiplym2 | glm M * v | 2000 | 0 | 0 |  |  |
| vector2_multiplym3 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  | README: sets self to mT * self |
| matrix3_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix2_multiply(A, B) | glm B * A | 2000 | 0 | 0 |  |  |
| matrix4_multiplyv4 | glm M * v | 2000 | 0 | 3.55271368e-15 |  |  |
| matrix4_multiplyv3 | glm (M * (v, 1)).xyz | 2000 | 0 | 3.3950064e-15 |  |  |
| matrix4_multiplyv2 | glm (M * (v, 0, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix3_multiplyv2 | glm (M * (v, 1)).xy | 2000 | 0 | 0 |  |  |
| matrix2_multiplyv2 | glm M * v | 2000 | 0 | 0 |  |  |
| matrix4_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix3_transpose | glm::transpose | 2000 | 0 | 0 |  |  |
| matrix4_determinant | glm::determinant | 2000 | 0 | 6.52811138e-16 |  |  |
| matrix4_determinant | Eigen long double | 2000 | 0 | 2.75890422e-14 |  | relative to |det|; entries up to 5 |
| matrix4_determinant | LAPACK dgetrf | 2000 | 0 | 5.96189764e-14 |  | LAPACK in double; single precision rounding of the matrix products |
| matrix3_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix2_determinant | glm::determinant | 2000 | 0 | 0 |  |  |
| matrix4_inverse | glm::inverse | 2000 | 0 | 9.13227429e-15 |  |  |
| matrix4_inverse | Eigen inverse | 2000 | 0 | 1.50808011e-15 |  |  |
| matrix4_inverse | LAPACK dgetri | 2000 | 0 | 3.8269431e-15 |  |  |
| matrix3_inverse | glm::inverse | 2000 | 0 | 0 |  |  |
| matrix2_inverse | glm::inverse | 2000 | 0 | 0 |  |  |

## inverse accuracy (largest error over the random matrices)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_inverse | long double | 2000 | 0 | 4.46374974e-16 |  | condition ~1e0; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 4.90474561e-16 |  | condition ~1e0; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 4.34434386e-16 |  | condition ~1e0; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.72104122e-16 |  | condition ~1e0; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 6.5228723e-13 |  | condition ~1e3; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 7.41513348e-13 |  | condition ~1e3; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 7.19270725e-13 |  | condition ~1e3; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 4.00372812e-14 |  | condition ~1e3; error relative to the largest element |
| matrix4_inverse | long double | 2000 | 0 | 7.51972968e-08 |  | condition ~1e6; error relative to the largest element |
| glm::inverse | long double | 2000 | 0 | 8.83380435e-08 |  | condition ~1e6; error relative to the largest element |
| Eigen inverse | long double | 2000 | 0 | 6.36939746e-08 |  | condition ~1e6; error relative to the largest element |
| LAPACK dgetri (double), rounded | long double | 2000 | 0 | 5.31063937e-11 |  | condition ~1e6; error relative to the largest element |

## matrices

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_reciprocal_condition | 1 / (norm(A) norm(A^-1)), infinity norm, in long double | 2000 | 0 | 2.62862401e-13 |  | relative error up to 1e-2 (single) or 1e-5 (double); single precision is limited by the float inverse: see the accuracy table |
| matrix4_normal_matrix | glm::inverseTranspose(mat3(M)) | 2000 | 0 | 4.22750471e-16 |  |  |

## matrix builders

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| matrix4_make_transformation_translationv3 | glm::translate(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_scalingv3 | glm::scale(I, v) | 2000 | 0 | 0 |  |  |
| matrix4_make_transformation_rotationf_x | glm::rotate(I, a, axis) | 2000 | 0 | 1.11022302e-16 |  |  |
| matrix4_make_transformation_rotationf_y | glm::rotate(I, a, axis) | 2000 | 0 | 1.11022302e-16 |  |  |
| matrix4_make_transformation_rotationf_z | glm::rotate(I, a, axis) | 2000 | 0 | 1.11022302e-16 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 1.11022302e-15 |  |  |
| matrix4_set_from_axisv3_angle | glm::rotate(I, a, axis) | 2000 | 0 | 8.8817842e-16 |  | axis not unit length |
| matrix4_set_from_quaternion | glm::mat4_cast | 2000 | 0 | 8.8817842e-16 |  |  |
| matrix4_set_from_quaternion | glm::mat4_cast(normalize(q)) | 2000 | 0 | 1.33226763e-15 |  | q not unit length |
| matrix4_make_transformation_rotationq | Eigen toRotationMatrix | 2000 | 0 | 8.8817842e-16 |  |  |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleZYX(z, y, x) = Rz Ry Rx | 2000 | 0 | 2.22044605e-16 |  | X first, then Y, then Z |
| matrix4_set_from_euler_anglesf3(x, y, z) | glm::eulerAngleXYZ(x, y, z) | 2000 | **2000** | 1.99960795 | -1.06088764, 4.7057751, -2.05367739 -> (0.00307101823, 0.0270332006, 0.999629819, 0, 0.00585761092, -0.999617869, 0.027014882, 0, 0.999978128, 0.00577247935, -0.00322819467, 0, 0, 0, 0, 1) | for reference: GLM's XYZ is Rx Ry Rz |
| matrix4_make_transformation_rotationv3(v) | glm::eulerAngleZYX(v.z, v.y, v.x) | 2000 | 0 | 2.22044605e-16 |  |  |
| matrix4_translatev3(M, v) | glm T(v) * M | 2000 | 0 | 0 |  | applies the translation after M |
| matrix4_translatev3(M, v) | glm::translate(M, v) = M * T(v) | 2000 | **2000** | 19.2141853 | (0.475496644, -0.0460960047, -0.409483266, -2.2477367, 0.0260637577, -0.437770007, 0.980424671, -6.82068125, -0.117215499, -0.284334641, -1.44310581, -9.95914538, 0, 0, 0, 1), (7.45161481, 7.37769133, -9.0808033) -> (0.475496644, -0.0460960047, -0.409483266, 5.20387811, 0.0260637577, -0.437770007, 0.980424671, 0.557010077, -0.117215499, -0.284334641, -1.44310581, -19.0399487, 0, 0, 0, 1) | for reference: GLM applies it before M |
| matrix4_rotatev3(M, axis, a) | glm R * M | 2000 | 0 | 7.10542736e-15 |  |  |
| matrix4_scalev3(M, v) | glm S * M | 2000 | 0 | 0 |  |  |
| matrix4_transformation_compose(s, q, t) | glm T * R * S | 2000 | 0 | 2.40346532e-15 |  |  |
| matrix4_transformation_decompose | glm::decompose | 2000 | 0 | 4.14321377e-16 |  | scale, rotation (q or -q), translation |
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
| matrix4_projection_perspective_fovy_rh | glm::perspectiveRH_ZO | 2000 | 0 | 3.31844785e-16 |  |  |
| matrix4_projection_perspective_fovy_lh | glm::perspectiveLH_ZO | 2000 | 0 | 2.54800087e-16 |  |  |
| matrix4_projection_ortho3d_rh | glm::orthoRH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_projection_ortho3d_lh | glm::orthoLH_ZO | 2000 | 0 | 0 |  |  |
| matrix4_view_lookat_rh | glm::lookAtRH | 2000 | 0 | 3.09564092e-15 |  |  |
| matrix4_view_lookat_lh | glm::lookAtLH | 2000 | 0 | 7.10542736e-15 |  |  |

## quaternions

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| quaternion_multiply(a, b) | glm a * b | 2000 | 0 | 1.58206781e-15 |  |  |
| quaternion_multiply(a, b) | Eigen a * b | 2000 | 0 | 1.71283819e-15 |  |  |
| quaternion_multiplyv3(q, v) | glm q * (v, 0) | 2000 | 0 | 1.77635684e-15 |  |  |
| quaternion_conjugate | glm::conjugate | 2000 | 0 | 0 |  |  |
| quaternion_inverse | glm::inverse | 2000 | 0 | 4.52018425e-16 |  | not unit length |
| quaternion_inverse | Eigen inverse | 2000 | 0 | 5.33310493e-16 |  | not unit length |
| quaternion_normalize | glm::normalize | 2000 | 0 | 2.22044605e-16 |  |  |
| quaternion_dot_product, norm, magnitude | glm::dot, dot(q, q), glm::length | 2000 | 0 | 8.8817842e-16 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis | 2000 | 0 | 2.22044605e-16 |  |  |
| quaternion_set_from_axis_anglev3 | Eigen AngleAxis | 2000 | 0 | 2.22044605e-16 |  |  |
| quaternion_set_from_axis_anglev3 | glm::angleAxis(a, normalize(axis)) | 2000 | 0 | 3.33066907e-16 |  | axis not unit length |
| quaternion_get_axis_anglev3 | glm::axis, glm::angle | 2000 | 0 | 2.22044605e-16 |  | as a rotation: rebuilt with angleAxis, q or -q |
| quaternion_get_axis_anglev3 | glm::angle | 2000 | **986** | 0.905763658 | (x 0.22164734, y 0.0480002748, z -0.141422102, w -0.963622446) -> 0.541111987 vs 5.74207332 | the angle itself (hypatia [0, pi], GLM [0, 2 pi]) |
| quaternion_get_axis_anglev3 | Eigen AngleAxis(q) | 2000 | 0 | 2.77555756e-16 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | glm::quat(vec3(x, y, z)) | 2000 | 0 | 3.33066907e-16 |  | as a rotation |
| quaternion_set_from_euler_anglesf3(x, y, z) | matrix4_set_from_euler_anglesf3(x, y, z) | 2000 | 0 | 8.8817842e-16 |  | the matrix of the quaternion |
| quaternion_get_euler_anglesf3 | glm::eulerAngles | 2000 | 0 | 2.88657986e-15 |  | angles; |y| < 1.5 |
| quaternion_get_euler_anglesf3 | the angles given to quaternion_set_from_euler_anglesf3 | 2000 | 0 | 4.66033831e-18 |  | round trip; |y| < 1.5 |
| quaternion_set_from_matrix4 | glm::quat_cast | 2000 | 0 | 7.77156117e-16 |  | q or -q |
| quaternion_set_from_matrix4 | Eigen Quaternion(Matrix3) | 2000 | 0 | 3.33066907e-16 |  | q or -q |
| quaternion_lerp | glm::lerp | 2000 | 0 | 0 |  |  |
| quaternion_nlerp | glm::normalize(glm::lerp) | 2000 | 0 | 3.33066907e-16 |  |  |
| quaternion_slerp | glm::slerp | 2000 | 0 | 3.33066907e-16 |  | components |
| quaternion_slerp | Eigen slerp | 2000 | 0 | 3.33066907e-16 |  | components |
| quaternion_slerp | glm::slerp | 2000 | 0 | 3.33066907e-16 |  | nearly the same: 1e-3 rad apart |
| quaternion_get_rotation_tov3 | glm::rotation(normalize(a), normalize(b)) | 2000 | 0 | 2.91211499e-13 |  | near opposite GLM is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | 0 | 2.55795385e-13 |  | near opposite Eigen is less accurate: see the accuracy table |
| quaternion_get_rotation_tov3 | Eigen FromTwoVectors | 2000 | 0 | 6.66133815e-16 |  | nearly opposite: 1e-3 rad from opposite; rotating a must give b |
| quaternion_angle_between | Eigen angularDistance | 2000 | 0 | 4.37577934e-16 |  |  |
| quaternion_angle_between | long double | 2000 | 0 | 2.87408443e-18 |  | 1e-4 rad apart |
| quaternion_difference | min(|a - b|^2, |a + b|^2) | 2000 | 0 | 4.13995879e-16 |  |  |
| quaternion_rotate_by_quaternion(a, b) | glm::normalize(a * b) | 2000 | 0 | 3.33066907e-16 |  |  |
| quaternion_rotate_by_axis_angle(q, axis, a) | glm::rotate(q, a, axis) = q * angleAxis | 2000 | 0 | 3.33066907e-16 |  |  |
| quaternion_rotate_by_euler_angles(q, x, y, z) | q * glm::quat(vec3(x, y, z)) | 2000 | 0 | 4.4408921e-16 |  | as a rotation |

## accuracy against long double (largest error over the inputs)

| test | compared with | n | over tol | max diff | worst case | note |
|---|---|---|---|---|---|---|
| vector3_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | random |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 3.96904731e-15 |  | random |
| vector2_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | random |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 2.50357244e-13 |  | random |
| vector3_angle_between | long double | 2000 | 0 | 1.60028241e-16 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 4.36424117e-13 |  | 1e-3 rad apart |
| vector2_angle_between | long double | 2000 | 0 | 2.15105711e-16 |  | 1e-3 rad apart |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 1.35311176e-12 |  | 1e-3 rad apart |
| vector3_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 3D | long double | 2000 | 0 | 4.52082816e-13 |  | 1e-3 rad from opposite |
| vector2_angle_between | long double | 2000 | 0 | 4.4408921e-16 |  | 1e-3 rad from opposite |
| glm::angle(normalize, normalize), 2D | long double | 2000 | 0 | 1.00865982e-11 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 6.95434818e-16 |  | random |
| glm::rotation | long double | 2000 | 0 | 8.80764804e-15 |  | random |
| Eigen FromTwoVectors | long double | 2000 | 0 | 8.85783886e-15 |  | random |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 5.20878508e-16 |  | 1e-3 rad from opposite |
| glm::rotation | long double | 2000 | 0 | 3.78736377e-13 |  | 1e-3 rad from opposite |
| Eigen FromTwoVectors | long double | 2000 | 0 | 3.78736377e-13 |  | 1e-3 rad from opposite |
| quaternion_get_rotation_tov3 | long double | 2000 | 0 | 4.72198439e-16 |  | exactly opposite (to = -2 from) |
| glm::rotation | long double | 2000 | 0 | 2 |  | exactly opposite (to = -2 from) |
| Eigen FromTwoVectors | long double | 2000 | 0 | 2.98023224e-08 |  | exactly opposite (to = -2 from) |
| matrix4_determinant | long double | 2000 | 0 | 3.75313456e-16 |  | condition up to 1e6; relative to the largest element^4 |
| glm::determinant | long double | 2000 | 0 | 5.35957708e-16 |  | condition up to 1e6; relative to the largest element^4 |
| Eigen determinant | long double | 2000 | 0 | 4.82758369e-16 |  | condition up to 1e6; relative to the largest element^4 |
| matrix4_reciprocal_condition | long double | 2000 | 0 | 1.12403164e-08 |  | condition up to 1e6; |log10(result / exact)| |
| 1 / (norm(A) norm(glm::inverse(A))) | long double | 2000 | 0 | 2.27161485e-08 |  | condition up to 1e6; |log10(result / exact)| |
| LAPACK dgecon estimate (double) | long double | 2000 | 0 | 0.587884457 |  | condition up to 1e6; |log10(result / exact)| |

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
| matrix4_projection_perspective_fovy_rh | zNear == zFar | (1.83048772, 0, 0, 0, 0, 1.83048772, 0, 0, 0, 0, inf, inf, 0, 0, -1, 0) | GLM (1.83048772, 0, 0, 0, 0, 1.83048772, 0, 0, 0, 0, inf, -inf, 0, 0, -1, 0) |
