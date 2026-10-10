# `glm::normalize` overflows and underflows: NaN, inf or zero for valid vectors

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::normalize(vec)` (`detail/func_geometric.inl`) |
| Kind | wrong result |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::normalize(v)` is `v * inversesqrt(dot(v, v))`.  The sum of squares overflows for
components above about 1e154 (double; 1e19 in float).  For components below about 1e-154
(1e-19 in float) it is subnormal and loses precision, and below about 1.6e-162 (2.6e-23 in
float) it is exactly 0.  In each case the unit vector is representable:
- (1e-200, 1e-200, 0) gives (inf, inf, NaN);
- (1e200, 1e200, 0) gives (0, 0, 0);
- a vector with one infinite component gives NaN in that component.

## Reproduction

```cpp
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>
#include <cstdio>

int main()
{
	glm::dvec3 tiny = glm::normalize(glm::dvec3(1e-200, 1e-200, 0));
	glm::dvec3 huge = glm::normalize(glm::dvec3(1e200, 1e200, 0));
	glm::dvec3 inf = glm::normalize(glm::dvec3(INFINITY, 1, 0));
	std::printf("normalize(1e-200, 1e-200, 0) = (%g, %g, %g)\n", tiny.x, tiny.y, tiny.z);
	std::printf("normalize(1e200, 1e200, 0)   = (%g, %g, %g)\n", huge.x, huge.y, huge.z);
	std::printf("normalize(inf, 1, 0)         = (%g, %g, %g)\n", inf.x, inf.y, inf.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
normalize(1e-200, 1e-200, 0) = (inf, inf, -nan)
normalize(1e200, 1e200, 0)   = (0, 0, 0)
normalize(inf, 1, 0)         = (-nan, 0, 0)
```

Expected: (0.707106781, 0.707106781, 0) for the first two, and (1, 0, 0) as the direction of (inf, 1, 0).

## Cause

[`detail/func_geometric.inl` line 88](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/func_geometric.inl#L88):
`return v * inversesqrt(dot(v, v));`.  `dot(v, v)` is computed in the type of `v`, so its
range is that of the squares, not that of the vector.

## How hypatia does it

`vector3_normalize` (all vector and quaternion normalizations share `hyp_normalize`)
divides by the length when the sum of the squares is between 1e-30 and 1e30; otherwise it
first divides by the largest component, so the squares cannot overflow or underflow.
Infinite components become +-1 and the others 0 before dividing.  Only an exactly zero
vector (or NaN) is left unchanged.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 v;

	vector3_normalize(vector3_setf3(&v, 1e-200, 1e-200, 0));
	printf("normalize(1e-200, 1e-200, 0) = (%.9g, %.9g, %g)\n", v.x, v.y, v.z);
	vector3_normalize(vector3_setf3(&v, 1e200, 1e200, 0));
	printf("normalize(1e200, 1e200, 0)   = (%.9g, %.9g, %g)\n", v.x, v.y, v.z);
	vector3_normalize(vector3_setf3(&v, INFINITY, 1, 0));
	printf("normalize(inf, 1, 0)         = (%g, %g, %g)\n", v.x, v.y, v.z);
	return 0;
}
```

```text
normalize(1e-200, 1e-200, 0) = (0.707106781, 0.707106781, 0)
normalize(1e200, 1e200, 0)   = (0.707106781, 0.707106781, 0)
normalize(inf, 1, 0)         = (1, 0, 0)
```

## Suggested fix

When `dot(v, v)` is outside a safe range such as 1e-30 to 1e30, divide `v` by its largest
absolute component first (or use `length` computed with scaling, as `hypot` does), then
normalize.  The fast path stays as it is.

## Checking

`compare/check_reports.py docs/reports/glm/08-normalize-overflow-underflow.md` builds both programs above and compares their output with this report. The harness: `results/double.md`, "Edge cases", `vector3_normalize`.
