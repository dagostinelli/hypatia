# hypatia compared with GLM, cglm, Eigen and LAPACK

Experimental branch `correctness-exp-glm`; not meant for master.

Libraries: GLM 1.0.1, cglm 0.9.4 (single precision only; cglm is float), Eigen 3.4.0,
and the system LAPACK 3.12 (double).  The tables are in `results/`:

- `results/double.md`: double precision, depth 0..1
- `results/single.md`: single precision, depth 0..1, with cglm
- `results/double_depth_no.md`: double precision with `HYP_DEPTH_MINUS_ONE_TO_ONE`

Each row runs 2000 random inputs (fixed seed), and some rows add inputs that are hard
for rounding: nearly parallel, nearly opposite, badly conditioned.  Each row gives the
largest difference, relative to max(1, |reference|).  The tolerance is 1e-10 (double)
or 2e-5 (single).  Where two libraries disagree, an "accuracy" section compares each one
with the exact answer computed in long double.  An "edge cases" section lists what each
library returns for zero, tiny, huge, NaN and inf inputs and for degenerate geometry.

## Bugs found and fixed in correctness-h

Each fix is a commit at the tail of `correctness-h`, with a test that fails before it.

| commit | problem | found by |
|---|---|---|
| `matrix4_inverse, matrix4_determinant: share the 2x2 minors` | The determinant came from a separate 24-term sum and the cofactors from triple products.  For a condition number of 1e3 the inverse was 18 times less accurate than GLM and Eigen, and 150 times less for 1e6 (double).  It is now as accurate as GLM and Eigen (6.5e-13 against 7.4e-13 and 7.2e-13). | inverse accuracy against long double |
| `vector2/3_angle_between: atan2 of the unit vectors` | acos of the rounded cosine gave NaN for about half of all parallel vector pairs, NaN for every pair 1e-4 rad apart in single precision, and NaN for a zero vector.  Now within 4.4e-16 (double) and 2.4e-7 (single) of the exact angle; GLM's `angle` is 10 to 20000 times further off. | GLM, long double |
| `quaternion_slerp: measure the angle with atan2; lerp only for equal quaternions` | Below 1 - HYP_EPSILON in the dot product (angles under about 0.009 rad) slerp used lerp, which was off by up to 2e-6 and not unit length.  GLM and Eigen differed by 3e-8 at 1e-3 rad. | GLM, Eigen |
| `Rotate by a quaternion or an axis of any length` | `vector3_rotate_by_quaternion` scaled the vector by \|q\|^2 for a non-unit q, while `matrix4_set_from_quaternion` (documented to rotate the same way) normalizes.  `quaternion_set_from_axis_anglef3` normalized the result, which changes the angle for a non-unit axis; `matrix4_set_from_axisf3_angle` gave a matrix that is not a rotation.  GLM's `rotate` normalizes the axis. | GLM |
| `matrix2/3/4_multiply: the doc comment had the order backwards` | The comment said self = self * mT; the code (and the README) do self = mT * self. | GLM, cglm |

## Agreement

After those fixes every comparison in double precision agrees to 1e-10 or better, except
the rows marked "for reference" below.  That covers vectors (normalize, length, distance,
dot, cross, lerp, clamp, min, max, project, rotation by a quaternion), matrices
(multiply, transpose, determinant, inverse, multiply by a vector), the builders
(translation, scaling, rotations, axis-angle, quaternion, Euler, compose, decompose,
normal matrix), the projections and views (perspective, ortho, lookat, right- and
left-handed, both depth ranges) and the quaternion functions (multiply, conjugate,
inverse, normalize, axis-angle both ways, Euler both ways, from a matrix, lerp, nlerp,
slerp, rotation between vectors, angle between).

## Convention differences (not bugs)

| hypatia | the others |
|---|---|
| `matrix4_set_from_euler_anglesf3(x, y, z)` rotates about X first, then Y, then Z: Rz Ry Rx | GLM `eulerAngleZYX(z, y, x)` and cglm `glm_euler_zyx` are the same; GLM `eulerAngleXYZ` and cglm `glm_euler_xyz` are Rx Ry Rz |
| `matrix4_translatev3(M, v)`, `_rotatev3`, `_scalev3` apply the new transform after M: T * M | GLM `translate(M, v)` applies it before M: M * T |
| `quaternion_get_axis_anglev3` gives an angle in [0, pi] (q and -q are the same rotation) | GLM `angle` gives [0, 2 pi]; Eigen `AngleAxis` [0, pi] |
| `quaternion_set_from_euler_anglesf3(x, y, z)` | matches GLM `quat(vec3(x, y, z))`; `quaternion_get_euler_anglesf3` matches GLM `eulerAngles` |
| `quaternion_slerp` takes the shortest arc | GLM `slerp` and Eigen `slerp` do too; cglm `glm_quat_slerp` and GLM `mix` do not |
| `quaternion_angle_between` is the angle of the rotation from one to the other | the same as Eigen `angularDistance` |

## Edge cases

| input | hypatia | GLM | Eigen | cglm |
|---|---|---|---|---|
| normalize zero vector | unchanged | NaN | unchanged | unchanged |
| normalize (1e-200, 1e-200, 0) | unit | inf and NaN | unchanged | (single) zero |
| normalize (1e200, 1e200, 0) | unit | zero | zero | (single) zero |
| normalize with an inf component | unit along the inf | NaN | NaN | NaN |
| normalize zero quaternion | unchanged | identity | unchanged | |
| inverse of zero quaternion | unchanged | NaN | unchanged | |
| project onto zero | zero | NaN | | |
| angle between a vector and zero | 0 | NaN | | |
| rotation between opposite vectors | a half turn, correct | correct for (1, 2, 3) and its negative in double; for a and -2a, and in single, it does not turn (lands on the start) | correct to 3e-8 (double), 7e-4 (single) | correct |
| rotation from a zero vector | identity | NaN | a quarter turn | |
| singular matrix inverse | NULL | NaN | | |
| lookat with eye == target | zero rows | NaN | | |
| rotate X by 2 * (quarter turn about Z) | (0, 1, 0) | (-3, 4, 0): neither a rotation nor a scaling | (-3, 4, 0) | (0, 1, 0): normalizes q |
| rotate X by the zero quaternion | X | X | X | X |
| matrix, quarter turn about (0, 0, 10) | X to (0, 1, 0) | X to (0, 1, 0): `rotate` normalizes the axis | X to (0, 10, 0): not a rotation | X to (0, 1, 0): `glm_rotate_make` normalizes |
| quaternion, quarter turn about (0, 0, 10) | unit, X to (0, 1, 0) | `angleAxis`: length 7.1, X to (-99, 10, 0) | `AngleAxis`: length 7.1, X to (-99, 10, 0) | `glm_quatv`: unit, X to (0, 1, 0) |
| matrix, quarter turn about the zero axis | identity | NaN | X to (0, 0, 0) (cos(angle) on the diagonal) | X to (0, 0, 0) |
| quaternion, quarter turn about the zero axis | identity | (0, 0, 0, 0.707): length 0.707, leaves X unchanged | the same as GLM | the same as GLM |

The opposite-vector case is a GLM problem, not a hypatia one: `glm::rotation` misses by
the full distance (2) for a and -2a in both precisions (the accuracy table).  Near
opposite (1e-3 rad), GLM and Eigen are off by 4e-13 in double where hypatia is at 5e-16,
and in single GLM, cglm and Eigen are off by 2e-4, 4e-3 and 8e-3 where hypatia stays at
3e-7.

## Single precision

In single precision, besides the convention rows:

- `vector2_angle_between` differs from GLM by 2e-4 for three vector pairs.  The accuracy
  table shows GLM is the one that is off (acos near 0 and pi loses half the digits).
- `matrix4_determinant` differs from LAPACK (double) by 3e-5 for two matrices.  That is
  float rounding of the products; GLM and Eigen in float are the same (2.2e-7 and 2.7e-7
  relative to the largest product, hypatia 2.6e-7).
- `matrix4_reciprocal_condition` is off by up to a factor of 17 for condition numbers
  near 1e6, and gives 0 for some matrices with condition numbers from 4e5 whose float
  determinant rounds to exactly zero (listed in the edge cases).  It is computed from the inverse, and in float a 4x4 inverse with condition 1e6
  has lost about all its digits: GLM, Eigen and cglm return inf there.  Not a bug, but
  a limit of computing it from the float inverse; LAPACK's estimate (pivoted LU) does
  not have it.

## Precision

`docs/comparison.md` reports the precision of hypatia (master, before and after the
precision work) against GLM, Eigen and cglm; the tables are also at the end of
`results/double.md` and `results/single.md`.

## Not compared

`vector4_cross_product`, `quaternion_axis_between_EXP` and `quaternion_cross_product_EXP`
have no counterpart.  `vector3_reflect_by_quaternion` was compared with its documented
formula q v q only.

## Running it

```sh
cd compare
./fetch.sh                        # GLM, cglm and Eigen at the pinned versions
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/compare_double > results/double.md
./build/compare_single > results/single.md
./build/compare_double_depth_no > results/double_depth_no.md
```

It needs a C++17 compiler and LAPACK (liblapack.so; no headers).  `-DHYPATIA_DIR=` points
it at another copy of hypatia.h, which is how the fixes were measured before they went in.
