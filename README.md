[![Build Status](https://travis-ci.org/dagostinelli/hypatia.svg?branch=master)](https://travis-ci.org/dagostinelli/hypatia)

Hypatia
=======

Hypatia, a Greek mathematician, 355-415 C.E. Considered by many to be the first female mathematician of note.

Hypatia is a pure-C math library.  It is almost 100% C89/C90 compliant.  This library is intended for use in 2d/3d graphics program (such as games).  Since it is not a general purpose math library, but a library meant for 3d graphics, certain opinions have been expressed in its design.  One of those design choices, intended to help with speed, is that all objects (quaternions, matrices, vectors) are mutable.  (That means that the objects change their values.)  This was a purposeful design choice. Construct your program around this choice.

A word about naming convention:  In pure-C code, math-related function names seem to end up either cryptic (m4mul), verbose (matrix4_multiplym4) or ambiguous (multiply).  C++ is a little better in this respect, because there is operator and function overloading (allows ambiguous names).  We don't have that in pure-C and Hypatia is intended for C-only projects.  Hypatia uses verbose names.  When Hypatia was shown around before its release, the chief complaint was "it has verbose names".  :-)  As an experiment, some \c \#defines were put in to alias the verbose names and so now we have the verbose and some cryptic names.  This is beta software until a few people use it for real and tell me that they want the verbose names or the cryptic names.  I would like to hear from you about this.

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

FAQ
---

- Can I trust this math library?  
A goal of the unit tests is to test each function against HYP_EPSILON which is defined in hypatia.h, currently as 1E-5.  In this beta release, a number of functions do not yet have unit tests proving 1E-5, but more are coming.

- Where is the matrix inverse function?  
It turns out that is pretty tough to get right and also do efficiently. It may be added some day if it is requested enough times.  In the mean time, what you could do instead is create an inverted view matrix (translate, then rotate, then scale instead of the other way around).

- Can this be compiled for asm.js?  
Yes, but it is experimental.
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

