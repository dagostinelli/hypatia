# How hypatia compares with GLM, Eigen and cglm

This report compares hypatia with GLM 1.0.1, Eigen 3.4.0 and cglm 0.9.4 (single precision
only; cglm has no double), and with LAPACK 3.12 for the inverse, determinant and condition
number.  It covers three versions of hypatia:

- **master**: 2.1.0-dev, before the correctness work;
- **before**: correctness-h at 35049bd, after the bug fixes, before the precision work;
- **now**: correctness-h at f30adff.

The measurements come from the harness in `compare/` on this branch (see "Reproducing").

## Summary

- **Correctness.**  Every function now agrees with GLM, Eigen and cglm in double precision
  to 1e-10 or better, except where the libraries use different conventions (listed
  below).  On master, 11 of the measured functions gave wrong results: in 17 of the 38
  double precision measurements master can take part in (13 of 38 in single).
- **Precision.**  Ranked by the mean error over 20000 inputs, hypatia now is the most
  precise of the four libraries, or tied with the best, in 34 of 41 measurements in both
  double and single precision (33 of 41 with another random seed).  It is clearly ahead of
  all of them (by more than 2% in the mean) in 10 in double and 11 in single.  Master
  was best or tied in 12 to 14, and worse than the best library in 24 or 25.
- **Where it trails**, the gap is 0.01 to 0.07 ulp in the mean in six cases; the seventh
  is `quaternion_get_rotation_tov3` for vectors 1e-6 rad apart (0.28 against Eigen's
  0.16 ulp).

| | double: best or tied | double: worse than the best | single: best or tied | single: worse than the best |
|---|---|---|---|---|
| master | 12 | 25 | 13 | 24 |
| before | 21 | 20 | 22 | 19 |
| now | 34 | 7 | 34 | 7 |

(Mean error, within 2%.  Measurements where a version has no such function are left out
of its count: master lacks `vector3_project`, `matrix4_normal_matrix` and
`quaternion_set_from_matrix4`.)

## Where hypatia is clearly ahead

Largest error over 20000 inputs, in ulps (units of the epsilon); the best of GLM, Eigen
and cglm for comparison.

| function | inputs | hypatia (largest / mean) | best other library | ratio |
|---|---|---|---|---|
| `quaternion_get_rotation_tov3` | 1e-3 rad from opposite | 2.21 / 0.35 | 4080 / 640 (Eigen) | 1850x |
| `quaternion_get_rotation_tov3` | random | 2.21 / 0.54 | 116 / 0.96 (GLM) | 52x |
| `vector3_rotate_by_quaternion` | q drifted to length 1 +- 1e-6 | 3.36 / 0.66 | 2.3e10 (GLM, Eigen: they assume a unit q) | |
| `vector3_rotate_by_quaternion` | unit q | 3.03 / 0.64 | 5.78 / 0.93 (GLM) | 1.9x |
| `quaternion_angle_between` | 1e-4 rad apart | 6190 / 220 | 9700 / 1800 (Eigen) | 1.6x |
| `matrix4_set_from_quaternion` | unit q | 2.83 / 0.62 | 3.92 / 0.90 (GLM) | 1.4x |
| `quaternion_angle_between` (single) | random | 1.65 / 0.33 | 7.49 / 0.28 (Eigen) | 4.5x |

Against GLM alone the differences are larger in places: `glm::angle` of a quaternion
loses all digits for small rotations (1.2e7 ulps for 1e-4 rad, hypatia 0.99), and
`glm::angle` of two vectors loses up to 1e-11 near 0 and pi in double (hypatia 4e-16).
`glm::rotation` gives no rotation at all for exactly opposite vectors.

## Where hypatia trails

Double precision; single is similar.

| function | inputs | hypatia mean | best other mean |
|---|---|---|---|
| `quaternion_get_rotation_tov3` | 1e-6 rad apart (landing error) | 0.28 | 0.16 (Eigen) |
| `quaternion_set_from_matrix4` | near half turns | 0.33 | 0.30 (GLM, Eigen) |
| `quaternion_slerp` | random, 1e-3 rad apart | 0.52, 0.57 | 0.50, 0.52 (Eigen) |
| `quaternion_set_from_axis_anglev3` | random | 0.31 | 0.29 (GLM, Eigen) |
| `quaternion_angle_between` | random (double) | 0.33 | 0.26 (Eigen) |
| `vector3_normalize` | components from 1e-20 to 1e20 | 0.13 | 0.12 (Eigen) |

In each of these the largest error is within 1 ulp of the best library's, except
`quaternion_get_rotation_tov3` for vectors 1e-6 rad apart (1.5 against 0.67).

## What changed since master

### Results that were wrong on master

These show as "wrong" in the tables (an error of 1e6 ulps or more).  The README's
"Changes in 2.1" lists each fix.

| function on master | what was wrong |
|---|---|
| `vector3_rotate_by_quaternion` | wrong for vectors not perpendicular to the axis (a sign error) |
| `matrix4_make_transformation_rotationq` | rotated the other way (left-handed) |
| `matrix4_set_from_euler_anglesf3_EXP` | another order and direction |
| `matrix4_projection_perspective_fovy_rh_EXP`, `matrix4_view_lookat_rh_EXP` | incorrect matrices |
| `matrix4_inverse` | refused matrices with a determinant below 1e-5 (the condition ~1e4 matrices) |
| `vector3_normalize` | left vectors shorter than 1e-5 unchanged |
| `quaternion_get_axis_anglev3` | lost all digits for small rotations; for w < 0 it gave an angle above pi (the same rotation, counted as wrong here, which compares in [0, pi]) |
| `quaternion_get_rotation_tov3` | wrong rotation for many vector pairs |
| `quaternion_slerp` | shortcuts near 0 and 1 and a sign jump between q and -q |
| `quaternion_angle_between_EXP` | did not treat q and -q as the same rotation |

### Bugs the comparison found

The comparison with the other libraries found these on correctness-h, each fixed with a
test that failed before the fix:

- `vector2/3_angle_between` returned NaN for about half of all parallel vector pairs and
  for every pair 1e-4 rad apart in single precision;
- `quaternion_slerp` used a linear interpolation for angles under about 0.009 rad, off by
  up to 2e-6 and not of unit length;
- `vector3_rotate_by_quaternion` scaled the vector by |q|^2 for a quaternion that is not of
  unit length, and the axis-angle functions changed the angle for an axis that is not of
  unit length;
- `matrix2/3/4_inverse` overflowed (inf) for very small invertible matrices;
- `matrix4_inverse` computed the determinant separately from the cofactors (a 24-term
  sum): for matrices with condition number 1e6 it was up to 150 times less precise than
  GLM and Eigen, on random matrices about 2 times (per unit of condition number);
- the doc comments of `matrixN_multiply` had the order backwards.

### The precision work (before to now)

- Normalizing divides once by the length when the sum of the squares is in range, as GLM
  and Eigen do; the scaled path, which divided twice, is kept for tiny, huge, infinite and
  NaN components.
- `quaternion_inverse`, `vectorN_project` and `matrix4_set_from_quaternion` divide by |q|^2
  or |v|^2 directly when it is in range.
- `quaternion_norm` and `quaternion_multiply` sum in pairs.
- `matrix4_inverse` and `matrix4_determinant` use 2x2 blocks, as Eigen does, and share the
  determinant.  Per unit of condition number this is about level with the previous
  version (mean 0.062 against 0.063 ulp on random matrices); against master (0.082) and in
  the worst case (0.41 against 0.90 on master) it is better.  The commit message of
  58eb5fb quotes a larger gain (mean 9 to 3.5 ulp); that figure was not divided by the
  condition number and came from a few nearly singular matrices.
- `vector3_rotate_by_quaternion` uses 2 (u.v) u + (w^2 - u.u) v + 2 w (u x v), over |q|^2.
  It is also faster: 17 ns per call, 21 ns before any of these changes.

## Conventions that differ (not errors)

| hypatia | the others |
|---|---|
| `matrix4_set_from_euler_anglesf3(x, y, z)` rotates about X, then Y, then Z (Rz Ry Rx) | GLM `eulerAngleZYX(z, y, x)` and cglm `glm_euler_zyx` are the same; GLM `eulerAngleXYZ` and cglm `glm_euler_xyz` are Rx Ry Rz |
| `matrix4_translatev3(M, v)` (and rotate, scale) applies the new transform after M | GLM `translate(M, v)` applies it before M |
| `quaternion_get_axis_anglev3` gives an angle in [0, pi] | GLM `angle` gives [0, 2 pi] |
| `quaternion_slerp` takes the shorter arc | GLM `slerp` and Eigen do too; cglm and GLM `mix` do not |

## Edge cases

Zero, tiny, huge, infinite and NaN input, and degenerate geometry (opposite vectors,
eye == target, a zero axis): hypatia gives a defined result in each case (the input
unchanged, the identity, or 0) where GLM usually gives NaN and Eigen and cglm vary.  The
full table is in `compare/REPORT.md` and at the end of `compare/results/*.md`.

## Method and limits

- Each measurement uses 20000 inputs from a fixed generator: random values, and inputs
  that are hard for rounding (nearly parallel, nearly opposite, badly conditioned, very
  small or large).  The same rounded inputs go to every library.
- The exact answer is computed in long double (64-bit mantissa) from those inputs.  The
  error is in units of the epsilon of the precision measured, relative to the largest
  component of the exact result (to the size of the terms for dot and cross products,
  products and determinants).
- The inverse is measured per unit of the condition number (|A| |A^-1| in the row-sum
  norm), and lookat per unit of 1 / sin of the angle between the view and up directions:
  any algorithm can lose that much, and without it a few nearly singular inputs dominate
  the numbers.
- The largest error moves by 0.1 to 0.3 ulp between random seeds; the mean is stable.  The
  counts above use the mean; with a second seed (`COMPARE_SEED=7`) they are 33 of 41 in both
  precisions.
- In single precision, `matrix4_reciprocal_condition` is limited by the float inverse: for
  condition numbers near 1e6 it can be 17 times off, or 0 when the float determinant
  rounds to zero.
- `vector4_cross_product` and the remaining experimental functions have no counterpart and
  are not compared.

## Reproducing

```sh
cd compare
./fetch.sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/compare_double > results/double.md      # agreement, edge cases, precision
./build/compare_single > results/single.md
```

For the master and before columns: `-DHYPATIA_DIR=<directory with that hypatia.h>` and
`-DCOMPARE_DEFS="PRECISION_ONLY"` (with `;HYP_MASTER` for master, which maps the master
names).  `COMPARE_SEED=n` picks another random sequence.

## Appendix: the precision tables

Largest / mean error in ulps over 20000 inputs (default seed).  Bold: the best of now,
GLM, Eigen and cglm.  "wrong": master's result is wrong (1e6 ulps or more); "n/a": the
version or library has no such function.
### Double precision

| function | inputs | master | before | now | GLM | Eigen |
|---|---|---|---|---|---|---|
| vector3_normalize | random | 1.09 / 0.32 | 1.12 / 0.35 | **1.09 / 0.32** | 1.34 / 0.37 | **1.09 / 0.32** |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | wrong | 1.09 / 0.15 | **1.14 / 0.13** | 1.38 / 0.19 | **1.14 / 0.12** |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | 1.26 / 0.25 | 0.992 / 0.33 | **1.26 / 0.25** | 1.34 / 0.31 | **1.26 / 0.25** |
| vector3_magnitude | random | 0.916 / 0.22 | 0.916 / 0.22 | **0.916 / 0.22** | **0.916 / 0.22** | **0.916 / 0.22** |
| vector3_dot_product | random | 1.09 / 0.16 | 1.09 / 0.16 | **1.09 / 0.16** | **1.09 / 0.16** | **1.09 / 0.16** |
| vector3_dot_product | nearly perpendicular | 0.61 / 0.09 | 0.61 / 0.09 | **0.61 / 0.09** | **0.61 / 0.09** | **0.61 / 0.09** |
| vector3_cross_product | random | 0.802 / 0.2 | 0.802 / 0.2 | **0.802 / 0.2** | **0.802 / 0.2** | **0.802 / 0.2** |
| vector3_cross_product | nearly parallel (1e-4 rad) | 0.462 / 0.11 | 0.462 / 0.11 | **0.462 / 0.11** | **0.462 / 0.11** | **0.462 / 0.11** |
| vector3_project | random | n/a | 2.4 / 0.29 | **1.59 / 0.21** | **1.59 / 0.21** | n/a |
| vector3_rotate_by_quaternion | unit q | wrong | 3 / 0.74 | **3.03 / 0.64** | 5.78 / 0.93 | 5.78 / 0.92 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | wrong | 3.42 / 0.76 | **3.36 / 0.66** | 2.29e+10 / 6e+09 | 2.29e+10 / 6e+09 |
| matrix4_multiply | random | 1.29 / 0.37 | 1.29 / 0.37 | **1.29 / 0.37** | **1.29 / 0.37** | **1.29 / 0.37** |
| matrix4_inverse | random entries (error / condition number) | 0.9 / 0.082 | 0.433 / 0.063 | **0.414 / 0.062** | 0.523 / 0.064 | 0.481 / 0.064 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.246 / 0.012 | 0.201 / 0.011 | **0.168 / 0.011** | 0.218 / 0.012 | 0.173 / 0.011 |
| matrix4_inverse | condition ~1e4 (error / condition number) | wrong | 13.5 / 0.96 | 14 / 0.96 | **13.7 / 1** | 15.6 / 0.96 |
| matrix3_inverse | random entries (error / condition number) | 0.555 / 0.078 | 0.555 / 0.075 | **0.555 / 0.075** | **0.555 / 0.078** | 0.642 / 0.079 |
| matrix4_determinant | random entries | 3.49 / 0.35 | 3.4 / 0.26 | 3.4 / 0.26 | **2.7 / 0.26** | 2.99 / 0.26 |
| matrix4_normal_matrix | rotation, scale and translation | n/a | 2.06 / 0.38 | **1.74 / 0.38** | 2.04 / 0.38 | n/a |
| matrix4_set_from_axisv3_angle | random | 3.34 / 0.56 | 3.97 / 0.59 | **3.13 / 0.56** | 4.9 / 0.71 | 4.01 / 0.56 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | 0.368 / 0.23 | 0.368 / 0.23 | **0.368 / 0.23** | **0.368 / 0.23** | **0.368 / 0.23** |
| matrix4_set_from_quaternion | unit q | wrong | 4.31 / 0.94 | **2.83 / 0.62** | 3.92 / 0.9 | 3.92 / 0.9 |
| matrix4_set_from_euler_anglesf3 | random | wrong | 1.26 / 0.39 | 1.26 / 0.39 | **1.22 / 0.39** | n/a |
| matrix4_projection_perspective_fovy_rh | random | wrong | 1.24 / 0.35 | 1.24 / 0.35 | **1.11 / 0.36** | n/a |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | wrong | 2.27 / 0.43 | **1.91 / 0.41** | 2.11 / 0.45 | n/a |
| quaternion_multiply | random | 1.1 / 0.3 | 1.2 / 0.3 | **0.942 / 0.29** | 1.08 / 0.3 | 1.03 / 0.29 |
| quaternion_normalize | random length | 1.25 / 0.34 | 1.17 / 0.35 | **1.18 / 0.33** | 1.47 / 0.39 | **1.18 / 0.33** |
| quaternion_inverse | random length | 1.54 / 0.4 | 2.3 / 0.63 | 1.41 / 0.39 | 1.4 / 0.39 | **1.33 / 0.39** |
| quaternion_set_from_axis_anglev3 | random | 1.39 / 0.32 | 1.65 / 0.39 | **1.32 / 0.31** | 1.51 / 0.29 | 1.51 / 0.29 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | 0.033 / 0.033 | 0.533 / 0.53 | **0.033 / 0.033** | **0.033 / 0.033** | **0.033 / 0.033** |
| quaternion_get_axis_anglev3 | random (axis * angle) | wrong | 1.49 / 0.42 | **1.43 / 0.39** | 49 / 0.7 | **1.43 / 0.39** |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | wrong | 1.34 / 0.4 | **0.99 / 0.35** | 1.18e+07 / 1.2e+07 | **0.99 / 0.35** |
| quaternion_set_from_matrix4 | random rotation | n/a | 1.72 / 0.43 | **1.98 / 0.4** | 3.39 / 0.44 | 2.42 / 0.4 |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) | n/a | 1.47 / 0.39 | 1.62 / 0.33 | **1.53 / 0.3** | **1.53 / 0.3** |
| quaternion_slerp | random | wrong | 2.03 / 0.52 | 2 / 0.52 | 2.01 / 0.53 | **1.93 / 0.5** |
| quaternion_slerp | 1e-3 rad apart | wrong | 2.15 / 0.57 | 2.21 / 0.57 | **2.2 / 0.55** | 2.4 / 0.52 |
| quaternion_slerp | 1e-6 rad apart | 1.22e+04 / 94 | 2.26 / 0.55 | 2.52 / 0.55 | **2.26 / 0.58** | 2.36 / 0.54 |
| quaternion_get_rotation_tov3 | random (landing error) | wrong | 2.75 / 0.61 | **2.21 / 0.54** | 116 / 0.96 | 116 / 0.86 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | wrong | 2.78 / 0.43 | **2.21 / 0.35** | 4.09e+03 / 8.4e+02 | 4.08e+03 / 6.4e+02 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | wrong | 1.58 / 0.32 | 1.54 / 0.28 | 0.666 / 0.16 | **0.636 / 0.16** |
| quaternion_angle_between | random | wrong | 7.38 / 0.35 | **2.87 / 0.33** | n/a | 2.99 / 0.26 |
| quaternion_angle_between | 1e-4 rad apart | wrong | 1.49e+04 / 1.9e+03 | **6.19e+03 / 2.2e+02** | n/a | 9.7e+03 / 1.8e+03 |

Best or tied with the best of GLM, Eigen: master 13 of 41 rows, before 22, now 31.

### Single precision

| function | inputs | master | before | now | GLM | Eigen | cglm |
|---|---|---|---|---|---|---|---|
| vector3_normalize | random | 1.22 / 0.32 | 1.11 / 0.35 | 1.22 / 0.32 | 1.29 / 0.37 | **1.11 / 0.32** | 1.29 / 0.37 |
| vector3_normalize | components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | wrong | 0.944 / 0.1 | **1.05 / 0.085** | 1.19 / 0.15 | **1.05 / 0.085** | 8.39e+06 / 1.7e+05 |
| vector3_normalize | one large, two small (1, 1e-5, 1e-5) | 0.00293 / 0.00028 | 0.00293 / 0.00028 | **0.00293 / 0.00028** | 0.5 / 0.075 | **0.00293 / 0.00028** | 0.5 / 0.075 |
| vector3_magnitude | random | 0.995 / 0.22 | 0.995 / 0.22 | 0.995 / 0.22 | 0.995 / 0.22 | **0.965 / 0.22** | 0.995 / 0.22 |
| vector3_dot_product | random | 0.951 / 0.16 | 0.951 / 0.16 | 0.951 / 0.16 | 0.951 / 0.16 | **0.948 / 0.16** | 0.951 / 0.16 |
| vector3_dot_product | nearly perpendicular | 0.557 / 0.091 | 0.557 / 0.091 | **0.557 / 0.091** | **0.557 / 0.091** | 0.56 / 0.091 | **0.557 / 0.091** |
| vector3_cross_product | random | 0.791 / 0.2 | 0.791 / 0.2 | **0.791 / 0.2** | **0.791 / 0.2** | **0.791 / 0.2** | **0.791 / 0.2** |
| vector3_cross_product | nearly parallel (1e-4 rad) | 0.45 / 0.11 | 0.45 / 0.11 | **0.45 / 0.11** | **0.45 / 0.11** | **0.45 / 0.11** | **0.45 / 0.11** |
| vector3_project | random | n/a | 2 / 0.3 | **1.64 / 0.21** | **1.64 / 0.21** | n/a | n/a |
| vector3_rotate_by_quaternion | unit q | wrong | 3 / 0.74 | **3.2 / 0.64** | 5.33 / 0.94 | 5.33 / 0.93 | 4.94 / 0.81 |
| vector3_rotate_by_quaternion | q of length 1 +- 1e-6 (drifted) | wrong | 2.86 / 0.76 | **2.97 / 0.66** | 42.2 / 11 | 42.2 / 11 | 4.02 / 0.81 |
| matrix4_multiply | random | 1.15 / 0.37 | 1.15 / 0.37 | **1.15 / 0.37** | **1.15 / 0.37** | **1.15 / 0.37** | **1.15 / 0.37** |
| matrix4_inverse | random entries (error / condition number) | 1.18 / 0.082 | 0.4 / 0.063 | 0.582 / 0.062 | **0.405 / 0.065** | 0.602 / 0.064 | 0.411 / 0.064 |
| matrix4_inverse | rotation, scale and translation (error / condition number) | 0.245 / 0.012 | 0.311 / 0.011 | 0.218 / 0.011 | 0.247 / 0.012 | **0.174 / 0.011** | 0.247 / 0.012 |
| matrix4_inverse | condition ~1e4 (error / condition number) | 1.16e+03 / 5.2e+02 | 13.6 / 0.96 | **13.7 / 0.97** | 17.6 / 1 | 16 / 0.97 | 17.6 / 1 |
| matrix3_inverse | random entries (error / condition number) | 0.794 / 0.079 | 0.794 / 0.075 | 0.794 / 0.075 | 0.794 / 0.079 | **0.578 / 0.079** | n/a |
| matrix4_determinant | random entries | 3.11 / 0.35 | 3.37 / 0.27 | **2.81 / 0.26** | 3.39 / 0.26 | 3.16 / 0.26 | 3.39 / 0.26 |
| matrix4_normal_matrix | rotation, scale and translation | n/a | 1.91 / 0.38 | 2.1 / 0.38 | **1.96 / 0.38** | n/a | n/a |
| matrix4_set_from_axisv3_angle | random | 3.65 / 0.57 | 3.38 / 0.6 | **3.22 / 0.57** | 4.15 / 0.71 | 3.68 / 0.57 | 4.15 / 0.71 |
| matrix4_set_from_axisv3_angle | angle 1e-4 | 0.0419 / 0.038 | 0.0419 / 0.038 | **0.0419 / 0.038** | **0.0419 / 0.038** | **0.0419 / 0.038** | **0.0419 / 0.038** |
| matrix4_set_from_quaternion | unit q | wrong | 4.62 / 0.94 | **2.78 / 0.62** | 4.51 / 0.91 | 4.51 / 0.91 | 3.16 / 0.72 |
| matrix4_set_from_euler_anglesf3 | random | wrong | 1.15 / 0.39 | **1.15 / 0.39** | 1.22 / 0.39 | n/a | 1.22 / 0.39 |
| matrix4_projection_perspective_fovy_rh | random | wrong | 1.21 / 0.35 | 1.21 / 0.35 | **1.19 / 0.36** | n/a | 1.33 / 0.38 |
| matrix4_view_lookat_rh | random (error * sin(view, up)) | wrong | 2.22 / 0.44 | **1.8 / 0.42** | 2.09 / 0.46 | n/a | 2.09 / 0.46 |
| quaternion_multiply | random | 1.15 / 0.3 | 1.22 / 0.3 | **1.02 / 0.29** | 1.15 / 0.3 | 1.23 / 0.29 | 1.15 / 0.3 |
| quaternion_normalize | random length | 1.25 / 0.34 | 1.08 / 0.35 | **1.14 / 0.33** | 1.34 / 0.39 | 1.19 / 0.33 | 1.19 / 0.33 |
| quaternion_inverse | random length | 1.55 / 0.4 | 2.4 / 0.62 | 1.43 / 0.39 | **1.35 / 0.39** | 1.43 / 0.39 | 1.56 / 0.43 |
| quaternion_set_from_axis_anglev3 | random | 1.45 / 0.32 | 1.54 / 0.39 | **1.34 / 0.32** | 1.5 / 0.29 | 1.5 / 0.29 | 1.82 / 0.35 |
| quaternion_set_from_axis_anglev3 | angle 1e-4 | 0.0105 / 0.01 | 0.0105 / 0.01 | **0.0105 / 0.01** | **0.0105 / 0.01** | **0.0105 / 0.01** | **0.0105 / 0.01** |
| quaternion_get_axis_anglev3 | random (axis * angle) | wrong | 1.5 / 0.42 | **1.37 / 0.38** | 36.5 / 0.75 | **1.37 / 0.4** | 8.76 / 0.71 |
| quaternion_get_axis_anglev3 | angle 1e-4 (axis * angle) | wrong | 1.02 / 0.28 | **0.533 / 0.093** | 2.28e+07 / 1.2e+07 | **0.533 / 0.092** | 1.01 / 0.38 |
| quaternion_set_from_matrix4 | random rotation | n/a | 1.68 / 0.43 | 2.19 / 0.4 | 3.5 / 0.44 | **2.12 / 0.4** | **2.12 / 0.42** |
| quaternion_set_from_matrix4 | near half turns (pi - 1e-3) | n/a | 1.53 / 0.39 | 1.51 / 0.34 | **1.43 / 0.31** | **1.43 / 0.31** | 1.58 / 0.35 |
| quaternion_slerp | random | 55.7 / 0.51 | 2.2 / 0.53 | 2.15 / 0.53 | 2.49 / 0.54 | **2.1 / 0.51** | 16.5 / 0.64 |
| quaternion_slerp | 1e-3 rad apart | 1.86 / 0.49 | 2.02 / 0.53 | 1.98 / 0.53 | 2.05 / 0.55 | **1.93 / 0.46** | 6.82e+03 / 3.2e+02 |
| quaternion_slerp | 1e-6 rad apart | 1.75 / 0.45 | 1.85 / 0.46 | **1.84 / 0.46** | 1.97 / 0.43 | 1.93 / 0.44 | 6.8 / 1.7 |
| quaternion_get_rotation_tov3 | random (landing error) | wrong | 2.68 / 0.62 | **2.58 / 0.54** | 99.4 / 0.97 | 48.8 / 0.86 | 83.8 / 0.79 |
| quaternion_get_rotation_tov3 | 1e-3 rad from opposite (landing error) | wrong | 2.73 / 0.43 | **2.23 / 0.35** | 4.39e+03 / 9.4e+02 | 1.8e+04 / 1.7e+04 | 8.39e+03 / 8.4e+03 |
| quaternion_get_rotation_tov3 | 1e-6 rad apart (landing error) | 7.69 / 6.6 | 1.63 / 0.3 | 1.41 / 0.26 | 8.73 / 8.4 | **0.686 / 0.16** | 8.73 / 8.4 |
| quaternion_angle_between | random | wrong | 4.16 / 0.35 | **1.65 / 0.33** | n/a | 7.49 / 0.28 | n/a |
| quaternion_angle_between | 1e-4 rad apart | wrong | 1.54e+04 / 1.9e+03 | **6.3e+03 / 2.2e+02** | n/a | 1.17e+04 / 1.9e+03 | n/a |

Best or tied with the best of GLM, Eigen, cglm: master 13 of 41 rows, before 22, now 27.
