# `normalized()` returns tiny vectors unchanged and huge vectors as zero

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `MatrixBase::normalized`, `normalize` (`Core/Dot.h`) |
| Kind | wrong result |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

`normalized()` divides by `sqrt(squaredNorm())`.  For components below about 1e-154 (1e-19
in float) the squared norm is subnormal and loses precision; below about 1.6e-162 (2.6e-23
in float) it is exactly 0 and the vector is returned unchanged, not of unit length.  Above
about 1.3e154 (1.8e19 in float) it overflows to inf and the result is zero; a vector with
an infinite component gives NaN.  `stableNormalized()` handles the first two cases, but
`normalized()` is the general function and is used inside Eigen: `FromTwoVectors` calls it
([`Geometry/Quaternion.h` lines 641-642](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L641-L642)).

## Reproduction

```cpp
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

int main()
{
	Eigen::Vector3d tiny = Eigen::Vector3d(1e-200, 1e-200, 0).normalized();
	Eigen::Vector3d huge = Eigen::Vector3d(1e200, 1e200, 0).normalized();
	Eigen::Vector3d inf = Eigen::Vector3d(INFINITY, 1, 0).normalized();
	std::printf("(1e-200, 1e-200, 0).normalized() = (%g, %g, %g), norm %g\n", tiny.x(), tiny.y(), tiny.z(), tiny.norm());
	std::printf("(1e200, 1e200, 0).normalized()   = (%g, %g, %g)\n", huge.x(), huge.y(), huge.z());
	std::printf("(inf, 1, 0).normalized()         = (%g, %g, %g)\n", inf.x(), inf.y(), inf.z());
	Eigen::Vector3d st = Eigen::Vector3d(1e-200, 1e-200, 0).stableNormalized();
	Eigen::Vector3d sh = Eigen::Vector3d(1e200, 1e200, 0).stableNormalized();
	std::printf("stableNormalized(): (%.9g, %.9g, %g) and (%.9g, %.9g, %g)\n", st.x(), st.y(), st.z(), sh.x(), sh.y(), sh.z());
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
(1e-200, 1e-200, 0).normalized() = (1e-200, 1e-200, 0), norm 0
(1e200, 1e200, 0).normalized()   = (0, 0, 0)
(inf, 1, 0).normalized()         = (-nan, 0, 0)
stableNormalized(): (0.707106781, 0.707106781, 0) and (0.707106781, 0.707106781, 0)
```

Expected: (0.707106781, 0.707106781, 0) for the first two, as `stableNormalized()` gives, and
(1, 0, 0) for the third.

## Cause

[`Core/Dot.h` lines 124-134](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Core/Dot.h#L124-L134):

```
RealScalar z = n.squaredNorm();
if(z>RealScalar(0))
  return n / numext::sqrt(z);
else
  return n;
```

## Suggested fix

In `normalized()`, fall back to `stableNormalized()` when `z` is subnormal or 0 (and the
vector is not exactly zero), or not finite.  The fast path for other values of `z` is
unchanged.

## How hypatia does it

`vector3_normalize` (all hypatia normalizations use `hyp_normalize`) divides by the length
when the sum of the squares is between 1e-30 and 1e30, and otherwise divides by the
largest component first.  Infinite components become +-1 and the others 0.  Only an
exactly zero vector, or NaN, is left unchanged; there is no separate "stable" variant to
choose.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
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

## Checking

`compare/check_reports.py docs/reports/eigen/04-normalized-underflow-overflow.md` builds
both programs above and compares their output with this report.  The harness:
`compare/results/double.md`, "Edge cases", `vector3_normalize`.
