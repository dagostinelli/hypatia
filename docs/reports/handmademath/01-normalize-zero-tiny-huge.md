# `HMM_NormV3`, `HMM_NormV4` and `HMM_NormQ` give NaN, inf or zero for zero, tiny and huge inputs

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_NormV2`, `HMM_NormV3`, `HMM_NormV4`, `HMM_NormQ`, and through them `HMM_QToM4`, `HMM_NLerp`, `HMM_SLerp` |
| Kind | NaN on degenerate input; wrong result |
| Precision | float |
| Status | still present in HandmadeMath v2.0.0 (the latest release, 2023-02-20) and at master 661fef0 (2026-03-17): the program prints the same |

## Summary

`HMM_NormV3(A)` returns `A * (1 / sqrt(dot(A, A)))`.  The zero vector gives `0 * inf`,
NaN in every component.  For components below about 1e-19 the dot product is subnormal and
loses precision; below about 2.6e-23 it is exactly 0 and the result is inf or NaN.  Above
about 1.8e19 the dot product overflows and the result is zero.  In all three cases the
direction of the input is representable.  `HMM_NormQ` calls `HMM_NormV4` and behaves the
same, and so does `HMM_QToM4`, which normalizes its quaternion first: a tiny quaternion
gives NaN, and a huge one the identity matrix instead of a quarter turn.

## Reproduction

```c
#include <HandmadeMath.h>
#include <stdio.h>

int main(void)
{
	float k[] = {0, 1e-25f, 1e25f};
	int i;

	for (i = 0; i < 3; i++) {
		HMM_Vec3 v = HMM_NormV3(HMM_V3(k[i], 2 * k[i], 2 * k[i]));
		HMM_Quat q = HMM_NormQ(HMM_Q(0, 0, k[i], k[i]));
		HMM_Mat4 m = HMM_QToM4(HMM_Q(0, 0, k[i], k[i]));
		printf("k = %g: HMM_NormV3((1, 2, 2) k) = (%g, %g, %g)\n", k[i], v.X, v.Y, v.Z);
		printf("        HMM_NormQ((0, 0, k, k)) = (%g, %g, %g, %g), HMM_QToM4: m00 = %g\n",
		       q.X, q.Y, q.Z, q.W, m.Elements[0][0]);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
k = 0: HMM_NormV3((1, 2, 2) k) = (-nan, -nan, -nan)
        HMM_NormQ((0, 0, k, k)) = (-nan, -nan, -nan, -nan), HMM_QToM4: m00 = -nan
k = 1e-25: HMM_NormV3((1, 2, 2) k) = (inf, inf, inf)
        HMM_NormQ((0, 0, k, k)) = (-nan, -nan, inf, inf), HMM_QToM4: m00 = -nan
k = 1e+25: HMM_NormV3((1, 2, 2) k) = (0, 0, 0)
        HMM_NormQ((0, 0, k, k)) = (0, 0, 0, 0), HMM_QToM4: m00 = 1
```

Expected: (1/3, 2/3, 2/3) and (0, 0, 0.707, 0.707) for the tiny and huge inputs, with
m00 near 0 (a quarter turn about Z); a defined value for the zero vector.

## Cause

[`HandmadeMath.h` lines 1067-1078](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L1067-L1078):
`return HMM_MulV3F(A, HMM_InvSqrtF(HMM_DotV3(A, A)));`, the same for `HMM_NormV4`;
[`HMM_InvSqrtF`, lines 536-545](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L536-L545), is `1.0f/HMM_SqrtF(Float)`;
[`HMM_NormQ`, lines 2328-2338](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L2328-L2338), calls `HMM_NormV4`.

## Suggested fix

Check the squared length: zero, return the input (or a documented value); outside a safe
range such as 1e-30 to 1e30, divide by the largest component first, then normalize.

## How hypatia does it

`vector3_normalize` and `quaternion_normalize` divide by the length when the squared
length is between 1e-30 and 1e30, and otherwise divide by the largest component first.
The zero vector is left unchanged.  `matrix4_set_from_quaternion` uses the same scaled
normalization when |q|^2 is outside that range, and gives the identity for the zero
quaternion.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float k[] = {0, 1e-25f, 1e25f};
	int i;

	for (i = 0; i < 3; i++) {
		struct vector3 v;
		struct quaternion q;
		struct matrix4 m;

		vector3_normalize(vector3_setf3(&v, k[i], 2 * k[i], 2 * k[i]));
		quaternion_normalize(quaternion_setf4(&q, 0, 0, k[i], k[i]));
		matrix4_set_from_quaternion(&m, quaternion_setf4(&q, 0, 0, k[i], k[i]));
		quaternion_normalize(&q);
		printf("k = %g: vector3_normalize((1, 2, 2) k) = (%g, %g, %g)\n", k[i], v.x, v.y, v.z);
		printf("        quaternion_normalize((0, 0, k, k)) = (%g, %g, %g, %g), matrix: m00 = %g\n",
		       q.x, q.y, q.z, q.w, m.m[0]);
	}
	return 0;
}
```

```text
k = 0: vector3_normalize((1, 2, 2) k) = (0, 0, 0)
        quaternion_normalize((0, 0, k, k)) = (0, 0, 0, 0), matrix: m00 = 1
k = 1e-25: vector3_normalize((1, 2, 2) k) = (0.333333, 0.666667, 0.666667)
        quaternion_normalize((0, 0, k, k)) = (0, 0, 0.707107, 0.707107), matrix: m00 = 5.96046e-08
k = 1e+25: vector3_normalize((1, 2, 2) k) = (0.333333, 0.666667, 0.666667)
        quaternion_normalize((0, 0, k, k)) = (0, 0, 0.707107, 0.707107), matrix: m00 = 5.96046e-08
```

## Checking

`compare/check_reports.py docs/reports/handmademath/01-normalize-zero-tiny-huge.md` builds
both programs above and compares their output with this report.
