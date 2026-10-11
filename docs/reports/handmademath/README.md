# Reports for HandmadeMath

Tested: HandmadeMath master, commit [661fef0](https://github.com/HandmadeMath/HandmadeMath/tree/661fef0893bccfe30342049e848b8d54e7430234).

| report | kind | status |
|---|---|---|
| [`HMM_NormV3`, `HMM_NormV4` and `HMM_NormQ` give NaN, inf or zero for zero, tiny and huge inputs](01-normalize-zero-tiny-huge.md) | NaN on degenerate input; wrong result | present in the latest release |
| [`HMM_InvQ` is NaN for the zero quaternion, and wrong for tiny and huge ones](02-invq-zero-tiny-huge.md) | NaN on degenerate input; wrong result | present in the latest release |
| [`HMM_QFromVecPair` is NaN for opposite vectors and loses most digits for nearly opposite ones](03-qfromvecpair-opposite.md) | NaN on valid input; precision | present at master (not in a release) |
| [`HMM_RotateV3Q` does not rotate when q is not unit length](04-rotatev3q-not-unit.md) | wrong result for non-unit q | present at master (not in a release) |
| [`HMM_SLerp` uses a normalized lerp up to 0.063 rad: angle errors 120 times the rounding](05-slerp-nlerp-threshold.md) | precision | present in the latest release |
| [`HMM_Rotate_RH` and `HMM_QFromAxisAngle_RH` give NaN for a zero axis](06-zero-axis-nan.md) | NaN on degenerate input | present in the latest release |
| [`HMM_LookAt_RH` gives NaN when the eye is at the target or looks along up](07-lookat-degenerate-nan.md) | NaN on degenerate input | present in the latest release |
