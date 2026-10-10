# `AngleAxis` with an axis that is not unit length gives a scaled quaternion and a non-rotation matrix

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `AngleAxis` (`Geometry/AngleAxis.h`), `Quaternion::operator=(AngleAxis)` (`Geometry/Quaternion.h`) |
| Kind | documented precondition, not checked |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

Eigen documents that the axis of an `AngleAxis` must be normalized, and does not check it.
With the axis (0, 0, 10), the quaternion has length 7.1 and turns X into (-99, 10, 0); the
matrix from `toRotationMatrix()` turns X into (0, 10, 0); with a zero axis the matrix is not
a rotation and the quaternion has length 0.71.  An axis that is almost unit length
(computed, rounded) passes silently with a proportional error.  GLM's `rotate` normalizes
the axis; hypatia's axis-angle functions do too.

## Reproduction

```cpp
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

int main()
{
	Eigen::AngleAxisd aa(M_PI / 2, Eigen::Vector3d(0, 0, 10));
	Eigen::Quaterniond q(aa);
	Eigen::Vector3d r = q * Eigen::Vector3d::UnitX();
	Eigen::Vector3d m = aa.toRotationMatrix() * Eigen::Vector3d::UnitX();
	std::printf("quaternion: (%.3g, %.3g, %.3g), norm %.3g\n", r.x(), r.y(), r.z(), q.norm());
	std::printf("matrix:     (%.3g, %.3g, %.3g)\n", m.x(), m.y(), m.z());

	Eigen::AngleAxisd zero(M_PI / 2, Eigen::Vector3d::Zero());
	Eigen::Vector3d z = zero.toRotationMatrix() * Eigen::Vector3d::UnitX();
	std::printf("zero axis:  matrix * X = (%.3g, %.3g, %.3g), quaternion norm %.3g\n", z.x(), z.y(), z.z(), Eigen::Quaterniond(zero).norm());
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
quaternion: (-99, 10, 0), norm 7.11
matrix:     (6.12e-17, 10, 0)
zero axis:  matrix * X = (6.12e-17, 0, 0), quaternion norm 0.707
```

Expected: (0, 1, 0) from both, and the identity for the zero axis; or an assertion in debug builds.

## Cause

[`Geometry/AngleAxis.h` lines 218-243](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/AngleAxis.h#L218-L243) and
[`Geometry/Quaternion.h` lines 561-569](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L561-L569) use
`m_axis` as given.  The class documentation warns: "the axis vector must be normalized".

## How hypatia does it

`quaternion_set_from_axis_anglev3` and `matrix4_set_from_axisv3_angle` normalize the axis
(with scaling, so no overflow); any non-zero length gives the same rotation, and the zero
axis gives the identity.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 axis;
	struct vector3 v;
	struct quaternion q;
	struct matrix4 m;

	vector3_setf3(&axis, 0, 0, 10);
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI / 2);
	vector3_rotate_by_quaternion(vector3_setf3(&v, 1, 0, 0), &q);
	printf("quaternion: (%.3g, %.3g, %.3g), norm %.3g\n", v.x, v.y, v.z, quaternion_magnitude(&q));
	matrix4_set_from_axisv3_angle(&m, &axis, HYP_PI / 2);
	matrix4_multiplyv3(&m, HYP_VECTOR3_UNIT_X, &v);
	printf("matrix:     (%.3g, %.3g, %.3g)\n", v.x, v.y, v.z);
	matrix4_set_from_axisv3_angle(&m, HYP_VECTOR3_ZERO, HYP_PI / 2);
	matrix4_multiplyv3(&m, HYP_VECTOR3_UNIT_X, &v);
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_ZERO, HYP_PI / 2);
	printf("zero axis:  matrix * X = (%.3g, %.3g, %.3g), quaternion norm %.3g\n", v.x, v.y, v.z, quaternion_magnitude(&q));
	return 0;
}
```

```text
quaternion: (2.22e-16, 1, 0), norm 1
matrix:     (6.12e-17, 1, 0)
zero axis:  matrix * X = (1, 0, 0), quaternion norm 1
```

## Suggested fix

Normalize the axis where it is used (one division per conversion), or at least
`eigen_assert(abs(m_axis.squaredNorm() - 1) < some tolerance)` in the conversions.

## Checking

`compare/check_reports.py docs/reports/eigen/07-angleaxis-axis-not-unit.md` builds both programs above and compares their output with this report.
