# Reports for GLM

Tested: GLM 1.0.1, commit [0af55cc](https://github.com/g-truc/glm/tree/0af55ccecd98d4e5a8d1fad7de25ba429d60e863).

| report | kind | status |
|---|---|---|
| [`glm::rotation` returns a degenerate quaternion for opposite vectors](01-rotation-opposite-vectors.md) | wrong result | present in the latest release |
| [`glm::rotation` returns the identity for vectors less than sqrt(2 epsilon) apart](02-rotation-small-angle-identity.md) | wrong result (accuracy) | present in the latest release |
| [`glm::rotation` loses most of its digits for nearly opposite vectors](03-rotation-near-opposite-precision.md) | precision | present in the latest release |
| [`glm::axis` returns an axis that is not unit length for small rotations](04-axis-small-rotations.md) | precision | present in the latest release |
| [`glm::quatLookAt` returns a non-rotation when up is nearly parallel to the direction](05-quatlookat-up-nearly-parallel.md) | wrong result | present in the latest release |
| [`q * v` does not rotate when q is not exactly unit length](06-quaternion-times-vector-not-unit.md) | wrong result for non-unit q; precision for drifted q | present in the latest release |
| [`glm::mix` of q and -q returns the zero quaternion](07-mix-opposite-quaternions.md) | wrong result | present in the latest release |
| [`glm::normalize` overflows and underflows: NaN, inf or zero for valid vectors](08-normalize-overflow-underflow.md) | wrong result | present in the latest release |
| [`glm::normalize` of the zero vector is NaN, and the NaN spreads](09-normalize-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm::angle(x, y)` loses its digits for nearly parallel and nearly opposite vectors](10-angle-acos-nearly-parallel.md) | precision | present in the latest release |
| [`glm::proj` onto the zero vector is NaN](11-proj-onto-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm::inverse` of the zero quaternion is NaN](12-inverse-zero-quaternion.md) | NaN on degenerate input | present in the latest release |
| [`glm::lookAt` gives NaN when the eye is at the target or looks along up](13-lookat-degenerate-nan.md) | NaN on degenerate input | present in the latest release |
| [`glm::rotation` with a zero vector is NaN](14-rotation-zero-vector.md) | NaN on degenerate input | present in the latest release |
| [`glm::angleAxis` and `glm::rotate` disagree on axes that are not unit length, and fail on a zero axis](15-axis-angle-zero-or-not-unit-axis.md) | inconsistent API; NaN on degenerate input | present in the latest release |
| [`glm::inverse` of a singular matrix returns NaN with no way to tell](16-inverse-singular-matrix-nan.md) | NaN on degenerate input; no error report | present in the latest release |
| [`glm::normalize` rounds three times instead of twice: more error than dividing by the length](17-normalize-two-roundings.md) | precision | present in the latest release |
| [Smaller precision differences: GLM has 3% to 21% more rounding error in thirteen measurements](18-precision-small-differences.md) | precision | present in the latest release |
| [`glm::normalize` of a tiny quaternion returns the identity, a different rotation; of a huge one, zero](19-normalize-quaternion-tiny-huge.md) | wrong result | present in the latest release |
| [`glm::rotate(q, angle, axis)` skips normalizing axes within 0.001 of unit length](20-rotate-quaternion-axis-threshold.md) | wrong result (accuracy) | present in the latest release |
