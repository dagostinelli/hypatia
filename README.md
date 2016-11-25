[![Build Status](https://travis-ci.org/dagostinelli/hypatia.svg?branch=master)](https://travis-ci.org/dagostinelli/hypatia)  [![Build status](https://ci.appveyor.com/api/projects/status/f86hqjfhmsq70a11/branch/master?svg=true)](https://ci.appveyor.com/project/dagostinelli/hypatia/branch/master)



Hypatia
=======

Hypatia, a Greek mathematician, 355-415 C.E. Considered by many to be the first female mathematician of note.

Hypatia is a pure-C math library.  It is almost 100% C89/C90 compliant.  This library is intended for use in 2d/3d graphics program (such as games).  Since it is not a general purpose math library, but a library meant for 3d graphics, certain opinions have been expressed in its design.  One of those design choices, intended to help with speed, is that all objects (quaternions, matrices, vectors) are mutable.  (That means that the objects change their values.)  This was a purposeful design choice. Construct your program around this choice.

A word about naming convention:  Hypatia uses verbose names. In pure-C code, math-related function names seem to end up either cryptic (m4mul), verbose (matrix4_multiplym4) or ambiguous (multiply).  C++ is a little better in this respect, because there is operator and function overloading (gracefully allows for ambiguous names).  When Hypatia was shown around before its release, the chief complaint was "it has verbose names".  As an experiment, some \#defines have been added to alias the verbose names.  At this point, the primary API is the verbose names and the experimental API has some of the shorter, cryptic names. In fact, only a small portion of the entire API has been aliased in this way.  My intention to keep one and toss the other. I would like your feedback about that.

Quick Start
----------
NOTE: This will install in /usr.  You probably don't want that.  But this is a quick start.
The best thing to do is to combine this library with your other code into a larger CMake project/solution.

1. Download, build and install
```
git clone https://github.com/dagostinelli/hypatia
cd hypatia
mkdir build
cd build
cmake ..
make && make install
```

2. Write some code
```
#include <stdio.h>
#include <assert.h>
#include <hypatia/hypatia.h>

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

3. Compile and run
```
gcc -lhypatia my_app.c && ./a.out
```

Building
--------

**Windows (Visual Studio)**
```
git clone https://github.com/dagostinelli/hypatia
mkdir build
cd build
cmake ..
make
```

**Linux**
```
git clone https://github.com/dagostinelli/hypatia
mkdir build
cd build
cmake ..
make
```

**Mac OSX**
The steps are the same as Linux.  Be sure to have at least version 3.0 of cmake installed
```
git clone https://github.com/dagostinelli/hypatia
mkdir build
cd build
cmake ..
make
```

**Additional Build Options**

- HYPATIA_SINGLE_PRECISION_FLOATS: (Default Off)
Enable single-precision floating point values instead of double (default)
by setting the `HYPATIA_SINGLE_PRECISION_FLOATS` to ON.

```
cmake .. -DHYPATIA_SINGLE_PRECISION_FLOATS=ON
cmake .. -DHYPATIA_SINGLE_PRECISION_FLOATS=OFF
```

- HYPATIA_BUILD_SHARED: (Default ON) Controls if the shared library is built
```
cmake .. -DHYPATIA_BUILD_SHARED=ON
cmake .. -DHYPATIA_BUILD_SHARED=OFF
```
- HYPATIA_BUILD_STATIC: (Default ON) Controls if the static library is built
```
cmake .. -DHYPATIA_BUILD_STATIC=ON
cmake .. -DHYPATIA_BUILD_STATIC=OFF
```

- HYPATIA_BUILD_DOCS: (Default ON) Build the help documents
```
cmake .. -DHYPATIA_BUILD_DOCS=ON
cmake .. -DHYPATIA_BUILD_DOCS=OFF
```

- HYPATIA_BUILD_TESTS: (Default ON) Build the unit tests
```
cmake .. -DHYPATIA_BUILD_TESTS=ON
cmake .. -DHYPATIA_BUILD_TESTS=OFF
```

- CMAKE_BUILD_TYPE: (Default Release) Set this to 'Release' or 'Debug'
```
cmake .. -CMAKE_BUILD_TYPE=Release
cmake .. -CMAKE_BUILD_TYPE=Debug
```

- CMAKE_INSTALL_PREFIX: (Default /usr) Allows you to specify where `make install` sends the output.
```
cmake .. -DCMAKE_INSTALL_PREFIX=~/hypatia/
cmake .. -DCMAKE_INSTALL_PREFIX=~/projects/myproject/
cmake .. -DCMAKE_INSTALL_PREFIX=~/experiments
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
```


Documentation
-------------
A great way to learn how to use the library is to review the
[unit tests](https://github.com/dagostinelli/hypatia/tree/master/test "Unit Tests")
and [documentation](http://dagostinelli.github.io/hypatia/)

Help pages [have been published online here](http://dagostinelli.github.io/hypatia/ "Documentation")

Can I trust this math library?
------------------------------
A goal of the unit tests is to test each function against HYP_EPSILON which
is defined in hypatia.h, currently as 1E-5.  A number of functions do not yet
have unit tests proving 1E-5, but more are coming.



