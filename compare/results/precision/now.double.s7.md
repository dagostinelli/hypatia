# hypatia precision: double precision

## Precision against long double

Largest / mean error in units of the double epsilon (relative to the largest component of the exact result, or to the size of the terms where noted), over 20000 inputs.  Lower is better; in bold, the smallest mean of each row and the means within 2% of it.

| function | inputs | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|---|
| vector3_normalize | random | **0.422 / 0.203** | 1.3 / 0.369 | 1.19 / 0.321 |  |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | **0.343 / 0.0666** | 1.28 / 0.185 | 1.07 / 0.124 |  |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | **0.25 / 0.125** | 1.3 / 0.308 | 1.11 / 0.249 |  |
| vector3_magnitude | random | **0.936 / 0.219** | **0.936 / 0.219** | **0.936 / 0.219** |  |
| vector3_dot_product | random | **1.2 / 0.16** | **1.2 / 0.16** | **1.2 / 0.16** |  |
| vector3_dot_product | nearly perpendicular | **0.557 / 0.0911** | **0.557 / 0.0911** | **0.557 / 0.0911** |  |
| vector3_cross_product | random | **0.814 / 0.197** | **0.814 / 0.197** | **0.814 / 0.197** |  |
| vector3_cross_product | nearly parallel (1e-4 rad) | **0.43 / 0.11** | **0.43 / 0.11** | **0.43 / 0.11** |  |
| vector3_project | random | **1.73 / 0.209** | **1.73 / 0.209** |  |  |
| vector3_rotate_by_quaternion | unit q | **3.22 / 0.621** | 3.77 / 0.682 | 3.23 / 0.684 |  |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | **3 / 0.657** | 2.22e+10 / 6.1e+09 | 2.22e+10 / 6.1e+09 |  |
| matrix4_multiply | random | **1.2 / 0.366** | **1.2 / 0.366** | **1.2 / 0.366** |  |
| matrix4_inverse | random entries (error / condition number) | **0.437 / 0.0549** | 0.431 / 0.0643 | 0.666 / 0.0643 |  |
| matrix4_inverse | rotation, scale and translation (error / condition number) | **0.134 / 0.00998** | 0.168 / 0.0113 | 0.169 / 0.0114 |  |
| matrix4_inverse | condition ~1e4 (error / condition number) | **2.76 / 0.137** | 18.1 / 1.02 | 15.6 / 0.943 |  |
| matrix3_inverse | random entries (error / condition number) | **0.481 / 0.0637** | 0.699 / 0.0797 | 0.591 / 0.0792 |  |
| matrix4_determinant | random entries | **2.53 / 0.216** | 2.48 / 0.262 | 2.8 / 0.263 |  |
| matrix4_normal_matrix | rotation, scale and translation | **1.64 / 0.339** | 1.79 / 0.38 |  |  |
| matrix4_set_from_axisv3_angle | random | **2.02 / 0.438** | 5.01 / 0.596 | **2.01 / 0.437** |  |
| matrix4_set_from_axisv3_angle | angle 1e-4 | **0.368 / 0.229** | **0.368 / 0.229** | **0.368 / 0.229** |  |
| matrix4_set_from_quaternion | unit q | **2.96 / 0.579** | 2.13 / 0.594 | 2.13 / 0.594 |  |
| matrix4_set_from_euler_anglesf3 | random | **1.13 / 0.393** | **1.17 / 0.391** |  |  |
| matrix4_projection_perspective_fovy_rh | random | **1.25 / 0.352** | **1.19 / 0.358** |  |  |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | **1.77 / 0.354** | 2.2 / 0.447 |  |  |
| quaternion_multiply | random | **1.01 / 0.29** | 1.31 / 0.303 | **0.951 / 0.29** |  |
| quaternion_normalize | random length | **0.483 / 0.198** | 1.37 / 0.355 | 1.19 / 0.282 |  |
| quaternion_inverse | random length | **1.42 / 0.385** | **1.39 / 0.387** | **1.51 / 0.387** |  |
| quaternion_set_from_axis_anglev3 | random | **0.903 / 0.238** | **0.903 / 0.237** | **0.903 / 0.237** |  |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | **0.033 / 0.033** | **0.033 / 0.033** | **0.033 / 0.033** |  |
| quaternion_get_axis_anglev3 | random (axis * angle) | **1.21 / 0.374** | 38.5 / 0.646 | 1.37 / 0.384 |  |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | **0.995 / 0.348** | 1.18e+07 / 1.18e+07 | **1.02 / 0.349** |  |
| quaternion_set_from_matrix4 | random rotation | **1.69 / 0.293** | 1.42 / 0.328 | 1.72 / 0.331 |  |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) | **1.42 / 0.27** | 1.45 / 0.304 | 1.45 / 0.304 |  |
| quaternion_slerp | random | **1.67 / 0.4** | 1.8 / 0.49 | 1.65 / 0.458 |  |
| quaternion_slerp | 1e-3 rad apart | **1.83 / 0.464** | 2.2 / 0.534 | 1.9 / 0.496 |  |
| quaternion_slerp | 1e-6 rad apart | **1.85 / 0.433** | 2.55 / 0.536 | 2.28 / 0.502 |  |
| quaternion_get_rotation_tov3 | random (landing error) | **2.26 / 0.465** | 96.9 / 0.955 | 96.9 / 0.875 |  |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | **2.36 / 0.353** | 5.07e+03 / 837 | 4.08e+03 / 636 |  |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | **0.561 / 0.122** | 0.792 / 0.161 | 0.635 / 0.159 |  |
| quaternion_angle_between | random | **0.903 / 0.221** |  | 3.45 / 0.257 |  |
| quaternion_angle_between | 1e-4 rad apart | **0.505 / 0.217** |  | 9.62e+03 / 1.77e+03 |  |
| vector3_reflect | random (relative to the length of v) | **3.22 / 0.459** | 5.34 / 0.662 |  |  |
| vector3_refract | random, eta 0.5 .. 2 | **36.2 / 0.322** | 48.7 / 0.414 |  |  |
| quaternion_set_look_rotation_rh | random | **19.1 / 0.35** | 5.36e+14 / 2.68e+10 |  |  |
| vector3_project_to_window | random camera, a point in the view | **654 / 2.21** | **654 / 2.21** |  |  |
| vector3_unproject_from_window | random camera, window depth 0 .. 0.9 | **57.3 / 1.8** | 64.9 / 2.19 |  |  |

### Oracle check

Each reference against a second computation of it: the largest disagreement over 20000 inputs, in the units of the table above.

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 0.000287 |
| vector rotation: quaternion product against the rotation matrix | 0.0021 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 0.00132 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 0.00142 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.333 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.338 |
| angle between quaternions 1e-4 rad apart: from a - b and a + b against from a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 0.00296 |
