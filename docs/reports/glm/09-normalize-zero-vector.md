# `glm::normalize` of the zero vector is NaN, and the NaN spreads

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::normalize(vec)` (`detail/func_geometric.inl`) |
| Kind | NaN on degenerate input |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::normalize((0, 0, 0))` computes `0 * inversesqrt(0) = 0 * inf = NaN` in every
component.  The functions built on it inherit the NaN for zero-length input:
`angle(normalize(a), normalize(b))` with a zero vector (below), `lookAt` with the eye at the
target ([13](13-lookat-degenerate-nan.md)), `rotate` about a zero axis
([15](15-axis-angle-zero-or-not-unit-axis.md)).  A zero vector occurs as an ordinary value
(a velocity at rest, an unset direction), and NaN propagates through every later operation.

## Reproduction

```cpp
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <cstdio>

int main()
{
	glm::dvec3 zero(0, 0, 0);
	glm::dvec3 n = glm::normalize(zero);
	std::printf("normalize(0, 0, 0) = (%g, %g, %g)\n", n.x, n.y, n.z);
	std::printf("angle(normalize(1, 2, 3), normalize(0, 0, 0)) = %g\n",
	            glm::angle(glm::normalize(glm::dvec3(1, 2, 3)), glm::normalize(zero)));
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
normalize(0, 0, 0) = (-nan, -nan, -nan)
angle(normalize(1, 2, 3), normalize(0, 0, 0)) = -nan
```

Expected: a defined value: the zero vector (it has no direction) and an angle of 0, or an assertion.

## Cause

[`detail/func_geometric.inl` line 88](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/func_geometric.inl#L88):
`v * inversesqrt(dot(v, v))` with `dot(v, v) = 0`.

## Suggested fix

Return the input when `dot(v, v) == 0`; the check costs one comparison next to an inverse
square root.  If NaN is the intended result, document it on `normalize` and the functions
built on it.

## How hypatia does it

`vector3_normalize` leaves the zero vector unchanged (it has no direction to keep), and the
functions that take directions define their result for it: `vector3_angle_between` returns
0 when either vector is zero.  A vector with a NaN component is left unchanged.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 zero;
	struct vector3 a;

	vector3_normalize(vector3_zero(&zero));
	printf("normalize(0, 0, 0) = (%g, %g, %g)\n", zero.x, zero.y, zero.z);
	printf("angle_between((1, 2, 3), (0, 0, 0)) = %g\n", vector3_angle_between(vector3_setf3(&a, 1, 2, 3), &zero));
	return 0;
}
```

```text
normalize(0, 0, 0) = (0, 0, 0)
angle_between((1, 2, 3), (0, 0, 0)) = 0
```

## Checking

`compare/check_reports.py docs/reports/glm/09-normalize-zero-vector.md` builds both
programs above and compares their output with this report.  The harness:
`compare/results/double.md`, "Edge cases".
