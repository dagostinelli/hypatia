# `Quaternion::FromTwoVectors` with a zero vector returns a quaternion of length 0.71

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `Quaternion::FromTwoVectors`, `setFromTwoVectors` (`Geometry/Quaternion.h`) |
| Kind | wrong result on degenerate input |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

If either vector is zero, `normalized()` returns it unchanged, `c = 0`, and the general
formula returns `w = sqrt(2)/2` with a zero vector part: a quaternion of length 0.71 that
is not a rotation.  A zero direction has no rotation to it; the identity is the defined
answer.

## Reproduction

```cpp
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

int main()
{
	Eigen::Quaterniond q = Eigen::Quaterniond::FromTwoVectors(Eigen::Vector3d(1, 2, 3), Eigen::Vector3d::Zero());
	std::printf("FromTwoVectors(a, 0) = (w %g, x %g, y %g, z %g), norm %g\n", q.w(), q.x(), q.y(), q.z(), q.norm());
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
FromTwoVectors(a, 0) = (w 0.707107, x 0, y 0, z 0), norm 0.707107
```

Expected: the identity (w 1), or a documented error.

## Cause

[`Geometry/Quaternion.h` lines 641-669](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L641-L669): the zero
vector survives `normalized()` and the formula divides a zero cross product by
`sqrt(2)`.

## Suggested fix

Return the identity when `a.squaredNorm()` or `b.squaredNorm()` is zero.

## How hypatia does it

`quaternion_get_rotation_tov3` returns the identity when either vector has zero length.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 a;
	struct quaternion q;

	vector3_setf3(&a, 1, 2, 3);
	quaternion_get_rotation_tov3(&a, HYP_VECTOR3_ZERO, &q);
	printf("get_rotation_tov3(a, 0) = (w %g, x %g, y %g, z %g), length %g\n", q.w, q.x, q.y, q.z, quaternion_magnitude(&q));
	return 0;
}
```

```text
get_rotation_tov3(a, 0) = (w 1, x 0, y 0, z 0), length 1
```

## Checking

`compare/check_reports.py docs/reports/eigen/06-fromtwovectors-zero-vector.md` builds both
programs above and compares their output with this report.
