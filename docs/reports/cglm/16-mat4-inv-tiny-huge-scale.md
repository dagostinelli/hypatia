# `glm_mat4_inv` gives inf or 0 for matrices with small or large entries

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_mat4_inv`, `glm_mat3_inv`, `glm_mat2_inv` |
| Kind | wrong result |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the first line prints -nan instead of inf, the second the same |

## Summary

The inverse divides the adjugate by the determinant through its reciprocal, \`1 / det\`.
For a matrix with small entries the determinant underflows and the reciprocal is inf; for
large entries the determinant overflows and the reciprocal is 0.  The inverse itself is in
range in both cases.  s times the 4x4 identity, which has condition number 1, shows it:
its determinant is s^4, so in float the inverse of 1e-10 I is inf instead of 1e10 I, and
the inverse of 1e10 I is 0 instead of 1e-10 I.

## Reproduction

```c
#include <cglm/cglm.h>
#include <stdio.h>

int main(void)
{
	float scales[] = {1e-10f, 1e10f};
	int i;

	for (i = 0; i < 2; i++) {
		mat4 m = GLM_MAT4_IDENTITY_INIT, inverse;

		glm_mat4_scale(m, scales[i]);
		glm_mat4_inv(m, inverse);
		printf("glm_mat4_inv(%g I): m00 = %g\n", scales[i], inverse[0][0]);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, \`-O2\`):

```text
glm_mat4_inv(1e-10 I): m00 = inf
glm_mat4_inv(1e+10 I): m00 = 0
```

Expected: m00 = 1 / s: 1e10 and 1e-10.

## Cause

[`simd/sse2/mat4.h` line 427](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/simd/sse2/mat4.h#L427) (the SSE path used on x86-64):
`x0 = _mm_div_ps(_mm_set1_ps(1.0f), glmm_vhadd(_mm_mul_ps(x0, r0)));`, then every row is
multiplied by `x0`; the scalar path does the same ([`mat4.h` line 689](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/mat4.h#L689)), as do
`glm_mat3_inv` ([`mat3.h` line 364](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/mat3.h#L364)) and `glm_mat2_inv`
([`mat2.h` line 275](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/mat2.h#L275)).

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

## Checking

\`compare/check_reports.py docs/reports/cglm/16-mat4-inv-tiny-huge-scale.md\` builds the programs above and
compares their output with this report.
