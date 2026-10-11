# `Quaternion::inverse()` returns zero for tiny and huge quaternions, which have inverses

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `QuaternionBase::inverse` (`Geometry/Quaternion.h`) |
| Kind | wrong result |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

`inverse()` divides the conjugate by `squaredNorm()`, and returns the zero quaternion
("an invalid result to flag the error") when that is not positive.  For components below
about 1e-154 (1e-19 in float) the squared norm is subnormal and loses precision; below
about 1.6e-162 (2.6e-23 in float) it is exactly 0, so a valid, invertible quaternion is
reported as having no inverse; above about 1.3e154 it overflows to inf and the
result is zero as well.  `normalized()` on a tiny quaternion returns it unchanged
([04](04-normalized-underflow-overflow.md)).

## Reproduction

```cpp
#include <Eigen/Geometry>
#include <cstdio>

int main()
{
	Eigen::Quaterniond tiny(1e-200, 0, 0, 0), huge(1e200, 0, 0, 0);
	std::printf("inverse(w 1e-200) = (w %g, x %g, y %g, z %g)\n", tiny.inverse().w(), tiny.inverse().x(), tiny.inverse().y(), tiny.inverse().z());
	std::printf("inverse(w 1e200)  = (w %g, x %g, y %g, z %g)\n", huge.inverse().w(), huge.inverse().x(), huge.inverse().y(), huge.inverse().z());
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
inverse(w 1e-200) = (w 0, x 0, y 0, z 0)
inverse(w 1e200)  = (w 0, x -0, y -0, z -0)
```

Expected: w = 1e200 and w = 1e-200.

## Cause

[`Geometry/Quaternion.h` lines 720-731](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L720-L731):

```
Scalar n2 = this->squaredNorm();
if (n2 > Scalar(0))
  return Quaternion<Scalar>(conjugate().coeffs() / n2);
else
{
  // return an invalid result to flag the error
  return Quaternion<Scalar>(Coefficients::Zero());
}
```

## Suggested fix

When `n2` is subnormal, zero after underflow, or inf, compute
`conjugate().coeffs() / norm() / norm()` with a stable norm (`stableNorm()`); return the
invalid result only for an exactly zero quaternion.

## How hypatia does it

`quaternion_inverse` divides the conjugate by |q|^2 when it is between 1e-30 and 1e30, and
otherwise computes `(q / |q|) / |q|` with a scaled normalization.  Only the zero
quaternion, which has no inverse, is left unchanged.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;

	quaternion_inverse(quaternion_setf4(&q, 0, 0, 0, 1e-200));
	printf("inverse(w 1e-200) = (w %g, x %g, y %g, z %g)\n", q.w, q.x, q.y, q.z);
	quaternion_inverse(quaternion_setf4(&q, 0, 0, 0, 1e200));
	printf("inverse(w 1e200)  = (w %g, x %g, y %g, z %g)\n", q.w, q.x, q.y, q.z);
	return 0;
}
```

```text
inverse(w 1e-200) = (w 1e+200, x -0, y -0, z -0)
inverse(w 1e200)  = (w 1e-200, x -0, y -0, z -0)
```

## Checking

`compare/check_reports.py docs/reports/eigen/09-quaternion-inverse-tiny-huge.md` builds
both programs above and compares their output with this report.
