# `HMM_InvQ` is NaN for the zero quaternion, and wrong for tiny and huge ones

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_InvQ` |
| Kind | NaN on degenerate input; wrong result |
| Precision | float |
| Status | still present in HandmadeMath v2.0.0 (the latest release, 2023-02-20) and at master 661fef0 (2026-03-17): the program prints the same |

## Summary

`HMM_InvQ` divides the conjugate by `HMM_DotQ(q, q)`.  For the zero quaternion that is
0 / 0, NaN.  For components below about 2.6e-23 the squared length is 0 and the result is
inf and NaN; above about 1.8e19 it overflows and the result is zero.  In both cases the
inverse is representable.

## Reproduction

```c
#include <HandmadeMath.h>
#include <stdio.h>

int main(void)
{
	float w[] = {0, 1e-25f, 1e25f};
	int i;

	for (i = 0; i < 3; i++) {
		HMM_Quat r = HMM_InvQ(HMM_Q(0, 0, 0, w[i]));
		printf("HMM_InvQ(w = %g) = (%g, %g, %g, %g)\n", w[i], r.X, r.Y, r.Z, r.W);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
HMM_InvQ(w = 0) = (-nan, -nan, -nan, -nan)
HMM_InvQ(w = 1e-25) = (-nan, -nan, -nan, inf)
HMM_InvQ(w = 1e+25) = (-0, -0, -0, 0)
```

Expected: a defined value for the zero quaternion (it has no inverse), w = 1e25 for the
second and w = 1e-25 for the third.

## Cause

[`HandmadeMath.h` lines 2314-2325](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L2314-L2325):
`return HMM_DivQF(Result, (HMM_DotQ(Left, Left)));`.

## Suggested fix

Check the squared length: zero, return the input; outside a safe range such as 1e-30 to
1e30, divide by the length twice.

## How hypatia does it

`quaternion_inverse` divides the conjugate by |q|^2 when it is between 1e-30 and 1e30, and
otherwise computes `(q / |q|) / |q|` with scaled normalization; the zero quaternion is
left unchanged.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float w[] = {0, 1e-25f, 1e25f};
	int i;

	for (i = 0; i < 3; i++) {
		struct quaternion q;

		quaternion_inverse(quaternion_setf4(&q, 0, 0, 0, w[i]));
		printf("quaternion_inverse(w = %g) = (%g, %g, %g, %g)\n", w[i], q.x, q.y, q.z, q.w);
	}
	return 0;
}
```

```text
quaternion_inverse(w = 0) = (0, 0, 0, 0)
quaternion_inverse(w = 1e-25) = (-0, -0, -0, 1e+25)
quaternion_inverse(w = 1e+25) = (-0, -0, -0, 1e-25)
```

## Checking

`compare/check_reports.py docs/reports/handmademath/02-invq-zero-tiny-huge.md` builds both
programs above and compares their output with this report.
