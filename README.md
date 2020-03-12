[![Build Status](https://travis-ci.org/dagostinelli/hypatia.svg?branch=master)](https://travis-ci.org/dagostinelli/hypatia)  [![Build status](https://ci.appveyor.com/api/projects/status/f86hqjfhmsq70a11/branch/master?svg=true)](https://ci.appveyor.com/project/dagostinelli/hypatia/branch/master)



Hypatia
=======

Quick Install
----------
The fastest way to get started is to grab the header file. It's self-contained.  The file is called `hypatia.h`

About
-----
Hypatia, a Greek mathematician, 355-415 C.E. Considered by many to be the first female mathematician of note.

Hypatia is a single-file-header, pure-C math library.  It is almost 100% C89/C90 compliant.  This library is intended for use in 2d/3d graphics program (such as games).  Since it is not a general purpose math library, but a library meant for 3d graphics, certain opinions have been expressed in its design.  One of those design choices, intended to help with speed, is that all objects (quaternions, matrices, vectors) are mutable.  (That means that the objects change their values.)  This was a purposeful design choice. Construct your program around this choice.

Quick Start
----------
The entire library is self-contained in this one header file.  The file can be used in a header mode or implementation mode.  The header mode is used by default and is what you are using when you simple `#include` this file.  It does not compile-in the actual implementation.  To compile-in the actual implementation, you need to also use implmentation mode.  The implementation mode requires the macro `HYPATIA_IMPLEMENTATION` exist in one .c/.cpp file in your project before an `#include <hypatia.h>`. 

Like so:

```
#define HYPATIA_IMPLEMENTATION
#include <hypatia.h>
```

Basic Usage
-----------
```
#include <stdio.h>
#include <assert.h>

#define HYPATIA_IMPLEMENTATION
#include "hypatia.h"

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

A word about convention
-----------------------

Hypatia uses verbose names. In pure-C code, math-related function names seem to end up either cryptic (m4mul), verbose (matrix4_multiplym4) or ambiguous (multiply).  C++ is a little better in this respect, because there is operator and function overloading (gracefully allows for ambiguous names).  When Hypatia was shown around before its release, the chief complaint was "it has verbose names".  

As an experiment, some \#defines have been added to alias the verbose names. (mat4, vec3, vec4, quat, etc)  At this point, the primary API is the verbose names and the experimental API has some of the shorter, cryptic names. In fact, only a small portion of the entire API has been aliased in this way.  My intention to keep one and toss the other. I would like your feedback about that.

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
