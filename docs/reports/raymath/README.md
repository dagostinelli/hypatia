# Reports for raymath

Tested: raymath of raylib 6.0, commit [dbc56a8](https://github.com/raysan5/raylib/tree/dbc56a87da87d973a9c5baa4e7438a9d20121d28).

| report | kind | status |
|---|---|---|
| [`Vector3Normalize` and `QuaternionNormalize` return tiny inputs unchanged and huge ones as zero](01-normalize-tiny-huge.md) | wrong result | present in the latest release |
| [`QuaternionInvert` gives inf and NaN for tiny quaternions and zero for huge ones](02-quaternioninvert-tiny-huge.md) | wrong result | present in the latest release |
| [`Vector3Project` onto the zero vector is NaN](03-project-onto-zero.md) | NaN on degenerate input | present in the latest release |
| [`Vector3RotateByQuaternion` and `QuaternionToMatrix` disagree, and neither rotates, when q is not unit length](04-rotate-by-quaternion-not-unit.md) | wrong result for non-unit q | present in the latest release |
| [`QuaternionFromVector3ToVector3` returns the zero quaternion for opposite vectors and loses digits for nearly opposite ones](05-from-vector3-to-vector3-opposite.md) | wrong result; precision | present in the latest release |
| [`QuaternionSlerp` uses a normalized lerp up to 0.64 rad: angle errors up to 9e-4 rad](06-slerp-nlerp-threshold.md) | wrong result (accuracy) | present in the latest release |
| [`QuaternionToAxisAngle` is inaccurate for small rotations: 2% error at 1e-3 rad](07-to-axis-angle-small-rotations.md) | precision | present in the latest release |
| [`MatrixRotate` with a zero axis returns cos(angle) times the identity](08-matrix-rotate-zero-axis.md) | wrong result on degenerate input | present in the latest release |
| [`MatrixInvert` gives inf or 0 for matrices with small or large entries](09-matrixinvert-tiny-huge-scale.md) | wrong result | present in the latest release |
| [`QuaternionSlerp` returns its first argument when the dot product rounds to 1](10-slerp-returns-first.md) | wrong result (accuracy) | present in the latest release |
