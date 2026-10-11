# `HMM_RotateV3Q` does not rotate when q is not unit length

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_RotateV3Q`, `HMM_RotateV3AxisAngle_RH`, `HMM_RotateV3AxisAngle_LH` |
| Kind | wrong result for non-unit q |
| Precision | float |
| Status | present at master 661fef0 (2026-03-17); not in a release: `HMM_RotateV3Q` was added after v2.0.0 |

## Summary

`HMM_RotateV3Q` uses `v + w t + q.xyz x t` with `t = 2 q.xyz x v`, the expansion of
`q v q*` for |q| = 1.  For any other length the result is not a rotation: twice the
quarter turn about Z takes x to (-3, 4, 0).  `HMM_QToM4` normalizes its quaternion first
and gives (0, 1, 0) for the same q, so the two functions disagree.  Quaternions that are
composed repeatedly drift from unit length; at a drift of 1e-3 the rotated vector is
2e-3 longer than the input.

## Reproduction

```c
#include <HandmadeMath.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	float h = sqrtf(0.5f);
	HMM_Quat twice = HMM_Q(0, 0, 2 * h, 2 * h), drifted = HMM_Q(0, 0, 1.001f * h, 1.001f * h);
	HMM_Vec3 x = HMM_V3(1, 0, 0), r = HMM_RotateV3Q(x, twice);
	HMM_Vec4 m = HMM_MulM4V4(HMM_QToM4(twice), HMM_V4(1, 0, 0, 0));

	printf("HMM_RotateV3Q(x, 2q)  = (%g, %g, %g)\n", r.X, r.Y, r.Z);
	printf("HMM_QToM4(2q) * x     = (%g, %g, %g)\n", m.X, m.Y, m.Z);
	r = HMM_RotateV3Q(x, drifted);
	printf("HMM_RotateV3Q(x, 1.001 q): length %.7g\n", HMM_LenV3(r));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
HMM_RotateV3Q(x, 2q)  = (-3, 4, 0)
HMM_QToM4(2q) * x     = (-1.19209e-07, 1, 0)
HMM_RotateV3Q(x, 1.001 q): length 1.002003
```

Expected: (0, 1, 0) for 2q, as `HMM_QToM4` gives, and length 1 for the drifted q.

## Cause

[`HandmadeMath.h` lines 2637-2643](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L2637-L2643):
`HMM_Vec3 t = HMM_MulV3F(HMM_Cross(Q.XYZ, V), 2); return HMM_AddV3(V, HMM_AddV3(HMM_MulV3F(t, Q.W), HMM_Cross(Q.XYZ, t)));`.
This is `q v q*` expanded with |q| = 1 substituted.

## Suggested fix

Divide by `HMM_DotQ(q, q)`: with u the vector part,
`(2 (u . v) u + (w*w - u . u) v + 2 w (u x v)) / dot(q, q)`, one division, a rotation for
any non-zero q.  If unit length is a requirement, document it on `HMM_RotateV3Q`.

## How hypatia does it

`vector3_rotate_by_quaternion` evaluates `(2 (u . v) u + (w^2 - u . u) v + 2 w (u x v)) /
|q|^2`, the rotation by `q / |q|` for any length of q; when |q|^2 is not between 1e-30 and
1e30 it normalizes q first.  It agrees with `matrix4_set_from_quaternion`.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float h = sqrtf(0.5f);
	struct quaternion twice, drifted;
	struct vector3 r, x;
	struct matrix4 m;

	quaternion_setf4(&twice, 0, 0, 2 * h, 2 * h);
	quaternion_setf4(&drifted, 0, 0, 1.001f * h, 1.001f * h);
	vector3_rotate_by_quaternion(vector3_setf3(&r, 1, 0, 0), &twice);
	printf("vector3_rotate_by_quaternion(x, 2q) = (%g, %g, %g)\n", r.x, r.y, r.z);
	matrix4_multiplyv3(matrix4_set_from_quaternion(&m, &twice), vector3_setf3(&x, 1, 0, 0), &r);
	printf("matrix4_set_from_quaternion(2q) * x = (%g, %g, %g)\n", r.x, r.y, r.z);
	vector3_rotate_by_quaternion(vector3_setf3(&r, 1, 0, 0), &drifted);
	printf("vector3_rotate_by_quaternion(x, 1.001 q): length %.7g\n", vector3_magnitude(&r));
	return 0;
}
```

```text
vector3_rotate_by_quaternion(x, 2q) = (0, 1, 0)
matrix4_set_from_quaternion(2q) * x = (0, 1, 0)
vector3_rotate_by_quaternion(x, 1.001 q): length 1
```

## Checking

`compare/check_reports.py docs/reports/handmademath/04-rotatev3q-not-unit.md` builds both
programs above and compares their output with this report.
