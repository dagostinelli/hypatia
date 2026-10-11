# `glm::inverse` gives inf or 0 for matrices with small or large entries

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `inverse` for `mat2`, `mat3`, `mat4` (`detail/func_matrix.inl`) |
| Kind | wrong result |
| Precision | float and double |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

The inverse divides the adjugate by the determinant through its reciprocal, \`1 / det\`.
For a matrix with small entries the determinant underflows and the reciprocal is inf; for
large entries the determinant overflows and the reciprocal is 0.  The inverse itself is in
range in both cases.  s times the 4x4 identity, which has condition number 1, shows it:
its determinant is s^4, so in float the inverse of 1e-10 I is inf instead of 1e10 I, and
the inverse of 1e10 I is 0 instead of 1e-10 I.  The program shows the same in double for 1e-80 I and 1e80 I.

## Reproduction

```cpp
#include <glm/glm.hpp>
#include <cstdio>
#include <initializer_list>

int main()
{
	for (float s : {1e-10f, 1e10f})
		std::printf("glm::inverse(%g I), float:  m00 = %g\n", s, glm::inverse(glm::mat4(s))[0][0]);
	for (double s : {1e-80, 1e80})
		std::printf("glm::inverse(%g I), double: m00 = %g\n", s, glm::inverse(glm::dmat4(s))[0][0]);
}
```

Output (x86-64, gcc 13.3, \`-O2\`):

```text
glm::inverse(1e-10 I), float:  m00 = inf
glm::inverse(1e+10 I), float:  m00 = 0
glm::inverse(1e-80 I), double: m00 = inf
glm::inverse(1e+80 I), double: m00 = 0
```

Expected: m00 = 1 / s: 1e10 and 1e-10, and 1e80 and 1e-80 in double.

## Cause

[`detail/func_matrix.inl` lines 401-403](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/func_matrix.inl#L401-L403):
`T OneOverDeterminant = static_cast<T>(1) / Dot1; return Inverse * OneOverDeterminant;`;
the same for `mat2` ([line 307](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/func_matrix.inl#L307)) and `mat3` ([line 326](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/func_matrix.inl#L326)).

## Suggested fix

Scale the matrix by a power of two so that its largest entry is near 1, invert, and scale
the inverse by the same power of two: the powers of two are exact, and the determinant of
the scaled matrix stays in range whenever the inverse is.

## How hypatia does it

\`matrix4_inverse\` (and \`matrix2_inverse\`, \`matrix3_inverse\`) scale the matrix by a power
of two so that its largest entry is at least 1 and less than 2, invert it, dividing by the
determinant rather than multiplying by its reciprocal, and scale the result back.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float scales[] = {1e-10f, 1e10f};
	int i;

	for (i = 0; i < 2; i++) {
		struct matrix4 m, inverse;

		matrix4_multiplyf(matrix4_identity(&m), scales[i]);
		matrix4_inverse(&m, &inverse);
		printf("matrix4_inverse(%g I): m00 = %g\n", scales[i], inverse.m[0]);
	}
	return 0;
}
```

```text
matrix4_inverse(1e-10 I): m00 = 1e+10
matrix4_inverse(1e+10 I): m00 = 1e-10
```

In double:

```c
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	double scales[] = {1e-80, 1e80};
	int i;

	for (i = 0; i < 2; i++) {
		struct matrix4 m, inverse;

		matrix4_multiplyf(matrix4_identity(&m), scales[i]);
		matrix4_inverse(&m, &inverse);
		printf("matrix4_inverse(%g I): m00 = %g\n", scales[i], inverse.m[0]);
	}
	return 0;
}
```

```text
matrix4_inverse(1e-80 I): m00 = 1e+80
matrix4_inverse(1e+80 I): m00 = 1e-80
```

## Checking

\`compare/check_reports.py docs/reports/glm/21-inverse-tiny-huge-scale.md\` builds the programs above and
compares their output with this report.
