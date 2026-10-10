# Smaller precision differences: Eigen has 2% to 10% more rounding error in seven measurements of six functions

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `q * v`, `toRotationMatrix`, `inverse` (3x3, 4x4), `AngleAxis(Quaternion)`, `slerp` |
| Kind | precision |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

Apart from the cases reported separately, the comparison harness measures Eigen with more
rounding error than hypatia, on the same inputs and against a long double reference, in
these functions (mean error in epsilons over 20000 inputs, double unless noted; each holds
with a second random seed):

| Eigen | hypatia | Eigen | ratio | hypatia's approach |
|---|---|---|---|---|
| `q * v` (unit q) | `vector3_rotate_by_quaternion` | 0.684 against 0.622 | 1.10 | `2 (u.v) u + (w^2 - u.u) v + 2 w (u x v)`, one division by `\|q\|^2` |
| `inverse()` 4x4, rotation, scale and translation | `matrix4_inverse` | 0.0114 against 0.0108 (per unit of condition) | 1.06 | 2x2 blocks, determinant shared with the cofactors |
| `inverse()` 3x3 | `matrix3_inverse` | 0.0787 against 0.0749 (per unit of condition) | 1.05 | division by the determinant instead of multiplication by its reciprocal |
| `toRotationMatrix()` | `matrix4_set_from_quaternion` | 0.598 against 0.581 | 1.03 | `2 / dot(q, q)` once, valid for any length of q |
| `AngleAxis(q)` (float) | `quaternion_get_axis_anglev3` | 0.396 against 0.384 | 1.03 | the axis by normalizing the vector part, the angle `2 atan2(\|v\|, w)` |
| `inverse()` 4x4, random | `matrix4_inverse` | 0.0637 against 0.0623 (per unit of condition) | 1.02 | 2x2 blocks, determinant shared with the cofactors |
| `slerp`, 1e-6 rad apart | `quaternion_slerp` | 0.502 against 0.491 | 1.02 | the angle from `2 atan2(\|s - t\|, \|s + t\|)` |

They are fractions of an epsilon, and add up only in long chains of transforms.  The
program below measures the fourth row.

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

## Suggested fix

Each row's last column describes a change that removes the difference.

## Checking

`compare/check_reports.py docs/reports/eigen/08-precision-small-differences.md` builds both programs above and compares their output with this report. Every row comes from `results/precision/summary.md` on the hypatia branch
correctness-exp-glm; `compare/reproduce.sh` regenerates it.
