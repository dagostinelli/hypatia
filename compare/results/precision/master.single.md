# hypatia precision: single precision, master (2.1.0-dev)

## Precision against long double

Largest / mean error in units of the float epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over 20000 inputs.  Lower is better; in bold, the smallest mean of each row and the means within 2% of it.

| function | inputs | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|---|
| vector3_normalize | random | **1.22 / 0.32** | 1.29 / 0.37 | **1.11 / 0.32** | 1.29 / 0.37 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 8.39e+06 / 3.09e+05 | 1.19 / 0.148 | **1.05 / 0.0851** | 8.39e+06 / 1.68e+05 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | **0.00293 / 0.000282** | 0.5 / 0.0747 | **0.00293 / 0.000282** | 0.5 / 0.0747 |
| vector3_magnitude | random | **0.995 / 0.217** | **0.995 / 0.217** | **0.965 / 0.217** | **0.995 / 0.217** |
| vector3_dot_product | random | **0.951 / 0.16** | **0.951 / 0.16** | **0.948 / 0.159** | **0.951 / 0.16** |
| vector3_dot_product | nearly perpendicular | **0.557 / 0.0915** | **0.557 / 0.0915** | **0.56 / 0.0909** | **0.557 / 0.0915** |
| vector3_cross_product | random | **0.791 / 0.196** | **0.791 / 0.196** | **0.791 / 0.196** | **0.791 / 0.196** |
| vector3_cross_product | nearly parallel (1e-4 rad) | **0.45 / 0.111** | **0.45 / 0.111** | **0.45 / 0.111** | **0.45 / 0.111** |
| vector3_project | random |  | **1.64 / 0.211** |  |  |
| vector3_rotate_by_quaternion | unit q | 2.05e+07 / 3.23e+06 | 3.54 / 0.703 | **3.98 / 0.689** | 3.38 / 0.713 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | 2.13e+07 / 3.2e+06 | 41.4 / 11.3 | 41.4 / 11.3 | **3.77 / 0.717** |
| matrix4_multiply | random | **1.15 / 0.366** | **1.15 / 0.366** | **1.15 / 0.366** | **1.15 / 0.366** |
| matrix4_inverse | random entries (error / condition number) | 1.18 / 0.0817 | **0.405 / 0.0646** | **0.602 / 0.0638** | **0.411 / 0.0645** |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.205 / 0.0121 | **0.206 / 0.0116** | **0.186 / 0.0115** | 0.206 / 0.0118 |
| matrix4_inverse | condition ~1e4 (error / condition number) | 1.16e+03 / 525 | 17.6 / 1.02 | **16 / 0.97** | 17.6 / 1.02 |
| matrix3_inverse | random entries (error / condition number) | **0.794 / 0.0788** | **0.794 / 0.0788** | **0.578 / 0.0788** |  |
| matrix4_determinant | random entries | 3.11 / 0.35 | **3.39 / 0.262** | **3.16 / 0.259** | **3.39 / 0.261** |
| matrix4_normal_matrix | rotation, scale and translation |  | **1.95 / 0.38** |  |  |
| matrix4_set_from_axisv3_angle | random | **2.21 / 0.443** | 4.19 / 0.608 | **2.33 / 0.443** | 4.19 / 0.608 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** |
| matrix4_set_from_quaternion | unit q | 1.68e+07 / 9.32e+06 | 2.25 / 0.595 | 2.25 / 0.595 | **2.89 / 0.58** |
| matrix4_set_from_euler_anglesf3 | random | 1.68e+07 / 1.42e+07 | **1.22 / 0.39** |  | **1.22 / 0.389** |
| matrix4_projection_perspective_fovy_rh | random | 1.66e+07 / 6.84e+06 | **1.19 / 0.359** |  | 1.33 / 0.38 |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | 1.68e+07 / 1.1e+07 | **2.54 / 0.458** |  | **2.54 / 0.458** |
| quaternion_multiply | random | 1.15 / 0.302 | 1.16 / 0.302 | **1.04 / 0.289** | 1.15 / 0.302 |
| quaternion_normalize | random length | **1.44 / 0.283** | 1.34 / 0.355 | **1.19 / 0.28** | **1.19 / 0.28** |
| quaternion_inverse | random length | 1.46 / 0.398 | **1.37 / 0.388** | **1.43 / 0.385** | 1.55 / 0.431 |
| quaternion_set_from_axis_anglev3 | random | 1.31 / 0.286 | **0.941 / 0.24** | **0.941 / 0.24** | 1.84 / 0.309 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** |
| quaternion_get_axis_anglev3 | random (axis * angle) | 13.6 / 0.648 | 51 / 0.715 | **1.45 / 0.396** | 6.92 / 0.704 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | 8.39e+06 / 8.39e+06 | 2.28e+07 / 1.19e+07 | **0.533 / 0.09** | 1.01 / 0.374 |
| quaternion_set_from_matrix4 | random rotation |  | **1.31 / 0.328** | **1.73 / 0.329** | 1.85 / 0.337 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) |  | **1.38 / 0.304** | **1.38 / 0.304** | 1.62 / 0.329 |
| quaternion_slerp | random | **55.7 / 0.47** | 1.85 / 0.499 | **1.85 / 0.468** | 16.6 / 0.61 |
| quaternion_slerp | 1e-3 rad apart | **1.47 / 0.414** | 1.96 / 0.575 | 1.64 / 0.427 | 5.89e+03 / 52.3 |
| quaternion_slerp | 1e-6 rad apart | **1.25 / 0.358** | **1.5 / 0.362** | **1.25 / 0.358** | 6.79 / 1.94 |
| quaternion_get_rotation_tov3 | random (landing error) | 1.68e+07 / 1.1e+07 | 99.4 / 0.965 | **48.8 / 0.863** | 99.3 / 0.907 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | 1.68e+07 / 1.68e+07 | **4.39e+03 / 942** | 1.8e+04 / 1.66e+04 | 8.39e+03 / 8.39e+03 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 7.69 / 6.64 | 8.73 / 8.39 | **0.686 / 0.159** | 8.73 / 8.39 |
| quaternion_angle_between | random | 169 / 0.781 |  | **3.46 / 0.273** |  |
| quaternion_angle_between | 1e-4 rad apart | inf |  | **1.19e+04 / 1.92e+03** |  |

### Oracle check

Each reference against a second computation of it: the largest disagreement over 20000 inputs, in the units of the table above.

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 4.35e-13 |
| vector rotation: quaternion product against the rotation matrix | 3.12e-12 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 2.48e-12 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 2.44e-12 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.399 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.321 |
| angle between quaternions 1e-4 rad apart: |a - b|, |a + b| against a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 4.94e-12 |
