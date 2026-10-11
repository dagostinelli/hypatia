# `inverse()` of a 4x4 matrix gives inf or 0 for small or large entries

| | |
|---|---|
| Library | Eigen 3.4.0 (3147391) |
| Function | `MatrixBase::inverse` for `Matrix4f` and `Matrix4d` (`LU/arch/InverseSize4.h`) |
| Kind | wrong result |
| Precision | float and double |
| Status | still present in Eigen 5.0.1 (the latest release) and at master 6bd3136 (2026-10-10): the program prints the same |

## Summary

The inverse divides the adjugate by the determinant through its reciprocal, \`1 / det\`.
For a matrix with small entries the determinant underflows and the reciprocal is inf; for
large entries the determinant overflows and the reciprocal is 0.  The inverse itself is in
range in both cases.  s times the 4x4 identity, which has condition number 1, shows it:
its determinant is s^4, so in float the inverse of 1e-10 I is inf instead of 1e10 I, and
the inverse of 1e10 I is 0 instead of 1e-10 I.  The program shows the same in double for 1e-80 I and 1e80 I.

## Reproduction

```cpp
#include <Eigen/Dense>
#include <cstdio>
#include <initializer_list>

int main()
{
	for (float s : {1e-10f, 1e10f})
		std::printf("(%g I).inverse(), float:  m00 = %g\n", s, (Eigen::Matrix4f::Identity() * s).inverse()(0, 0));
	for (double s : {1e-80, 1e80})
		std::printf("(%g I).inverse(), double: m00 = %g\n", s, (Eigen::Matrix4d::Identity() * s).inverse()(0, 0));
}
```

Output (x86-64, gcc 13.3, \`-O2\`):

```text
(1e-10 I).inverse(), float:  m00 = inf
(1e+10 I).inverse(), float:  m00 = 0
(1e-80 I).inverse(), double: m00 = inf
(1e+80 I).inverse(), double: m00 = 0
```

Expected: m00 = 1 / s: 1e10 and 1e-10, and 1e80 and 1e-80 in double.

## Cause

[`LU/arch/InverseSize4.h` lines 117-121](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/LU/arch/InverseSize4.h#L117-L121) (float):
`Packet4f rd = pdiv(pset1<Packet4f>(1.0f), det);`, and the blocks of the result are multiplied by `rd`
([lines 149-152](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/LU/arch/InverseSize4.h#L149-L152));
the same in double ([lines 271-288](https://gitlab.com/libeigen/eigen/-/blob/3147391d946bb4b6c68edd901f2add6ac1f31f8c/Eigen/src/LU/arch/InverseSize4.h#L271-L288)).

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

\`compare/check_reports.py docs/reports/eigen/10-inverse-tiny-huge-scale.md\` builds the programs above and
compares their output with this report.
