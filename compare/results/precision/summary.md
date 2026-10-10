The results of GLM, Eigen and cglm are identical, digit for digit, in the runs against master, before and now, for both seeds: every version of hypatia was measured on the same inputs.

## Counts

Mean error within 2% of the best of the other libraries ("best or tied"), more than 2% below it ("ahead"), more than 2% above it ("behind").  ">= 1e6": largest error 1e6 ulps or more (counted in "behind" too).

| seed | precision | version | best or tied | ahead | behind | >= 1e6 | measurements |
|---|---|---|---|---|---|---|---|
| default seed | double | master | 13 | 0 | 24 | 15 | 37 |
| default seed | double | before | 19 | 7 | 22 | 0 | 41 |
| default seed | double | now | 32 | 11 | 9 | 0 | 41 |
| default seed | single | master | 16 | 1 | 21 | 11 | 37 |
| default seed | single | before | 19 | 5 | 22 | 0 | 41 |
| default seed | single | now | 32 | 10 | 9 | 0 | 41 |
| COMPARE_SEED=7 | double | master | 14 | 0 | 23 | 14 | 37 |
| COMPARE_SEED=7 | double | before | 20 | 7 | 21 | 0 | 41 |
| COMPARE_SEED=7 | double | now | 32 | 10 | 9 | 0 | 41 |
| COMPARE_SEED=7 | single | master | 16 | 1 | 21 | 11 | 37 |
| COMPARE_SEED=7 | single | before | 19 | 6 | 22 | 0 | 41 |
| COMPARE_SEED=7 | single | now | 31 | 11 | 10 | 0 | 41 |

## Where hypatia now is ahead (double, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | best other / hypatia | same, seed 7 |
|---|---|---|---|---|---|
| `vector3_rotate_by_quaternion` | q of length 1 +- 1e-6 (drifted) | 3.39 / 0.655 | 2.22e+10 / 6.05e+09 (GLM) | 9.24e+09 | 9.28e+09 |
| `quaternion_get_rotation_tov3` | 1e-3 rad from opposite (landing error) | 2.21 / 0.35 | 4.08e+03 / 639 (Eigen) | 1.83e+03 | 1.83e+03 |
| `quaternion_angle_between` | 1e-4 rad apart | 4.02e+03 / 205 | 9.69e+03 / 1.77e+03 (Eigen) | 8.63 | 8.59 |
| `quaternion_get_rotation_tov3` | random (landing error) | 2.21 / 0.538 | 116 / 0.864 (Eigen) | 1.61 | 1.64 |
| `matrix4_view_lookat_rh` | random (error * sin(view, up)) | 2.01 / 0.407 | 2.31 / 0.45 (GLM) | 1.11 | 1.1 |
| `vector3_rotate_by_quaternion` | unit q | 4.21 / 0.622 | 3.57 / 0.684 (Eigen) | 1.1 | 1.1 |
| `matrix4_inverse` | rotation, scale and translation (error / condition number) | 0.179 / 0.0108 | 0.169 / 0.0114 (Eigen) | 1.06 | 1.05 |
| `matrix3_inverse` | random entries (error / condition number) | 0.555 / 0.0749 | 0.555 / 0.0784 (GLM) | 1.05 | 1.04 |
| `matrix4_set_from_quaternion` | unit q | 2.69 / 0.581 | 2.26 / 0.598 (GLM) | 1.03 | 1.03 |
| `matrix4_inverse` | random entries (error / condition number) | 0.414 / 0.0623 | 0.481 / 0.0637 (Eigen) | 1.02 | 1.03 |
| `quaternion_slerp` | 1e-6 rad apart | 1.9 / 0.491 | 1.89 / 0.502 (Eigen) | 1.02 | 1.02 |

## Where hypatia now is behind (double, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | hypatia / best other | same, seed 7 |
|---|---|---|---|---|---|
| `quaternion_get_rotation_tov3` | 1e-6 rad apart (landing error) | 1.54 / 0.275 | 0.636 / 0.159 (Eigen) | 1.73 | 1.74 |
| `quaternion_angle_between` | random | 2.09 / 0.329 | 2.9 / 0.257 (Eigen) | 1.28 | 1.3 |
| `quaternion_set_from_axis_anglev3` | random | 1.32 / 0.3 | 0.897 / 0.238 (GLM) | 1.26 | 1.27 |
| `matrix4_set_from_axisv3_angle` | random | 2.51 / 0.496 | 2.14 / 0.439 (Eigen) | 1.13 | 1.13 |
| `quaternion_set_from_matrix4` | near half turns (pi - 1e-3) | 1.66 / 0.332 | 1.41 / 0.303 (GLM) | 1.1 | 1.08 |
| `quaternion_set_from_matrix4` | random rotation | 1.8 / 0.357 | 1.45 / 0.326 (GLM) | 1.1 | 1.08 |
| `vector3_normalize` | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 1.14 / 0.131 | 1.14 / 0.124 (Eigen) | 1.06 | 1.06 |
| `quaternion_slerp` | 1e-3 rad apart | 2 / 0.518 | 2.1 / 0.496 (Eigen) | 1.04 | 1.04 |
| `quaternion_slerp` | random | 1.74 / 0.468 | 1.62 / 0.457 (Eigen) | 1.02 | 1.02 |

## Where hypatia now is ahead (single, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | best other / hypatia | same, seed 7 |
|---|---|---|---|---|---|
| `quaternion_get_rotation_tov3` | 1e-3 rad from opposite (landing error) | 2.23 / 0.348 | 4.39e+03 / 942 (GLM) | 2.71e+03 | 2.67e+03 |
| `quaternion_angle_between` | 1e-4 rad apart | 4.25e+03 / 207 | 1.19e+04 / 1.92e+03 (Eigen) | 9.28 | 9.19 |
| `quaternion_get_rotation_tov3` | random (landing error) | 2.58 / 0.536 | 48.8 / 0.863 (Eigen) | 1.61 | 1.63 |
| `vector3_rotate_by_quaternion` | unit q | 3.06 / 0.62 | 3.98 / 0.689 (Eigen) | 1.11 | 1.1 |
| `vector3_rotate_by_quaternion` | q of length 1 +- 1e-6 (drifted) | 3.24 / 0.649 | 3.77 / 0.717 (cglm) | 1.1 | 1.11 |
| `matrix4_view_lookat_rh` | random (error * sin(view, up)) | 1.94 / 0.416 | 2.54 / 0.458 (GLM) | 1.1 | 1.09 |
| `matrix4_inverse` | rotation, scale and translation (error / condition number) | 0.185 / 0.0109 | 0.186 / 0.0115 (Eigen) | 1.06 | 1.06 |
| `matrix3_inverse` | random entries (error / condition number) | 0.794 / 0.0753 | 0.794 / 0.0788 (GLM) | 1.05 | 1.05 |
| `quaternion_get_axis_anglev3` | random (axis * angle) | 1.35 / 0.384 | 1.45 / 0.396 (Eigen) | 1.03 | 1.03 |
| `matrix4_inverse` | random entries (error / condition number) | 0.582 / 0.0619 | 0.602 / 0.0638 (Eigen) | 1.03 | 1.03 |

## Where hypatia now is behind (single, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | hypatia / best other | same, seed 7 |
|---|---|---|---|---|---|
| `quaternion_get_rotation_tov3` | 1e-6 rad apart (landing error) | 1.41 / 0.259 | 0.686 / 0.159 (Eigen) | 1.63 | 1.65 |
| `quaternion_set_from_axis_anglev3` | random | 1.47 / 0.304 | 0.941 / 0.24 (GLM) | 1.27 | 1.27 |
| `quaternion_angle_between` | random | 1.69 / 0.333 | 3.46 / 0.273 (Eigen) | 1.22 | 1.23 |
| `quaternion_set_from_matrix4` | near half turns (pi - 1e-3) | 1.48 / 0.353 | 1.38 / 0.304 (GLM) | 1.16 | 1.16 |
| `matrix4_set_from_axisv3_angle` | random | 2.69 / 0.503 | 2.33 / 0.443 (Eigen) | 1.14 | 1.13 |
| `quaternion_slerp` | 1e-3 rad apart | 1.67 / 0.474 | 1.64 / 0.427 (Eigen) | 1.11 | 1.12 |
| `quaternion_set_from_matrix4` | random rotation | 1.81 / 0.356 | 1.31 / 0.328 (GLM) | 1.09 | 1.09 |
| `quaternion_slerp` | 1e-6 rad apart | 1.47 / 0.376 | 1.25 / 0.358 (Eigen) | 1.05 | 1.05 |
| `quaternion_slerp` | random | 1.85 / 0.478 | 1.85 / 0.468 (Eigen) | 1.02 | 1.02 |

## Where now is less precise than master (default seed)

Mean error more than 2% above master's.

| precision | function | inputs | master (largest / mean) | now (largest / mean) |
|---|---|---|---|---|
| double | `matrix4_set_from_axisv3_angle` | random | 2.14 / 0.439 | 2.51 / 0.496 |
| double | `quaternion_set_from_axis_anglev3` | random | 1.32 / 0.285 | 1.32 / 0.3 |
| single | `matrix4_set_from_axisv3_angle` | random | 2.21 / 0.443 | 2.69 / 0.503 |
| single | `quaternion_set_from_axis_anglev3` | random | 1.31 / 0.286 | 1.47 / 0.304 |
| single | `quaternion_slerp` | 1e-3 rad apart | 1.47 / 0.414 | 1.67 / 0.474 |
| single | `quaternion_slerp` | 1e-6 rad apart | 1.25 / 0.358 | 1.47 / 0.376 |

## Noise: the same measurement with two seeds

Relative change of each mean error between the default seed and COMPARE_SEED=7, over every library and measurement (rows with "inf" or a mean of 0 left out).  Differences between libraries smaller than this are not meaningful.

| precision | measurements | median change | 90th percentile | largest |
|---|---|---|---|---|
| double | 176 | 0.4% | 1.2% | 5.4% |
| single | 217 | 0.44% | 1.1% | 4.5% |

## All measurements (double, default seed)

Largest / mean error in ulps.  Bold: the smallest mean among now and the other libraries, and the means within 2% of it.  Empty: no such function (or, for master, no function of that meaning).

| function | inputs | master | before | now | GLM | Eigen |
|---|---|---|---|---|---|---|
| vector3_normalize | random | 1.09 / 0.321 | 1.12 / 0.345 | **1.09 / 0.321** | 1.34 / 0.371 | **1.09 / 0.321** |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 4.5e+15 / 2.37e+14 | 1.09 / 0.152 | 1.14 / 0.131 | 1.38 / 0.185 | **1.14 / 0.124** |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | 1.26 / 0.248 | 0.992 / 0.334 | **1.26 / 0.248** | 1.34 / 0.307 | **1.26 / 0.248** |
| vector3_magnitude | random | 0.916 / 0.217 | 0.916 / 0.217 | **0.916 / 0.217** | **0.916 / 0.217** | **0.916 / 0.217** |
| vector3_dot_product | random | 1.09 / 0.161 | 1.09 / 0.161 | **1.09 / 0.161** | **1.09 / 0.161** | **1.09 / 0.161** |
| vector3_dot_product | nearly perpendicular | 0.61 / 0.0897 | 0.61 / 0.0897 | **0.61 / 0.0897** | **0.61 / 0.0897** | **0.61 / 0.0897** |
| vector3_cross_product | random | 0.802 / 0.197 | 0.802 / 0.197 | **0.802 / 0.197** | **0.802 / 0.197** | **0.802 / 0.197** |
| vector3_cross_product | nearly parallel (1e-4 rad) | 0.462 / 0.111 | 0.462 / 0.111 | **0.462 / 0.111** | **0.462 / 0.111** | **0.462 / 0.111** |
| vector3_project | random |  | 2.4 / 0.294 | **1.59 / 0.211** | **1.59 / 0.211** |  |
| vector3_rotate_by_quaternion | unit q | 1.1e+16 / 1.74e+15 | 3.14 / 0.739 | **4.21 / 0.622** | 3.57 / 0.686 | 3.57 / 0.684 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | 1.14e+16 / 1.72e+15 | 3.16 / 0.772 | **3.39 / 0.655** | 2.22e+10 / 6.05e+09 | 2.22e+10 / 6.05e+09 |
| matrix4_multiply | random | 1.29 / 0.367 | 1.29 / 0.367 | **1.29 / 0.367** | **1.29 / 0.367** | **1.29 / 0.367** |
| matrix4_inverse | random entries (error / condition number) | 0.9 / 0.0817 | 0.433 / 0.0631 | **0.414 / 0.0623** | 0.523 / 0.0641 | 0.481 / 0.0637 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.186 / 0.0121 | 0.179 / 0.011 | **0.179 / 0.0108** | 0.155 / 0.0115 | 0.169 / 0.0114 |
| matrix4_inverse | condition ~1e4 (error / condition number) | 6.21e+11 / 2.82e+11 | 13.5 / 0.958 | **14 / 0.963** | 13.7 / 1.01 | **15.6 / 0.956** |
| matrix3_inverse | random entries (error / condition number) | 0.555 / 0.0784 | 0.555 / 0.0749 | **0.555 / 0.0749** | 0.555 / 0.0784 | 0.642 / 0.0787 |
| matrix4_determinant | random entries | 3.49 / 0.349 | 3.4 / 0.265 | **3.4 / 0.259** | **2.7 / 0.26** | **2.99 / 0.258** |
| matrix4_normal_matrix | rotation, scale and translation |  | 1.86 / 0.38 | **1.86 / 0.38** | **2.04 / 0.38** |  |
| matrix4_set_from_axisv3_angle | random | 2.14 / 0.439 | 4.04 / 0.594 | 2.51 / 0.496 | 4.45 / 0.599 | **2.14 / 0.439** |
| matrix4_set_from_axisv3_angle | angle 1e-4 | 0.368 / 0.229 | 0.368 / 0.229 | **0.368 / 0.229** | **0.368 / 0.229** | **0.368 / 0.229** |
| matrix4_set_from_quaternion | unit q | 9e+15 / 5.01e+15 | 4.78 / 0.904 | **2.69 / 0.581** | 2.26 / 0.598 | 2.26 / 0.598 |
| matrix4_set_from_euler_anglesf3 | random | 9.01e+15 / 7.6e+15 | 1.26 / 0.393 | **1.26 / 0.393** | **1.22 / 0.39** |  |
| matrix4_projection_perspective_fovy_rh | random | 8.93e+15 / 3.67e+15 | 1.24 / 0.35 | **1.24 / 0.35** | **1.11 / 0.357** |  |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | 9.01e+15 / 5.88e+15 | 1.96 / 0.43 | **2.01 / 0.407** | 2.31 / 0.45 |  |
| quaternion_multiply | random | 1.27 / 0.3 | 1.27 / 0.3 | **1.01 / 0.289** | 1.18 / 0.298 | **1.07 / 0.289** |
| quaternion_normalize | random length | 1.26 / 0.284 | 1.16 / 0.361 | **1.18 / 0.28** | 1.46 / 0.356 | **1.19 / 0.28** |
| quaternion_inverse | random length | 1.46 / 0.396 | 2.58 / 0.609 | **1.41 / 0.383** | **1.4 / 0.385** | **1.36 / 0.385** |
| quaternion_set_from_axis_anglev3 | random | 1.32 / 0.285 | 1.68 / 0.398 | 1.32 / 0.3 | **0.897 / 0.238** | **0.897 / 0.238** |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | 0.033 / 0.033 | 0.533 / 0.533 | **0.033 / 0.033** | **0.033 / 0.033** | **0.033 / 0.033** |
| quaternion_get_axis_anglev3 | random (axis * angle) | 31.7 / 0.592 | 1.5 / 0.417 | **1.33 / 0.383** | 70.1 / 0.645 | **1.33 / 0.383** |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | 1.41e+06 / 1.41e+06 | 1.34 / 0.401 | **0.989 / 0.349** | 1.18e+07 / 1.18e+07 | **0.989 / 0.349** |
| quaternion_set_from_matrix4 | random rotation |  | 1.8 / 0.439 | 1.8 / 0.357 | **1.45 / 0.326** | **1.45 / 0.329** |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) |  | 1.55 / 0.422 | 1.66 / 0.332 | **1.41 / 0.303** | **1.41 / 0.303** |
| quaternion_slerp | random | 2.99e+10 / 1.5e+06 | 1.83 / 0.467 | 1.74 / 0.468 | 2.07 / 0.487 | **1.62 / 0.457** |
| quaternion_slerp | 1e-3 rad apart | 1.41e+08 / 9.36e+07 | 1.93 / 0.519 | 2 / 0.518 | 1.9 / 0.535 | **2.1 / 0.496** |
| quaternion_slerp | 1e-6 rad apart | 1.4e+04 / 94.4 | 1.84 / 0.488 | **1.9 / 0.491** | 2 / 0.538 | 1.89 / 0.502 |
| quaternion_get_rotation_tov3 | random (landing error) | 9.01e+15 / 5.9e+15 | 2.75 / 0.615 | **2.21 / 0.538** | 116 / 0.957 | 116 / 0.864 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | 9.01e+15 / 9.01e+15 | 2.78 / 0.434 | **2.21 / 0.35** | 4.09e+03 / 844 | 4.08e+03 / 639 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 4e+09 / 3.56e+09 | 1.58 / 0.321 | 1.54 / 0.275 | 0.666 / 0.163 | **0.636 / 0.159** |
| quaternion_angle_between | random | 610 / 0.845 | 7.65 / 0.361 | 2.09 / 0.329 |  | **2.9 / 0.257** |
| quaternion_angle_between | 1e-4 rad apart | 9.87e+08 / 1.71e+08 | 1.62e+04 / 2.1e+03 | **4.02e+03 / 205** |  | 9.69e+03 / 1.77e+03 |

## All measurements (single, default seed)

Largest / mean error in ulps.  Bold: the smallest mean among now and the other libraries, and the means within 2% of it.  Empty: no such function (or, for master, no function of that meaning).

| function | inputs | master | before | now | GLM | Eigen | cglm |
|---|---|---|---|---|---|---|---|
| vector3_normalize | random | 1.22 / 0.32 | 1.11 / 0.348 | **1.22 / 0.32** | 1.29 / 0.37 | **1.11 / 0.32** | 1.29 / 0.37 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 8.39e+06 / 3.09e+05 | 0.944 / 0.102 | **1.05 / 0.0851** | 1.19 / 0.148 | **1.05 / 0.0851** | 8.39e+06 / 1.68e+05 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | 0.00293 / 0.000282 | 0.00293 / 0.000282 | **0.00293 / 0.000282** | 0.5 / 0.0747 | **0.00293 / 0.000282** | 0.5 / 0.0747 |
| vector3_magnitude | random | 0.995 / 0.217 | 0.995 / 0.217 | **0.995 / 0.217** | **0.995 / 0.217** | **0.965 / 0.217** | **0.995 / 0.217** |
| vector3_dot_product | random | 0.951 / 0.16 | 0.951 / 0.16 | **0.951 / 0.16** | **0.951 / 0.16** | **0.948 / 0.159** | **0.951 / 0.16** |
| vector3_dot_product | nearly perpendicular | 0.557 / 0.0915 | 0.557 / 0.0915 | **0.557 / 0.0915** | **0.557 / 0.0915** | **0.56 / 0.0909** | **0.557 / 0.0915** |
| vector3_cross_product | random | 0.791 / 0.196 | 0.791 / 0.196 | **0.791 / 0.196** | **0.791 / 0.196** | **0.791 / 0.196** | **0.791 / 0.196** |
| vector3_cross_product | nearly parallel (1e-4 rad) | 0.45 / 0.111 | 0.45 / 0.111 | **0.45 / 0.111** | **0.45 / 0.111** | **0.45 / 0.111** | **0.45 / 0.111** |
| vector3_project | random |  | 2 / 0.296 | **1.64 / 0.211** | **1.64 / 0.211** |  |  |
| vector3_rotate_by_quaternion | unit q | 2.05e+07 / 3.23e+06 | 2.95 / 0.737 | **3.06 / 0.62** | 3.54 / 0.703 | 3.98 / 0.689 | 3.38 / 0.713 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | 2.13e+07 / 3.2e+06 | 2.89 / 0.762 | **3.24 / 0.649** | 41.4 / 11.3 | 41.4 / 11.3 | 3.77 / 0.717 |
| matrix4_multiply | random | 1.15 / 0.366 | 1.15 / 0.366 | **1.15 / 0.366** | **1.15 / 0.366** | **1.15 / 0.366** | **1.15 / 0.366** |
| matrix4_inverse | random entries (error / condition number) | 1.18 / 0.0817 | 0.4 / 0.063 | **0.582 / 0.0619** | 0.405 / 0.0646 | 0.602 / 0.0638 | 0.411 / 0.0645 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.205 / 0.0121 | 0.192 / 0.011 | **0.185 / 0.0109** | 0.206 / 0.0116 | 0.186 / 0.0115 | 0.206 / 0.0118 |
| matrix4_inverse | condition ~1e4 (error / condition number) | 1.16e+03 / 525 | 13.6 / 0.965 | **13.7 / 0.97** | 17.6 / 1.02 | **16 / 0.97** | 17.6 / 1.02 |
| matrix3_inverse | random entries (error / condition number) | 0.794 / 0.0788 | 0.794 / 0.0753 | **0.794 / 0.0753** | 0.794 / 0.0788 | 0.578 / 0.0788 |  |
| matrix4_determinant | random entries | 3.11 / 0.35 | 3.37 / 0.265 | **2.81 / 0.256** | 3.39 / 0.262 | **3.16 / 0.259** | **3.39 / 0.261** |
| matrix4_normal_matrix | rotation, scale and translation |  | 2.15 / 0.382 | **2.15 / 0.382** | **1.95 / 0.38** |  |  |
| matrix4_set_from_axisv3_angle | random | 2.21 / 0.443 | 3.71 / 0.599 | 2.69 / 0.503 | 4.19 / 0.608 | **2.33 / 0.443** | 4.19 / 0.608 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | 0.0419 / 0.0384 | 0.0419 / 0.0384 | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** |
| matrix4_set_from_quaternion | unit q | 1.68e+07 / 9.32e+06 | 4.9 / 0.91 | **2.89 / 0.579** | 2.25 / 0.595 | 2.25 / 0.595 | **2.89 / 0.58** |
| matrix4_set_from_euler_anglesf3 | random | 1.68e+07 / 1.42e+07 | 1.15 / 0.392 | **1.15 / 0.392** | **1.22 / 0.39** |  | **1.22 / 0.389** |
| matrix4_projection_perspective_fovy_rh | random | 1.66e+07 / 6.84e+06 | 1.21 / 0.353 | **1.21 / 0.353** | **1.19 / 0.359** |  | 1.33 / 0.38 |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | 1.68e+07 / 1.1e+07 | 1.79 / 0.434 | **1.94 / 0.416** | 2.54 / 0.458 |  | 2.54 / 0.458 |
| quaternion_multiply | random | 1.15 / 0.302 | 1.15 / 0.302 | **1.06 / 0.29** | 1.16 / 0.302 | **1.04 / 0.289** | 1.15 / 0.302 |
| quaternion_normalize | random length | 1.44 / 0.283 | 1.15 / 0.358 | **1.13 / 0.279** | 1.34 / 0.355 | **1.19 / 0.28** | **1.19 / 0.28** |
| quaternion_inverse | random length | 1.46 / 0.398 | 2.33 / 0.607 | **1.43 / 0.387** | **1.37 / 0.388** | **1.43 / 0.385** | 1.55 / 0.431 |
| quaternion_set_from_axis_anglev3 | random | 1.31 / 0.286 | 1.55 / 0.397 | 1.47 / 0.304 | **0.941 / 0.24** | **0.941 / 0.24** | 1.84 / 0.309 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | 0.0105 / 0.0105 | 0.0105 / 0.0105 | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** |
| quaternion_get_axis_anglev3 | random (axis * angle) | 13.6 / 0.648 | 1.48 / 0.417 | **1.35 / 0.384** | 51 / 0.715 | 1.45 / 0.396 | 6.92 / 0.704 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | 8.39e+06 / 8.39e+06 | 1.03 / 0.284 | **0.532 / 0.09** | 2.28e+07 / 1.19e+07 | **0.533 / 0.09** | 1.01 / 0.374 |
| quaternion_set_from_matrix4 | random rotation |  | 1.78 / 0.443 | 1.81 / 0.356 | **1.31 / 0.328** | **1.73 / 0.329** | 1.85 / 0.337 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) |  | 1.66 / 0.436 | 1.48 / 0.353 | **1.38 / 0.304** | **1.38 / 0.304** | 1.62 / 0.329 |
| quaternion_slerp | random | 55.7 / 0.47 | 1.85 / 0.479 | 1.85 / 0.478 | 1.85 / 0.499 | **1.85 / 0.468** | 16.6 / 0.61 |
| quaternion_slerp | 1e-3 rad apart | 1.47 / 0.414 | 1.67 / 0.475 | 1.67 / 0.474 | 1.96 / 0.575 | **1.64 / 0.427** | 5.89e+03 / 52.3 |
| quaternion_slerp | 1e-6 rad apart | 1.25 / 0.358 | 1.52 / 0.378 | 1.47 / 0.376 | **1.5 / 0.362** | **1.25 / 0.358** | 6.79 / 1.94 |
| quaternion_get_rotation_tov3 | random (landing error) | 1.68e+07 / 1.1e+07 | 2.68 / 0.616 | **2.58 / 0.536** | 99.4 / 0.965 | 48.8 / 0.863 | 99.3 / 0.907 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | 1.68e+07 / 1.68e+07 | 2.73 / 0.433 | **2.23 / 0.348** | 4.39e+03 / 942 | 1.8e+04 / 1.66e+04 | 8.39e+03 / 8.39e+03 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 7.69 / 6.64 | 1.63 / 0.298 | 1.41 / 0.259 | 8.73 / 8.39 | **0.686 / 0.159** | 8.73 / 8.39 |
| quaternion_angle_between | random | 169 / 0.781 | 8.11 / 0.361 | 1.69 / 0.333 |  | **3.46 / 0.273** |  |
| quaternion_angle_between | 1e-4 rad apart | inf | 1.7e+04 / 2.12e+03 | **4.25e+03 / 207** |  | 1.19e+04 / 1.92e+03 |  |

## Oracle check (now, default seed)

double:

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

single:

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

