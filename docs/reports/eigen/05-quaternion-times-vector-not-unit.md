# `q * v` does not rotate when q is not exactly unit length

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `QuaternionBase::operator*(Vector3)`, `_transformVector` (`Geometry/Quaternion.h`) |
| Kind | wrong result for non-unit q; precision for drifted q |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

`q * v` uses `v + w uv + vec x uv` with `uv = 2 vec x v`, the expansion of `q v q*` for
|q| = 1.  For any other length the result is not a rotation: twice the quarter turn about Z
takes X to (-3, 4, 0).  Quaternions composed repeatedly drift from unit length; at a drift
of 1e-6 the error is 2e-6 relative (6e9 double epsilons in the mean over random inputs).
`toRotationMatrix()` has the same assumption and also takes X to (-3, 4, 0).

## Reproduction

```cpp
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

int main()
{
	Eigen::Quaterniond quarter(Eigen::AngleAxisd(M_PI / 2, Eigen::Vector3d::UnitZ()));
	Eigen::Quaterniond twice(2 * quarter.coeffs());
	Eigen::Vector3d r = twice * Eigen::Vector3d::UnitX();
	std::printf("2q * X = (%g, %g, %g)\n", r.x(), r.y(), r.z());

	Eigen::Quaterniond drifted((1 + 1e-6) * quarter.coeffs());
	Eigen::Vector3d d = drifted * Eigen::Vector3d::UnitX();
	std::printf("q (length 1 + 1e-6) * X = (%.17g, %.17g, %g), length %.17g\n", d.x(), d.y(), d.z(), d.norm());
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
2q * X = (-3, 4, 0)
q (length 1 + 1e-6) * X = (-2.0000009997023227e-06, 1.0000020000009999, 0), length 1.0000020000029999
```

Expected: (0, 1, 0) in both cases.

## Cause

[`Geometry/Quaternion.h` lines 531-541](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L531-L541):

```
Vector3 uv = this->vec().cross(v);
uv += uv;
return v + this->w() * uv + this->vec().cross(uv);
```

## How hypatia does it

`vector3_rotate_by_quaternion` evaluates `(2 (u . v) u + (w^2 - u . u) v + 2 w (u x v)) /
|q|^2`: the rotation by `q / |q|` for any length, at the cost of one division, with no
separate normalization.  The zero quaternion leaves the vector unchanged.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion quarter;
	struct quaternion q;
	struct vector3 v;

	quaternion_set_from_axis_anglev3(&quarter, HYP_VECTOR3_UNIT_Z, HYP_PI / 2);
	quaternion_multiplyf(quaternion_set(&q, &quarter), 2);
	vector3_rotate_by_quaternion(vector3_setf3(&v, 1, 0, 0), &q);
	printf("2q * X = (%g, %g, %g)\n", v.x, v.y, v.z);
	quaternion_multiplyf(quaternion_set(&q, &quarter), 1 + 1e-6);
	vector3_rotate_by_quaternion(vector3_setf3(&v, 1, 0, 0), &q);
	printf("q (length 1 + 1e-6) * X = (%.17g, %.17g, %g), length %.17g\n", v.x, v.y, v.z, vector3_magnitude(&v));
	return 0;
}
```

```text
2q * X = (2.22045e-16, 1, 0)
q (length 1 + 1e-6) * X = (1.1102208041824381e-16, 1, 0), length 1
```

## Suggested fix

Divide by `squaredNorm()`: `(2 vec.dot(v) vec + (w*w - vec.squaredNorm()) v + 2 w vec.cross(v))
/ squaredNorm()`.  If unit length is a requirement of `operator*`, state it in its
documentation and assert it in debug builds.

## Checking

`compare/check_reports.py docs/reports/eigen/05-quaternion-times-vector-not-unit.md` builds both programs above and compares their output with this report. The harness: `results/precision/now.double.md`, `vector3_rotate_by_quaternion`, "q of
length 1 +- 1e-6" (Eigen 6.05e9 epsilons in the mean, hypatia 0.66) and "unit q" (Eigen
0.684, hypatia 0.622).
