# `glm::lookAt` gives NaN when the eye is at the target or looks along up

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::lookAtRH`, `glm::lookAtLH` (`ext/matrix_transform.hpp`) |
| Kind | NaN on degenerate input |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`lookAt` normalizes `center - eye` and `cross(f, up)`.  When the eye is at the target the
first is the zero vector; when the camera looks straight along `up` the second is.  Either
way `normalize` returns NaN and most of the view matrix is NaN.  A camera that reaches its
target or looks straight up or down is common in interactive code; NaN in the view matrix
corrupts every vertex.

## Reproduction

```cpp
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>
#include <cstdio>

#include <glm/ext/matrix_transform.hpp>

static void print(const char *what, const glm::dmat4 &m)
{
	std::printf("%s: row 0 = (%g, %g, %g, %g)\n", what, m[0][0], m[1][0], m[2][0], m[3][0]);
}

int main()
{
	glm::dvec3 up(0, 1, 0);
	print("eye == target   ", glm::lookAtRH(glm::dvec3(0, 0, 5), glm::dvec3(0, 0, 5), up));
	print("looking along up", glm::lookAtRH(glm::dvec3(0, 0, 0), glm::dvec3(0, 5, 0), up));
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
eye == target   : row 0 = (-nan, -nan, -nan, nan)
looking along up: row 0 = (-nan, -nan, -nan, nan)
```

Expected: a finite matrix (or an assertion): there is no view direction, or no right axis.

## Cause

[`ext/matrix_transform.inl` lines 153-157](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/matrix_transform.inl#L153-L157):

```
vec<3, T, Q> const f(normalize(center - eye));
vec<3, T, Q> const s(normalize(cross(f, up)));
```

with `normalize` returning NaN for the zero vector
([09-normalize-zero-vector.md](09-normalize-zero-vector.md)).

## How hypatia does it

`matrix4_view_lookat_rh` uses `vector3_normalize`, which leaves the zero vector unchanged,
so the degenerate axes are zero rows and the matrix stays finite.  Neither library can
produce a meaningful view without a direction; hypatia's result does not spread NaN.  For
an orientation, `quaternion_set_look_rotation_rh` handles up parallel to the direction by
returning the shortest rotation from -Z to the direction.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static void print(const char *what, const struct matrix4 *m)
{
	printf("%s: row 0 = (%g, %g, %g, %g)\n", what, m->r00, m->r01, m->r02, m->r03);
}

int main(void)
{
	struct matrix4 m;
	struct vector3 eye;
	struct vector3 target;

	vector3_setf3(&eye, 0, 0, 5);
	print("eye == target   ", matrix4_view_lookat_rh(&m, &eye, &eye, HYP_VECTOR3_UNIT_Y));
	vector3_setf3(&target, 0, 5, 0);
	print("looking along up", matrix4_view_lookat_rh(&m, HYP_VECTOR3_ZERO, &target, HYP_VECTOR3_UNIT_Y));
	return 0;
}
```

```text
eye == target   : row 0 = (0, 0, 0, -0)
looking along up: row 0 = (0, 0, 0, -0)
```

## Suggested fix

Fix `normalize` for the zero vector ([09](09-normalize-zero-vector.md)), or check the two
lengths in `lookAt` and fall back (for example, to the identity rotation, or to another
up axis when `cross(f, up)` vanishes).

## Checking

`compare/check_reports.py docs/reports/glm/13-lookat-degenerate-nan.md` builds both programs above and compares their output with this report.
