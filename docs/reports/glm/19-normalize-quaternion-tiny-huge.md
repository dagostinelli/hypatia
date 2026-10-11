# `glm::normalize` of a tiny quaternion returns the identity, a different rotation; of a huge one, zero

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::normalize(qua)` (`ext/quaternion_geometric.hpp`) |
| Kind | wrong result |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::normalize(q)` multiplies by `1 / length(q)`.  For components below about 1e-154
(1e-19 in float) the sum of squares is subnormal and the length loses precision.  Below
about 1.6e-162 (2.6e-23 in float) the length is exactly 0 and GLM returns the identity:
(w 1e-200, z 1e-200), a quarter turn about Z, becomes no rotation.  Above about 1.3e154
the length overflows to inf and the result is the zero quaternion.  Quaternions are
scale-invariant as rotations, so any non-zero quaternion has a well-defined unit
quaternion.

## Reproduction

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdio>

int main()
{
	glm::dquat t = glm::normalize(glm::dquat(1e-200, 0, 0, 1e-200));
	glm::dquat h = glm::normalize(glm::dquat(1e200, 0, 0, 1e200));
	std::printf("normalize(w 1e-200, z 1e-200) = (w %.9g, x %g, y %g, z %.9g)\n", t.w, t.x, t.y, t.z);
	std::printf("normalize(w 1e200, z 1e200)   = (w %.9g, x %g, y %g, z %.9g)\n", h.w, h.x, h.y, h.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
normalize(w 1e-200, z 1e-200) = (w 1, x 0, y 0, z 0)
normalize(w 1e200, z 1e200)   = (w 0, x 0, y 0, z 0)
```

Expected: (w 0.707106781, z 0.707106781) in both cases: a quarter turn about Z.

## Cause

[`ext/quaternion_geometric.inl` lines 17-24](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/quaternion_geometric.inl#L17-L24):

```
T len = length(q);
if(len <= static_cast<T>(0)) // Problem
	return qua<T, Q>::wxyz(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
T oneOverLen = static_cast<T>(1) / len;
return qua<T, Q>::wxyz(q.w * oneOverLen, q.x * oneOverLen, q.y * oneOverLen, q.z * oneOverLen);
```

`length` squares the components; the comment "Problem" marks the branch that turns an
underflow into the identity.

## Suggested fix

Return the identity (or the input) only for an exactly zero quaternion; otherwise, when
`dot(q, q)` is outside a safe range such as 1e-30 to 1e30, divide by the largest component
first.  The same applies to `glm::normalize` of vectors
([08](08-normalize-overflow-underflow.md)).

## How hypatia does it

`quaternion_normalize` uses `hyp_normalize`: it divides by the length when the sum of the
squares is between 1e-30 and 1e30, and otherwise divides by the largest component first,
so tiny and huge quaternions normalize to the same unit quaternion as any other multiple.
Only the zero quaternion (or NaN) is left unchanged.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;

	quaternion_normalize(quaternion_setf4(&q, 0, 0, 1e-200, 1e-200));
	printf("normalize(w 1e-200, z 1e-200) = (w %.9g, x %g, y %g, z %.9g)\n", q.w, q.x, q.y, q.z);
	quaternion_normalize(quaternion_setf4(&q, 0, 0, 1e200, 1e200));
	printf("normalize(w 1e200, z 1e200)   = (w %.9g, x %g, y %g, z %.9g)\n", q.w, q.x, q.y, q.z);
	return 0;
}
```

```text
normalize(w 1e-200, z 1e-200) = (w 0.707106781, x 0, y 0, z 0.707106781)
normalize(w 1e200, z 1e200)   = (w 0.707106781, x 0, y 0, z 0.707106781)
```

## Checking

`compare/check_reports.py docs/reports/glm/19-normalize-quaternion-tiny-huge.md` builds
both programs above and compares their output with this report.
