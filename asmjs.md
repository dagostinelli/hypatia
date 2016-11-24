
asm.js
-----------
Hypaita can be compiled for asm.js. But it is experimental.  You'll have
to specify all of the .c files manually because there is not yet a CMake
target for this yet.

```
cd /where/emsdk/is/stored
source emsdk_env.sh
cd /where/hypatia/is/stored/src
emcc matrix4.c quaternion.c vector2.c scalar.c vector3.c ../test/main.c -I .
```
then run the unit tests
```
$> node a.out.js
quaternion_all_tests
matrix4_all_tests
vector3_all_tests
ALL TESTS PASSED
Tests run: 36
```
