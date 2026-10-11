# `vec3_norm` gives NaN, inf or zero for zero, tiny and huge vectors

| | |
|---|---|
| Library | linmath.h master 3eef828 (2022-06-19) |
| Function | `vec2_norm`, `vec3_norm`, `vec4_norm`, `quat_norm`, and through them `quat_rotate`, `mat4x4_look_at` |
| Kind | NaN on degenerate input; wrong result |
| Precision | float |
| Status | present at master 3eef828 (2022-06-19, the latest commit; linmath.h has no releases) |

## Summary

`vec3_norm` scales by `1.f / vec3_len(v)`.  The zero vector gives `0 * inf`, NaN in every
component.  For components below about 2.6e-23 the squared length underflows to 0 and the
result is inf.  Above about 1.8e19 it overflows to inf and the result is zero.  The
direction is representable in all three cases.

## Reproduction

```c
#include <linmath.h>
#include <stdio.h>

int main(void)
{
	float k[] = {0, 1e-25f, 1e25f};
	int i;

	for (i = 0; i < 3; i++) {
		vec3 v = {k[i], 2 * k[i], 2 * k[i]}, r;

		vec3_norm(r, v);
		printf("vec3_norm((1, 2, 2) %g) = (%g, %g, %g)\n", k[i], r[0], r[1], r[2]);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
vec3_norm((1, 2, 2) 0) = (-nan, -nan, -nan)
vec3_norm((1, 2, 2) 1e-25) = (inf, inf, inf)
vec3_norm((1, 2, 2) 1e+25) = (0, 0, 0)
```

Expected: (1/3, 2/3, 2/3) for the tiny and huge vectors; a defined value for the zero
vector.

## Cause

[`linmath.h` lines 46-50](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L46-L50):
`float k = 1.f / vec##n##_len(v); vec##n##_scale(r, v, k);`.

## Suggested fix

Check the squared length: zero, return the input; outside a safe range such as 1e-30 to
1e30, divide by the largest component first, then normalize.

## How hypatia does it

`vector3_normalize` divides by the length when the squared length is between 1e-30 and
1e30, and otherwise divides by the largest component first.  The zero vector is left
unchanged.

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

		vector3_normalize(vector3_setf3(&v, k[i], 2 * k[i], 2 * k[i]));
		printf("vector3_normalize((1, 2, 2) %g) = (%g, %g, %g)\n", k[i], v.x, v.y, v.z);
	}
	return 0;
}
```

```text
vector3_normalize((1, 2, 2) 0) = (0, 0, 0)
vector3_normalize((1, 2, 2) 1e-25) = (0.333333, 0.666667, 0.666667)
vector3_normalize((1, 2, 2) 1e+25) = (0.333333, 0.666667, 0.666667)
```

## Checking

`compare/check_reports.py docs/reports/linmath/02-norm-zero-tiny-huge.md` builds both
programs above and compares their output with this report.
