# `QuaternionInvert` gives inf and NaN for tiny quaternions and zero for huge ones

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `QuaternionInvert` |
| Kind | wrong result |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

`QuaternionInvert` multiplies the conjugate by `1.0f/lengthSq`.  For components around
1e-20 the squared length is subnormal (1e-40) and its reciprocal overflows to inf: the
result is inf and NaN.  Below about 2.6e-23 the squared length is 0 and the quaternion is
returned unchanged, not inverted.  Above about 1.8e19 it overflows and the result is zero.
The inverse is representable in all three cases.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <stdio.h>

int main(void)
{
	float w[] = {1e-20f, 1e-25f, 1e25f};
	int i;

	for (i = 0; i < 3; i++) {
		Quaternion r = QuaternionInvert((Quaternion){0, 0, 0, w[i]});
		printf("QuaternionInvert(w = %g) = (%g, %g, %g, %g)\n", w[i], r.x, r.y, r.z, r.w);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
QuaternionInvert(w = 1e-20) = (-nan, -nan, -nan, inf)
QuaternionInvert(w = 1e-25) = (0, 0, 0, 1e-25)
QuaternionInvert(w = 1e+25) = (-0, -0, -0, 0)
```

Expected: w = 1e20, 1e25 and 1e-25.

## Cause

[`raymath.h` lines 2211-2228](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2211-L2228):
`float invLength = 1.0f/lengthSq;` with `lengthSq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w`.

## Suggested fix

Outside a safe range such as 1e-30 to 1e30 for the squared length, divide by the length
twice (normalizing with scaling first); divide instead of multiplying by the reciprocal.

## How hypatia does it

`quaternion_inverse` divides the conjugate by |q|^2 when it is between 1e-30 and 1e30, and
otherwise computes `(q / |q|) / |q|` with scaled normalization.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float w[] = {1e-20f, 1e-25f, 1e25f};
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
quaternion_inverse(w = 1e-20) = (-0, -0, -0, 1e+20)
quaternion_inverse(w = 1e-25) = (-0, -0, -0, 1e+25)
quaternion_inverse(w = 1e+25) = (-0, -0, -0, 1e-25)
```

## Checking

`compare/check_reports.py docs/reports/raymath/02-quaternioninvert-tiny-huge.md` builds both
programs above and compares their output with this report.
