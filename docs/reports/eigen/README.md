# Reports for Eigen

Tested: Eigen 3.4.0, commit [3147391](https://gitlab.com/libeigen/eigen/tree/3147391d946bb4b6c68edd901f2add6ac1f31f8c).

| report | kind | status |
|---|---|---|
| [`Quaternion::FromTwoVectors` is accurate to only sqrt(epsilon) for opposite vectors](01-fromtwovectors-opposite.md) | precision | present in the latest release |
| [`Quaternion::FromTwoVectors` loses digits for vectors near opposite](02-fromtwovectors-near-opposite-precision.md) | precision | present in the latest release |
| [`Quaternion::angularDistance` loses digits for nearly equal rotations](03-angulardistance-small-angles.md) | precision | present in the latest release |
| [`normalized()` returns tiny vectors unchanged and huge vectors as zero](04-normalized-underflow-overflow.md) | wrong result | present in the latest release |
| [`q * v` does not rotate when q is not exactly unit length](05-quaternion-times-vector-not-unit.md) | wrong result for non-unit q; precision for drifted q | present in the latest release |
| [`Quaternion::FromTwoVectors` with a zero vector returns a quaternion of length 0.71](06-fromtwovectors-zero-vector.md) | wrong result on degenerate input | present in the latest release |
| [`AngleAxis` with an axis that is not unit length gives a scaled quaternion and a non-rotation matrix](07-angleaxis-axis-not-unit.md) | documented precondition, not checked | present in the latest release |
| [Smaller precision differences: Eigen has 2% to 10% more rounding error in seven measurements of six functions](08-precision-small-differences.md) | precision | present in the latest release |
| [`Quaternion::inverse()` returns zero for tiny and huge quaternions, which have inverses](09-quaternion-inverse-tiny-huge.md) | wrong result | present in the latest release |
