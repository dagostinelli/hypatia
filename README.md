[![Build Status](https://travis-ci.org/dagostinelli/hypatia.svg?branch=master)](https://travis-ci.org/dagostinelli/hypatia)  [![Build status](https://ci.appveyor.com/api/projects/status/f86hqjfhmsq70a11/branch/master?svg=true)](https://ci.appveyor.com/project/dagostinelli/hypatia/branch/master)



Hypatia
=======

Hypatia, a Greek mathematician, 355-415 C.E. Considered by many to be the first female mathematician of note.

Hypatia is a pure-C math library.  It is almost 100% C89/C90 compliant.  This library is intended for use in 2d/3d graphics program (such as games).  Since it is not a general purpose math library, but a library meant for 3d graphics, certain opinions have been expressed in its design.  One of those design choices, intended to help with speed, is that all objects (quaternions, matrices, vectors) are mutable.  (That means that the objects change their values.)  This was a purposeful design choice. Construct your program around this choice.

A word about naming convention:  Hypatia uses verbose names. In pure-C code, math-related function names seem to end up either cryptic (m4mul), verbose (matrix4_multiplym4) or ambiguous (multiply).  C++ is a little better in this respect, because there is operator and function overloading (gracefully allows for ambiguous names).  When Hypatia was shown around before its release, the chief complaint was "it has verbose names".  As an experiment, some \#defines have been added to alias the verbose names.  At this point, the primary API is the verbose names and the experimental API has some of the shorter, cryptic names. In fact, only a small portion of the entire API has been aliased in this way.  My intention to keep one and toss the other. I would like your feedback about that.

Building
--------
```
git clone https://github.com/dagostinelli/hypatia
mkdir build
cd build
cmake -G "Unix Makefiles" ../hypatia/ -DCMAKE_INSTALL_PREFIX=../install
make && make install
```


Documentation
-------------
Help pages [have been published online here](http://dagostinelli.github.io/hypatia/ "Documentation")


Quick Start
-----------
A great way to learn how to use the library is to review the unit tests.


FAQ
---

- Can I trust this math library?
A goal of the unit tests is to test each function against HYP_EPSILON which is defined in hypatia.h, currently as 1E-5.  In this beta release, a number of functions do not yet have unit tests proving 1E-5, but more are coming.

- Where is the matrix inverse function?
It turns out that is pretty tough to get right and also do efficiently. It may be added some day if it is requested enough times.  In the mean time, what you could do instead is create an inverted view matrix (translate, then rotate, then scale instead of the other way around).

asm.js
-----------
This library can be compiled for asm.js. But it is experimental.

```
cd /where/emsdk/is/stored
source emsdk_env.sh
cd /where/hypatia/is/stored/src
emcc matrix4.c quaternion.c vector2.c scalar.c vector3.c ../test/main.c -I .
```
then run it
```
$> node a.out.js
quaternion_all_tests
matrix4_all_tests
vector3_all_tests
ALL TESTS PASSED
Tests run: 36
```
