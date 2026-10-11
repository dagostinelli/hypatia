# Smaller precision differences: cglm has from 4% more to 265 times the rounding error of hypatia in twenty-four measurements

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | normalization, `glm_mat4_inv`, `glm_quat_slerp`, `glm_quat_axis` / `glm_quat_angle`, `glm_quat_from_vecs`, `glm_vec3_reflect`, `glm_rotate_make`, `glm_quatv`, `glm_lookat`, `glm_quat_for`, `glm_mat4_det`, `glm_mat4_quat`, `glm_quat_rotatev`, `glm_quat_inv`, `glm_perspective`, `glm_quat_mul` |
| Kind | precision |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

Apart from the cases reported separately, the comparison harness measures cglm with more
rounding error than hypatia, on the same inputs and against a long double reference, in
these measurements (mean error in float epsilons over 20000 inputs; each holds with a
second random seed):

| cglm | hypatia | cglm | ratio | hypatia's approach |
|---|---|---|---|---|
| `glm_vec3_normalize`, one large, two small (1, 1e-5, 1e-5) | `vector3_normalize` | 0.0747 against 0.000282 | 264.89 | scaled by a power of two, then each component rounded once (exact products) |
| `glm_mat4_inv`, condition ~1e4 | `matrix4_inverse` | 1.02 against 0.144 (per unit of condition) | 7.08 | 2x2 minors rounded once, determinant shared with the cofactors, division by it |
| `glm_quat_slerp`, 1e-6 rad apart | `quaternion_slerp` | 1.94 against 0.31 | 6.26 | the angle from `2 atan2(\| \|t\| s - \|s\| t \|, \| \|t\| s + \|s\| t \|)` with exact products, the two terms combined with one rounding |
| `glm_quat_axis` * `glm_quat_angle`, angle 1e-4 | `quaternion_get_axis_anglev3` | 0.374 against 0.0899 | 4.16 | the vector part divided by the length that gives the angle `2 atan2(\|v\|, w)` |
| `glm_quat_from_vecs`, random | `quaternion_get_rotation_tov3` | 0.907 against 0.468 | 1.94 | half angle from `\|f - t\|` and `\|f + t\|`, the axis from the shorter of them |
| `glm_quat_axis` * `glm_quat_angle`, random | `quaternion_get_axis_anglev3` | 0.704 against 0.373 | 1.89 | the vector part divided by the length that gives the angle `2 atan2(\|v\|, w)` |
| `glm_vec3_normalize`, random | `vector3_normalize` | 0.37 against 0.203 | 1.82 | scaled by a power of two, then each component rounded once (exact products) |
| `glm_quat_slerp`, random | `quaternion_slerp` | 0.61 against 0.408 | 1.50 | the angle from `2 atan2(\| \|t\| s - \|s\| t \|, \| \|t\| s + \|s\| t \|)` with exact products, the two terms combined with one rounding |
| `glm_vec3_reflect`, random | `vector3_reflect` | 0.664 against 0.466 | 1.42 | divides by `dot(n, n)`; no separate normalization |
| `glm_quat_normalize`, random length | `quaternion_normalize` | 0.28 against 0.198 | 1.41 | scaled by a power of two, then each component rounded once (exact products) |
| `glm_rotate_make`, random | `matrix4_set_from_axisv3_angle` | 0.608 against 0.442 | 1.38 | the axis normalized by `hyp_normalize` |
| `glm_quatv`, random | `quaternion_set_from_axis_anglev3` | 0.309 against 0.24 | 1.29 | the axis normalized by `hyp_normalize`, the result not normalized again |
| `glm_lookat`, random (error * sin(view, up)) | `matrix4_view_lookat_rh` | 0.458 against 0.36 | 1.27 | the axes normalized by `hyp_normalize` |
| `glm_quat_for`, random | `quaternion_set_look_rotation_rh` | 0.434 against 0.348 | 1.25 | the rotation matrix and Shepperd's method, normalized by `hyp_normalize` |
| `glm_mat4_det`, random entries | `matrix4_determinant` | 0.261 against 0.215 | 1.21 | 2x2 minors rounded once, determinant from the blocks |
| `glm_mat4_inv`, rotation, scale and translation | `matrix4_inverse` | 0.0118 against 0.01 (per unit of condition) | 1.18 | 2x2 minors rounded once, determinant shared with the cofactors, division by it |
| `glm_mat4_quat`, near half turns (pi - 1e-3) | `quaternion_set_from_matrix4` | 0.329 against 0.279 | 1.18 | Shepperd's method, the result normalized by `hyp_normalize` |
| `glm_mat4_inv`, random entries | `matrix4_inverse` | 0.0645 against 0.0552 (per unit of condition) | 1.17 | 2x2 minors rounded once, determinant shared with the cofactors, division by it |
| `glm_quat_rotatev`, unit q | `vector3_rotate_by_quaternion` | 0.713 against 0.62 | 1.15 | `(2 (u.v) u + (w^2 - u.u) v + 2 w (u x v)) / \|q\|^2`, one division |
| `glm_mat4_quat`, random rotation | `quaternion_set_from_matrix4` | 0.337 against 0.295 | 1.14 | Shepperd's method, the result normalized by `hyp_normalize` |
| `glm_quat_inv`, random length | `quaternion_inverse` | 0.431 against 0.387 | 1.11 | the conjugate divided by `dot(q, q)` |
| `glm_quat_rotatev`, q of length 1 +- 1e-6 (drifted) | `vector3_rotate_by_quaternion` | 0.717 against 0.649 | 1.10 | `(2 (u.v) u + (w^2 - u.u) v + 2 w (u x v)) / \|q\|^2`, one division |
| `glm_perspective`, random | `matrix4_projection_perspective_fovy_rh` | 0.38 against 0.353 | 1.08 | divides by `zNear - zFar` instead of multiplying by its reciprocal |
| `glm_quat_mul`, random | `quaternion_multiply` | 0.302 against 0.29 | 1.04 | the four products summed in pairs |

Several rows share one cause: cglm divides by multiplying with a reciprocal (`1.0f /
norm`, `1.0f / (nearZ - farZ)`, `1.0f / norm2`), which rounds twice.  The program below
measures the `glm_vec3_normalize`, random row on its own inputs (0.289 against 0.157; the
harness measures relative to the largest component).

## Reproduction

```c
#include <cglm/cglm.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	long double sum = 0;
	int i, k;
	for (i = 0; i < 200000; i++) {
		vec3 v = {(float)(20 * rnd() - 10), (float)(20 * rnd() - 10), (float)(20 * rnd() - 10)}, c;
		long double n = sqrtl((long double)v[0] * v[0] + (long double)v[1] * v[1] + (long double)v[2] * v[2]), e = 0;
		glm_vec3_normalize_to(v, c);
		for (k = 0; k < 3; k++)
			e = fmaxl(e, fabsl(c[k] - v[k] / n));
		sum += e / FLT_EPSILON;
	}
	printf("glm_vec3_normalize: mean error %.3Lf epsilons\n", sum / 200000);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
glm_vec3_normalize: mean error 0.289 epsilons
```

Expected: at most the error of a division by the length; the hypatia program below, which
rounds each component once, prints a mean error of 0.157 epsilons.

## Cause

[`vec3.h` lines 670-681](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec3.h#L670-L681) (`glm_vec3_normalize_to`, as `glm_vec3_normalize`): `glm_vec3_scale(v, 1.0f / norm, dest)` rounds
the reciprocal and then each product.  The other rows each have the cause named in the
last column, the converse of hypatia's approach.

## Suggested fix

Divide by the norm instead of multiplying by its reciprocal in the normalization, and
see the last column for the other rows.

## How hypatia does it

See the last column of the table.  The harness (`compare/` on the hypatia branch
correctness-reports) measures every row; `docs/comparison.md` describes the method.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	long double sum = 0;
	int i, k;
	for (i = 0; i < 200000; i++) {
		struct vector3 v, h;
		long double n, e = 0;
		vector3_setf3(&v, (float)(20 * rnd() - 10), (float)(20 * rnd() - 10), (float)(20 * rnd() - 10));
		vector3_normalize(vector3_set(&h, &v));
		n = sqrtl((long double)v.x * v.x + (long double)v.y * v.y + (long double)v.z * v.z);
		for (k = 0; k < 3; k++)
			e = fmaxl(e, fabsl(h.v[k] - v.v[k] / n));
		sum += e / FLT_EPSILON;
	}
	printf("vector3_normalize: mean error %.3Lf epsilons\n", sum / 200000);
	return 0;
}
```

```text
vector3_normalize: mean error 0.157 epsilons
```

## Checking

`compare/check_reports.py docs/reports/cglm/12-precision-small-differences.md` builds both
programs above and compares their output with this report.  Every row comes from
`compare/results/precision/summary.md` on the hypatia branch correctness-reports;
`compare/reproduce.sh` regenerates it.
