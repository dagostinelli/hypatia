# `glm::rotation` returns a degenerate quaternion for opposite vectors

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::rotation(orig, dest)` (`gtx/quaternion.hpp`) |
| Kind | wrong result |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

For two unit vectors that point in opposite directions, `glm::rotation` is meant to return
a half turn.  After rounding, the dot product of two opposite unit vectors can be
-1 + 2.2e-16 rather than -1, which misses the special case `cosTheta < -1 + epsilon`.  The
general formula then divides a zero cross product by `sqrt(2 (1 + cosTheta))` and returns
(w 1.05e-8, x 0, y 0, z 0): not a unit quaternion and not a half turn.  The vector is left
where it was instead of being turned around.

## Reproduction

```cpp
// glm::rotation for vectors in opposite directions
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cstdio>

int main()
{
	glm::dvec3 a(0.56619844751721171, -0.21123414636181392, 0.68037543430941905);
	glm::dvec3 from = glm::normalize(a);
	glm::dvec3 to = glm::normalize(-2.0 * a);
	glm::dquat q = glm::rotation(from, to);
	glm::dvec3 r = q * from;

	std::printf("dot(from, to) = %.17g\n", glm::dot(from, to));
	std::printf("q = (w %.3g, x %.3g, y %.3g, z %.3g), length %.3g\n", q.w, q.x, q.y, q.z, glm::length(q));
	std::printf("q * from = (%.6f, %.6f, %.6f)\n", r.x, r.y, r.z);
	std::printf("to       = (%.6f, %.6f, %.6f)\n", to.x, to.y, to.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
dot(from, to) = -0.99999999999999978
q = (w 1.05e-08, x 0, y 0, z 0), length 1.05e-08
q * from = (0.622192, -0.232124, 0.747660)
to       = (-0.622192, 0.232124, -0.747660)
```

Expected: a unit quaternion with w = 0 whose rotation takes `from` to `to`.

## Cause

[`gtx/quaternion.inl` lines 122-158](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtx/quaternion.inl#L122-L158):
the opposite case is detected with `cosTheta < -1 + epsilon<T>()`, a test on a rounded dot
product.  Inputs that miss it by one rounding fall through to (lines 148-157)

```
rotationAxis = cross(orig, dest);

T s = sqrt((T(1) + cosTheta) * static_cast<T>(2));
T invs = static_cast<T>(1) / s;

return qua<T, Q>::wxyz(
	s * static_cast<T>(0.5f),
	rotationAxis.x * invs,
	rotationAxis.y * invs,
	rotationAxis.z * invs);
```

which has no information left: for opposite vectors `cross(orig, dest)` is (0, 0, 0), and
`s = sqrt((1 + cosTheta) * 2)` is the square root of a rounding error (2.1e-8 here), so the
result is (w 1.05e-8, x 0, y 0, z 0).  Vectors close to opposite (1e-3 rad) are affected in the same way to a lesser
degree: see [03-rotation-near-opposite-precision.md](03-rotation-near-opposite-precision.md).

## How hypatia does it

`quaternion_get_rotation_tov3` normalizes both vectors and takes the half angle from two
lengths that stay accurate for any angle: for unit vectors at angle a,
|f + t| = 2 cos(a/2) and |f - t| = 2 sin(a/2).  The axis is f x (f + t), which has the
direction of f x t but does not vanish into rounding noise as the vectors become opposite.
Only when that axis is exactly zero (f and t exactly parallel or opposite after rounding)
does it need a special case: the identity when the half-angle cosine is positive (same
direction), otherwise a half turn about the cross product of f with the coordinate axis
least aligned with it.  The
result is normalized, so it is always a unit quaternion.

```c
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 from;
	struct vector3 to;
	struct vector3 r;
	struct quaternion q;

	vector3_setf3(&from, 0.56619844751721171, -0.21123414636181392, 0.68037543430941905);
	vector3_setf3(&to, -2 * from.x, -2 * from.y, -2 * from.z);
	quaternion_get_rotation_tov3(&from, &to, &q);
	vector3_rotate_by_quaternion(vector3_normalize(vector3_set(&r, &from)), &q);
	vector3_normalize(&to);

	printf("q = (w %.3g, x %.3g, y %.3g, z %.3g), length %.3g\n", q.w, q.x, q.y, q.z, quaternion_magnitude(&q));
	printf("q * from = (%.6f, %.6f, %.6f)\n", r.x, r.y, r.z);
	printf("to       = (%.6f, %.6f, %.6f)\n", to.x, to.y, to.z);
	return 0;
}
```

```text
q = (w 0, x -0.769, y 0, z 0.64), length 1
q * from = (-0.622192, 0.232124, -0.747660)
to       = (-0.622192, 0.232124, -0.747660)
```

## Suggested fix

Do not branch on the rounded dot product.  With `f = normalize(orig)` and `t =
normalize(dest)`, take `c = length(f + t) / 2` and `s = length(f - t) / 2` (the cosine and
sine of half the angle) and the axis `normalize(cross(f, f + t))`.  If that cross product
is exactly zero and `c > 0` (same direction), return the identity; if it is zero and `c`
is 0 (opposite), use `normalize(cross(f, e))` with `e` the coordinate axis of the smallest
component of `f`, and `s = 1`.  Return `quat(c, axis * s)`, normalized.

## Checking

`compare/check_reports.py docs/reports/glm/01-rotation-opposite-vectors.md` builds both
programs above and compares their output with this report.  The comparison harness in
`compare/` measures the same case over 2000 inputs (`results/double.md`, table "accuracy
against long double", row `glm::rotation`, "exactly opposite (to = -2 from)"): the largest
error is 2, the largest possible distance between unit vectors, against 2.4e-16 for
`quaternion_get_rotation_tov3`.
