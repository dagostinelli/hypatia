# `Quaternion::FromTwoVectors` is accurate to only sqrt(epsilon) for opposite vectors

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `Quaternion::FromTwoVectors`, `setFromTwoVectors` (`Geometry/Quaternion.h`) |
| Kind | precision |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

For nearly opposite vectors (`c < -1 + dummy_precision`) `setFromTwoVectors` finds the
axis with an SVD and takes `w = sqrt((1 + c) / 2)`.  For vectors that are exactly opposite
before normalization, rounding can make `c` slightly greater than -1 (here
-1 + 5.6e-16), so `w = 1.67e-8` instead of 0 and the rotation misses by 3.3e-8 rad: 1.5e8
double epsilons.  In float, where `dummy_precision` is 1e-5, every pair within 4.5e-3 rad
of opposite takes this branch; at 1e-3 rad from opposite the comparison harness measures a
mean error of 1.66e4 float epsilons.

## Reproduction

```cpp
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

int main()
{
	Eigen::Vector3d a(-0.87163762276602796, -0.55513069106039159, 0.27296314354658269);
	Eigen::Vector3d b = -2 * a;
	Eigen::Quaterniond q = Eigen::Quaterniond::FromTwoVectors(a, b);
	std::printf("c = b.normalized().dot(a.normalized()) = %.17g\n", b.normalized().dot(a.normalized()));
	std::printf("q.w = %.3g (exact: 0)\n", q.w());
	std::printf("landing error |q a - b| / |b| = %.3g = %.3g epsilons\n",
	            (q * a.normalized() - b.normalized()).norm(), (q * a.normalized() - b.normalized()).norm() / DBL_EPSILON);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
c = b.normalized().dot(a.normalized()) = -0.99999999999999944
q.w = 1.67e-08 (exact: 0)
landing error |q a - b| / |b| = 3.33e-08 = 1.5e+08 epsilons
```

Expected: w = 0 to within an epsilon, and a landing error of a few epsilons.

## Cause

[`Geometry/Quaternion.h` lines 653-664](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L653-L664):

```
if (c < Scalar(-1)+NumTraits<Scalar>::dummy_precision())
{
  ...
  Scalar w2 = (Scalar(1)+c)*Scalar(0.5);
  this->w() = sqrt(w2);
```

The axis from the SVD is accurate, but `1 + c` near zero is a rounding error of `c`, and
its square root is sqrt(epsilon) in size.

## Suggested fix

Take the half angle from `(v0 + v1).norm() / 2` (cosine) and `(v0 - v1).norm() / 2` (sine)
instead of `sqrt((1 + c) / 2)`; keep the SVD axis for the opposite branch, or use
`v0.cross(v0 + v1)` as the axis whenever it is not exactly zero.

## How hypatia does it

`quaternion_get_rotation_tov3` normalizes both vectors and takes the half angle from two
lengths that stay accurate for any angle: for unit vectors at angle a,
|f + t| = 2 cos(a/2) and |f - t| = 2 sin(a/2).  The axis is f x (f + t), which has the
direction of f x t but stays accurate as the vectors become opposite.  There is no
threshold: only an exactly zero axis (vectors exactly parallel or opposite after rounding)
is a special case.  The result is normalized, so it is always a unit quaternion.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 a;
	struct vector3 b;
	struct vector3 r;
	struct quaternion q;

	vector3_setf3(&a, -0.87163762276602796, -0.55513069106039159, 0.27296314354658269);
	vector3_setf3(&b, -2 * a.x, -2 * a.y, -2 * a.z);
	quaternion_get_rotation_tov3(&a, &b, &q);
	vector3_rotate_by_quaternion(vector3_normalize(vector3_set(&r, &a)), &q);
	vector3_subtract(&r, vector3_normalize(&b));
	printf("q.w = %.3g\n", q.w);
	printf("landing error = %.3g = %.3g epsilons\n", vector3_magnitude(&r), vector3_magnitude(&r) / DBL_EPSILON);
	return 0;
}
```

```text
q.w = 0
landing error = 0 = 0 epsilons
```

## Checking

`compare/check_reports.py docs/reports/eigen/01-fromtwovectors-opposite.md` builds both
programs above and compares their output with this report.  The harness:
`compare/results/double.md`, "accuracy against long double", `Eigen FromTwoVectors`,
"exactly opposite" (2.98e-8; hypatia 2.4e-16), and
`compare/results/precision/now.single.md`, "1e-3 rad from opposite" (Eigen 1.66e4 epsilons
in the mean, hypatia 0.35).
