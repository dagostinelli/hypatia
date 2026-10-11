# `HMM_QFromVecPair` is NaN for opposite vectors and loses most digits for nearly opposite ones

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_QFromVecPair`, `HMM_QFromNormPair` |
| Kind | NaN on valid input; precision |
| Precision | float |
| Status | present at master 661fef0 (2026-03-17); not in a release: `HMM_QFromVecPair` was added after v2.0.0 |

## Summary

`HMM_QFromNormPair(a, b)` builds (a x b, 1 + a . b) and normalizes it.  For opposite
vectors that is (0, 0, 0, 0), and normalizing it gives NaN, although a half turn about any
axis perpendicular to a is a valid answer.  For nearly opposite vectors `1 + a . b`
cancels: from x to (-1, 1e-3, 0) the w component comes out as 0.000477 instead of 0.0005,
and the quaternion takes x to a direction 4.6e-5 away from the target, about 390 float
epsilons.

## Reproduction

```c
#include <HandmadeMath.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	HMM_Vec3 x = HMM_V3(1, 0, 0), to = HMM_NormV3(HMM_V3(-1, 1e-3f, 0)), r;
	HMM_Quat q = HMM_QFromVecPair(x, HMM_V3(-1, 0, 0));

	printf("x to -x: (%g, %g, %g, %g)\n", q.X, q.Y, q.Z, q.W);
	q = HMM_QFromVecPair(x, to);
	r = HMM_RotateV3Q(x, q);
	printf("x to (-1, 1e-3, 0): (%g, %g, %g, %.6g), takes x to (%.7g, %.7g, %g), %.2g from the target\n",
	       q.X, q.Y, q.Z, q.W, r.X, r.Y, r.Z, HMM_LenV3(HMM_SubV3(r, to)));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
x to -x: (-nan, -nan, -nan, -nan)
x to (-1, 1e-3, 0): (0, -0, 1, 0.000476837), takes x to (-0.9999995, 0.0009536745, 0), 4.6e-05 from the target
```

Expected: a half turn about an axis perpendicular to x, such as (0, 0, 1, 0), for the first;
w = 0.0005 for the second (half the angle between the vectors is pi/2 - 0.0005).

## Cause

[`HandmadeMath.h` lines 2603-2621](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L2603-L2621):
`Result.XYZ = HMM_Cross(Left, Right); Result.W = 1.0f + HMM_DotV3(Left, Right);`, then
`HMM_NormQ` ([01](01-normalize-zero-tiny-huge.md)).

## Suggested fix

Compute w from `|a - b|` and `|a + b|`, which have no cancellation: with unit a and b,
the half angle is `atan2(|a - b|, |a + b|)`.  For `|a + b|` zero (opposite vectors), return
a half turn about a perpendicular axis.

## How hypatia does it

`quaternion_get_rotation_tov3` takes the half angle from `atan2(|f - t|, |f + t|)` of the
normalized vectors, and for opposite vectors returns a half turn about an axis
perpendicular to the first.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 x, to, r;
	struct quaternion q;

	vector3_setf3(&x, 1, 0, 0);
	quaternion_get_rotation_tov3(&x, vector3_setf3(&to, -1, 0, 0), &q);
	printf("x to -x: (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	vector3_normalize(vector3_setf3(&to, -1, 1e-3f, 0));
	quaternion_get_rotation_tov3(&x, &to, &q);
	vector3_rotate_by_quaternion(vector3_set(&r, &x), &q);
	printf("x to (-1, 1e-3, 0): (%g, %g, %g, %.6g), takes x to (%.7g, %.7g, %g), %.2g from the target\n",
	       q.x, q.y, q.z, q.w, r.x, r.y, r.z, vector3_distance(&r, &to));
	return 0;
}
```

```text
x to -x: (0, 0, 1, 0)
x to (-1, 1e-3, 0): (0, 0, 1, 0.0005), takes x to (-0.9999995, 0.0009999996, 0), 0 from the target
```

## Checking

`compare/check_reports.py docs/reports/handmademath/03-qfromvecpair-opposite.md` builds both
programs above and compares their output with this report.
