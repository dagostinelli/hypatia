# `glm::normalize` rounds twice: about 15% to 75% more error than dividing by the length

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::normalize(vec)`, `glm::normalize(qua)`; through them `glm::rotate`, `glm::reflect` with a normalized normal |
| Kind | precision |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::normalize(v)` multiplies by `inversesqrt(dot(v, v))`: the reciprocal square root is
rounded, then each product is rounded.  Dividing by `sqrt(dot(v, v))` rounds the square
root and the quotient, and the quotient's rounding is the only one that reaches the
result at full size.  Over random vectors the program below measures a mean error of 0.289
epsilons with GLM and 0.250 with division (16% more); the comparison harness, which measures
relative to the largest component, 0.371 against 0.321; for components spread over a wide range 0.185 against
0.131 in double (41%) and 0.148 against 0.085 in float (74%).  For a vector with one
large and two small components, (1, 1e-5, 1e-5), division rounds the large component to
exactly 1 and the mean error in float is 0.0003 epsilons; GLM's product with the rounded
reciprocal gives 0.075.  Quaternion normalization,
`glm::rotate`'s normalized axis and reflection about a normalized normal carry the same
extra error (27%, 21% and 43% in the comparison harness).

## Reproduction

```cpp
// glm::normalize: mean error against long double, in epsilons
#include <glm/glm.hpp>
#include <cfloat>
#include <cmath>
#include <cstdio>

static unsigned long long state = 88172645463325252ULL;
static double rnd() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main()
{
	long double sum = 0;
	for (int i = 0; i < 100000; i++) {
		glm::dvec3 v(20 * rnd() - 10, 20 * rnd() - 10, 20 * rnd() - 10);
		glm::dvec3 g = glm::normalize(v);
		long double n = sqrtl((long double)v.x * v.x + (long double)v.y * v.y + (long double)v.z * v.z);
		long double e = 0;
		for (int k = 0; k < 3; k++)
			e = fmaxl(e, fabsl(g[k] - v[k] / n));
		sum += e / DBL_EPSILON;
	}
	std::printf("glm::normalize: mean error %.3Lf epsilons\n", sum / 100000);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
glm::normalize: mean error 0.289 epsilons
```

Expected: the error of a correctly rounded division, about a third of an epsilon on average.

## Cause

[`detail/func_geometric.inl` line 88](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/func_geometric.inl#L88):
`return v * inversesqrt(dot(v, v));`.  `inversesqrt` is `1 / sqrt(x)`, itself rounded, so
each component is rounded three times (sqrt, reciprocal, product) instead of twice (sqrt,
quotient).

## How hypatia does it

`hyp_normalize`, used by every hypatia normalization, divides each component by the length
(`v[i] /= length`) when the sum of squares is in range, and scales first when it is not
([08](08-normalize-overflow-underflow.md)).

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	long double sum = 0;
	int i, k;
	for (i = 0; i < 100000; i++) {
		struct vector3 v, h;
		long double n, e = 0;
		vector3_setf3(&v, 20 * rnd() - 10, 20 * rnd() - 10, 20 * rnd() - 10);
		vector3_normalize(vector3_set(&h, &v));
		n = sqrtl((long double)v.x * v.x + (long double)v.y * v.y + (long double)v.z * v.z);
		for (k = 0; k < 3; k++)
			e = fmaxl(e, fabsl(h.v[k] - v.v[k] / n));
		sum += e / DBL_EPSILON;
	}
	printf("vector3_normalize: mean error %.3Lf epsilons\n", sum / 100000);
	return 0;
}
```

```text
vector3_normalize: mean error 0.250 epsilons
```

## Suggested fix

`return v / sqrt(dot(v, v));` (or `v / length(v)`).  The division costs a few cycles more
than the multiplication on most hardware; if speed is the reason for `inversesqrt`, an
option (`GLM_FORCE_PRECISE_NORMALIZE`, for example) would let users choose.

## Checking

`compare/check_reports.py docs/reports/glm/17-normalize-two-roundings.md` builds both programs above and compares their output with this report. The harness: `results/precision/now.double.md` and `now.single.md`,
`vector3_normalize`, `quaternion_normalize`, `matrix4_set_from_axisv3_angle` (GLM `rotate`)
and `vector3_reflect` rows.
