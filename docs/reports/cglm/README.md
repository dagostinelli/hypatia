# Reports for cglm

Tested: cglm 0.9.4, commit [1796cc5](https://github.com/recp/cglm/tree/1796cc5ce298235b615dc7a4750b8c3ba56a05dd).

| report | kind | status |
|---|---|---|
| [`glm_vec3_refract` has a sign error: it gives the refracted direction only at normal incidence](01-refract-sign-error.md) | wrong result | fixed upstream |
| [`glm_quat_slerp` returns a near-zero quaternion for nearly equal rotations of opposite sign](02-slerp-fallback-ignores-sign.md) | wrong result | present in the latest release |
| [`glm_quat_slerp` returns its first argument when the dot product rounds to 1](03-slerp-small-angles.md) | precision | present in the latest release |
| [`glm_vec3_normalize` sets every vector shorter than 1.19e-7 to zero, and long vectors overflow](04-normalize-zeroes-short-vectors.md) | wrong result | present in the latest release |
| [`glm_quat_from_vecs` returns the identity for vectors up to 0.26 degrees apart, and a wrong half turn near opposite](05-quat-from-vecs-thresholds.md) | wrong result | present in the latest release |
| [`glm_rotate_make` with a zero (or short) axis returns a matrix that is not a rotation](06-rotate-make-zero-axis.md) | wrong result on degenerate input | present in the latest release |
| [`glm_quatv` with a zero (or short) axis returns a quaternion of length cos(angle/2)](07-quatv-zero-axis.md) | wrong result on degenerate input | present in the latest release |
| [`glm_quat_for` returns a quaternion of length 0.71 when up is parallel to the direction](08-quat-for-up-parallel.md) | wrong result on degenerate input | present in the latest release |
| [`glm_vec3_angle` returns 0 for vectors up to about 6e-4 rad apart, and NaN for a zero vector](09-vec3-angle-acos.md) | precision; NaN on degenerate input | present in the latest release |
| [`glm_vec3_proj` onto the zero vector is NaN](10-proj-onto-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm_quat_inv` of the zero quaternion is NaN; tiny and huge quaternions overflow](11-quat-inv-zero.md) | NaN on degenerate input | present in the latest release |
| [Smaller precision differences: cglm has more rounding error than hypatia in seventeen measurements](12-precision-small-differences.md) | precision | present in the latest release |
| [`glm_quat_normalize` turns a tiny quaternion into the identity, a different rotation, and a huge one into zero](13-quat-normalize-tiny-huge.md) | wrong result | present in the latest release |
| [`glm_vec4_normalize` on SSE sets every vector shorter than 3.45e-4 to zero](14-vec4-normalize-sse-threshold.md) | wrong result | present in the latest release |
