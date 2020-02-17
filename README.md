[![Build Status](https://travis-ci.org/dagostinelli/hypatia.svg?branch=master)](https://travis-ci.org/dagostinelli/hypatia)  [![Build status](https://ci.appveyor.com/api/projects/status/f86hqjfhmsq70a11/branch/master?svg=true)](https://ci.appveyor.com/project/dagostinelli/hypatia/branch/master)



Hypatia
=======

Hypatia, a Greek mathematician, 355-415 C.E. Considered by many to be the first female mathematician of note.

Hypatia is a pure-C math library.  It is almost 100% C89/C90 compliant.  This library is intended for use in 2d/3d graphics program (such as games).  Since it is not a general purpose math library, but a library meant for 3d graphics, certain opinions have been expressed in its design.  One of those design choices, intended to help with speed, is that all objects (quaternions, matrices, vectors) are mutable.  (That means that the objects change their values.)  This was a purposeful design choice. Construct your program around this choice.

A word about naming convention:  Hypatia uses verbose names. In pure-C code, math-related function names seem to end up either cryptic (m4mul), verbose (matrix4_multiplym4) or ambiguous (multiply).  C++ is a little better in this respect, because there is operator and function overloading (gracefully allows for ambiguous names).  When Hypatia was shown around before its release, the chief complaint was "it has verbose names".  As an experiment, some \#defines have been added to alias the verbose names.  At this point, the primary API is the verbose names and the experimental API has some of the shorter, cryptic names. In fact, only a small portion of the entire API has been aliased in this way.  My intention to keep one and toss the other. I would like your feedback about that.

Quick Start
----------
The fastest way to get started is to grab the single file header distribution. It's in the `/dist/single` folder.

The entire library is self-contained in this one single header.  The file can be used in a header mode or implementation mode.  The header mode is used by default and is what you are using when you simple `#include` this file.  It does not compile-in the actual implementation.  To compile-in the actual implementation, you need to use implmentation mode.  The implementation mode requires the macro `HYPATIA_IMPLEMENTATION` exist in one .c/.cpp file in your project before `#include <hypatia.h>`. Like so:

```
#define HYPATIA_IMPLEMENTATION
#include <hypatia.h>
```

Basic Usage
-----------
```
#include <stdio.h>
#include <assert.h>
#include <hypatia/hypatia.h> /* or #include "hypatia.h" depending on how you installed it */

int main(int argc, char *argv)
{
	struct vector3 a;
	struct vector3 b;
	struct vector3 r;

	printf("Using Hypatia Version:%s\n", HYPATIA_VERSION);

	vector3_setf3(&a, 3.0f, -3.0f, 1.0f);
	vector3_setf3(&b, 4.0f, 9.0f, 2.0f);

	vector3_cross_product(&r, &a, &b);

	assert(scalar_equalsf(r.x, -15.0f));
	assert(scalar_equalsf(r.y, -2.0f));
	assert(scalar_equalsf(r.z, 39.0f));

	return 0;
}

```

Documentation
-------------
A great way to learn how to use the library is to review the
[unit tests](https://github.com/dagostinelli/hypatia/tree/master/test "Unit Tests").

View the official [documentation](http://dagostinelli.github.io/hypatia/) here.

Can I trust this math library?
------------------------------
A goal of the unit tests is to test each function against HYP_EPSILON which is defined in hypatia.h, currently as 1E-5.  A number of functions do not yet have unit tests proving 1E-5, but more are coming.
