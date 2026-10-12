[![CI](https://github.com/dagostinelli/hypatia/actions/workflows/ci.yml/badge.svg)](https://github.com/dagostinelli/hypatia/actions/workflows/ci.yml)



Hypatia
=======

Quick Install
----------
The fastest way to get started is to grab the header file. It's self-contained.  The file is called `hypatia.h`

About
-----
Hypatia, a Greek mathematician, 355-415 C.E. Considered by many to be the first female mathematician of note.

Hypatia is a single-file-header, pure-C math library.  It is almost 100% C89/C90 compliant.  This library is intended for use in 2d/3d graphics program (such as games).  Since it is not a general purpose math library, but a library meant for 3d graphics, certain opinions have been expressed in its design.  One of those design choices, intended to help with speed, is that all objects (quaternions, matrices, vectors) are mutable.  (That means that the objects change their values.)  This was a purposeful design choice. Construct your program around this choice.

With CMake, either add the source tree to your project:

```
add_subdirectory(hypatia)
target_link_libraries(your_program PRIVATE hypatia::hypatia)
```

or install it (`cmake -B build && cmake --install build`) and use `find_package(hypatia 2.0 REQUIRED)` with the same `target_link_libraries` line.  The target adds the include directory and links the C math library where needed.

Quick Start
----------
The entire library is self-contained in this one header file.  The file can be used in a header mode or implementation mode.  The header mode is used by default and is what you are using when you simply `#include` this file.  It does not compile-in the actual implementation.  To compile-in the actual implementation, you need to also use implementation mode.  The implementation mode requires the macro `HYPATIA_IMPLEMENTATION` exist in one .c/.cpp file in your project before an `#include <hypatia.h>`.

Like so:

```
#define HYPATIA_IMPLEMENTATION
#include <hypatia.h>
```

No build system is needed.  On Linux and other Unix systems, link the C math library, for example `cc example.c -lm`.

Basic Usage
-----------
```
#include <assert.h>
#include <stdio.h>

#define HYPATIA_IMPLEMENTATION
#include "hypatia.h"

int main(void)
{
	struct vector3 a;
	struct vector3 b;
	struct vector3 r;

	printf("Using Hypatia Version:%s\n", HYPATIA_VERSION);

	vector3_setf3(&a, HYP_FLOAT_C(3.0), -HYP_FLOAT_C(3.0), HYP_FLOAT_C(1.0));
	vector3_setf3(&b, HYP_FLOAT_C(4.0), HYP_FLOAT_C(9.0), HYP_FLOAT_C(2.0));

	vector3_cross_product(&r, &a, &b);

	assert(scalar_equalsf(r.x, -HYP_FLOAT_C(15.0)));
	assert(scalar_equalsf(r.y, -HYP_FLOAT_C(2.0)));
	assert(scalar_equalsf(r.z, HYP_FLOAT_C(39.0)));

	return 0;
}

```

Configuration
-------------
Define these before including `hypatia.h`.  Use the same definitions in every file that
includes it, except `HYPATIA_IMPLEMENTATION` and `HYPAPI`.

**Building**

| Macro | Effect | Default |
|---|---|---|
| `HYPATIA_IMPLEMENTATION` | Compiles the implementation into this file.  Define it in exactly one file. | |
| `HYP_STATIC` | Makes every function `static` (and inline), so each file that includes the implementation gets its own private copy. | |
| `HYPAPI` | The linkage of every function.  For a Windows DLL: `__declspec(dllexport)` when building it, `__declspec(dllimport)` when using it. | empty |
| `HYP_INLINE` | The inline keyword for the small helper functions. | `__inline` (MSVC), `__inline__` |

**Precision**

| Macro | Effect | Default |
|---|---|---|
| `HYPATIA_SINGLE_PRECISION_FLOATS` | `HYP_FLOAT` is `float` instead of `double`. | |
| `HYPATIA_LONG_DOUBLE_PRECISION_FLOATS` | `HYP_FLOAT` is `long double` instead of `double`.  Needs the C99 math functions (`sinl`, `cosl`, ...).  On MSVC and on macOS arm64, `long double` is the same as `double`. | |
| `HYPATIA_QUAD_PRECISION_FLOATS` | `HYP_FLOAT` is IEEE binary128 (emulated in software on most machines) instead of `double`, in the spelling the compiler has.  GCC 7 or later with glibc 2.26 or later: `_Float128` and `sinf128`, ...; nothing extra to link.  A 113-bit `long double` (AArch64 Linux, any compiler): `long double` and `sinl`, ... (C99); nothing extra to link.  Clang, MinGW GCC, or GCC with an older C library, on x86-64: `__float128` and `sinq`, ... from libquadmath; link with `-lquadmath`.  With GCC's `_Float128`, `hypatia.h` defines `__STDC_WANT_IEC_60559_TYPES_EXT__` before it includes `<math.h>`, and declares `sinf128` and the others itself when a file included `<math.h>` first.  To use another 128-bit type, define `HYP_FLOAT`, `HYP_FLOAT_C` and the math macros. | |
| `HYP_FAST` | Plain formulas instead of exact arithmetic: normalize, the lengths and angles, slerp, the rotation functions and the inverses do 2.5 to 10 times less work.  Every input still has a defined result, and the robust formulas stay (atan2, no cancellation near parallel or opposite vectors).  What goes is the last ulp: errors are about 1 to 2 ulps instead of half an ulp, and a tiny angle between vectors of different lengths is accurate in absolute terms, not to the last digit.  The measured errors are in test/precision/baseline_fast_single.txt and baseline_fast_double.txt. | |
| `HYP_EPSILON` | The tolerance of the `*_equals` functions: absolute for values up to 1 in size, relative above 1. | `1E-5` |

**Standard library**

| Macro | Effect | Default |
|---|---|---|
| `HYP_NO_STDIO` | Leaves out `<stdio.h>` and the `hyp_*_print` debug functions. | |
| `HYP_NO_C_MATH` | Leaves out `<math.h>`.  Then define all eight math macros below. | |
| `HYP_SQRT`, `HYP_FMOD`, `HYP_SIN`, `HYP_COS`, `HYP_TAN`, `HYP_ASIN`, `HYP_ACOS`, `HYP_ATAN2` | The math functions. | `sqrt`, `fmod`, ... from `<math.h>`, converted to `HYP_FLOAT`; `sqrtl`, `fmodl`, ... with `HYPATIA_LONG_DOUBLE_PRECISION_FLOATS`; `sqrtf128`, `sqrtl` or `sqrtq`, ... with `HYPATIA_QUAD_PRECISION_FLOATS` |
| `HYP_MEMSET(a, b, c)` | Replaces `memset`. | `memset` from `<string.h>` |
| `HYP_RANDOM()`, `HYP_RANDOM_MAX` | The random number source of `scalar_random_rangef` and the `*_set_random_unit` functions: `HYP_RANDOM()` returns an integer from 0 to `HYP_RANDOM_MAX`; define both or neither. | `rand()`, `RAND_MAX` |

**Projections**

| Macro | Effect | Default |
|---|---|---|
| `HYP_DEPTH_MINUS_ONE_TO_ONE` | The projection functions map depth to -1..1 (OpenGL) instead of 0..1 (Direct3D, Vulkan, Metal). | |

**Deprecation**

| Macro | Effect | Default |
|---|---|---|
| `HYP_NO_DEPRECATED` | Leaves out the deprecated macros (`HYP_RANDOM_FLOAT`, `HYP_PIOVER180`, `HYP_PIUNDER180`, and the `_EXP` names of the functions that left the experimental group), to check that code no longer uses them. | |

Changes in 2.1
--------------
Results that differ from 2.0:

* The matrix4 rotations (`matrix4_make_transformation_rotationf_x/_y/_z`, `_rotationq`, `matrix4_rotatev3`) follow the right-hand rule; in 2.0 they turned the other way.
* matrix2 and matrix3 are applied as M * v, like matrix4; the elements of their rotation matrices are transposed from 2.0.
* `vector3_rotate_by_quaternion` was wrong for vectors not perpendicular to the axis.
* `matrix4_multiplyv2` treats z as 0.
* `quaternion_get_euler_anglesf3` takes each angle from one atan2: accurate near a pitch of +-pi/2, where the asin it used lost half the digits, and correct for quaternions of any length.
* `quaternion_get_rotation_tov3` returns a unit quaternion for vectors of any length; `quaternion_get_axis_anglev3` is accurate for small rotations and returns a unit axis and an angle in [0, pi].
* Normalizing leaves only an exactly zero vector unchanged (2.0: anything shorter than 1e-5); `vector2_normalize` no longer gives NaN.
* `quaternion_inverse`, `quaternion_lerp` and `quaternion_slerp` have no shortcuts near 0 or 1, and slerp stays accurate for nearly equal quaternions; with a negative dot product slerp ends at -end, the same rotation as end.
* `quaternion_is_pure` compares w relative to the length of the quaternion.
* `scalar_equalsf` and the `*_equals` functions compare relative to the size of the values above 1 (2.0: always the absolute difference, so large values one rounding apart could differ), and equal infinities are equal.
* `matrix2/3/4_inverse` return NULL only for an exactly zero determinant, and `matrix2/3/4_invert` return NULL when there is no inverse.
* `matrix2/3/4_inverse` invert matrices with very small or very large entries whenever the inverse is in range: they scale the matrix by a power of two first, so the determinant does not underflow or overflow.
* Normalizing is correctly rounded: each component is the exact quotient rounded once (within half an ulp), for vectors of any length.  It uses exact products (Dekker's splitting), which costs more arithmetic than a plain square root and division.
* `vector2_angle_between` and `vector3_angle_between` take the angle from the vectors as given, without normalizing them, in the same way: within an ulp also for nearly parallel vectors.
* `quaternion_angle_between` and `quaternion_slerp` take the angle from the quaternions as given, without normalizing them, with exact products and sums: within an ulp also for nearly equal rotations.  `quaternion_slerp` combines its two terms with one rounding.
* `quaternion_get_rotation_tov3` takes the axis of nearly equal vectors from their difference; `quaternion_get_axis_anglev3` divides the axis by the same length that gives the angle; `quaternion_set_from_axis_anglev3` no longer normalizes its result a second time.
* The 2x2 minors in the determinants, the inverses and `matrix4_normal_matrix` are rounded once each.
* `matrix4_inverse` and `matrix4_determinant` are as accurate as GLM and Eigen for badly conditioned matrices (2.0 lost up to 100 times more).
* `vector2_angle_between` and `vector3_angle_between` are accurate for parallel and nearly parallel vectors (2.0 could give NaN) and give 0 for a zero vector.
* `vector3_rotate_by_quaternion` rotates by a quaternion of any length without scaling the vector; the axis-angle functions accept an axis of any length.
* `HYP_WRAP` wraps into [start, limit) for any start.
* The former `_EXP` functions: perspective, ortho and lookat give correct projections; the Euler angle functions rotate about X, then Y, then Z; `matrix4_multiplyv3_EXP` is `matrix4_multiplyv3`; `quaternion_angle_between` and `quaternion_difference` treat q and -q as the same rotation; `matrix4_transformation_decompose` returns the rotation.
* `quaternion_axis_between_EXP` and `quaternion_cross_product_EXP` are deprecated: they are not the operations their names describe (`HYP_NO_DEPRECATED` removes them).

Documentation
-------------
A great way to learn how to use the library is to review the
[unit tests](https://github.com/dagostinelli/hypatia/tree/master/test "Unit Tests").

View the official [documentation](http://dagostinelli.github.io/hypatia/) here.

Can I trust this math library?
------------------------------
A goal of the unit tests is to test each function against HYP_EPSILON which is defined in hypatia.h, currently as 1E-5.  A number of functions do not yet have unit tests proving 1E-5, but more are coming.

Compared with other math libraries for graphics, the concrete differences are:

* quaternions that are not unit length still rotate correctly, because hypatia divides by |q|²;
* small angles stay accurate in the angle-between, axis-angle and slerp functions;
* very large and very small values don't break normalize or inverse;
* double, long double and quad precision modes are available.

The unit tests answer "is it right?"  They check the conventions (rotation direction, multiplication order, handedness, depth range), the edge cases (zero vectors, opposite vectors, matrices with no inverse) and that every compiler and platform in CI gets the same results.  What they don't catch is a function getting less accurate.  An error that grows from half an ulp to fifty ulps is still well inside 1E-5.

The precision gate answers "how close is it?"  test/precision/precision.c measures the error of each function against `_Float128` references, on random inputs and on the hard cases (nearly parallel vectors, small angles, badly conditioned matrices).  The results are kept in test/precision/baseline_single.txt and baseline_double.txt.  CI fails a change that makes any mean error more than 1% worse, or any largest error more than 10% worse, in the default mode or with `HYP_FAST` (baseline_fast_single.txt and baseline_fast_double.txt).  If a change improves a number, update the baseline in the same commit.  The gate only runs on GCC/Linux, and it doesn't know about conventions, so the gates need each other.

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -DHYPATIA_PRECISION_CHECK=ON
cmake --build build --target precision_single precision_double
ctest --test-dir build -R precision_check --output-on-failure
build/test/precision_double > test/precision/baseline_double.txt   # to update a baseline
```

The speed gate answers "how much work is it?"  test/speed/speed.c calls each function on a fixed set of inputs, and test/speed/speed.sh counts the instructions per call with valgrind.  It counts instructions rather than timing, because a time on a shared CI runner can be off by tens of percent, while the count comes out the same on every run.  CI fails a change that makes any function take more than 2% more instructions than test/speed/baseline_single.txt or baseline_double.txt.  As with precision, update the baseline in the same commit when a number gets better, or when a change is meant to buy precision with speed (say so in the commit).

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -DHYPATIA_SPEED_CHECK=ON
cmake --build build --target speed_single speed_double
ctest --test-dir build -R speed_check --output-on-failure
sh test/speed/speed.sh build/test/speed_double > test/speed/baseline_double.txt   # to update a baseline
```

None of the gates checks API compatibility, or measures the precision of the long double and quad modes.

Parts of this library, its tests and documentation were written with AI assistance (Claude) under the maintainer's direction and review.

A word about convention
-----------------------

Hypatia uses verbose names. In pure-C code, math-related function names seem to end up either cryptic (m4mul), verbose (matrix4_multiplym4) or ambiguous (multiply).  C++ is a little better in this respect, because there is operator and function overloading (gracefully allows for ambiguous names).  When Hypatia was shown around before its release, the chief complaint was "it has verbose names".

As an experiment, some \#defines have been added to alias the verbose names. (mat4, vec3, vec4, quat, etc)  At this point, the primary API is the verbose names and the experimental API has some of the shorter, cryptic names. In fact, only a small portion of the entire API has been aliased in this way.  My intention to keep one and toss the other. I would like your feedback about that.

Conventions:

* Rotations follow the right-hand rule: a positive angle turns counterclockwise when the axis points toward the viewer.  Angles are in radians.
* Most functions change their first argument in place and return it, so calls can be chained; the others write their result to a parameter named `vR`, `mR` or `qR`.
* A matrix is applied to a vector as M * v (`matrix4_multiplyv3`, `matrix3_multiplyv2`, `vector3_multiplym4`, ...).  `rRC` is row R, column C, and `cCR` is the same element named column first; `m[]` numbers the elements row by row.  Translation is in the last column (`r03`, `r13`, `r23` in a matrix4; `r02`, `r12` in a matrix3).
* `matrixN_multiply(self, mT)` sets self to mT * self: mT is applied after self.
* The `_rh` projection and view functions are right-handed (the camera looks down -Z) and the `_lh` ones left-handed (+Z).  Depth maps to 0 at the near plane and 1 at the far plane, or -1 to 1 with `HYP_DEPTH_MINUS_ONE_TO_ONE`.
* Quaternions are (x, y, z, w) with w the real part.  A unit quaternion is a rotation, and q and -q are the same rotation.

New in 2.1:

* Long double precision: `HYPATIA_LONG_DOUBLE_PRECISION_FLOATS`.
* Quad precision: `HYPATIA_QUAD_PRECISION_FLOATS`, with `_Float128` (GCC), a 113-bit `long double` (AArch64) or `__float128` and libquadmath (Clang, MinGW).
* A fast mode: `HYP_FAST`.

Coding Standard
---------------

* Mathematical correctness comes first; a design choice that conflicts with it is a bug
* Ensure C89/C90 compatibility (exceptions: the anonymous unions in the types, <stdint.h> / uint8_t, the C99 math functions in long double mode, and the 128-bit type with its literals and math functions in quad mode)
* Warnings are errors; fix the code, never turn a warning off
* Check that the coding style is consistent with the rest of the codebase.
	- curly brace placement
	- indent with tabs
	- lower case function names for public-facing functions
	- all CAPS for public constants.  HYP is the prefix for public constants
	- use 'self' for describing the function context
	- math entities are mutable
	- prefer no casts; use HYP_FLOAT_C for floating point literals and choose types that match instead of casting (exceptions: the math macros convert C89's double results to HYP_FLOAT, scalar_random_rangef converts the random integers, and hyp_print_value converts to double for printf)
	- comments are neutral, factual and short
	- blank line at the end of every file
* Don't interfere with the user's program: no stray macros, no declarations without definitions, any include order works
* Every configuration macro works alone and combined, and is tested
* Follow the library's own conventions (e.g. the right-hand rule)
* Remove unused code; change public names only by deprecation (@deprecated, HYP_NO_DEPRECATED)
* Build and run the tests: `cmake -B build && cmake --build build && ctest --test-dir build`
* Building also lints the code with strict warnings as errors
* The Makefile has shortcuts for local development (`make help` lists them): `make configure`, then `make test` builds and runs the tests
* Test how people use the library: compilers, platforms, Release, 32-bit, static, shared, each configuration macro, the README example
* Detect compiler features, not versions
* Make a new check fail once before trusting it
* One topic per commit; a fix ships with the check that enforces it
* No false steps in the history; every commit builds and passes
* Don't loosen a test to make it pass; a failing test is a finding

Numerical principles:

* Every input has a defined result: zero, tiny, huge, infinite and NaN components, and degenerate geometry (parallel or opposite vectors, eye == target, a zero axis)
* Decide on exact conditions, not tolerances: only an exactly zero determinant has no inverse, only an exactly zero vector cannot be normalized
* Choose the formula that keeps its digits in the hard cases: atan2 rather than acos for angles, differences of unit vectors rather than 1 - dot
* Scale before squaring so that lengths neither overflow nor underflow; divide by a value rather than multiply by a reciprocal that can overflow
* A function of rotations gives the same result for q and -q
* Measure precision instead of assuming it: the error in ulps against a reference in higher precision, on random and on hard inputs, with the reference itself checked a second way
* The default is precise: a speedup there must give the same bits (the precision output is unchanged) and less work (the speed gate).  `HYP_FAST` gives up the last ulps for speed, never correctness: every input still has a defined result
* Compare with other libraries on the same inputs, and publish the commands that reproduce each claim

Unit Tests
--------------------------------
Mathematical bug fixes should have a unit test with 1 or more test cases indicating
what the problem was and now ensuring that it is fixed.


Contributions
-------------

This project accepts and welcomes contributions.

The rules are pretty simple:

1) if you can certify the below:
(Borrowed from the Linux Kernel Project)

```
Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

        (a) The contribution was created in whole or in part by me and I
            have the right to submit it under the open source license
            indicated in the file; or

        (b) The contribution is based upon previous work that, to the best
            of my knowledge, is covered under an appropriate open source
            license and I have the right under that license to submit that
            work with modifications, whether created in whole or in part
            by me, under the same open source license (unless I am
            permitted to submit under a different license), as indicated
            in the file; or

        (c) The contribution was provided directly to me by some other
            person who certified (a), (b) or (c) and I have not modified
            it.

        (d) I understand and agree that this project and the contribution
            are public and that a record of the contribution (including all
            personal information I submit with it, including my sign-off) is
            maintained indefinitely and may be redistributed consistent with
            this project or the open source license(s) involved.
```

2) then you just add a line saying:

```
	Signed-off-by: Random J Developer <random@developer.example.org>
```

You must use your real name.

This will be done for you automatically if you use `git commit -s`.


Credits
-------

Along the way, I found that some of the terminology used concerning quaternions was not consistent.  For example, there is confusion about the difference between a quaternion's "norm", "normalizing a quaternion" and the quaternion's "magnitude".  Not having a formal background in linear algebra, I found things like this very confusing. Part of the reason that I wanted to build this library was to finally learn all of this material for myself; once having done that, share it with others.

**Matrix and Quaternion FAQ**

The Matrix-Quaternion FAQ was extremely helpful as far as teaching me the math. I originally came upon that document a long time ago as a student (late 1990's) and had forgotten about it until a friend (Jason Hughes) reintroduced me to it.  At that reintroduction, I got the feeling  hat this document was in danger of being lost at the bottom of the Internet.  At the time, it was rather hard to find.  I decided to "save it" by including it in this library in the "docs" folder.  If the original authors do not appreciate this, I am happy to remove it.  Keep in mind however that the doc does say: "Feel free to distribute or copy this FAQ as you please." I credit it as a source for some of the routines used in Hypatia and I hereby thank its authors for their hardwork in its creation.

**Web Sites**

When I read through the Mat-Quat FAQ, I found the notes about corrections that were made, but I still found that there were differences between what the doc says, various pages around the Internet say and some math books that I assumed were reliable texts.  I would like to credit some of those reliable sources here:

- Euclidean Space - http://www.euclideanspace.com
- Math Works - http://www.mathworks.com
- Wolfram Alpha Website http://www.wolframalpha.com
- Jeremiah van Oosten- http://3dgep.com/understanding-quaternion
- Wikipedia - https://en.wikipedia.org/wiki
- Stack Overflow - http://www.stackoverflow.com
- Endo Digital - http://www.endodigital.com
- Paul Berner, et.al. - http://www.sedris.org/wg8home/Documents/WG80485.pdf

**People**

As I went a long, I also found that I needed help from people willing to talk to me about this. I credit those kind people here:

 - Jason Hughes
 - The librarians at the Austin Community College - Cedar Park, Texas

**Author**

The hypatia math library is primarily the work of one author:

 - Darryl T. Agostinelli - <http://www.darrylagostinelli.com>
