# `Quaternion::angularDistance` loses digits for nearly equal rotations

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `QuaternionBase::angularDistance` (`Geometry/Quaternion.h`) |
| Kind | precision |
| Precision | double and float |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

`angularDistance` forms `d = this * other.conjugate()` and returns
`2 atan2(|vec(d)|, |w(d)|)`.  For nearly equal rotations `vec(d)` is a difference of
products of nearly equal numbers, so its rounding error is an epsilon of the inputs, not of
the small result.  For rotations 1e-4 rad apart the program below measures a mean error of
1775 double epsilons relative to the angle (largest 9264), against 256 (largest 4558) with
hypatia; the comparison harness, over other inputs, 1770 against 205.  The program's
reference is the exact angle between the rounded inputs.  hypatia's remaining error comes
from normalizing the rounded inputs in double; the subtraction that follows is exact.

## Reproduction

```cpp
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

static unsigned long long state = 88172645463325252ULL;
static double rnd() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

/* the angle between the rotations a and b, in long double from the rounded inputs */
static long double exact(const double a[4], const double b[4])
{
	long double na = 0, nb = 0, s = 0, c = 0, dot = 0;
	for (int k = 0; k < 4; k++) { na += (long double)a[k] * a[k]; nb += (long double)b[k] * b[k]; dot += (long double)a[k] * b[k]; }
	for (int k = 0; k < 4; k++) {
		long double u = a[k] / sqrtl(na), v = (dot < 0 ? -1 : 1) * b[k] / sqrtl(nb);
		s += (u - v) * (u - v);
		c += (u + v) * (u + v);
	}
	return 4 * atan2l(sqrtl(s), sqrtl(c));
}

int main()
{
	long double sum = 0, largest = 0;
	for (int i = 0; i < 20000; i++) {
		/* a random unit quaternion, and one 1e-4 rad from it, both rounded */
		long double q[4], n = 0, axis[3], m = 0;
		for (int k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
		for (int k = 0; k < 3; k++) { axis[k] = 2 * rnd() - 1; m += axis[k] * axis[k]; }
		long double h = sinl(0.5e-4L) / sqrtl(m), c = cosl(0.5e-4L);
		long double d[4] = {axis[0] * h, axis[1] * h, axis[2] * h, c};  /* x y z w */
		long double p[4] = {q[3] * d[0] + q[0] * d[3] + q[1] * d[2] - q[2] * d[1],
		                    q[3] * d[1] - q[0] * d[2] + q[1] * d[3] + q[2] * d[0],
		                    q[3] * d[2] + q[0] * d[1] - q[1] * d[0] + q[2] * d[3],
		                    q[3] * d[3] - q[0] * d[0] - q[1] * d[1] - q[2] * d[2]};
		double a[4], b[4];
		for (int k = 0; k < 4; k++) { a[k] = (double)(q[k] / sqrtl(n)); b[k] = (double)(p[k] / sqrtl(n)); }
		Eigen::Quaterniond ea(a[3], a[0], a[1], a[2]), eb(b[3], b[0], b[1], b[2]);
		long double o = exact(a, b), e = fabsl(ea.angularDistance(eb) - o) / o / DBL_EPSILON;
		sum += e;
		largest = e > largest ? e : largest;
	}
	std::printf("angularDistance, rotations 1e-4 rad apart: largest %.0Lf, mean %.0Lf epsilons\n", largest, sum / 20000);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
angularDistance, rotations 1e-4 rad apart: largest 9264, mean 1775 epsilons
```

Expected: as close to the exact angle of the rounded inputs as hypatia gets.

## Cause

[`Geometry/Quaternion.h` lines 764-769](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L764-L769):

```
Quaternion<Scalar> d = (*this) * other.conjugate();
return Scalar(2) * atan2( d.vec().norm(), numext::abs(d.w()) );
```

The vector part of `a b*` is `w_b v_a - w_a v_b - v_a x v_b`: each term is of the size of the
inputs and they cancel to a result 1e-4 times smaller.

## Suggested fix

With `a` and `b` normalized and `b` negated when `a.dot(b) < 0`:
`return 4 * atan2((a.coeffs() - b.coeffs()).norm(), (a.coeffs() + b.coeffs()).norm());`.

## How hypatia does it

`quaternion_angle_between` normalizes both quaternions, picks the sign of `b` that makes
`a . b >= 0` (q and -q are the same rotation), and returns
`4 atan2(|a - b|, |a + b|)`.  For unit quaternions at half-angle t, `|a - b| = 2 sin(t/2)`
and `|a + b| = 2 cos(t/2)`; the differences of the components are exact for nearly equal
quaternions (Sterbenz), so no digits cancel.  If either quaternion has zero length the
result is 0.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

static long double exact(const double a[4], const double b[4])
{
	long double na = 0, nb = 0, s = 0, c = 0, dot = 0;
	int k;
	for (k = 0; k < 4; k++) { na += (long double)a[k] * a[k]; nb += (long double)b[k] * b[k]; dot += (long double)a[k] * b[k]; }
	for (k = 0; k < 4; k++) {
		long double u = a[k] / sqrtl(na), v = (dot < 0 ? -1 : 1) * b[k] / sqrtl(nb);
		s += (u - v) * (u - v);
		c += (u + v) * (u + v);
	}
	return 4 * atan2l(sqrtl(s), sqrtl(c));
}

int main(void)
{
	long double sum = 0, largest = 0;
	int i, k;
	for (i = 0; i < 20000; i++) {
		long double q[4], n = 0, axis[3], m = 0, h, c, d[4], p[4], o, e;
		double a[4], b[4];
		struct quaternion ha, hb;
		for (k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
		for (k = 0; k < 3; k++) { axis[k] = 2 * rnd() - 1; m += axis[k] * axis[k]; }
		h = sinl(0.5e-4L) / sqrtl(m); c = cosl(0.5e-4L);
		d[0] = axis[0] * h; d[1] = axis[1] * h; d[2] = axis[2] * h; d[3] = c;
		p[0] = q[3] * d[0] + q[0] * d[3] + q[1] * d[2] - q[2] * d[1];
		p[1] = q[3] * d[1] - q[0] * d[2] + q[1] * d[3] + q[2] * d[0];
		p[2] = q[3] * d[2] + q[0] * d[1] - q[1] * d[0] + q[2] * d[3];
		p[3] = q[3] * d[3] - q[0] * d[0] - q[1] * d[1] - q[2] * d[2];
		for (k = 0; k < 4; k++) { a[k] = (double)(q[k] / sqrtl(n)); b[k] = (double)(p[k] / sqrtl(n)); }
		quaternion_setf4(&ha, a[0], a[1], a[2], a[3]);
		quaternion_setf4(&hb, b[0], b[1], b[2], b[3]);
		o = exact(a, b);
		e = fabsl(quaternion_angle_between(&ha, &hb) - o) / o / DBL_EPSILON;
		sum += e;
		largest = e > largest ? e : largest;
	}
	printf("quaternion_angle_between, rotations 1e-4 rad apart: largest %.0Lf, mean %.0Lf epsilons\n", largest, sum / 20000);
	return 0;
}
```

```text
quaternion_angle_between, rotations 1e-4 rad apart: largest 4558, mean 256 epsilons
```

## Checking

`compare/check_reports.py docs/reports/eigen/03-angulardistance-small-angles.md` builds
both programs above and compares their output with this report.  The harness:
`compare/results/precision/now.double.md`, `quaternion_angle_between`, "1e-4 rad apart".
Its reference is computed in _Float128 and checked a second way.
