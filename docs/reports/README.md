# Reports for GLM, Eigen, cglm, HandmadeMath, raymath and linmath.h

Each file describes one case where another library gives a wrong result, NaN, or
measurably more rounding error than hypatia, in the form of an issue report for that
library: what happens, a program that shows it, its output, the cause in the library's
source (linked at the tested commit), a suggested fix, how hypatia handles the same case
with a program that shows it, and how to check.

The programs in every report are built and run by `compare/check_reports.py`, which
compares their output with the text of the report:

```sh
cd compare
./fetch.sh                 # the libraries at the tested commits
python3 check_reports.py   # every report; or name one or more .md files
```

The reports cite the commits the comparison was run against (GLM 1.0.1, cglm 0.9.4, Eigen
3.4.0; HandmadeMath master 661fef0, raymath of raylib 6.0, linmath.h master 3eef828).
Every program was also run against the latest releases (GLM 1.0.3, cglm 0.9.6, Eigen
5.0.1, HandmadeMath v2.0.0, raylib 6.0) and the master branches of 2026-10-10 with
`COMPARE_EXT=<directory with the libraries> python3 check_reports.py`: each prints the
same, except cglm's refract, fixed in cglm 0.9.5; raymath's rotation between opposite
vectors, changed at master (see its Status line); and two HandmadeMath functions that are
not in a release yet.  Each report has a Status line.

The outputs were produced on x86-64 Linux with gcc 13.3 at `-O2` (no `-ffast-math`, no FMA).
Other platforms can differ in the last digits printed.  The measurements quoted from the
comparison harness come from `compare/results/` (see `docs/comparison.md`).

## GLM (20 reports)

| report | kind | status |
|---|---|---|
| [`glm::rotation` returns a degenerate quaternion for opposite vectors](glm/01-rotation-opposite-vectors.md) | wrong result | present in the latest release |
| [`glm::rotation` returns the identity for vectors less than sqrt(2 epsilon) apart](glm/02-rotation-small-angle-identity.md) | wrong result (accuracy) | present in the latest release |
| [`glm::rotation` loses most of its digits for nearly opposite vectors](glm/03-rotation-near-opposite-precision.md) | precision | present in the latest release |
| [`glm::axis` returns an axis that is not unit length for small rotations](glm/04-axis-small-rotations.md) | precision | present in the latest release |
| [`glm::quatLookAt` returns a non-rotation when up is nearly parallel to the direction](glm/05-quatlookat-up-nearly-parallel.md) | wrong result | present in the latest release |
| [`q * v` does not rotate when q is not exactly unit length](glm/06-quaternion-times-vector-not-unit.md) | wrong result for non-unit q; precision for drifted q | present in the latest release |
| [`glm::mix` of q and -q returns the zero quaternion](glm/07-mix-opposite-quaternions.md) | wrong result | present in the latest release |
| [`glm::normalize` overflows and underflows: NaN, inf or zero for valid vectors](glm/08-normalize-overflow-underflow.md) | wrong result | present in the latest release |
| [`glm::normalize` of the zero vector is NaN, and the NaN spreads](glm/09-normalize-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm::angle(x, y)` loses its digits for nearly parallel and nearly opposite vectors](glm/10-angle-acos-nearly-parallel.md) | precision | present in the latest release |
| [`glm::proj` onto the zero vector is NaN](glm/11-proj-onto-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm::inverse` is NaN for the zero quaternion, and wrong for tiny and huge ones](glm/12-inverse-zero-quaternion.md) | NaN on degenerate input; wrong result | present in the latest release |
| [`glm::lookAt` gives NaN when the eye is at the target or looks along up](glm/13-lookat-degenerate-nan.md) | NaN on degenerate input | present in the latest release |
| [`glm::rotation` with a zero vector is NaN](glm/14-rotation-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm::angleAxis` and `glm::rotate` disagree on axes that are not unit length, and fail on a zero axis](glm/15-axis-angle-zero-or-not-unit-axis.md) | inconsistent API; NaN on degenerate input | present in the latest release |
| [`glm::inverse` of a singular matrix returns NaN with no way to tell](glm/16-inverse-singular-matrix-nan.md) | NaN on degenerate input; no error report | present in the latest release |
| [`glm::normalize` rounds three times instead of twice: more error than dividing by the length](glm/17-normalize-two-roundings.md) | precision | present in the latest release |
| [Smaller precision differences: GLM has 3% to 21% more rounding error in thirteen measurements](glm/18-precision-small-differences.md) | precision | present in the latest release |
| [`glm::normalize` of a tiny quaternion returns the identity, a different rotation; of a huge one, zero](glm/19-normalize-quaternion-tiny-huge.md) | wrong result | present in the latest release |
| [`glm::rotate(q, angle, axis)` skips normalizing axes within 0.001 of unit length](glm/20-rotate-quaternion-axis-threshold.md) | wrong result (accuracy) | present in the latest release |

## Eigen (9 reports)

| report | kind | status |
|---|---|---|
| [`Quaternion::FromTwoVectors` is accurate to only sqrt(epsilon) for opposite vectors](eigen/01-fromtwovectors-opposite.md) | precision | present in the latest release |
| [`Quaternion::FromTwoVectors` loses digits for vectors near opposite](eigen/02-fromtwovectors-near-opposite-precision.md) | precision | present in the latest release |
| [`Quaternion::angularDistance` loses digits for nearly equal rotations](eigen/03-angulardistance-small-angles.md) | precision | present in the latest release |
| [`normalized()` returns tiny vectors unchanged and huge vectors as zero](eigen/04-normalized-underflow-overflow.md) | wrong result | present in the latest release |
| [`q * v` does not rotate when q is not exactly unit length](eigen/05-quaternion-times-vector-not-unit.md) | wrong result for non-unit q; precision for drifted q | present in the latest release |
| [`Quaternion::FromTwoVectors` with a zero vector returns a quaternion of length 0.71](eigen/06-fromtwovectors-zero-vector.md) | wrong result on degenerate input | present in the latest release |
| [`AngleAxis` with an axis that is not unit length gives a scaled quaternion and a non-rotation matrix](eigen/07-angleaxis-axis-not-unit.md) | documented precondition, not checked | present in the latest release |
| [Smaller precision differences: Eigen has 2% to 10% more rounding error in seven measurements of six functions](eigen/08-precision-small-differences.md) | precision | present in the latest release |
| [`Quaternion::inverse()` returns zero for tiny and huge quaternions, which have inverses](eigen/09-quaternion-inverse-tiny-huge.md) | wrong result | present in the latest release |

## cglm (15 reports)

| report | kind | status |
|---|---|---|
| [`glm_vec3_refract` has a sign error: it gives the refracted direction only at normal incidence](cglm/01-refract-sign-error.md) | wrong result | fixed upstream |
| [`glm_quat_slerp` returns a near-zero quaternion for nearly equal rotations of opposite sign](cglm/02-slerp-fallback-ignores-sign.md) | wrong result | present in the latest release |
| [`glm_quat_slerp` returns its first argument when the dot product rounds to 1](cglm/03-slerp-small-angles.md) | precision | present in the latest release |
| [`glm_vec3_normalize` sets every vector shorter than 1.19e-7 to zero, and long vectors overflow](cglm/04-normalize-zeroes-short-vectors.md) | wrong result | present in the latest release |
| [`glm_quat_from_vecs` returns the identity for vectors up to 0.26 degrees apart, and a wrong half turn near opposite](cglm/05-quat-from-vecs-thresholds.md) | wrong result | present in the latest release |
| [`glm_rotate_make` with a zero (or short) axis returns a matrix that is not a rotation](cglm/06-rotate-make-zero-axis.md) | wrong result on degenerate input | present in the latest release |
| [`glm_quatv` with a zero (or short) axis returns a quaternion of length cos(angle/2)](cglm/07-quatv-zero-axis.md) | wrong result on degenerate input | present in the latest release |
| [`glm_quat_for` returns a quaternion of length 0.71 when up is parallel to the direction](cglm/08-quat-for-up-parallel.md) | wrong result on degenerate input | present in the latest release |
| [`glm_vec3_angle` returns 0 for vectors up to about 6e-4 rad apart, and NaN for a zero vector](cglm/09-vec3-angle-acos.md) | precision; NaN on degenerate input | present in the latest release |
| [`glm_vec3_proj` onto the zero vector is NaN](cglm/10-proj-onto-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm_quat_inv` is NaN for the zero quaternion, and wrong for tiny and huge ones](cglm/11-quat-inv-zero.md) | NaN on degenerate input; wrong result | present in the latest release |
| [Smaller precision differences: cglm has more rounding error than hypatia in seventeen measurements](cglm/12-precision-small-differences.md) | precision | present in the latest release |
| [`glm_quat_normalize` turns a tiny quaternion into the identity, a different rotation, and a huge one into zero](cglm/13-quat-normalize-tiny-huge.md) | wrong result | present in the latest release |
| [`glm_vec4_normalize` on SSE sets every vector shorter than 3.45e-4 to zero](cglm/14-vec4-normalize-sse-threshold.md) | wrong result | present in the latest release |
| [`glm_quat_mat4` and `glm_quat_mat3` scale by 2 / |q| instead of 2 / |q|^2](cglm/15-quat-mat4-non-unit.md) | wrong result | present in the latest release |

## HandmadeMath (7 reports)

| report | kind | status |
|---|---|---|
| [`HMM_NormV3`, `HMM_NormV4` and `HMM_NormQ` give NaN, inf or zero for zero, tiny and huge inputs](handmademath/01-normalize-zero-tiny-huge.md) | NaN on degenerate input; wrong result | present in the latest release |
| [`HMM_InvQ` is NaN for the zero quaternion, and wrong for tiny and huge ones](handmademath/02-invq-zero-tiny-huge.md) | NaN on degenerate input; wrong result | present in the latest release |
| [`HMM_QFromVecPair` is NaN for opposite vectors and loses most digits for nearly opposite ones](handmademath/03-qfromvecpair-opposite.md) | NaN on valid input; precision | present at master (not in a release) |
| [`HMM_RotateV3Q` does not rotate when q is not unit length](handmademath/04-rotatev3q-not-unit.md) | wrong result for non-unit q | present at master (not in a release) |
| [`HMM_SLerp` uses a normalized lerp up to 0.063 rad: angle errors 120 times the rounding](handmademath/05-slerp-nlerp-threshold.md) | precision | present in the latest release |
| [`HMM_Rotate_RH` and `HMM_QFromAxisAngle_RH` give NaN for a zero axis](handmademath/06-zero-axis-nan.md) | NaN on degenerate input | present in the latest release |
| [`HMM_LookAt_RH` gives NaN when the eye is at the target or looks along up](handmademath/07-lookat-degenerate-nan.md) | NaN on degenerate input | present in the latest release |

## raymath (8 reports)

| report | kind | status |
|---|---|---|
| [`Vector3Normalize` and `QuaternionNormalize` return tiny inputs unchanged and huge ones as zero](raymath/01-normalize-tiny-huge.md) | wrong result | present in the latest release |
| [`QuaternionInvert` gives inf and NaN for tiny quaternions and zero for huge ones](raymath/02-quaternioninvert-tiny-huge.md) | wrong result | present in the latest release |
| [`Vector3Project` onto the zero vector is NaN](raymath/03-project-onto-zero.md) | NaN on degenerate input | present in the latest release |
| [`Vector3RotateByQuaternion` and `QuaternionToMatrix` disagree, and neither rotates, when q is not unit length](raymath/04-rotate-by-quaternion-not-unit.md) | wrong result for non-unit q | present in the latest release |
| [`QuaternionFromVector3ToVector3` returns the zero quaternion for opposite vectors and loses digits for nearly opposite ones](raymath/05-from-vector3-to-vector3-opposite.md) | wrong result; precision | present in the latest release |
| [`QuaternionSlerp` uses a normalized lerp up to 0.64 rad: angle errors up to 9e-4 rad](raymath/06-slerp-nlerp-threshold.md) | wrong result (accuracy) | present in the latest release |
| [`QuaternionToAxisAngle` is inaccurate for small rotations: 2% error at 1e-3 rad](raymath/07-to-axis-angle-small-rotations.md) | precision | present in the latest release |
| [`MatrixRotate` with a zero axis returns cos(angle) times the identity](raymath/08-matrix-rotate-zero-axis.md) | wrong result on degenerate input | present in the latest release |

## linmath.h (5 reports)

| report | kind | status |
|---|---|---|
| [`quat_from_mat4x4` returns the wrong rotation: the identity becomes a half turn about x](linmath/01-quat-from-mat4x4-wrong.md) | wrong result | present at the latest commit (no releases) |
| [`vec3_norm` gives NaN, inf or zero for zero, tiny and huge vectors](linmath/02-norm-zero-tiny-huge.md) | NaN on degenerate input; wrong result | present at the latest commit (no releases) |
| [`quat_mul_vec3` and `mat4x4_from_quat` disagree, and neither rotates, when q is not unit length](linmath/03-quat-not-unit.md) | wrong result for non-unit q | present at the latest commit (no releases) |
| [`mat4x4_rotate` ignores the rotation when the axis is shorter than 1e-4, and `quat_rotate` is NaN for a zero axis](linmath/04-rotate-short-axis-ignored.md) | wrong result; NaN on degenerate input | present at the latest commit (no releases) |
| [`mat4x4_look_at` gives NaN when the eye is at the target or looks along up](linmath/05-look-at-degenerate-nan.md) | NaN on degenerate input | present at the latest commit (no releases) |
