# hypatia precision: single precision, master (2.1.0-dev)

## Precision against long double

Largest / mean error in units of the float epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over 20000 inputs.  Lower is better; in bold, the smallest mean of each row and the means within 2% of it.

| function | inputs | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|---|
| vector3_normalize | random | **1.18 / 0.321** | 1.29 / 0.369 | **1.17 / 0.322** | 1.29 / 0.369 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 8.39e+06 / 3.37e+05 | 1.29 / 0.146 | **1.01 / 0.0826** | 8.39e+06 / 1.76e+05 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | **0.00302 / 0.000279** | 0.5 / 0.0782 | **0.00302 / 0.000279** | 0.5 / 0.0782 |
| vector3_magnitude | random | **0.993 / 0.218** | **0.993 / 0.218** | **0.903 / 0.218** | **0.993 / 0.218** |
| vector3_dot_product | random | **0.958 / 0.16** | **0.958 / 0.16** | **1 / 0.16** | **0.958 / 0.16** |
| vector3_dot_product | nearly perpendicular | **0.563 / 0.0921** | **0.563 / 0.0921** | **0.587 / 0.0914** | **0.563 / 0.0921** |
| vector3_cross_product | random | **0.769 / 0.197** | **0.769 / 0.197** | **0.769 / 0.197** | **0.769 / 0.197** |
| vector3_cross_product | nearly parallel (1e-4 rad) | **0.469 / 0.11** | **0.469 / 0.11** | **0.469 / 0.11** | **0.469 / 0.11** |
| vector3_project | random |  | **1.77 / 0.21** |  |  |
| vector3_rotate_by_quaternion | unit q | 2.37e+07 / 3.21e+06 | **3.4 / 0.702** | **3.32 / 0.688** | 3.42 / 0.716 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | 2.05e+07 / 3.18e+06 | 42.5 / 11.4 | 42.5 / 11.4 | **3.64 / 0.72** |
| matrix4_multiply | random | **1.27 / 0.367** | **1.27 / 0.367** | **1.27 / 0.367** | **1.27 / 0.367** |
| matrix4_inverse | random entries (error / condition number) | 1.16 / 0.0816 | **0.474 / 0.0645** | **0.463 / 0.064** | **0.523 / 0.0643** |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.205 / 0.012 | **0.142 / 0.0114** | **0.151 / 0.0113** | 0.161 / 0.0117 |
| matrix4_inverse | condition ~1e4 (error / condition number) | 1.07e+03 / 524 | 15.3 / 1.02 | **13 / 0.952** | 15.3 / 1.02 |
| matrix3_inverse | random entries (error / condition number) | **0.566 / 0.0788** | **0.566 / 0.0788** | **0.645 / 0.0793** |  |
| matrix4_determinant | random entries | 3.11 / 0.354 | 3.57 / 0.262 | **2.8 / 0.256** | **2.98 / 0.257** |
| matrix4_normal_matrix | rotation, scale and translation |  | **2.03 / 0.379** |  |  |
| matrix4_set_from_axisv3_angle | random | **1.91 / 0.441** | 4.79 / 0.601 | **2.03 / 0.44** | 4.79 / 0.601 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** |
| matrix4_set_from_quaternion | unit q | 1.68e+07 / 9.31e+06 | 2.1 / 0.594 | 2.1 / 0.594 | **2.83 / 0.581** |
| matrix4_set_from_euler_anglesf3 | random | 1.68e+07 / 1.42e+07 | **1.19 / 0.391** |  | **1.13 / 0.391** |
| matrix4_projection_perspective_fovy_rh | random | 1.61e+07 / 6.83e+06 | **1.14 / 0.358** |  | 1.4 / 0.378 |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | 1.68e+07 / 1.1e+07 | **2.1 / 0.457** |  | **2.1 / 0.457** |
| quaternion_multiply | random | 1.33 / 0.3 | 1.33 / 0.299 | **1.03 / 0.289** | 1.33 / 0.3 |
| quaternion_normalize | random length | **1.24 / 0.285** | 1.37 / 0.353 | **1.16 / 0.281** | **1.16 / 0.281** |
| quaternion_inverse | random length | 1.6 / 0.4 | **1.38 / 0.389** | **1.4 / 0.386** | 1.52 / 0.429 |
| quaternion_set_from_axis_anglev3 | random | 1.34 / 0.286 | **0.9 / 0.239** | **0.9 / 0.239** | 1.88 / 0.306 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** |
| quaternion_get_axis_anglev3 | random (axis * angle) | 15.3 / 0.649 | 12 / 0.709 | **1.45 / 0.396** | 14 / 0.708 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | 8.39e+06 / 8.39e+06 | 2.28e+07 / 1.2e+07 | **0.532 / 0.0894** | 1.01 / 0.371 |
| quaternion_set_from_matrix4 | random rotation |  | **1.5 / 0.328** | **1.5 / 0.33** | 1.69 / 0.339 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) |  | **1.43 / 0.305** | **1.41 / 0.305** | 1.48 / 0.33 |
| quaternion_slerp | random | **1.79 / 0.466** | 2.01 / 0.499 | **1.79 / 0.466** | 75.5 / 0.616 |
| quaternion_slerp | 1e-3 rad apart | **1.45 / 0.412** | 1.92 / 0.572 | 1.82 / 0.424 | 6.13e+03 / 53.5 |
| quaternion_slerp | 1e-6 rad apart | **1.3 / 0.359** | **1.42 / 0.363** | **1.3 / 0.359** | 6.84 / 1.93 |
| quaternion_get_rotation_tov3 | random (landing error) | 1.68e+07 / 1.11e+07 | 92 / 0.956 | **80.3 / 0.875** | 92 / 0.9 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | 1.68e+07 / 1.68e+07 | **4.4e+03 / 928** | 1.8e+04 / 1.66e+04 | 8.39e+03 / 8.39e+03 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 7.64 / 6.65 | 8.74 / 8.39 | **0.631 / 0.157** | 8.74 / 8.39 |
| quaternion_angle_between | random | 243 / 0.775 |  | **4.14 / 0.27** |  |
| quaternion_angle_between | 1e-4 rad apart | inf |  | **1.08e+04 / 1.92e+03** |  |

### Oracle check

Each reference against a second computation of it: the largest disagreement over 20000 inputs, in the units of the table above.

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 4.83e-13 |
| vector rotation: quaternion product against the rotation matrix | 3.29e-12 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 2.67e-12 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 2.47e-12 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.356 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.342 |
| angle between quaternions 1e-4 rad apart: |a - b|, |a + b| against a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 4.55e-12 |
