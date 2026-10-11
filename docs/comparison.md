# hypatia compared with GLM, Eigen and cglm

This report measures the precision and correctness of three versions of hypatia against
GLM, Eigen and cglm, and explains how to check every number in it.  The generated
results it quotes are committed in `compare/results/`; `compare/reproduce.sh`
regenerates them.

| version | commit | what it is |
|---|---|---|
| master | 125ab87 | 2.1.0-dev, before the correctness work |
| before | e479a29 | correctness after every bug fix; the normalize, rotation and inverse commits it contains also carry their precision changes, and only 1effbda (divide by \|q\|^2 directly) and f090db3 (sums in pairs) come after it |
| now | ffe28b0 | correctness when measured |

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
| before | 28 of 41 | 10 | 13 | 0 | 28 of 41 | 10 | 13 | 0 |
| now | 46 of 46 | 29 | 0 | 0 | 46 of 46 | 27 | 0 | 0 |

- **Best or tied:** hypatia's mean error is no more than 2% above the smallest mean of
  GLM, Eigen and cglm on the same inputs.  **Ahead** and **behind** mean more than 2%
  below or above it.  With a second seed (`COMPARE_SEED=7`) the counts for now are the
  same: 46, 29 and 0 in double, and 46, 27 and 0 in single.  Master and before have 37 and 41
  measurements: the others are of functions they do not have.
- **The large margins are on near-degenerate inputs.**
  - Quaternions that have drifted from unit length: GLM and Eigen assume unit length.
  - The angle between rotations 1e-4 rad apart: mean error 8080 times lower than Eigen's.
  - Rotations between nearly opposite vectors: 1780 times lower.
  - Matrix inverse at condition 1e4: 7 times lower per unit of condition.

  On random inputs the differences between the libraries are 2% to 90% in the mean,
  i.e. fractions of an ulp.
- **Behind:** none.  In every measurement, in both precisions and with both seeds,
  hypatia's mean error is at most 2% above the smallest of GLM, Eigen and cglm.  The
  normalization is correctly rounded (each component within half an ulp), and the angle,
  slerp and 2x2 minors use exact products and sums.
- **Master:**
  - fails all 13 known-answer checks, which before and now pass: 11 wrong results and
    2 losses of precision (`compare/results/probe/`);
  - an error of 1e6 ulps or more in 15 of 37 double measurements.
- **Bug fixes:** each of 9 ships with a test that fails without the fix and passes with
  it (`compare/results/verify_fixes.txt`).
- **Added functions:** reflect, refract, the off-center frustum, the infinite
  perspective, project and unproject to the window, and the look rotation.
  - They agree with GLM to 1e-13 in double.
  - Against long double, reflect, refract, unproject and the look rotation are ahead of
    GLM, and project is tied.
  - Measuring them found two defects in the other libraries: cglm 0.9.4's refract has a
    sign error (fixed in cglm 0.9.5), and GLM's `quatLookAt` fails when up is nearly
    parallel to the view direction (below).
- **Reports:** every case where another library is wrong or less precise is written up as
  an issue report with a program that shows it, in `docs/reports/` (74 reports: 49 for GLM,
  Eigen and cglm, and 25 for HandmadeMath, raymath and linmath.h, which the precision
  tables do not cover; 73 still present upstream).
- **Regressions:** none: now is not less precise than master in any measurement.

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
  nearest rotation.  Under it hypatia first measured behind GLM (0.357 against 0.326);
  with the correctly rounded normalization it is ahead (0.296).

## Results

### Where hypatia now is ahead (double)

The ten largest margins; `compare/results/precision/summary.md` lists all 29.

| function | inputs | now (largest / mean) | best other (largest / mean) | ratio of means |
|---|---|---|---|---|
| `vector3_rotate_by_quaternion` | q of length 1 ± 1e-6 | 3.39 / 0.655 | 2.2e10 / 6.0e9 (GLM; Eigen the same) | 9.2e9 |
| `quaternion_angle_between` | 1e-4 rad apart | 0.505 / 0.219 | 9690 / 1770 (Eigen) | 8080 |
| `quaternion_get_rotation_tov3` | 1e-3 rad from opposite | 2.71 / 0.358 | 4080 / 639 (Eigen) | 1780 |
| `matrix4_inverse` | condition ~1e4, per unit of condition | 2.5 / 0.137 | 15.6 / 0.956 (Eigen) | 6.98 |
| `vector3_normalize` | (1, 1e-5, 1e-5) | 0.25 / 0.126 | 1.26 / 0.248 (Eigen) | 1.97 |
| `vector3_normalize` | components 1e-20 to 1e20 | 0.351 / 0.065 | 1.14 / 0.124 (Eigen) | 1.91 |
| `quaternion_get_rotation_tov3` | random | 2.15 / 0.463 | 116 / 0.864 (Eigen) | 1.87 |
| `vector3_normalize` | random | 0.426 / 0.203 | 1.09 / 0.321 (Eigen) | 1.58 |
| `vector3_reflect` | random | 4.13 / 0.458 | 4.48 / 0.657 (GLM) | 1.43 |
| `quaternion_normalize` | random length | 0.463 / 0.199 | 1.19 / 0.28 (Eigen) | 1.41 |

Single precision is similar.  GLM and Eigen rotate a vector by q as if q had unit length.
For a q that has drifted to length 1 ± 1e-6, the result is scaled by |q|², which is the
error in the first row.  hypatia divides by |q|².

### Where hypatia now is behind (double)

None, in double or single, with either seed.

### Functions added on correctness

These have no counterpart on master or before, so those columns are empty.

| function | counterpart | agreement (double) | precision against long double, now against the best other (mean) |
|---|---|---|---|
| `vector2_reflect`, `vector3_reflect` | GLM `reflect`, cglm `glm_vec3_reflect` | 5e-15 | 0.458 against 0.657 (GLM) |
| `vector3_refract` | GLM `refract`, cglm `glm_vec3_refract` | 2e-15 | 0.321 against 0.413 (GLM) |
| `matrix4_projection_frustum_rh/lh` | GLM `frustumRH/LH_ZO/NO` | exact | |
| `matrix4_projection_perspective_fovy_infinite_rh/lh` | GLM `infinitePerspectiveRH/LH_ZO/NO` | 4e-16 | |
| `vector3_project_to_window` | GLM `projectZO/NO` | 2e-13 | 2.2 against 2.2 (GLM; the same arithmetic) |
| `vector3_unproject_from_window` | GLM `unProjectZO/NO` | 2e-14 | 1.77 against 2.17 (GLM) |
| `quaternion_set_look_rotation_rh/lh` | GLM `quatLookAtRH/LH`, cglm `glm_quat_for` | 6e-16 | 0.35 against 0.417 (GLM) |

The window measurements use points inside the view; points near the plane of the eye
make every library's result arbitrarily large.  Project ties GLM: f090db3 sums the products of
`matrix4_multiplyv4` in pairs, as GLM does.

Two defects in the other libraries showed up:
- **cglm 0.9.4 `glm_vec3_refract`** computes k = 1 + eta² − (eta n·v)², where Snell's
  law gives 1 − eta² + (eta n·v)².  It disagrees with GLM and hypatia on every input
  (`results/single.md`) and is off by a mean of 9e6 float ulps.  cglm fixed it in 0.9.5.
- **GLM `quatLookAt`** divides the right axis by max(1e-5, |right|²) instead of
  normalizing it.  When up is nearly parallel to the view direction, the result is not a
  rotation.  On one input of the second seed it is off by 5e14 ulps in double; hypatia's
  largest error there is 19.

### Where now is less precise than master

None.  Earlier versions of correctness were less precise than master in six
measurements: the axis-angle builders, which now accept an axis of any length and
normalize it, and slerp in single precision, where master's linear shortcut is below
float rounding at 1e-3 rad.  The correctly rounded normalization, the axis-angle builders
no longer normalizing their result a second time, and slerp's exact products removed all
six.

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
| 96c072f | sign in `vector3_rotate_by_quaternion` | test/test_quaternion.c(229) |
| fb163c4 | matrix rotation builders follow the right-hand rule | test/test_quaternion.c(1180) |
| d68e63b | normalize anything but an exactly zero vector; scale before squaring | test/test_quaternion.c(947) |
| 1588eb7 | only an exactly zero determinant has no inverse | test/test_matrix2.c(370) |
| f2b1f22 | slerp: shortest arc, exact at the ends, no linear shortcut near parallel | test/test_quaternion.c(1025) |
| 9b67752 | `get_rotation_tov3` and `get_axis_anglev3` accurate for any length | test/test_quaternion.c(1194) |
| 8e6eea3 | `vector2/3_angle_between`: no NaN for parallel vectors | test/test_vector2.c(505) |
| b7ff6bd | rotate by a quaternion or an axis of any length | test/test_quaternion.c(1292) |
| 79cd6d9 | `matrix2/3/4_inverse`: no overflow for very small or large matrices; precision for ill-conditioned ones | test/test_matrix2.c(495) |

### Earlier claims corrected

- **An earlier commit message for `matrix4_inverse`** (2x2 blocks, as Eigen) quoted a gain
  from 9 to 3.5 ulps in the mean.  That figure was not divided by the condition number and
  came from a few nearly singular matrices.  Per unit of condition, `matrix4_inverse` has
  a mean of 0.0623 on random entries, against 0.0641 for GLM and 0.0637 for Eigen.
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
