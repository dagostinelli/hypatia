# `HMM_Rotate_RH` and `HMM_QFromAxisAngle_RH` give NaN for a zero axis

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_Rotate_RH`, `HMM_Rotate_LH`, `HMM_QFromAxisAngle_RH`, `HMM_QFromAxisAngle_LH` |
| Kind | NaN on degenerate input |
| Precision | float |
| Status | still present in HandmadeMath v2.0.0 (the latest release, 2023-02-20) and at master 661fef0 (2026-03-17): the program prints the same |

## Summary

Both functions normalize the axis with `HMM_NormV3`, which returns NaN for the zero vector
([01](01-normalize-zero-tiny-huge.md)).  A zero axis, for example the cross product of two
parallel vectors, gives a matrix with 9 NaN entries and a quaternion with a NaN vector
part.  An axis of length 1e-25 gives inf and NaN the same way.  A rotation about no axis
has a natural value, the identity.

## Reproduction

```c
#include <HandmadeMath.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	HMM_Vec3 axes[] = {HMM_V3(0, 0, 0), HMM_V3(0, 0, 1e-25f)};
	int i, c, r;

	for (i = 0; i < 2; i++) {
		HMM_Mat4 m = HMM_Rotate_RH(1, axes[i]);
		HMM_Quat q = HMM_QFromAxisAngle_RH(axes[i], 1);
		int nans = 0;
		for (c = 0; c < 4; c++)
			for (r = 0; r < 4; r++)
				nans += isnan(m.Elements[c][r]) != 0;
		printf("axis z = %g: HMM_Rotate_RH %d of 16 NaN; HMM_QFromAxisAngle_RH (%g, %g, %g, %g)\n",
		       axes[i].Z, nans, q.X, q.Y, q.Z, q.W);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
axis z = 0: HMM_Rotate_RH 9 of 16 NaN; HMM_QFromAxisAngle_RH (-nan, -nan, -nan, 0.877583)
axis z = 1e-25: HMM_Rotate_RH 8 of 16 NaN; HMM_QFromAxisAngle_RH (-nan, -nan, inf, 0.877583)
```

Expected: the identity for the zero axis, and the rotation by 1 rad about Z for the axis
(0, 0, 1e-25).

## Cause

[`HandmadeMath.h` lines 1945-1970](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L1945-L1970) and
[lines 2579-2592](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L2579-L2592): `Axis = HMM_NormV3(Axis);` and
`HMM_Vec3 AxisNormalized = HMM_NormV3(Axis);`.

## Suggested fix

Normalize with zero and scale checks ([01](01-normalize-zero-tiny-huge.md)) and return the
identity for a zero axis.

## How hypatia does it

`matrix4_set_from_axisv3_angle` and `quaternion_set_from_axis_anglev3` normalize the axis
with scaled normalization, which leaves the zero vector unchanged; a zero axis gives the
identity.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float z[] = {0, 1e-25f};
	int i, k;

	for (i = 0; i < 2; i++) {
		struct vector3 axis;
		struct matrix4 m;
		struct quaternion q;
		int nans = 0;

		vector3_setf3(&axis, 0, 0, z[i]);
		matrix4_set_from_axisv3_angle(&m, &axis, 1);
		quaternion_set_from_axis_anglev3(&q, &axis, 1);
		for (k = 0; k < 16; k++)
			nans += isnan(m.m[k]) != 0;
		printf("axis z = %g: matrix %d of 16 NaN, m00 %g; quaternion (%g, %g, %g, %g)\n",
		       z[i], nans, m.m[0], q.x, q.y, q.z, q.w);
	}
	return 0;
}
```

```text
axis z = 0: matrix 0 of 16 NaN, m00 1; quaternion (0, 0, 0, 1)
axis z = 1e-25: matrix 0 of 16 NaN, m00 0.540302; quaternion (0, 0, 0.479426, 0.877583)
```

## Checking

`compare/check_reports.py docs/reports/handmademath/06-zero-axis-nan.md` builds both
programs above and compares their output with this report.
