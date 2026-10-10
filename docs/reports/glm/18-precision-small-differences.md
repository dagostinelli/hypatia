# Smaller precision differences: GLM has 3% to 21% more rounding error in thirteen measurements

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `q * v`, `lookAt`, `unProject`, `refract`, `inverse` (mat3, mat4), `slerp`, `mat4_cast`, `quat * quat` |
| Kind | precision |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

Apart from the larger cases reported separately, the comparison harness measures GLM with
more rounding error than hypatia, on the same inputs and against a long double reference,
in these functions (mean error in epsilons over 20000 inputs, double; each holds with a
second random seed):

| GLM | hypatia | GLM | ratio | hypatia's approach |
|---|---|---|---|---|
| `glm::unProject` | `vector3_unproject_from_window` | 2.17 against 1.86 | 1.17 | the matrix inverse by 2x2 blocks |
| `glm::lookAtRH` | `matrix4_view_lookat_rh` | 0.450 against 0.407 | 1.11 | the normalization of `hyp_normalize` |
| `q * v` (unit q) | `vector3_rotate_by_quaternion` | 0.686 against 0.622 | 1.10 | `(2 (u.v) u + (w^2 - u.u) v + 2 w (u x v)) / \|q\|^2` |
| `glm::slerp`, 1e-6 rad apart | `quaternion_slerp` | 0.538 against 0.491 | 1.10 | the angle from `2 atan2(\|s - t\|, \|s + t\|)` |
| `glm::refract` | `vector3_refract` | 0.413 against 0.384 | 1.08 | the normalization of `hyp_normalize` |
| `glm::inverse` (mat4, rotation, scale and translation) | `matrix4_inverse` | 0.0115 against 0.0108 (per unit of condition) | 1.06 | 2x2 blocks, determinant shared with the cofactors |
| `glm::inverse` (mat4, condition 1e4) | `matrix4_inverse` | 1.01 against 0.963 | 1.05 | 2x2 blocks, determinant shared with the cofactors |
| `glm::inverse` (mat3) | `matrix3_inverse` | 0.0784 against 0.0749 (per unit of condition) | 1.05 | division by the determinant instead of multiplication by its reciprocal |
| `glm::inverse` (mat4, random) | `matrix4_inverse` | 0.0641 against 0.0623 (per unit of condition) | 1.03 | 2x2 blocks, determinant shared with the cofactors |
| `glm::slerp`, random | `quaternion_slerp` | 0.487 against 0.468 | 1.04 | the angle from `2 atan2(\|s - t\|, \|s + t\|)` |
| `quat * quat` | `quaternion_multiply` | 0.298 against 0.289 | 1.03 | the four products summed in pairs |
| `glm::mat4_cast` | `matrix4_set_from_quaternion` | 0.598 against 0.581 | 1.03 | `2 / dot(q, q)` once, for any length of q |
| `glm::slerp`, 1e-3 rad apart (float) | `quaternion_slerp` | 0.575 against 0.474 | 1.21 | the angle from `2 atan2(\|s - t\|, \|s + t\|)` |

These are fractions of an epsilon and rarely matter alone; they add up in long chains of
transforms.  The program below measures the third row.

## Reproduction

```cpp
// q * v for unit quaternions: mean error against long double, in epsilons
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cfloat>
#include <cmath>
#include <cstdio>

static unsigned long long state = 88172645463325252ULL;
static double rnd() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main()
{
	long double sum = 0;
	for (int i = 0; i < 100000; i++) {
		long double q[4], n = 0;
		for (int k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
		glm::dquat g((double)(q[3] / sqrtl(n)), (double)(q[0] / sqrtl(n)), (double)(q[1] / sqrtl(n)), (double)(q[2] / sqrtl(n)));
		glm::dvec3 v(20 * rnd() - 10, 20 * rnd() - 10, 20 * rnd() - 10);
		glm::dvec3 r = g * v;
		/* the rotation by the rounded q, in long double */
		long double w = g.w, u[3] = {g.x, g.y, g.z}, m = w * w + u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
		long double c[3] = {u[1] * v.z - u[2] * v.y, u[2] * v.x - u[0] * v.z, u[0] * v.y - u[1] * v.x};
		long double d = u[0] * v.x + u[1] * v.y + u[2] * v.z, big = 0, e = 0;
		for (int k = 0; k < 3; k++) {
			long double o = (2 * d * u[k] + (w * w - (m - w * w)) * v[k] + 2 * w * c[k]) / m;
			big = fmaxl(big, fabsl(o));
			e = fmaxl(e, fabsl(r[k] - o));
		}
		sum += e / big / DBL_EPSILON;
	}
	std::printf("glm q * v: mean error %.3Lf epsilons\n", sum / 100000);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
glm q * v: mean error 0.684 epsilons
```

Expected: as small as the inputs allow; the hypatia program below prints a mean error of
0.621 epsilons.

## Cause

Each row has its own small cause.  For `q * v`
([`detail/type_quat.inl` lines 359-366](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/type_quat.inl#L359-L366))
the formula assumes `|q| = 1` exactly, while a unit quaternion rounded to double is off by
about an epsilon, and the nested cross product `u x (u x v)` rounds twice; hypatia uses
two dot products and one cross product, and divides by `dot(q, q)`.

## How hypatia does it

See the last column of the table.  The harness (`compare/` on the hypatia branch
correctness-exp-glm) measures every row; `docs/comparison.md` describes the method.

```c
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
	for (i = 0; i < 100000; i++) {
		long double q[4], n = 0, w, u[3], m, c[3], d, big = 0, e = 0;
		struct quaternion h;
		struct vector3 v, r;
		for (k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
		quaternion_setf4(&h, (double)(q[0] / sqrtl(n)), (double)(q[1] / sqrtl(n)), (double)(q[2] / sqrtl(n)), (double)(q[3] / sqrtl(n)));
		vector3_setf3(&v, 20 * rnd() - 10, 20 * rnd() - 10, 20 * rnd() - 10);
		vector3_rotate_by_quaternion(vector3_set(&r, &v), &h);
		w = h.w; u[0] = h.x; u[1] = h.y; u[2] = h.z;
		m = w * w + u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
		c[0] = u[1] * v.z - u[2] * v.y; c[1] = u[2] * v.x - u[0] * v.z; c[2] = u[0] * v.y - u[1] * v.x;
		d = u[0] * v.x + u[1] * v.y + u[2] * v.z;
		for (k = 0; k < 3; k++) {
			long double o = (2 * d * u[k] + (w * w - (m - w * w)) * v.v[k] + 2 * w * c[k]) / m;
			big = fmaxl(big, fabsl(o));
			e = fmaxl(e, fabsl(r.v[k] - o));
		}
		sum += e / big / DBL_EPSILON;
	}
	printf("vector3_rotate_by_quaternion: mean error %.3Lf epsilons\n", sum / 100000);
	return 0;
}
```

```text
vector3_rotate_by_quaternion: mean error 0.621 epsilons
```

## Suggested fix

Each row's last column describes a change that removes the difference; none costs more
than a division or two additions.

## Checking

`compare/check_reports.py docs/reports/glm/18-precision-small-differences.md` builds both programs above and compares their output with this report. Every row comes from `results/precision/summary.md` on the hypatia branch
correctness-exp-glm; `compare/reproduce.sh` regenerates it.
