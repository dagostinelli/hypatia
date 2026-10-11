# Smaller precision differences: Eigen has from 2% more to 7 times the rounding error of hypatia in eighteen measurements

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `inverse` (3x3, 4x4), `normalized`, `determinant`, `angularDistance`, `slerp`, `Quaternion(matrix)`, `q * v`, `toRotationMatrix`, `AngleAxis(Quaternion)` |
| Kind | precision |
| Precision | double |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

Apart from the cases reported separately, the comparison harness measures Eigen with more
rounding error than hypatia, on the same inputs and against a long double reference, in
these functions (mean error in epsilons over 20000 inputs, double; each holds
with a second random seed):

| Eigen | hypatia | Eigen | ratio | hypatia's approach |
|---|---|---|---|---|
| `inverse()` 4x4, condition ~1e4 | `matrix4_inverse` | 0.956 against 0.137 (per unit of condition) | 6.98 | 2x2 minors rounded once, determinant shared with the cofactors, division by it |
| `normalized()`, one large, two small (1, 1e-5, 1e-5) | `vector3_normalize` | 0.248 against 0.126 | 1.97 | scaled by a power of two, then each component rounded once (exact products) |
| `normalized()`, components 1e-20 .. 1e20 (single 1e-15 .. 1e15) | `vector3_normalize` | 0.124 against 0.065 | 1.91 | scaled by a power of two, then each component rounded once (exact products) |
| `normalized()`, random | `vector3_normalize` | 0.321 against 0.203 | 1.58 | scaled by a power of two, then each component rounded once (exact products) |
| `Quaternion::normalized()`, random length | `quaternion_normalize` | 0.28 against 0.199 | 1.41 | scaled by a power of two, then each component rounded once (exact products) |
| `inverse()` 3x3, random entries | `matrix3_inverse` | 0.0787 against 0.0632 (per unit of condition) | 1.25 | 2x2 minors rounded once, division by the determinant |
| `determinant()` 4x4, random entries | `matrix4_determinant` | 0.258 against 0.214 | 1.21 | 2x2 minors rounded once, determinant from the blocks |
| `angularDistance`, random | `quaternion_angle_between` | 0.257 against 0.221 | 1.16 | `4 atan2(\| \|b\| a - \|a\| b \|, \| \|b\| a + \|a\| b \|)` with exact products, no normalization |
| `inverse()` 4x4, random entries | `matrix4_inverse` | 0.0637 against 0.0552 (per unit of condition) | 1.15 | 2x2 minors rounded once, determinant shared with the cofactors, division by it |
| `slerp`, 1e-6 rad apart | `quaternion_slerp` | 0.502 against 0.437 | 1.15 | the angle from `2 atan2(\| \|t\| s - \|s\| t \|, \| \|t\| s + \|s\| t \|)` with exact products, the two terms combined with one rounding |
| `slerp`, random | `quaternion_slerp` | 0.457 against 0.399 | 1.15 | the angle from `2 atan2(\| \|t\| s - \|s\| t \|, \| \|t\| s + \|s\| t \|)` with exact products, the two terms combined with one rounding |
| `inverse()` 4x4, rotation, scale and translation | `matrix4_inverse` | 0.0114 against 0.00997 (per unit of condition) | 1.14 | 2x2 minors rounded once, determinant shared with the cofactors, division by it |
| `Quaternion(matrix)`, near half turns (pi - 1e-3) | `quaternion_set_from_matrix4` | 0.303 against 0.271 | 1.12 | Shepperd's method, the result normalized by `hyp_normalize` |
| `Quaternion(matrix)`, random rotation | `quaternion_set_from_matrix4` | 0.329 against 0.296 | 1.11 | Shepperd's method, the result normalized by `hyp_normalize` |
| `q * v`, unit q | `vector3_rotate_by_quaternion` | 0.684 against 0.622 | 1.10 | `(2 (u.v) u + (w^2 - u.u) v + 2 w (u x v)) / \|q\|^2`, one division |
| `slerp`, 1e-3 rad apart | `quaternion_slerp` | 0.496 against 0.462 | 1.07 | the angle from `2 atan2(\| \|t\| s - \|s\| t \|, \| \|t\| s + \|s\| t \|)` with exact products, the two terms combined with one rounding |
| `toRotationMatrix()`, unit q | `matrix4_set_from_quaternion` | 0.598 against 0.581 | 1.03 | `2 / dot(q, q)` once, for any length of q |
| `AngleAxis(q)`, random | `quaternion_get_axis_anglev3` | 0.383 against 0.374 | 1.02 | the vector part divided by the length that gives the angle `2 atan2(\|v\|, w)` |

Each difference is a fraction of an epsilon.  The program below measures the `toRotationMatrix()` row on
its own inputs (0.5942 against 0.5794).

## Reproduction

```cpp
// toRotationMatrix of unit quaternions: mean error against long double, in epsilons
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

static unsigned long long state = 88172645463325252ULL;
static double rnd() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

/* the rotation matrix of the rounded unit quaternion (x y z w), in long double; returns
 * the largest element difference from m (row-major) in epsilons */
static long double error(const double q[4], const double m[9])
{
	long double x = q[0], y = q[1], z = q[2], w = q[3], n = x * x + y * y + z * z + w * w, s = 2 / n, e = 0;
	long double r[9] = {1 - s * (y * y + z * z), s * (x * y - z * w), s * (x * z + y * w),
	                    s * (x * y + z * w), 1 - s * (x * x + z * z), s * (y * z - x * w),
	                    s * (x * z - y * w), s * (y * z + x * w), 1 - s * (x * x + y * y)};
	for (int k = 0; k < 9; k++)
		e = fmaxl(e, fabsl(m[k] - r[k]));
	return e / DBL_EPSILON;
}

int main()
{
	long double sum = 0;
	for (int i = 0; i < 200000; i++) {
		long double c[4], n = 0;
		for (int k = 0; k < 4; k++) { c[k] = 2 * rnd() - 1; n += c[k] * c[k]; }
		double q[4];
		for (int k = 0; k < 4; k++) q[k] = (double)(c[k] / sqrtl(n));
		Eigen::Matrix3d r = Eigen::Quaterniond(q[3], q[0], q[1], q[2]).toRotationMatrix();
		double m[9];
		for (int k = 0; k < 9; k++) m[k] = r(k / 3, k % 3);
		sum += error(q, m);
	}
	std::printf("toRotationMatrix: mean error %.4Lf epsilons\n", sum / 200000);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
toRotationMatrix: mean error 0.5942 epsilons
```

Expected: about the same as hypatia; the hypatia program below prints a mean error of
0.5794 epsilons.

## Cause

[`Geometry/Quaternion.h` lines 592-625](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L592-L625)
builds the matrix from `2 x`, `2 y`, `2 z` as if `|q| = 1` exactly.  A unit quaternion
rounded to double is off unit length by about an epsilon, and that deviation goes into
every element; hypatia scales by `2 / dot(q, q)`, which accounts for it.  The other rows
each have their own small cause (see the last column).

## Suggested fix

Each row's last column describes a change that removes the difference.

## How hypatia does it

See the last column of the table.  The harness (`compare/` on the hypatia branch
correctness-reports) measures every row; `docs/comparison.md` describes the method.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

/* the rotation matrix of the rounded unit quaternion (x y z w), in long double; returns
 * the largest element difference from m (row-major) in epsilons */
static long double error(const double q[4], const double m[9])
{
	long double x = q[0], y = q[1], z = q[2], w = q[3], n = x * x + y * y + z * z + w * w, s = 2 / n, e = 0;
	long double r[9] = {1 - s * (y * y + z * z), s * (x * y - z * w), s * (x * z + y * w),
	                    s * (x * y + z * w), 1 - s * (x * x + z * z), s * (y * z - x * w),
	                    s * (x * z - y * w), s * (y * z + x * w), 1 - s * (x * x + y * y)};
	int k;
	for (k = 0; k < 9; k++)
		e = fmaxl(e, fabsl(m[k] - r[k]));
	return e / DBL_EPSILON;
}

int main(void)
{
	long double sum = 0;
	int i, k;
	for (i = 0; i < 200000; i++) {
		long double c[4], n = 0;
		double q[4], m[9];
		struct quaternion h;
		struct matrix4 r;
		for (k = 0; k < 4; k++) { c[k] = 2 * rnd() - 1; n += c[k] * c[k]; }
		for (k = 0; k < 4; k++) q[k] = (double)(c[k] / sqrtl(n));
		quaternion_setf4(&h, q[0], q[1], q[2], q[3]);
		matrix4_set_from_quaternion(&r, &h);
		for (k = 0; k < 9; k++) m[k] = r.m[(k / 3) * 4 + k % 3];
		sum += error(q, m);
	}
	printf("matrix4_set_from_quaternion: mean error %.4Lf epsilons\n", sum / 200000);
	return 0;
}
```

```text
matrix4_set_from_quaternion: mean error 0.5794 epsilons
```

## Checking

`compare/check_reports.py docs/reports/eigen/08-precision-small-differences.md` builds both
programs above and compares their output with this report.  Every row comes from
`compare/results/precision/summary.md` on the hypatia branch correctness-reports;
`compare/reproduce.sh` regenerates it.
