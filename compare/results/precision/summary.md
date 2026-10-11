The results of GLM, Eigen and cglm are identical, digit for digit, in the runs against master, before and now, for both seeds: every version of hypatia was measured on the same inputs.

## Counts

Mean error within 2% of the best of the other libraries ("best or tied"), more than 2% below it ("ahead"), more than 2% above it ("behind").  ">= 1e6": largest error 1e6 ulps or more (counted in "behind" too).

| seed | precision | version | best or tied | ahead | behind | >= 1e6 | measurements |
|---|---|---|---|---|---|---|---|
| default seed | double | master | 13 | 0 | 24 | 15 | 37 |
| default seed | double | before | 28 | 10 | 13 | 0 | 41 |
| default seed | double | now | 46 | 29 | 0 | 0 | 46 |
| default seed | single | master | 16 | 1 | 21 | 11 | 37 |
| default seed | single | before | 28 | 10 | 13 | 0 | 41 |
| default seed | single | now | 46 | 27 | 0 | 0 | 46 |
| COMPARE_SEED=7 | double | master | 14 | 0 | 23 | 14 | 37 |
| COMPARE_SEED=7 | double | before | 28 | 9 | 13 | 0 | 41 |
| COMPARE_SEED=7 | double | now | 46 | 29 | 0 | 0 | 46 |
| COMPARE_SEED=7 | single | master | 16 | 1 | 21 | 11 | 37 |
| COMPARE_SEED=7 | single | before | 27 | 11 | 14 | 0 | 41 |
| COMPARE_SEED=7 | single | now | 46 | 27 | 0 | 0 | 46 |

## Where hypatia now is ahead (double, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | best other / hypatia | same, seed 7 |
|---|---|---|---|---|---|
| `vector3_rotate_by_quaternion` | q of length 1 +- 1e-6 (drifted) | 3.39 / 0.655 | 2.22e+10 / 6.05e+09 (GLM) | 9.24e+09 | 9.28e+09 |
| `quaternion_angle_between` | 1e-4 rad apart | 0.505 / 0.219 | 9.69e+03 / 1.77e+03 (Eigen) | 8.08e+03 | 8.16e+03 |
| `quaternion_get_rotation_tov3` | 1e-3 rad from opposite (landing error) | 2.71 / 0.358 | 4.08e+03 / 639 (Eigen) | 1.78e+03 | 1.8e+03 |
| `matrix4_inverse` | condition ~1e4 (error / condition number) | 2.5 / 0.137 | 15.6 / 0.956 (Eigen) | 6.98 | 6.88 |
| `vector3_normalize` | one large, two small (1, 1e-5, 1e-5) | 0.25 / 0.126 | 1.26 / 0.248 (Eigen) | 1.97 | 1.99 |
| `vector3_normalize` | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 0.351 / 0.065 | 1.14 / 0.124 (Eigen) | 1.91 | 1.86 |
| `quaternion_get_rotation_tov3` | random (landing error) | 2.15 / 0.463 | 116 / 0.864 (Eigen) | 1.87 | 1.88 |
| `vector3_normalize` | random | 0.426 / 0.203 | 1.09 / 0.321 (Eigen) | 1.58 | 1.58 |
| `vector3_reflect` | random (relative to the length of v) | 4.13 / 0.458 | 4.48 / 0.657 (GLM) | 1.43 | 1.44 |
| `quaternion_normalize` | random length | 0.463 / 0.199 | 1.19 / 0.28 (Eigen) | 1.41 | 1.42 |
| `quaternion_get_rotation_tov3` | 1e-6 rad apart (landing error) | 0.601 / 0.122 | 0.636 / 0.159 (Eigen) | 1.3 | 1.3 |
| `vector3_refract` | random, eta 0.5 .. 2 | 60.3 / 0.321 | 60.3 / 0.413 (GLM) | 1.29 | 1.29 |
| `matrix4_view_lookat_rh` | random (error * sin(view, up)) | 1.77 / 0.351 | 2.31 / 0.45 (GLM) | 1.28 | 1.26 |
| `matrix3_inverse` | random entries (error / condition number) | 0.464 / 0.0632 | 0.555 / 0.0784 (GLM) | 1.24 | 1.24 |
| `vector3_unproject_from_window` | random camera, window depth 0 .. 0.9 | 86.9 / 1.77 | 100 / 2.17 (GLM) | 1.23 | 1.22 |
| `matrix4_determinant` | random entries | 2.77 / 0.214 | 2.99 / 0.258 (Eigen) | 1.21 | 1.21 |
| `quaternion_set_look_rotation_rh` | random | 12.4 / 0.35 | 14.8 / 0.417 (GLM) | 1.19 | 7.66e+10 |
| `quaternion_angle_between` | random | 0.916 / 0.221 | 2.9 / 0.257 (Eigen) | 1.16 | 1.16 |
| `matrix4_inverse` | random entries (error / condition number) | 0.51 / 0.0552 | 0.481 / 0.0637 (Eigen) | 1.15 | 1.17 |
| `quaternion_slerp` | 1e-6 rad apart | 1.76 / 0.437 | 1.89 / 0.502 (Eigen) | 1.15 | 1.16 |
| `quaternion_slerp` | random | 1.57 / 0.399 | 1.62 / 0.457 (Eigen) | 1.15 | 1.15 |
| `matrix4_inverse` | rotation, scale and translation (error / condition number) | 0.169 / 0.00997 | 0.169 / 0.0114 (Eigen) | 1.14 | 1.13 |
| `matrix4_normal_matrix` | rotation, scale and translation | 1.69 / 0.339 | 2.04 / 0.38 (GLM) | 1.12 | 1.12 |
| `quaternion_set_from_matrix4` | near half turns (pi - 1e-3) | 1.53 / 0.271 | 1.41 / 0.303 (GLM) | 1.12 | 1.13 |
| `quaternion_set_from_matrix4` | random rotation | 1.8 / 0.296 | 1.45 / 0.326 (GLM) | 1.1 | 1.12 |
| `vector3_rotate_by_quaternion` | unit q | 4.21 / 0.622 | 3.57 / 0.684 (Eigen) | 1.1 | 1.1 |
| `quaternion_slerp` | 1e-3 rad apart | 1.83 / 0.462 | 2.1 / 0.496 (Eigen) | 1.07 | 1.07 |
| `matrix4_set_from_quaternion` | unit q | 2.69 / 0.581 | 2.26 / 0.598 (GLM) | 1.03 | 1.03 |
| `quaternion_get_axis_anglev3` | random (axis * angle) | 1.28 / 0.374 | 1.33 / 0.383 (Eigen) | 1.02 | 1.03 |

## Where hypatia now is behind (double, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | hypatia / best other | same, seed 7 |
|---|---|---|---|---|---|

## Where hypatia now is ahead (single, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | best other / hypatia | same, seed 7 |
|---|---|---|---|---|---|
| `quaternion_angle_between` | 1e-4 rad apart | 0.307 / 0.153 | 1.19e+04 / 1.92e+03 (Eigen) | 1.25e+04 | 1.25e+04 |
| `quaternion_get_rotation_tov3` | 1e-3 rad from opposite (landing error) | 2.21 / 0.349 | 4.39e+03 / 942 (GLM) | 2.7e+03 | 2.65e+03 |
| `matrix4_inverse` | condition ~1e4 (error / condition number) | 2.97 / 0.144 | 16 / 0.97 (Eigen) | 6.74 | 6.61 |
| `quaternion_get_rotation_tov3` | random (landing error) | 2.12 / 0.468 | 48.8 / 0.863 (Eigen) | 1.84 | 1.89 |
| `vector3_normalize` | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 0.351 / 0.047 | 1.05 / 0.0851 (Eigen) | 1.81 | 1.75 |
| `vector3_normalize` | random | 0.427 / 0.203 | 1.11 / 0.32 (Eigen) | 1.58 | 1.59 |
| `vector3_reflect` | random (relative to the length of v) | 3.59 / 0.466 | 4.41 / 0.664 (GLM) | 1.42 | 1.43 |
| `quaternion_normalize` | random length | 0.481 / 0.198 | 1.19 / 0.28 (Eigen) | 1.41 | 1.43 |
| `vector3_refract` | random, eta 0.5 .. 2 | 99.5 / 0.318 | 186 / 0.421 (GLM) | 1.32 | 1.33 |
| `quaternion_get_rotation_tov3` | 1e-6 rad apart (landing error) | 0.598 / 0.122 | 0.686 / 0.159 (Eigen) | 1.3 | 1.3 |
| `matrix4_view_lookat_rh` | random (error * sin(view, up)) | 1.47 / 0.36 | 2.54 / 0.458 (GLM) | 1.27 | 1.27 |
| `matrix3_inverse` | random entries (error / condition number) | 0.574 / 0.0635 | 0.794 / 0.0788 (GLM) | 1.24 | 1.24 |
| `quaternion_angle_between` | random | 0.868 / 0.221 | 3.46 / 0.273 (Eigen) | 1.24 | 1.22 |
| `vector3_unproject_from_window` | random camera, window depth 0 .. 0.9 | 70.3 / 1.77 | 58.1 / 2.16 (GLM) | 1.22 | 1.22 |
| `matrix4_determinant` | random entries | 2.21 / 0.215 | 3.16 / 0.259 (Eigen) | 1.2 | 1.17 |
| `quaternion_set_look_rotation_rh` | random | 7.7 / 0.348 | 18.1 / 0.417 (GLM) | 1.2 | 1.23 |
| `matrix4_inverse` | random entries (error / condition number) | 0.568 / 0.0552 | 0.602 / 0.0638 (Eigen) | 1.16 | 1.16 |
| `quaternion_slerp` | 1e-6 rad apart | 1.18 / 0.31 | 1.25 / 0.358 (Eigen) | 1.15 | 1.17 |
| `matrix4_inverse` | rotation, scale and translation (error / condition number) | 0.15 / 0.01 | 0.186 / 0.0115 (Eigen) | 1.15 | 1.14 |
| `quaternion_slerp` | random | 1.5 / 0.408 | 1.85 / 0.468 (Eigen) | 1.15 | 1.14 |
| `matrix4_normal_matrix` | rotation, scale and translation | 1.7 / 0.34 | 1.95 / 0.38 (GLM) | 1.12 | 1.12 |
| `quaternion_set_from_matrix4` | random rotation | 1.78 / 0.295 | 1.31 / 0.328 (GLM) | 1.11 | 1.11 |
| `vector3_rotate_by_quaternion` | unit q | 3.06 / 0.62 | 3.98 / 0.689 (Eigen) | 1.11 | 1.1 |
| `vector3_rotate_by_quaternion` | q of length 1 +- 1e-6 (drifted) | 3.24 / 0.649 | 3.77 / 0.717 (cglm) | 1.1 | 1.11 |
| `quaternion_set_from_matrix4` | near half turns (pi - 1e-3) | 1.53 / 0.279 | 1.38 / 0.304 (GLM) | 1.09 | 1.09 |
| `quaternion_get_axis_anglev3` | random (axis * angle) | 1.23 / 0.373 | 1.45 / 0.396 (Eigen) | 1.06 | 1.06 |
| `quaternion_slerp` | 1e-3 rad apart | 1.47 / 0.415 | 1.64 / 0.427 (Eigen) | 1.03 | 1.01 |

## Where hypatia now is behind (single, default seed)

Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.

| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | hypatia / best other | same, seed 7 |
|---|---|---|---|---|---|

## Where now is less precise than master (default seed)

Mean error more than 2% above master's.

| precision | function | inputs | master (largest / mean) | now (largest / mean) |
|---|---|---|---|---|

## Noise: the same measurement with two seeds

Relative change of each mean error between the default seed and COMPARE_SEED=7, over every library and measurement (rows with "inf" or a mean of 0 left out).  Differences between libraries smaller than this are not meaningful.

| precision | measurements | median change | 90th percentile | largest |
|---|---|---|---|---|
| double | 186 | 0.4% | 1.3% | 1e+02% |
| single | 229 | 0.42% | 1.2% | 99% |

The measurements whose mean changes by more than 5%:

| precision | library | function | inputs | default seed | COMPARE_SEED=7 |
|---|---|---|---|---|---|
| double | hypatia master | `quaternion_angle_between` | random | 610 / 0.845 | 398 / 0.799 |
| double | GLM | `quaternion_set_look_rotation_rh` | random | 14.8 / 0.417 | 5.36e+14 / 2.68e+10 |
| single | GLM | `quaternion_set_look_rotation_rh` | random | 18.1 / 0.417 | 9.99e+05 / 50.4 |

## All measurements (double, default seed)

Largest / mean error in ulps.  Bold: the smallest mean among now and the other libraries, and the means within 2% of it.  Empty: no such function (or, for master, no function of that meaning).

| function | inputs | master | before | now | GLM | Eigen |
|---|---|---|---|---|---|---|
| vector3_normalize | random | 1.09 / 0.321 | 1.09 / 0.321 | **0.426 / 0.203** | 1.34 / 0.371 | 1.09 / 0.321 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 4.5e+15 / 2.37e+14 | 1.14 / 0.131 | **0.351 / 0.065** | 1.38 / 0.185 | 1.14 / 0.124 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | 1.26 / 0.248 | 1.26 / 0.248 | **0.25 / 0.126** | 1.34 / 0.307 | 1.26 / 0.248 |
| vector3_magnitude | random | 0.916 / 0.217 | 0.916 / 0.217 | **0.916 / 0.217** | **0.916 / 0.217** | **0.916 / 0.217** |
| vector3_dot_product | random | 1.09 / 0.161 | 1.09 / 0.161 | **1.09 / 0.161** | **1.09 / 0.161** | **1.09 / 0.161** |
| vector3_dot_product | nearly perpendicular | 0.61 / 0.0897 | 0.61 / 0.0897 | **0.61 / 0.0897** | **0.61 / 0.0897** | **0.61 / 0.0897** |
| vector3_cross_product | random | 0.802 / 0.197 | 0.802 / 0.197 | **0.802 / 0.197** | **0.802 / 0.197** | **0.802 / 0.197** |
| vector3_cross_product | nearly parallel (1e-4 rad) | 0.462 / 0.111 | 0.462 / 0.111 | **0.462 / 0.111** | **0.462 / 0.111** | **0.462 / 0.111** |
| vector3_project | random |  | 1.88 / 0.284 | **1.59 / 0.211** | **1.59 / 0.211** |  |
| vector3_rotate_by_quaternion | unit q | 1.1e+16 / 1.74e+15 | 4.21 / 0.629 | **4.21 / 0.622** | 3.57 / 0.686 | 3.57 / 0.684 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | 1.14e+16 / 1.72e+15 | 3.39 / 0.657 | **3.39 / 0.655** | 2.22e+10 / 6.05e+09 | 2.22e+10 / 6.05e+09 |
| matrix4_multiply | random | 1.29 / 0.367 | 1.29 / 0.367 | **1.29 / 0.367** | **1.29 / 0.367** | **1.29 / 0.367** |
| matrix4_inverse | random entries (error / condition number) | 0.9 / 0.0817 | 0.414 / 0.0623 | **0.51 / 0.0552** | 0.523 / 0.0641 | 0.481 / 0.0637 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.186 / 0.0121 | 0.179 / 0.0108 | **0.169 / 0.00997** | 0.155 / 0.0115 | 0.169 / 0.0114 |
| matrix4_inverse | condition ~1e4 (error / condition number) | 6.21e+11 / 2.82e+11 | 14 / 0.963 | **2.5 / 0.137** | 13.7 / 1.01 | 15.6 / 0.956 |
| matrix3_inverse | random entries (error / condition number) | 0.555 / 0.0784 | 0.555 / 0.0749 | **0.464 / 0.0632** | 0.555 / 0.0784 | 0.642 / 0.0787 |
| matrix4_determinant | random entries | 3.49 / 0.349 | 3.4 / 0.259 | **2.77 / 0.214** | 2.7 / 0.26 | 2.99 / 0.258 |
| matrix4_normal_matrix | rotation, scale and translation |  | 1.86 / 0.38 | **1.69 / 0.339** | 2.04 / 0.38 |  |
| matrix4_set_from_axisv3_angle | random | 2.14 / 0.439 | 2.51 / 0.496 | **2.14 / 0.439** | 4.45 / 0.599 | **2.14 / 0.439** |
| matrix4_set_from_axisv3_angle | angle 1e-4 | 0.368 / 0.229 | 0.368 / 0.229 | **0.368 / 0.229** | **0.368 / 0.229** | **0.368 / 0.229** |
| matrix4_set_from_quaternion | unit q | 9e+15 / 5.01e+15 | 3.42 / 0.75 | **2.69 / 0.581** | 2.26 / 0.598 | 2.26 / 0.598 |
| matrix4_set_from_euler_anglesf3 | random | 9.01e+15 / 7.6e+15 | 1.26 / 0.393 | **1.26 / 0.393** | **1.22 / 0.39** |  |
| matrix4_projection_perspective_fovy_rh | random | 8.93e+15 / 3.67e+15 | 1.24 / 0.35 | **1.24 / 0.35** | **1.11 / 0.357** |  |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | 9.01e+15 / 5.88e+15 | 2.01 / 0.407 | **1.77 / 0.351** | 2.31 / 0.45 |  |
| quaternion_multiply | random | 1.27 / 0.3 | 1.27 / 0.3 | **1.01 / 0.289** | 1.18 / 0.298 | **1.07 / 0.289** |
| quaternion_normalize | random length | 1.26 / 0.284 | 1.18 / 0.28 | **0.463 / 0.199** | 1.46 / 0.356 | 1.19 / 0.28 |
| quaternion_inverse | random length | 1.46 / 0.396 | 2.22 / 0.496 | **1.41 / 0.383** | **1.4 / 0.385** | **1.36 / 0.385** |
| quaternion_set_from_axis_anglev3 | random | 1.32 / 0.285 | 1.32 / 0.3 | **0.897 / 0.238** | **0.897 / 0.238** | **0.897 / 0.238** |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | 0.033 / 0.033 | 0.033 / 0.033 | **0.033 / 0.033** | **0.033 / 0.033** | **0.033 / 0.033** |
| quaternion_get_axis_anglev3 | random (axis * angle) | 31.7 / 0.592 | 1.33 / 0.383 | **1.28 / 0.374** | 70.1 / 0.645 | 1.33 / 0.383 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | 1.41e+06 / 1.41e+06 | 0.989 / 0.349 | **1.01 / 0.346** | 1.18e+07 / 1.18e+07 | **0.989 / 0.349** |
| quaternion_set_from_matrix4 | random rotation |  | 1.8 / 0.357 | **1.8 / 0.296** | 1.45 / 0.326 | 1.45 / 0.329 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) |  | 1.66 / 0.332 | **1.53 / 0.271** | 1.41 / 0.303 | 1.41 / 0.303 |
| quaternion_slerp | random | 2.99e+10 / 1.5e+06 | 1.74 / 0.469 | **1.57 / 0.399** | 2.07 / 0.487 | 1.62 / 0.457 |
| quaternion_slerp | 1e-3 rad apart | 1.41e+08 / 9.36e+07 | 2 / 0.517 | **1.83 / 0.462** | 1.9 / 0.535 | 2.1 / 0.496 |
| quaternion_slerp | 1e-6 rad apart | 1.4e+04 / 94.4 | 1.9 / 0.491 | **1.76 / 0.437** | 2 / 0.538 | 1.89 / 0.502 |
| quaternion_get_rotation_tov3 | random (landing error) | 9.01e+15 / 5.9e+15 | 2.21 / 0.538 | **2.15 / 0.463** | 116 / 0.957 | 116 / 0.864 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | 9.01e+15 / 9.01e+15 | 2.21 / 0.35 | **2.71 / 0.358** | 4.09e+03 / 844 | 4.08e+03 / 639 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 4e+09 / 3.56e+09 | 1.54 / 0.275 | **0.601 / 0.122** | 0.666 / 0.163 | 0.636 / 0.159 |
| quaternion_angle_between | random | 610 / 0.845 | 2.09 / 0.333 | **0.916 / 0.221** |  | 2.9 / 0.257 |
| quaternion_angle_between | 1e-4 rad apart | 9.87e+08 / 1.71e+08 | 4.02e+03 / 205 | **0.505 / 0.219** |  | 9.69e+03 / 1.77e+03 |
| vector3_reflect | random (relative to the length of v) |  |  | **4.13 / 0.458** | 4.48 / 0.657 |  |
| vector3_refract | random, eta 0.5 .. 2 |  |  | **60.3 / 0.321** | 60.3 / 0.413 |  |
| quaternion_set_look_rotation_rh | random |  |  | **12.4 / 0.35** | 14.8 / 0.417 |  |
| vector3_project_to_window | random camera, a point in the view |  |  | **260 / 2.2** | **260 / 2.2** |  |
| vector3_unproject_from_window | random camera, window depth 0 .. 0.9 |  |  | **86.9 / 1.77** | 100 / 2.17 |  |

## All measurements (single, default seed)

Largest / mean error in ulps.  Bold: the smallest mean among now and the other libraries, and the means within 2% of it.  Empty: no such function (or, for master, no function of that meaning).

| function | inputs | master | before | now | GLM | Eigen | cglm |
|---|---|---|---|---|---|---|---|
| vector3_normalize | random | 1.22 / 0.32 | 1.22 / 0.32 | **0.427 / 0.203** | 1.29 / 0.37 | 1.11 / 0.32 | 1.29 / 0.37 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | 8.39e+06 / 3.09e+05 | 1.05 / 0.0851 | **0.351 / 0.047** | 1.19 / 0.148 | 1.05 / 0.0851 | 8.39e+06 / 1.68e+05 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | 0.00293 / 0.000282 | 0.00293 / 0.000282 | **0.00293 / 0.000282** | 0.5 / 0.0747 | **0.00293 / 0.000282** | 0.5 / 0.0747 |
| vector3_magnitude | random | 0.995 / 0.217 | 0.995 / 0.217 | **0.995 / 0.217** | **0.995 / 0.217** | **0.965 / 0.217** | **0.995 / 0.217** |
| vector3_dot_product | random | 0.951 / 0.16 | 0.951 / 0.16 | **0.951 / 0.16** | **0.951 / 0.16** | **0.948 / 0.159** | **0.951 / 0.16** |
| vector3_dot_product | nearly perpendicular | 0.557 / 0.0915 | 0.557 / 0.0915 | **0.557 / 0.0915** | **0.557 / 0.0915** | **0.56 / 0.0909** | **0.557 / 0.0915** |
| vector3_cross_product | random | 0.791 / 0.196 | 0.791 / 0.196 | **0.791 / 0.196** | **0.791 / 0.196** | **0.791 / 0.196** | **0.791 / 0.196** |
| vector3_cross_product | nearly parallel (1e-4 rad) | 0.45 / 0.111 | 0.45 / 0.111 | **0.45 / 0.111** | **0.45 / 0.111** | **0.45 / 0.111** | **0.45 / 0.111** |
| vector3_project | random |  | 1.97 / 0.282 | **1.64 / 0.211** | **1.64 / 0.211** |  |  |
| vector3_rotate_by_quaternion | unit q | 2.05e+07 / 3.23e+06 | 3.42 / 0.628 | **3.06 / 0.62** | 3.54 / 0.703 | 3.98 / 0.689 | 3.38 / 0.713 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | 2.13e+07 / 3.2e+06 | 3.22 / 0.652 | **3.24 / 0.649** | 41.4 / 11.3 | 41.4 / 11.3 | 3.77 / 0.717 |
| matrix4_multiply | random | 1.15 / 0.366 | 1.15 / 0.366 | **1.15 / 0.366** | **1.15 / 0.366** | **1.15 / 0.366** | **1.15 / 0.366** |
| matrix4_inverse | random entries (error / condition number) | 1.18 / 0.0817 | 0.582 / 0.0619 | **0.568 / 0.0552** | 0.405 / 0.0646 | 0.602 / 0.0638 | 0.411 / 0.0645 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.205 / 0.0121 | 0.185 / 0.0109 | **0.15 / 0.01** | 0.206 / 0.0116 | 0.186 / 0.0115 | 0.206 / 0.0118 |
| matrix4_inverse | condition ~1e4 (error / condition number) | 1.16e+03 / 525 | 13.7 / 0.97 | **2.97 / 0.144** | 17.6 / 1.02 | 16 / 0.97 | 17.6 / 1.02 |
| matrix3_inverse | random entries (error / condition number) | 0.794 / 0.0788 | 0.794 / 0.0753 | **0.574 / 0.0635** | 0.794 / 0.0788 | 0.578 / 0.0788 |  |
| matrix4_determinant | random entries | 3.11 / 0.35 | 2.81 / 0.256 | **2.21 / 0.215** | 3.39 / 0.262 | 3.16 / 0.259 | 3.39 / 0.261 |
| matrix4_normal_matrix | rotation, scale and translation |  | 2.15 / 0.382 | **1.7 / 0.34** | 1.95 / 0.38 |  |  |
| matrix4_set_from_axisv3_angle | random | 2.21 / 0.443 | 2.69 / 0.503 | **2.21 / 0.442** | 4.19 / 0.608 | **2.33 / 0.443** | 4.19 / 0.608 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | 0.0419 / 0.0384 | 0.0419 / 0.0384 | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** | **0.0419 / 0.0384** |
| matrix4_set_from_quaternion | unit q | 1.68e+07 / 9.32e+06 | 3.53 / 0.747 | **2.89 / 0.579** | 2.25 / 0.595 | 2.25 / 0.595 | **2.89 / 0.58** |
| matrix4_set_from_euler_anglesf3 | random | 1.68e+07 / 1.42e+07 | 1.15 / 0.392 | **1.15 / 0.392** | **1.22 / 0.39** |  | **1.22 / 0.389** |
| matrix4_projection_perspective_fovy_rh | random | 1.66e+07 / 6.84e+06 | 1.21 / 0.353 | **1.21 / 0.353** | **1.19 / 0.359** |  | 1.33 / 0.38 |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | 1.68e+07 / 1.1e+07 | 1.94 / 0.416 | **1.47 / 0.36** | 2.54 / 0.458 |  | 2.54 / 0.458 |
| quaternion_multiply | random | 1.15 / 0.302 | 1.15 / 0.302 | **1.06 / 0.29** | 1.16 / 0.302 | **1.04 / 0.289** | 1.15 / 0.302 |
| quaternion_normalize | random length | 1.44 / 0.283 | 1.13 / 0.279 | **0.481 / 0.198** | 1.34 / 0.355 | 1.19 / 0.28 | 1.19 / 0.28 |
| quaternion_inverse | random length | 1.46 / 0.398 | 2.47 / 0.496 | **1.43 / 0.387** | **1.37 / 0.388** | **1.43 / 0.385** | 1.55 / 0.431 |
| quaternion_set_from_axis_anglev3 | random | 1.31 / 0.286 | 1.47 / 0.304 | **1.02 / 0.24** | **0.941 / 0.24** | **0.941 / 0.24** | 1.84 / 0.309 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | 0.0105 / 0.0105 | 0.0105 / 0.0105 | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** | **0.0105 / 0.0105** |
| quaternion_get_axis_anglev3 | random (axis * angle) | 13.6 / 0.648 | 1.35 / 0.384 | **1.23 / 0.373** | 51 / 0.715 | 1.45 / 0.396 | 6.92 / 0.704 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | 8.39e+06 / 8.39e+06 | 0.532 / 0.09 | **0.533 / 0.0899** | 2.28e+07 / 1.19e+07 | **0.533 / 0.09** | 1.01 / 0.374 |
| quaternion_set_from_matrix4 | random rotation |  | 1.81 / 0.356 | **1.78 / 0.295** | 1.31 / 0.328 | 1.73 / 0.329 | 1.85 / 0.337 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) |  | 1.48 / 0.353 | **1.53 / 0.279** | 1.38 / 0.304 | 1.38 / 0.304 | 1.62 / 0.329 |
| quaternion_slerp | random | 55.7 / 0.47 | 1.85 / 0.478 | **1.5 / 0.408** | 1.85 / 0.499 | 1.85 / 0.468 | 16.6 / 0.61 |
| quaternion_slerp | 1e-3 rad apart | 1.47 / 0.414 | 1.67 / 0.475 | **1.47 / 0.415** | 1.96 / 0.575 | 1.64 / 0.427 | 5.89e+03 / 52.3 |
| quaternion_slerp | 1e-6 rad apart | 1.25 / 0.358 | 1.47 / 0.376 | **1.18 / 0.31** | 1.5 / 0.362 | 1.25 / 0.358 | 6.79 / 1.94 |
| quaternion_get_rotation_tov3 | random (landing error) | 1.68e+07 / 1.1e+07 | 2.58 / 0.536 | **2.12 / 0.468** | 99.4 / 0.965 | 48.8 / 0.863 | 99.3 / 0.907 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | 1.68e+07 / 1.68e+07 | 2.23 / 0.348 | **2.21 / 0.349** | 4.39e+03 / 942 | 1.8e+04 / 1.66e+04 | 8.39e+03 / 8.39e+03 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 7.69 / 6.64 | 1.41 / 0.259 | **0.598 / 0.122** | 8.73 / 8.39 | 0.686 / 0.159 | 8.73 / 8.39 |
| quaternion_angle_between | random | 169 / 0.781 | 1.78 / 0.336 | **0.868 / 0.221** |  | 3.46 / 0.273 |  |
| quaternion_angle_between | 1e-4 rad apart | inf | 4.25e+03 / 207 | **0.307 / 0.153** |  | 1.19e+04 / 1.92e+03 |  |
| vector3_reflect | random (relative to the length of v) |  |  | **3.59 / 0.466** | 4.41 / 0.664 |  | 4.41 / 0.664 |
| vector3_refract | random, eta 0.5 .. 2 |  |  | **99.5 / 0.318** | 186 / 0.421 |  | 2.5e+07 / 9.32e+06 |
| quaternion_set_look_rotation_rh | random |  |  | **7.7 / 0.348** | 18.1 / 0.417 |  | 18.1 / 0.434 |
| vector3_project_to_window | random camera, a point in the view |  |  | **275 / 2.26** | **275 / 2.26** |  |  |
| vector3_unproject_from_window | random camera, window depth 0 .. 0.9 |  |  | **70.3 / 1.77** | 58.1 / 2.16 |  |  |

## Oracle check (now, default seed)

double:

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 0.000512 |
| vector rotation: quaternion product against the rotation matrix | 0.00205 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 0.0014 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 0.00125 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.376 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.351 |
| angle between quaternions 1e-4 rad apart: from a - b and a + b against from a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 0.00346 |

single:

| reference | largest disagreement |
|---|---|
| matrix4 inverse: cofactors (Eigen inverse()) against full pivoting LU, per unit of condition | 4.26e-13 |
| vector rotation: quaternion product against the rotation matrix | 2.97e-12 |
| slerp, 1e-3 rad apart: Eigen (acos) against the atan2 form | 2.99e-12 |
| slerp, 1e-6 rad apart: Eigen (acos) against the atan2 form | 2.44e-12 |
| quaternion from matrix, random: Eigen (long double) against the nearest rotation (SVD) | 0.355 |
| quaternion from matrix, near half turns: Eigen (long double) against the nearest rotation (SVD) | 0.31 |
| angle between quaternions 1e-4 rad apart: from a - b and a + b against from a conj(b), both in _Float128 | 0 |
| axis-angle matrix: Rodrigues against through a quaternion | 4.9e-12 |

