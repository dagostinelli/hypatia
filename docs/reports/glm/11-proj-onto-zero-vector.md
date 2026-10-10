# `glm::proj` onto the zero vector is NaN

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::proj(x, Normal)` (`gtx/projection.hpp`) |
| Kind | NaN on degenerate input |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::proj(x, n)` is `dot(x, n) / dot(n, n) * n`.  For `n = 0` that is 0/0 = NaN in every
component.  The projection onto the zero vector is the zero vector (the component of `x`
along no direction).  Also, `dot(n, n)` overflows for components above about 1e154.
Below about 1e-154 it is subnormal and loses precision, and below about 1.6e-162 it is
exactly 0, which gives NaN and inf for valid input: the second line below uses 1e-200.

## Reproduction

```cpp
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>
#include <cstdio>

#include <glm/gtx/projection.hpp>

int main()
{
	glm::dvec3 p = glm::proj(glm::dvec3(1, 2, 3), glm::dvec3(0, 0, 0));
	glm::dvec3 q = glm::proj(glm::dvec3(1, 2, 3), glm::dvec3(0, 1e-200, 0));
	std::printf("proj((1, 2, 3), (0, 0, 0))     = (%g, %g, %g)\n", p.x, p.y, p.z);
	std::printf("proj((1, 2, 3), (0, 1e-200, 0)) = (%g, %g, %g)\n", q.x, q.y, q.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
proj((1, 2, 3), (0, 0, 0))     = (-nan, -nan, -nan)
proj((1, 2, 3), (0, 1e-200, 0)) = (-nan, inf, -nan)
```

Expected: (0, 0, 0), and (0, 2, 0).

## Cause

[`gtx/projection.inl` line 8](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtx/projection.inl#L8):
`return glm::dot(x, Normal) / glm::dot(Normal, Normal) * Normal;`.

## How hypatia does it

`vector3_project` divides by |n|^2 when it is between 1e-30 and 1e30; otherwise it projects
onto the unit vector in the direction of n (scaled normalization, no overflow or
underflow).  Projecting onto the zero vector gives the zero vector.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 v;
	struct vector3 onto;

	vector3_project(vector3_setf3(&v, 1, 2, 3), vector3_zero(&onto));
	printf("project((1, 2, 3), (0, 0, 0))     = (%g, %g, %g)\n", v.x, v.y, v.z);
	vector3_project(vector3_setf3(&v, 1, 2, 3), vector3_setf3(&onto, 0, 1e-200, 0));
	printf("project((1, 2, 3), (0, 1e-200, 0)) = (%g, %g, %g)\n", v.x, v.y, v.z);
	return 0;
}
```

```text
project((1, 2, 3), (0, 0, 0))     = (0, 0, 0)
project((1, 2, 3), (0, 1e-200, 0)) = (0, 2, 0)
```

## Suggested fix

Return the zero vector when `Normal` is zero; when `dot(Normal, Normal)` is outside a safe
range such as 1e-30 to 1e30, normalize `Normal` with scaling first and return
`dot(x, n) * n`.

## Checking

`compare/check_reports.py docs/reports/glm/11-proj-onto-zero-vector.md` builds both programs above and compares their output with this report.
