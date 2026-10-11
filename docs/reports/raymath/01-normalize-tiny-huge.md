# `Vector3Normalize` and `QuaternionNormalize` return tiny inputs unchanged and huge ones as zero

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `Vector3Normalize`, `QuaternionNormalize`, and the same code inlined in `QuaternionNlerp`, `QuaternionFromAxisAngle`, `MatrixLookAt` |
| Kind | wrong result |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

`Vector3Normalize` computes `sqrtf(x*x + y*y + z*z)` and leaves the vector unchanged when
that is 0.  For components below about 2.6e-23 the sum of squares underflows to 0, so a
vector such as (1, 2, 2) x 1e-25 is returned unchanged, with length 3e-25 instead of 1.
Above about 1.8e19 the sum overflows to inf and the result is zero.
`QuaternionNormalize` does the same: a tiny quarter turn about Z stays tiny, a huge one
becomes the zero quaternion.  The direction is representable in all these cases.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <stdio.h>

int main(void)
{
	float k[] = {1e-25f, 1e25f};
	int i;

	for (i = 0; i < 2; i++) {
		Vector3 v = Vector3Normalize((Vector3){k[i], 2 * k[i], 2 * k[i]});
		Quaternion q = QuaternionNormalize((Quaternion){0, 0, k[i], k[i]});
		printf("k = %g: Vector3Normalize((1, 2, 2) k) = (%g, %g, %g)\n", k[i], v.x, v.y, v.z);
		printf("        QuaternionNormalize((0, 0, k, k)) = (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
k = 1e-25: Vector3Normalize((1, 2, 2) k) = (1e-25, 2e-25, 2e-25)
        QuaternionNormalize((0, 0, k, k)) = (0, 0, 1e-25, 1e-25)
k = 1e+25: Vector3Normalize((1, 2, 2) k) = (0, 0, 0)
        QuaternionNormalize((0, 0, k, k)) = (0, 0, 0, 0)
```

Expected: (1/3, 2/3, 2/3) and (0, 0, 0.707, 0.707) in both cases.

## Cause

[`raymath.h` lines 804-819](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L804-L819):
`float length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); if (length != 0.0f) { ... }`;
[lines 2194-2208](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2194-L2208), the same for quaternions with
`if (length == 0.0f) length = 1.0f;`.

## Suggested fix

Outside a safe range such as 1e-30 to 1e30 for the squared length, divide by the largest
component first, then normalize; leave only the exactly zero vector unchanged.

## How hypatia does it

`vector3_normalize` and `quaternion_normalize` first scale the vector by a power of two so
that its largest component is between 1 and 2, which is exact, so the squares can neither
overflow nor underflow; then divide by the length with exact products and sums, so each
component is the exact quotient rounded once (within half an ulp).

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float k[] = {1e-25f, 1e25f};
	int i;

	for (i = 0; i < 2; i++) {
		struct vector3 v;
		struct quaternion q;

		vector3_normalize(vector3_setf3(&v, k[i], 2 * k[i], 2 * k[i]));
		quaternion_normalize(quaternion_setf4(&q, 0, 0, k[i], k[i]));
		printf("k = %g: vector3_normalize((1, 2, 2) k) = (%g, %g, %g)\n", k[i], v.x, v.y, v.z);
		printf("        quaternion_normalize((0, 0, k, k)) = (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	}
	return 0;
}
```

```text
k = 1e-25: vector3_normalize((1, 2, 2) k) = (0.333333, 0.666667, 0.666667)
        quaternion_normalize((0, 0, k, k)) = (0, 0, 0.707107, 0.707107)
k = 1e+25: vector3_normalize((1, 2, 2) k) = (0.333333, 0.666667, 0.666667)
        quaternion_normalize((0, 0, k, k)) = (0, 0, 0.707107, 0.707107)
```

## Checking

`compare/check_reports.py docs/reports/raymath/01-normalize-tiny-huge.md` builds both
programs above and compares their output with this report.
