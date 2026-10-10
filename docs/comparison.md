# hypatia compared with GLM, Eigen and cglm

This report measures the precision and correctness of three versions of hypatia against
GLM, Eigen and cglm, and explains how to check every number in it.  The generated
results it quotes are committed in `compare/results/`; `compare/reproduce.sh`
regenerates them.

| version | commit | what it is |
|---|---|---|
| master | 125ab87 | 2.1.0-dev, before the correctness work |
| before | 35049bd | correctness-h after the bug fixes, before the precision work |
| now | 25260ab | correctness-h when measured; the later 5c52596 changes only `scalar_equals_epsilonf`, which the measurements do not use |

| library | version | commit |
|---|---|---|
| GLM | 1.0.1 | 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 |
| Eigen | 3.4.0 | 3147391d946bb4b6c68edd901f2add6ac1f31f8c |
| cglm | 0.9.4 (single precision only; cglm has no double) | 1796cc5ce298235b615dc7a4750b8c3ba56a05dd |
| LAPACK | 3.12.0, Ubuntu liblapack3 3.12.0-3build1.1 (in the full comparison only) | |

## Summary

From `compare/results/precision/summary.md`, default seed:

| | double: best or tied | ahead | behind | 1e6 ulps or more | single: best or tied | ahead | behind | 1e6 ulps or more |
|---|---|---|---|---|---|---|---|---|
| master | 13 of 37 | 0 | 24 | 15 | 16 of 37 | 1 | 21 | 11 |
| before | 19 of 41 | 7 | 22 | 0 | 19 of 41 | 5 | 22 | 0 |
| now | 36 of 46 | 14 | 10 | 0 | 36 of 46 | 13 | 10 | 0 |

- **Best or tied:** hypatia's mean error is no more than 2% above the smallest mean of
  GLM, Eigen and cglm on the same inputs.  **Ahead** and **behind** mean more than 2%
  below or above it.  With a second seed (`COMPARE_SEED=7`) the counts for now are 37,
  14 and 9 in double, and 36, 14 and 10 in single.  Master and before have 37 and 41
  measurements: the others are of functions they do not have.
- **The large margins are on near-degenerate inputs.**
  - Rotations between nearly opposite vectors: mean error 1830 times lower than Eigen's.
  - Quaternions that have drifted from unit length: GLM and Eigen assume unit length.
  - The angle between nearly equal rotations: 8.6 times lower.

  On random inputs the differences between the libraries are 2% to 60% in the mean,
  i.e. fractions of an ulp.
- **Behind:** the largest gap is `quaternion_get_rotation_tov3` for vectors 1e-6 rad
  apart, a mean of 0.275 ulp against Eigen's 0.159.  The other nine are 2% to 28%
  behind, each less than 0.1 ulp in the mean.
- **Master:**
  - fails all 13 known-answer checks, which before and now pass: 11 wrong results and
    2 losses of precision (`compare/results/probe/`);
  - an error of 1e6 ulps or more in 15 of 37 double measurements.
- **Bug fixes:** each of 12 ships with a test that fails without the fix and passes with
  it (`compare/results/verify_fixes.txt`).
- **Added functions:** reflect, refract, the off-center frustum, the infinite
  perspective, project and unproject to the window, and the look rotation.
  - They agree with GLM to 1e-13 in double.
  - Against long double, reflect, refract and unproject are ahead of GLM, project is tied,
    and the look rotation is 5% behind in the mean.
  - Measuring them found two defects in the other libraries: cglm 0.9.4's refract has a
    sign error (fixed in cglm 0.9.5), and GLM's `quatLookAt` fails when up is nearly
    parallel to the view direction (below).
- **Reports:** every case where another library is wrong or less precise is written up as
  an issue report with a program that shows it, in `docs/reports/` (43 reports; 42 still
  present in the latest releases).
- **Regressions:** now is less precise than master in six measurements, by 5% to 15% in
  the mean.  They are listed below with the reason.

## How to check these results

Three levels, from seconds to minutes.  All of them need git and a C compiler; the third
also needs cmake, a C++17 compiler, python3 and LAPACK.

1. **Known answers, no other library** (`compare/probe.c`).  There are 13 checks with
   exact answers.  Each uses only the version's own functions; for example, a matrix is
   applied with that version's own `matrix4_multiplyv3`.  So the checks don't depend on
   a layout convention.

   ```sh
   cd compare
   mkdir -p /tmp/master && git show 125ab87:hypatia.h > /tmp/master/hypatia.h
   cc -std=c90 -I/tmp/master -DMASTER probe.c -lm && ./a.out    # -DMASTER: master's names
   ```

2. **The bug fixes** (`compare/verify_fixes.sh`).  For each fix commit, the script builds
   and runs that commit's own test suite twice: with its `hypatia.h`, and with the
   `hypatia.h` of the commit before it.  It prints the first failing assertion.

3. **The precision tables** (`compare/reproduce.sh`, about 5 minutes).  The script:
   - downloads GLM, cglm and Eigen and checks their commit hashes;
   - takes `hypatia.h` for master, before and now from git;
   - builds the harness for each version, with `-Wall -Wextra -Werror`;
   - runs it in double and single precision with two seeds;
   - runs the probe;
   - writes `results/precision/summary.md` with `summarize.py` and the platform details
     to `results/environment.txt`.

   The counts and tables in this report come from that summary.  Other versions can be
   measured with `MASTER=`, `BEFORE=` and `NOW=` (any git revision).

The random generator is a self-contained xorshift64, so a run on another machine uses
the same inputs.  The last digits of the results can still differ where the platform's
`long double` or math library differ (see "Limits").

## Method

### Inputs

Each precision measurement uses 20000 inputs: random values, and inputs that are hard
for rounding.  The hard inputs are:
- vectors nearly parallel, perpendicular or opposite;
- rotations nearly equal or near a half turn;
- matrices with condition number about 1e4;
- components from 1e-20 to 1e20;
- quaternions 1e-6 off unit length.

The inputs are built in long double or with Eigen and rounded to the precision measured,
without calling hypatia.  Every version and library gets the same rounded inputs.
`summarize.py` checks this: the results of GLM, Eigen and cglm must be identical, digit
for digit, in the runs against master, before and now, or it stops.

### Reference

The exact answer is computed from the rounded inputs in long double (64-bit mantissa on
x86-64), mostly with Eigen.  There are three exceptions:
- the quaternion of a rotation matrix is that of the nearest rotation (U Vᵀ of its SVD),
  because a rounded rotation matrix is not exactly orthogonal;
- the angle between two rotations is computed in _Float128;
- the rotation between two vectors is judged by where it takes the first vector:
  `landing_error` rotates the unit `from` by the normalized result and measures the
  distance to the unit `to`, in long double.

### Error

The error is the largest component error, divided by the largest component of the
reference, in units of the epsilon of the precision measured (ulps).  Three kinds of
measurement are scaled differently:
- dot and cross products, matrix products and determinants: relative to the size of the
  terms (|a| |b|, or the row norms);
- the inverse: divided by the condition number |A| |A⁻¹| (row-sum norm), which is how
  much any algorithm can lose;
- lookat: multiplied by the sine of the angle between view and up, for the same reason.

Without these scalings a handful of nearly singular inputs dominate the numbers.
Quaternions are compared up to sign, since q and −q are the same rotation.

### Conventions

Conventions are mapped before measuring, the same way for every library:
- axis-angle results with an angle in (π, 2π) become 2π − angle about −axis (GLM and
  master);
- angles between rotations in (π, 2π) become 2π − angle (master);
- Euler angles are compared with the matching GLM function (`eulerAngleZYX`).

`compare/hyp_master.h` maps master's experimental names.  Master has no
`vector3_project`, `matrix4_normal_matrix` or `quaternion_set_from_matrix4`; those rows
are left out of its counts.

### Aggregation and ranking

Each cell is "largest / mean" over the 20000 inputs.  Rankings use the mean, because the
largest error moves by 0.1 to 0.3 ulp from seed to seed.  A NaN counts as an infinite
error.

The 2% tie band is set against measured noise.  Between the two seeds, a library's mean
moves by a median of 0.4%, and 1.3% at the 90th percentile.  Every change above 5% is
listed in the summary:
- master's `quaternion_angle_between` on random input, by 5.4%;
- GLM's `quatLookAt` on random input, which fails on one input of the second seed.

The ratio of hypatia's mean to the best other library's moves much less, by at most 2%
in double and 3.5% in single, because both see the same inputs.  Read ratios below about
1.05 as ties.

### Checking the reference

The harness computes each reference a second way and reports the largest disagreement,
in the units of the tables ("Oracle check" at the end of each results file).  Double
precision:

| reference | second computation | largest disagreement (ulps) |
|---|---|---|
| matrix4 inverse (cofactors, Eigen) | full-pivoting LU, per unit of condition | 0.0005 |
| vector rotation (quaternion product) | the rotation matrix | 0.002 |
| slerp 1e-3 and 1e-6 rad apart (Eigen, acos) | the atan2 form | 0.0014 |
| axis-angle matrix (Rodrigues) | through a quaternion | 0.0035 |
| angle between rotations 1e-4 rad apart (\|a − b\|, \|a + b\|, in _Float128) | a b* in _Float128 | 0 |
| quaternion from a rotation matrix (Eigen, long double) | the nearest rotation (SVD) | 0.38 (random), 0.35 (near half turns) |

The check changed two references while this report was written:
- **Angle between rotations.**  The earlier long double reference lost up to 7 ulps for
  rotations 1e-4 rad apart, which is the size of the effect being measured.  It now uses
  _Float128.
- **Quaternion from a rotation matrix.**  The reference was Eigen's own method in long
  double.  On a matrix that is not quite orthogonal the methods give different
  answers, and that reference favoured Eigen's method.  With the inputs built by
  hypatia's own function the methods differed by up to 2.3 ulps; with the inputs now
  built in long double, by up to 0.38.  The reference is now the
  nearest rotation; GLM is best under it, and hypatia moved from tied to behind (0.357
  against 0.326).

## Results

### Where hypatia now is ahead (double)

| function | inputs | now (largest / mean) | best other (largest / mean) | ratio of means |
|---|---|---|---|---|
| `vector3_rotate_by_quaternion` | q of length 1 ± 1e-6 | 3.39 / 0.655 | 2.2e10 / 6.0e9 (GLM; Eigen the same) | 9.2e9 |
| `quaternion_get_rotation_tov3` | 1e-3 rad from opposite | 2.21 / 0.35 | 4080 / 639 (Eigen) | 1830 |
| `quaternion_angle_between` | 1e-4 rad apart | 4020 / 205 | 9690 / 1770 (Eigen) | 8.6 |
| `quaternion_get_rotation_tov3` | random | 2.21 / 0.538 | 116 / 0.864 (Eigen) | 1.6 |
| `vector3_reflect` | random | 4.13 / 0.458 | 4.48 / 0.657 (GLM) | 1.43 |
| `vector3_unproject_from_window` | random camera, window depth 0 to 0.9 | 86.9 / 1.86 | 100 / 2.17 (GLM) | 1.17 |
| `matrix4_view_lookat_rh` | random | 2.01 / 0.407 | 2.31 / 0.45 (GLM) | 1.11 |
| `vector3_rotate_by_quaternion` | unit q | 4.21 / 0.622 | 3.57 / 0.684 (Eigen) | 1.10 |

Six more are ahead by 2% to 8%: `vector3_refract`, the inverses,
`matrix4_set_from_quaternion`, and slerp 1e-6 rad apart.  Single precision is similar.  All of them are in the summary.

GLM and Eigen rotate a vector by q as if q had unit length.  For a q that has drifted to
length 1 ± 1e-6, the result is scaled by |q|², which is the error in the first row.
hypatia divides by |q|².

### Where hypatia now is behind (double)

| function | inputs | now (largest / mean) | best other (largest / mean) | ratio of means |
|---|---|---|---|---|
| `quaternion_get_rotation_tov3` | 1e-6 rad apart | 1.54 / 0.275 | 0.636 / 0.159 (Eigen) | 1.73 |
| `quaternion_angle_between` | random | 2.09 / 0.329 | 2.9 / 0.257 (Eigen) | 1.28 |
| `quaternion_set_from_axis_anglev3` | random | 1.32 / 0.30 | 0.897 / 0.238 (GLM) | 1.26 |
| `matrix4_set_from_axisv3_angle` | random | 2.51 / 0.496 | 2.14 / 0.439 (Eigen) | 1.13 |
| `quaternion_set_from_matrix4` | random; near half turns | 1.8 / 0.357; 1.66 / 0.332 | 1.45 / 0.326; 1.41 / 0.303 (GLM) | 1.10; 1.10 |
| `vector3_normalize` | components 1e-20 to 1e20 | 1.14 / 0.131 | 1.14 / 0.124 (Eigen) | 1.06 |
| `quaternion_set_look_rotation_rh` | random | 12.3 / 0.436 | 14.8 / 0.417 (GLM) | 1.05 |
| `quaternion_slerp` | 1e-3 rad apart; random | 2 / 0.518; 1.74 / 0.468 | 2.1 / 0.496; 1.62 / 0.457 (Eigen) | 1.04; 1.02 |

### Functions added on correctness-h

These have no counterpart on master or before, so those columns are empty.

| function | counterpart | agreement (double) | precision against long double, now against the best other (mean) |
|---|---|---|---|
| `vector2_reflect`, `vector3_reflect` | GLM `reflect`, cglm `glm_vec3_reflect` | 5e-15 | 0.458 against 0.657 (GLM) |
| `vector3_refract` | GLM `refract`, cglm `glm_vec3_refract` | 2e-15 | 0.384 against 0.413 (GLM) |
| `matrix4_projection_frustum_rh/lh` | GLM `frustumRH/LH_ZO/NO` | exact | |
| `matrix4_projection_perspective_fovy_infinite_rh/lh` | GLM `infinitePerspectiveRH/LH_ZO/NO` | 4e-16 | |
| `vector3_project_to_window` | GLM `projectZO/NO` | 2e-13 | 2.2 against 2.2 (GLM; the same arithmetic) |
| `vector3_unproject_from_window` | GLM `unProjectZO/NO` | 2e-14 | 1.86 against 2.17 (GLM) |
| `quaternion_set_look_rotation_rh/lh` | GLM `quatLookAtRH/LH`, cglm `glm_quat_for` | 6e-16 | 0.436 against 0.417 (GLM) |

The window measurements use points inside the view; points near the plane of the eye
make every library's result arbitrarily large.  Project ties GLM after 25260ab, which
sums the products of `matrix4_multiplyv4` in pairs as GLM does; before it hypatia was 5%
behind.  The look rotation shares the gap of `quaternion_set_from_matrix4`, which it
calls.

Two defects in the other libraries showed up:
- **cglm 0.9.4 `glm_vec3_refract`** computes k = 1 + eta² − (eta n·v)², where Snell's
  law gives 1 − eta² + (eta n·v)².  It disagrees with GLM and hypatia on every input
  (`results/single.md`) and is off by a mean of 9e6 float ulps.  cglm fixed it in 0.9.5.
- **GLM `quatLookAt`** divides the right axis by max(1e-5, |right|²) instead of
  normalizing it.  When up is nearly parallel to the view direction, the result is not a
  rotation.  On one input of the second seed it is off by 5e14 ulps in double; hypatia's
  largest error there is 19.

### Where now is less precise than master

| precision | function | inputs | master | now |
|---|---|---|---|---|
| double | `matrix4_set_from_axisv3_angle` | random | 2.14 / 0.439 | 2.51 / 0.496 |
| double | `quaternion_set_from_axis_anglev3` | random | 1.32 / 0.285 | 1.32 / 0.30 |
| single | `matrix4_set_from_axisv3_angle` | random | 2.21 / 0.443 | 2.69 / 0.503 |
| single | `quaternion_set_from_axis_anglev3` | random | 1.31 / 0.286 | 1.47 / 0.304 |
| single | `quaternion_slerp` | 1e-3 rad apart | 1.47 / 0.414 | 1.67 / 0.474 |
| single | `quaternion_slerp` | 1e-6 rad apart | 1.25 / 0.358 | 1.47 / 0.376 |

- **Axis-angle builders.**  They now accept an axis of any length (87032b1) and
  normalize it, which adds a rounding when the axis is already unit.  Master required a
  unit axis.
- **Slerp, single precision.**  Master falls back to an unnormalized linear
  interpolation when the dot product is within 1e-5 of 1.  At 1e-3 rad that error is
  below float rounding.  In double the same shortcut is off by about 1e8 ulps (the probe
  below).

### Master: known-answer checks

From `compare/results/probe/master.txt`.  Before and now pass all 13.

| check | master gives | expected |
|---|---|---|
| `vector3_normalize` of (1, 2, 2) × 1e-20 | the input unchanged | (1/3, 2/3, 2/3) |
| `vector3_rotate_by_quaternion` keeps the length | 1.073 → 0.871 | 1.073 |
| `matrix4_make_transformation_rotationq` applied with `matrix4_multiplyv3`, against `vector3_rotate_by_quaternion` with the same q | x → −y | x → y, as the quaternion |
| `matrix4_set_from_euler_anglesf3_EXP(0, 0, a)` against `matrix4_set_from_axisv3_angle_EXP(z, a)` | rotates by −a | rotates by a, as the axis-angle matrix |
| `matrix4_inverse` of diag(0.01, 0.01, 0.01, 1) (determinant 1e-6) | the identity | diag(100, 100, 100, 1) |
| perspective, fovy 60°: a point on the top edge of the view | y / w = 0.15 | 1 |
| lookat: where the eye (1, 2, 5) goes | (0, 0, 10.95) | the origin |
| `quaternion_slerp(identity, 90° about z, 1e-6)` | the identity | a turn of 1.57e-6 rad |
| `quaternion_slerp`, 1e-3 rad apart, t = 0.5: the length | 1 − 3.1e-8 | 1 |
| `quaternion_get_rotation_tov3`, x to y | (0, 0, 1, 1) | (0, 0, 0.707, 0.707) |
| `quaternion_get_rotation_tov3`, x to −x | (0, 0, 0, 0) | a half turn |
| `quaternion_angle_between_EXP`, 1e-4 rad | relative error 2.9e-9 | 1e-4 |
| `quaternion_get_axis_anglev3`, 1e-4 rad | relative error 5.6e-9 | 1e-4 |

The causes are visible in master's source:
- an absolute tolerance of 1e-5 (`scalar_equalsf`) used for decisions: a singular
  determinant, t = 0 or 1, parallel quaternions, a zero vector;
- `cot(fovy) / 2` where `cot(fovy / 2)` is meant;
- a translation in lookat whose sign doesn't match its rotation;
- `2 acos(w)` and `2 acos(dot)`, which lose half the digits near zero angles.

The last two checks are a loss of precision rather than a wrong result.

### The bug fixes, each with its test

From `compare/results/verify_fixes.txt`: each fix commit's tests pass with the fix and
fail with the `hypatia.h` of the commit before it.

| commit | fix | first assertion that fails without it |
|---|---|---|
| 2700d4b | sign in `vector3_rotate_by_quaternion` | test/test_quaternion.c(229) |
| 8b69e46 | matrix rotation builders follow the right-hand rule | test/test_quaternion.c(1180) |
| 3ff02c7 | normalize anything but an exactly zero vector; scale before squaring | test/test_quaternion.c(947) |
| 5ffe649 | only an exactly zero determinant has no inverse | test/test_matrix2.c(370) |
| 9ebeeb8 | slerp: shortest arc, exact at the ends | test/test_quaternion.c(1004) |
| f5137e6 | `get_rotation_tov3` and `get_axis_anglev3` accurate for any length | test/test_quaternion.c(1194) |
| f1f9ad2 | `vector2/3_angle_between`: no NaN for parallel vectors | test/test_vector2.c(464) |
| 735a70e | slerp: no linear shortcut below 0.009 rad | test/test_quaternion.c(1275) |
| 87032b1 | rotate by a quaternion or an axis of any length | test/test_quaternion.c(1292) |
| ed5c45a | `matrix4_inverse`: no overflow for very small matrices | test/test_matrix4.c(1170) |
| 082ad4a | `matrix2/3_inverse`: the same | test/test_matrix2.c(495) |
| 860b94d | `matrix4_inverse`: precision for ill-conditioned matrices | test/test_matrix4.c(1126) |

The first six fix defects that are on master.  The last six fix defects that this
comparison found in intermediate versions of correctness-h.

### Earlier claims corrected

- **The commit message of 58eb5fb** quotes a gain for `matrix4_inverse` from 9 to 3.5
  ulps in the mean.  That figure was not divided by the condition number and came from
  a few nearly singular matrices.  Per unit of condition the change is about level with
  before: a mean of 0.0623 against 0.0631, and a largest error of 0.414 against 0.433.
- **The first version of this report** counted now as best or tied in 34 of 41.  Three
  corrections bring that to 32:
  - the input generators called hypatia's own functions, so each version was measured
    on slightly different inputs;
  - master's angles were compared without the [0, π] mapping already applied to GLM;
  - the two references above were replaced.

## Conventions that differ (not errors)

| hypatia | the others |
|---|---|
| `matrix4_set_from_euler_anglesf3(x, y, z)` rotates about X, then Y, then Z (Rz Ry Rx) | GLM `eulerAngleZYX(z, y, x)` and cglm `glm_euler_zyx` are the same; GLM `eulerAngleXYZ` and cglm `glm_euler_xyz` are Rx Ry Rz |
| `matrix4_translatev3(M, v)` (and rotate, scale) applies the new transform after M | GLM `translate(M, v)` applies it before M |
| `quaternion_get_axis_anglev3` gives an angle in [0, π] | GLM `angle` gives [0, 2π] |
| `quaternion_slerp` takes the shorter arc | GLM `slerp`, Eigen and cglm do too; GLM `mix` does not |

## Agreement and edge cases

The full comparison of now (`compare/results/double.md`, `single.md`) compares each
function with its counterparts on 2000 random inputs.
- **Agreement:** in double, every row agrees to 1e-10 except the three rows that compare
  across the conventions above.  In single, cglm's refract also disagrees (above).
- **Edge cases:** zero, tiny, huge, infinite and NaN input, and degenerate geometry
  (opposite vectors, eye == target, a zero axis).  hypatia gives a defined result in
  each case (the input unchanged, the identity, or 0) where GLM usually gives NaN, and
  Eigen and cglm vary.
- **GLM:** the same files show that `glm::rotation` doesn't rotate exactly opposite
  vectors onto each other, and that `glm::angle` of two vectors is about 1e-12 off in
  double near 0 and π (it uses acos).

## Limits

- **hypatia was tuned against this benchmark.**  The precision work chose formulas by
  their results here, so other inputs could rank the libraries differently.  The
  measurements, the input kinds and the 2% band were chosen by the same people who did
  the work.
- **The measurements are a selection:** 46 of them, of 30 functions.
  `vector4_cross_product`, the remaining experimental functions and functions without a
  counterpart in the other libraries are not measured.
- **One platform:** x86-64 with SSE2, no FMA, gcc 13.3, `-O3` without `-ffast-math`
  (`compare/results/environment.txt`).
  - With FMA contraction, on ARM or with another compiler, the last digits of every
    library change, and Eigen may take other vector paths.
  - Where `long double` is the same as double (MSVC; macOS on ARM), the reference is no
    more precise than the double results, and the double tables can't be reproduced
    there.
- **The reference is long double**, accurate to about 1e-19 relative, 2000 times finer
  than a double ulp.  The oracle check covers the cases where that is not enough.
- **cglm is float only**, so it appears in the single precision tables only.
- **Most differences are small.**  On random inputs they are a few hundredths of an ulp:
  real (they hold across seeds), but they rarely matter in practice.  The libraries
  differ by orders of magnitude only on the near-degenerate inputs.
- **Speed is not measured.**

## Files

| file | contents |
|---|---|
| `compare/reproduce.sh` | regenerates everything below |
| `compare/fetch.sh` | downloads the other libraries at pinned commits |
| `compare/verify_fixes.sh` | runs each fix's tests with and without the fix |
| `compare/probe.c` | the known-answer checks |
| `compare/summarize.py` | the counts and tables, from the precision results |
| `compare/compare_precision.inc` | the precision measurements and the oracle check |
| `compare/results/precision/summary.md` | every count and table, including the full precision tables per version and library |
| `compare/results/precision/{master,before,now}.{double,single}[.s7].md` | the raw precision results per version, precision and seed |
| `compare/results/probe/*.txt` | the probe, per version |
| `compare/results/verify_fixes.txt` | the fixes |
| `compare/results/{double,single,double_depth_no}.md` | the full comparison of now: agreement, edge cases, accuracy and precision |
| `compare/results/environment.txt` | the platform, compilers, flags and commits |
