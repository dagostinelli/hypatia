# hypatia precision: double precision, master (2.1.0-dev)

## Precision against long double

Largest / mean error in units of the double epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over 20000 inputs.  Lower is better; in bold, the smallest mean of each row and the means within 2% of it.

| function | inputs | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|---|
| vector3_normalize | random | **1.09 / 0.321** | 1.34 / 0.371 | **1.09 / 0.321** |  |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 4.5e+15 / 2.37e+14 | 1.38 / 0.185 | **1.14 / 0.124** |  |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | **1.26 / 0.248** | 1.34 / 0.307 | **1.26 / 0.248** |  |
| vector3_magnitude | random | **0.916 / 0.217** | **0.916 / 0.217** | **0.916 / 0.217** |  |
| vector3_dot_product | random | **1.09 / 0.161** | **1.09 / 0.161** | **1.09 / 0.161** |  |
| vector3_dot_product | nearly perpendicular | **0.61 / 0.0897** | **0.61 / 0.0897** | **0.61 / 0.0897** |  |
| vector3_cross_product | random | **0.802 / 0.197** | **0.802 / 0.197** | **0.802 / 0.197** |  |
| vector3_cross_product | nearly parallel (1e-4 rad) | **0.462 / 0.111** | **0.462 / 0.111** | **0.462 / 0.111** |  |
| vector3_project | random |  | **1.59 / 0.211** |  |  |
| vector3_rotate_by_quaternion | unit q | 1.1e+16 / 1.74e+15 | **3.57 / 0.686** | **3.57 / 0.684** |  |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | 1.14e+16 / 1.72e+15 | **2.22e+10 / 6.05e+09** | **2.22e+10 / 6.05e+09** |  |
| matrix4_multiply | random | **1.29 / 0.367** | **1.29 / 0.367** | **1.29 / 0.367** |  |
| matrix4_inverse | random entries (error / condition number) | 0.9 / 0.0817 | **0.523 / 0.0641** | **0.481 / 0.0637** |  |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.186 / 0.0121 | **0.155 / 0.0115** | **0.169 / 0.0114** |  |
| matrix4_inverse | condition ~1e4 (error / condition number) | 6.21e+11 / 2.82e+11 | 13.7 / 1.01 | **15.6 / 0.956** |  |
| matrix3_inverse | random entries (error / condition number) | **0.555 / 0.0784** | **0.555 / 0.0784** | **0.642 / 0.0787** |  |
| matrix4_determinant | random entries | 3.49 / 0.349 | **2.7 / 0.26** | **2.99 / 0.258** |  |
| matrix4_normal_matrix | rotation, scale and translation |  | **2.04 / 0.38** |  |  |
| matrix4_set_from_axisv3_angle | random | **2.14 / 0.439** | 4.45 / 0.599 | **2.14 / 0.439** |  |
| matrix4_set_from_axisv3_angle | angle 1e-4 | **0.368 / 0.229** | **0.368 / 0.229** | **0.368 / 0.229** |  |
| matrix4_set_from_quaternion | unit q | 9e+15 / 5.01e+15 | **2.26 / 0.598** | **2.26 / 0.598** |  |
| matrix4_set_from_euler_anglesf3 | random | 9.01e+15 / 7.6e+15 | **1.22 / 0.39** |  |  |
| matrix4_projection_perspective_fovy_rh | random | 8.93e+15 / 3.67e+15 | **1.11 / 0.357** |  |  |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | 9.01e+15 / 5.88e+15 | **2.31 / 0.45** |  |  |
| quaternion_multiply | random | 1.27 / 0.3 | 1.18 / 0.298 | **1.07 / 0.289** |  |
| quaternion_normalize | random length | **1.26 / 0.284** | 1.46 / 0.356 | **1.19 / 0.28** |  |
| quaternion_inverse | random length | 1.46 / 0.396 | **1.4 / 0.385** | **1.36 / 0.385** |  |
| quaternion_set_from_axis_anglev3 | random | 1.32 / 0.285 | **0.897 / 0.238** | **0.897 / 0.238** |  |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | **0.033 / 0.033** | **0.033 / 0.033** | **0.033 / 0.033** |  |
| quaternion_get_axis_anglev3 | random (axis * angle) | 31.7 / 0.592 | 70.1 / 0.645 | **1.33 / 0.383** |  |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | 1.41e+06 / 1.41e+06 | 1.18e+07 / 1.18e+07 | **0.989 / 0.349** |  |
| quaternion_set_from_matrix4 | random rotation |  | **1.45 / 0.326** | **1.45 / 0.329** |  |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) |  | **1.41 / 0.303** | **1.41 / 0.303** |  |
| quaternion_slerp | random | 2.99e+10 / 1.5e+06 | 2.07 / 0.487 | **1.62 / 0.457** |  |
| quaternion_slerp | 1e-3 rad apart | 1.41e+08 / 9.36e+07 | 1.9 / 0.535 | **2.1 / 0.496** |  |
| quaternion_slerp | 1e-6 rad apart | 1.4e+04 / 94.4 | 2 / 0.538 | **1.89 / 0.502** |  |
| quaternion_get_rotation_tov3 | random (landing error) | 9.01e+15 / 5.9e+15 | 116 / 0.957 | **116 / 0.864** |  |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | 9.01e+15 / 9.01e+15 | 4.09e+03 / 844 | **4.08e+03 / 639** |  |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 4e+09 / 3.56e+09 | 0.666 / 0.163 | **0.636 / 0.159** |  |
| quaternion_angle_between | random | 610 / 0.845 |  | **2.9 / 0.257** |  |
| quaternion_angle_between | 1e-4 rad apart | 9.87e+08 / 1.71e+08 |  | **9.69e+03 / 1.77e+03** |  |

### Oracle check

Each reference against a second computation of it: the largest disagreement over 20000 inputs, in the units of the table above.

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 0.000267 |
| vector rotation: quaternion product against the rotation matrix | 0.00175 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 0.00129 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 0.00132 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.378 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.309 |
| angle between quaternions 1e-4 rad apart: |a - b|, |a + b| against a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 0.00359 |
