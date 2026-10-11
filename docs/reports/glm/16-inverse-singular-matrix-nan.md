# `glm::inverse` of a singular matrix returns NaN with no way to tell

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::inverse(mat4)` (`detail/func_matrix.inl`) |
| Kind | NaN on degenerate input; no error report |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

For a singular matrix `glm::inverse` divides by a zero determinant and returns a matrix of
NaN.  The caller has no indication other than checking every element; computing
`determinant` first costs a second evaluation and is not the same arithmetic.

## Reproduction

```cpp
#include <glm/glm.hpp>
#include <cmath>
#include <cstdio>

int main()
{
	glm::dmat4 m(1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11, 12,  13, 14, 15, 16);  // rank 2
	glm::dmat4 inv = glm::inverse(m);
	std::printf("determinant = %g\n", glm::determinant(m));
	int nans = 0;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			nans += std::isnan(inv[c][r]);
	std::printf("inverse[0] = (%g, %g, %g, %g), %d of 16 entries NaN\n", inv[0][0], inv[0][1], inv[0][2], inv[0][3], nans);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
determinant = 0
inverse[0] = (-nan, -nan, -nan, -nan), 16 of 16 entries NaN
```

Expected: a reported failure.

## Cause

[`detail/func_matrix.inl` lines 401-403](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/func_matrix.inl#L401-L403):
`T OneOverDeterminant = static_cast<T>(1) / Dot1; return Inverse * OneOverDeterminant;`
with no check.

## Suggested fix

Return a success flag (an overload with an out parameter, or `bool inverse(m, &result)`),
or document that a singular input gives NaN.

## How hypatia does it

`matrix4_inverse(self, result)` returns NULL, and leaves `result` unchanged, when the
determinant is exactly zero (`matrix4_invert` does the same in place).  Any other matrix is
inverted; `matrix4_reciprocal_condition` estimates how reliable the inverse of a nearly
singular matrix is.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct matrix4 m;
	struct matrix4 inverse;
	int i;

	for (i = 0; i < 16; i++) {
		m.m[i] = i + 1;
	}
	printf("determinant = %g\n", matrix4_determinant(&m));
	printf("matrix4_inverse returned %s\n", matrix4_inverse(&m, &inverse) == NULL ? "NULL" : "a matrix");
	return 0;
}
```

```text
determinant = 0
matrix4_inverse returned NULL
```

## Checking

`compare/check_reports.py docs/reports/glm/16-inverse-singular-matrix-nan.md` builds both
programs above and compares their output with this report.
