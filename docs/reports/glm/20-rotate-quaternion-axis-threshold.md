# `glm::rotate(q, angle, axis)` skips normalizing axes within 0.001 of unit length

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::rotate(qua, angle, vec3)` (`ext/quaternion_transform.hpp`) |
| Kind | wrong result (accuracy) |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::rotate` for quaternions normalizes the axis only if its length differs from 1 by more
than 0.001.  An axis of length 1.0009 is used as it is: the quaternion has length 1.00045,
and `q * v` then stretches the vector by 1.0009 and turns it 1.8e-3 rad off.  The error is
proportional to how far the axis is from unit length, up to 0.1%, and nothing reports it.

## Reproduction

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdio>

int main()
{
	glm::dquat q = glm::rotate(glm::dquat(1, 0, 0, 0), glm::pi<double>() / 2, glm::dvec3(0, 0, 1.0009));
	glm::dvec3 r = q * glm::dvec3(1, 0, 0);
	std::printf("|q| = %.9g\n", glm::length(q));
	std::printf("quarter turn of X about (0, 0, 1.0009): (%.9g, %.9g, %g), length %.9g\n", r.x, r.y, r.z, glm::length(r));
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
|q| = 1.0004501
quarter turn of X about (0, 0, 1.0009): (-0.00180081, 1.0009, 0), length 1.00090162
```

Expected: a unit quaternion, and X turned to (0, 1, 0).

## Cause

[`ext/quaternion_transform.inl` lines 4-22](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/quaternion_transform.inl#L4-L22):

```
// Axis of rotation must be normalised
T len = glm::length(Tmp);
if(abs(len - static_cast<T>(1)) > static_cast<T>(0.001))
{
	T oneOverLen = static_cast<T>(1) / len;
	...
```

## Suggested fix

Normalize unconditionally (the threshold saves one division, at the cost of an error of up
to 0.1%), and return `q` unchanged for a zero axis.

## How hypatia does it

`quaternion_rotate_by_axis_angle` builds the rotation with `quaternion_set_from_axis_anglev3`,
which always normalizes the axis (with scaling) and returns a unit quaternion; a zero axis
gives the identity.

```c
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;
	struct vector3 axis;
	struct vector3 r;

	quaternion_identity(&q);
	quaternion_rotate_by_axis_angle(&q, vector3_setf3(&axis, 0, 0, 1.0009), HYP_PI / 2);
	vector3_rotate_by_quaternion(vector3_setf3(&r, 1, 0, 0), &q);
	printf("|q| = %.9g\n", quaternion_magnitude(&q));
	printf("quarter turn of X about (0, 0, 1.0009): (%.9g, %.9g, %g), length %.9g\n", r.x, r.y, r.z, vector3_magnitude(&r));
	return 0;
}
```

```text
|q| = 1
quarter turn of X about (0, 0, 1.0009): (2.22044605e-16, 1, 0), length 1
```

## Checking

`compare/check_reports.py docs/reports/glm/20-rotate-quaternion-axis-threshold.md` builds
both programs above and compares their output with this report.
