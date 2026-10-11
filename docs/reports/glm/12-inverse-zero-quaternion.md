# `glm::inverse` is NaN for the zero quaternion, and wrong for tiny and huge ones

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::inverse(qua)` (`ext/quaternion_common.hpp`) |
| Kind | NaN on degenerate input; wrong result |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::inverse(q)` is `conjugate(q) / dot(q, q)`: for the zero quaternion, 0/0 = NaN in
every component.  `dot(q, q)` also overflows for components above about 1.3e154, and very
large quaternions, which have inverses, give 0.  For components below about 1e-154 it is
subnormal and loses precision, and below about 1.6e-162 it is exactly 0: the quaternion
with w = 1e-200 gives inf in w and NaN (0/0) in x, y and z.

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
	glm::dquat z = glm::inverse(glm::dquat(0, 0, 0, 0));
	glm::dquat t = glm::inverse(glm::dquat(1e-200, 0, 0, 0));
	glm::dquat h = glm::inverse(glm::dquat(1e200, 0, 0, 0));
	std::printf("inverse(0)          = (w %g, x %g, y %g, z %g)\n", z.w, z.x, z.y, z.z);
	std::printf("inverse(w = 1e-200) = (w %g, x %g, y %g, z %g)\n", t.w, t.x, t.y, t.z);
	std::printf("inverse(w = 1e200)  = (w %g, x %g, y %g, z %g)\n", h.w, h.x, h.y, h.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
inverse(0)          = (w nan, x -nan, y -nan, z -nan)
inverse(w = 1e-200) = (w inf, x -nan, y -nan, z -nan)
inverse(w = 1e200)  = (w 0, x -0, y -0, z -0)
```

Expected: a defined value for the zero quaternion (it has no inverse), w = 1e200 for the second
and w = 1e-200 for the third.

## Cause

[`ext/quaternion_common.inl` lines 119-122](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/quaternion_common.inl#L119-L122):
`return conjugate(q) / dot(q, q);`.

## Suggested fix

Check `dot(q, q)`: for a zero quaternion, return the input (or document NaN); outside a
safe range such as 1e-30 to 1e30, divide by the length twice, with a scaled normalization.

## How hypatia does it

`quaternion_inverse` divides the conjugate by |q|^2 when it is between 1e-30 and 1e30;
otherwise it computes `(q / |q|) / |q|` with a scaled normalization.  The zero quaternion,
which has no inverse, is left unchanged.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;

	quaternion_setf4(&q, 0, 0, 0, 0);
	quaternion_inverse(&q);
	printf("inverse(0)          = (w %g, x %g, y %g, z %g)\n", q.w, q.x, q.y, q.z);
	quaternion_setf4(&q, 0, 0, 0, 1e-200);
	quaternion_inverse(&q);
	printf("inverse(w = 1e-200) = (w %g, x %g, y %g, z %g)\n", q.w, q.x, q.y, q.z);
	quaternion_setf4(&q, 0, 0, 0, 1e200);
	quaternion_inverse(&q);
	printf("inverse(w = 1e200)  = (w %g, x %g, y %g, z %g)\n", q.w, q.x, q.y, q.z);
	return 0;
}
```

```text
inverse(0)          = (w 0, x 0, y 0, z 0)
inverse(w = 1e-200) = (w 1e+200, x -0, y -0, z -0)
inverse(w = 1e200)  = (w 1e-200, x -0, y -0, z -0)
```

## Checking

`compare/check_reports.py docs/reports/glm/12-inverse-zero-quaternion.md` builds both
programs above and compares their output with this report.
