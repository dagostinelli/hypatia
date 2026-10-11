# `Quaternion::FromTwoVectors` loses digits for vectors near opposite

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `Quaternion::FromTwoVectors`, `setFromTwoVectors` (`Geometry/Quaternion.h`) |
| Kind | precision |
| Precision | double |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

For unit vectors 1e-3 rad from opposite (outside the SVD branch in double), the general
formula divides `v0.cross(v1)` by `sqrt(2 (1 + c))`.  Both the cross product of nearly
opposite vectors and `1 + c` carry cancelled rounding errors.  The rotation puts `v0` up to
4080 double epsilons away from `v1` in the program below (mean 600); over the 20000 inputs
of the comparison harness the largest is also 4080 (mean 640).  On random inputs the
largest is 116 epsilons.  hypatia's largest is 1.8 for the same inputs (2.2 in the harness,
for both).

## Reproduction

```cpp
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cfloat>
#include <cmath>
#include <cstdio>

static unsigned long long state = 88172645463325252ULL;
static double rnd() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

/* how far the rotation by q (normalized) puts unit f from unit t, in long double */
static long double landing(const double f[3], const double t[3], const double q[4] /* x y z w */)
{
	long double n = sqrtl((long double)q[0] * q[0] + (long double)q[1] * q[1] + (long double)q[2] * q[2] + (long double)q[3] * q[3]);
	long double u[3] = {q[0] / n, q[1] / n, q[2] / n}, w = q[3] / n, nf = 0, nt = 0, e = 0;
	for (int i = 0; i < 3; i++) { nf += (long double)f[i] * f[i]; nt += (long double)t[i] * t[i]; }
	long double v[3] = {f[0] / sqrtl(nf), f[1] / sqrtl(nf), f[2] / sqrtl(nf)};
	long double c[3] = {u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
	long double cc[3] = {u[1] * c[2] - u[2] * c[1], u[2] * c[0] - u[0] * c[2], u[0] * c[1] - u[1] * c[0]};
	for (int i = 0; i < 3; i++) { long double r = v[i] + 2 * (w * c[i] + cc[i]) - t[i] / sqrtl(nt); e += r * r; }
	return sqrtl(e);
}

/* a unit vector, and one at angle a from its opposite, both rounded to double */
static void near_opposite(double f[3], double t[3], long double a)
{
	long double v[3], p[3], n = 0, m = 0;
	for (int i = 0; i < 3; i++) { v[i] = 2 * rnd() - 1; n += v[i] * v[i]; }
	for (int i = 0; i < 3; i++) v[i] /= sqrtl(n);
	p[0] = v[1]; p[1] = -v[0]; p[2] = 0;                  /* perpendicular to v */
	for (int i = 0; i < 3; i++) m += p[i] * p[i];
	for (int i = 0; i < 3; i++) { f[i] = (double)v[i]; t[i] = (double)(-v[i] * cosl(a) + p[i] / sqrtl(m) * sinl(a)); }
}

int main()
{
	long double largest = 0, sum = 0;
	for (int i = 0; i < 2000; i++) {
		double f[3], t[3];
		near_opposite(f, t, 1e-3L);
		Eigen::Quaterniond e = Eigen::Quaterniond::FromTwoVectors(Eigen::Vector3d(f[0], f[1], f[2]), Eigen::Vector3d(t[0], t[1], t[2]));
		double q[4] = {e.x(), e.y(), e.z(), e.w()};
		long double err = landing(f, t, q) / DBL_EPSILON;
		largest = err > largest ? err : largest;
		sum += err;
	}
	std::printf("FromTwoVectors, 1e-3 rad from opposite: largest %.0Lf, mean %.0Lf epsilons\n", largest, sum / 2000);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
FromTwoVectors, 1e-3 rad from opposite: largest 4079, mean 602 epsilons
```

Expected: a few epsilons.

## Cause

[`Geometry/Quaternion.h` lines 665-669](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/Geometry/Quaternion.h#L665-L669):

```
Vector3 axis = v0.cross(v1);
Scalar s = sqrt((Scalar(1)+c)*Scalar(2));
Scalar invs = Scalar(1)/s;
this->vec() = axis * invs;
```

## Suggested fix

As in [01](01-fromtwovectors-opposite.md): the half angle from `(v0 + v1).norm()` and
`(v0 - v1).norm()`, the axis from `v0.cross(v0 + v1)`.  This removes the branch on
`dummy_precision` as well.

## How hypatia does it

`quaternion_get_rotation_tov3` normalizes both vectors and takes the half angle from two
lengths that stay accurate for any angle: for unit vectors at angle a,
|f + t| = 2 cos(a/2) and |f - t| = 2 sin(a/2).  The axis is f x (f + t), which has the
direction of f x t but stays accurate as the vectors become opposite.  There is no
threshold: only an exactly zero axis (vectors exactly parallel or opposite after rounding)
is a special case.  The result is normalized, so it is always a unit quaternion.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

/* how far the rotation by q (normalized) puts unit f from unit t, in long double */
static long double landing(const double f[3], const double t[3], const double q[4] /* x y z w */)
{
	long double n = sqrtl((long double)q[0] * q[0] + (long double)q[1] * q[1] + (long double)q[2] * q[2] + (long double)q[3] * q[3]);
	long double u[3] = {q[0] / n, q[1] / n, q[2] / n}, w = q[3] / n, nf = 0, nt = 0, e = 0;
	for (int i = 0; i < 3; i++) { nf += (long double)f[i] * f[i]; nt += (long double)t[i] * t[i]; }
	long double v[3] = {f[0] / sqrtl(nf), f[1] / sqrtl(nf), f[2] / sqrtl(nf)};
	long double c[3] = {u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
	long double cc[3] = {u[1] * c[2] - u[2] * c[1], u[2] * c[0] - u[0] * c[2], u[0] * c[1] - u[1] * c[0]};
	for (int i = 0; i < 3; i++) { long double r = v[i] + 2 * (w * c[i] + cc[i]) - t[i] / sqrtl(nt); e += r * r; }
	return sqrtl(e);
}

/* a unit vector, and one at angle a from its opposite, both rounded to double */
static void near_opposite(double f[3], double t[3], long double a)
{
	long double v[3], p[3], n = 0, m = 0;
	for (int i = 0; i < 3; i++) { v[i] = 2 * rnd() - 1; n += v[i] * v[i]; }
	for (int i = 0; i < 3; i++) v[i] /= sqrtl(n);
	p[0] = v[1]; p[1] = -v[0]; p[2] = 0;                  /* perpendicular to v */
	for (int i = 0; i < 3; i++) m += p[i] * p[i];
	for (int i = 0; i < 3; i++) { f[i] = (double)v[i]; t[i] = (double)(-v[i] * cosl(a) + p[i] / sqrtl(m) * sinl(a)); }
}

int main(void)
{
	long double largest = 0, sum = 0;
	int i;
	for (i = 0; i < 2000; i++) {
		double f[3], t[3];
		struct vector3 from, to;
		struct quaternion h;
		near_opposite(f, t, 1e-3L);
		vector3_setf3(&from, f[0], f[1], f[2]);
		vector3_setf3(&to, t[0], t[1], t[2]);
		quaternion_get_rotation_tov3(&from, &to, &h);
		{
			double q[4] = {h.x, h.y, h.z, h.w};
			long double e = landing(f, t, q) / DBL_EPSILON;
			largest = e > largest ? e : largest;
			sum += e;
		}
	}
	printf("quaternion_get_rotation_tov3, 1e-3 rad from opposite: largest %.1Lf, mean %.2Lf epsilons\n", largest, sum / 2000);
	return 0;
}
```

```text
quaternion_get_rotation_tov3, 1e-3 rad from opposite: largest 1.8, mean 0.32 epsilons
```

## Checking

`compare/check_reports.py docs/reports/eigen/02-fromtwovectors-near-opposite-precision.md`
builds both programs above and compares their output with this report.  The harness:
`compare/results/precision/now.double.md`, `quaternion_get_rotation_tov3`, "1e-3 rad from
opposite" and "random".
