# `HMM_LookAt_RH` gives NaN when the eye is at the target or looks along up

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_LookAt_RH`, `HMM_LookAt_LH` |
| Kind | NaN on degenerate input |
| Precision | float |
| Status | still present in HandmadeMath v2.0.0 (the latest release, 2023-02-20) and at master 661fef0 (2026-03-17): the program prints the same |

## Summary

`HMM_LookAt_RH` normalizes `center - eye` and `cross(F, up)` with `HMM_NormV3`, which
returns NaN for the zero vector ([01](01-normalize-zero-tiny-huge.md)).  With the eye at
the target, 12 of the 16 entries of the view matrix are NaN; looking along up, 8.  Every
vertex transformed by such a matrix is NaN.

## Reproduction

```c
#include <HandmadeMath.h>
#include <math.h>
#include <stdio.h>

static void print(const char *what, HMM_Mat4 m)
{
	int c, r, nans = 0;
	for (c = 0; c < 4; c++)
		for (r = 0; r < 4; r++)
			nans += isnan(m.Elements[c][r]) != 0;
	printf("%s: %d of 16 entries NaN\n", what, nans);
}

int main(void)
{
	HMM_Vec3 up = HMM_V3(0, 1, 0);

	print("eye == target   ", HMM_LookAt_RH(HMM_V3(0, 0, 5), HMM_V3(0, 0, 5), up));
	print("looking along up", HMM_LookAt_RH(HMM_V3(0, 0, 0), HMM_V3(0, 5, 0), up));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
eye == target   : 12 of 16 entries NaN
looking along up: 8 of 16 entries NaN
```

Expected: a finite matrix (or an assertion): there is no view direction, or no right axis.

## Cause

[`HandmadeMath.h` lines 2041-2050](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L2041-L2050):
`HMM_Vec3 F = HMM_NormV3(HMM_SubV3(Center, Eye)); HMM_Vec3 S = HMM_NormV3(HMM_Cross(F, Up));`.

## Suggested fix

Normalize with a zero check ([01](01-normalize-zero-tiny-huge.md)), or check the two lengths
and fall back (for example, to another up axis when `cross(F, up)` vanishes).

## How hypatia does it

`matrix4_view_lookat_rh` uses `vector3_normalize`, which leaves the zero vector unchanged,
so the degenerate axes are zero rows and the matrix stays finite.  Neither library can
produce a meaningful view without a direction; hypatia's result does not spread NaN.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static void print(const char *what, const struct matrix4 *m)
{
	int i, nans = 0;
	for (i = 0; i < 16; i++)
		nans += isnan(m->m[i]) != 0;
	printf("%s: %d of 16 entries NaN\n", what, nans);
}

int main(void)
{
	struct matrix4 m;
	struct vector3 eye, target;

	vector3_setf3(&eye, 0, 0, 5);
	print("eye == target   ", matrix4_view_lookat_rh(&m, &eye, &eye, HYP_VECTOR3_UNIT_Y));
	vector3_setf3(&target, 0, 5, 0);
	print("looking along up", matrix4_view_lookat_rh(&m, HYP_VECTOR3_ZERO, &target, HYP_VECTOR3_UNIT_Y));
	return 0;
}
```

```text
eye == target   : 0 of 16 entries NaN
looking along up: 0 of 16 entries NaN
```

## Checking

`compare/check_reports.py docs/reports/handmademath/07-lookat-degenerate-nan.md` builds both
programs above and compares their output with this report.
