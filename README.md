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
| `HYP_EPSILON` | The tolerance of the `*_equals` functions. | `1E-5` |

**Standard library**

| Macro | Effect | Default |
|---|---|---|
| `HYP_NO_STDIO` | Leaves out `<stdio.h>` and the `hyp_*_print` debug functions. | |
| `HYP_NO_C_MATH` | Leaves out `<math.h>`.  Then define all eight math macros below. | |
| `HYP_SQRT`, `HYP_FMOD`, `HYP_SIN`, `HYP_COS`, `HYP_TAN`, `HYP_ASIN`, `HYP_ACOS`, `HYP_ATAN2` | The math functions. | `sqrt`, `fmod`, ... from `<math.h>`, converted to `HYP_FLOAT` |
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

* `vector3_rotate_by_quaternion` was wrong for vectors not perpendicular to the axis.
* `matrix4_multiplyv2` treats z as 0.
* Normalizing leaves only an exactly zero vector unchanged (2.0: anything shorter than 1e-5); `vector2_normalize` no longer gives NaN.
* `quaternion_inverse` and `quaternion_lerp` have no shortcuts near 0 or 1.
* `quaternion_is_pure` compares w relative to the length of the quaternion.
* `matrix2/3/4_inverse` return NULL only for an exactly zero determinant, and `matrix2/3/4_invert` return NULL when there is no inverse.
* The former `_EXP` functions: perspective, ortho and lookat give correct projections; the Euler angle functions rotate about X, then Y, then Z; `matrix4_multiplyv3_EXP` is `matrix4_multiplyv3`; `quaternion_angle_between` and `quaternion_difference` treat q and -q as the same rotation; `matrix4_transformation_decompose` returns the rotation.

Documentation
-------------
A great way to learn how to use the library is to review the
[unit tests](https://github.com/dagostinelli/hypatia/tree/master/test "Unit Tests").

View the official [documentation](http://dagostinelli.github.io/hypatia/) here.

Can I trust this math library?
------------------------------
A goal of the unit tests is to test each function against HYP_EPSILON which is defined in hypatia.h, currently as 1E-5.  A number of functions do not yet have unit tests proving 1E-5, but more are coming.

A word about convention
-----------------------

Hypatia uses verbose names. In pure-C code, math-related function names seem to end up either cryptic (m4mul), verbose (matrix4_multiplym4) or ambiguous (multiply).  C++ is a little better in this respect, because there is operator and function overloading (gracefully allows for ambiguous names).  When Hypatia was shown around before its release, the chief complaint was "it has verbose names".

As an experiment, some \#defines have been added to alias the verbose names. (mat4, vec3, vec4, quat, etc)  At this point, the primary API is the verbose names and the experimental API has some of the shorter, cryptic names. In fact, only a small portion of the entire API has been aliased in this way.  My intention to keep one and toss the other. I would like your feedback about that.

Rotations follow the right-hand rule: a positive angle turns counterclockwise when the axis points toward the viewer.

Coding Standard
---------------

* Mathematical correctness comes first; a design choice that conflicts with it is a bug
* Ensure C89/C90 compatibility (exceptions: the anonymous unions in the types, and <stdint.h> / uint8_t)
* Warnings are errors; fix the code, never turn a warning off
* Check that the coding style is consistent with the rest of the codebase.
	- curly brace placement
	- indent with tabs
	- lower case function names for public-facing functions
	- all CAPS for public constants.  HYP is the prefix for public constants
	- use 'self' for describing the function context
	- math entities are mutable
	- prefer no casts; use HYP_FLOAT_C for floating point literals and choose types that match instead of casting (exceptions: the math macros convert C89's double results to HYP_FLOAT, and scalar_random_rangef converts the random integers)
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
