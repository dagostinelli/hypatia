# `Vector3Project` onto the zero vector is NaN

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `Vector3Project`, `Vector3Reject` |
| Kind | NaN on degenerate input |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

`Vector3Project(v1, v2)` computes `v2 * (dot(v1, v2) / dot(v2, v2))`.  For `v2 = 0` that
is `0 * (0 / 0)`, NaN in every component, and `Vector3Reject`, `v1 - project`, is NaN
too.  The projection onto the zero vector has a natural value, zero.  For components of v2
above about 1.8e19, \`dot(v2, v2)\` overflows and the projection is zero instead of the
component of v1 along v2.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <stdio.h>

int main(void)
{
	Vector3 v = {1, 2, 3}, p = Vector3Project(v, (Vector3){0, 0, 0}), r = Vector3Reject(v, (Vector3){0, 0, 0});
	Vector3 h = Vector3Project(v, (Vector3){1e25f, 0, 0});

	printf("Vector3Project(v, 0) = (%g, %g, %g), Vector3Reject(v, 0) = (%g, %g, %g)\n", p.x, p.y, p.z, r.x, r.y, r.z);
	printf("Vector3Project(v, (1e25, 0, 0)) = (%g, %g, %g)\n", h.x, h.y, h.z);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
Vector3Project(v, 0) = (-nan, -nan, -nan), Vector3Reject(v, 0) = (-nan, -nan, -nan)
Vector3Project(v, (1e25, 0, 0)) = (0, 0, 0)
```

Expected: (0, 0, 0) and (1, 2, 3) for the zero vector; (1, 0, 0) for the projection onto
the x axis.

## Cause

[`raymath.h` lines 822-836](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L822-L836):
`float mag = v1dv2/v2dv2;`.

## Suggested fix

Return zero when `dot(v2, v2)` is zero; outside a safe range such as 1e-30 to 1e30,
normalize v2 with scaling and use `dot(v1, u) u`.

## How hypatia does it

`vector3_project` returns zero for the zero vector, and for |onto|^2 outside 1e-30 to
1e30 projects onto the direction of `onto` found with scaled normalization.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 v, onto;

	vector3_project(vector3_setf3(&v, 1, 2, 3), vector3_setf3(&onto, 0, 0, 0));
	printf("vector3_project(v, 0) = (%g, %g, %g)\n", v.x, v.y, v.z);
	vector3_project(vector3_setf3(&v, 1, 2, 3), vector3_setf3(&onto, 1e25f, 0, 0));
	printf("vector3_project(v, (1e25, 0, 0)) = (%g, %g, %g)\n", v.x, v.y, v.z);
	return 0;
}
```

```text
vector3_project(v, 0) = (0, 0, 0)
vector3_project(v, (1e25, 0, 0)) = (1, 0, 0)
```

## Checking

`compare/check_reports.py docs/reports/raymath/03-project-onto-zero.md` builds both
programs above and compares their output with this report.
