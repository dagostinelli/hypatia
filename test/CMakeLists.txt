
INCLUDE(../cmake/CMakeBuildFlags.cmake)
INCLUDE(CheckCSourceCompiles)

SET(HYP_PRECISIONS single double long_double)

# quad precision where the compiler has _Float128 and the C library has
# sinf128 and the other _Float128 math functions
STRING(REPLACE ";" " " CMAKE_REQUIRED_FLAGS "${HYP_QUAD_PRECISION_FLAGS}")
if(HYPATIA_HAS_LIBM)
	SET(CMAKE_REQUIRED_LIBRARIES m)
endif()
CHECK_C_SOURCE_COMPILES("
#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <math.h>
int main(void)
{
	volatile _Float128 x = 0.5f128;
	return sinf128(x) > 0.25f128 ? 0 : 1;
}
" HYP_HAS_QUAD_PRECISION)
UNSET(CMAKE_REQUIRED_FLAGS)
UNSET(CMAKE_REQUIRED_LIBRARIES)
if(HYP_HAS_QUAD_PRECISION)
	LIST(APPEND HYP_PRECISIONS quad)
endif()

# the precision flags and definitions for a target; precision is single,
# double, long_double or quad
function(hyp_precision target precision)
	if(precision STREQUAL "single")
		TARGET_COMPILE_OPTIONS(${target} PRIVATE ${HYP_SINGLE_PRECISION_FLAGS})
		TARGET_COMPILE_DEFINITIONS(${target} PRIVATE HYPATIA_SINGLE_PRECISION_FLOATS)
	elseif(precision STREQUAL "long_double")
		TARGET_COMPILE_OPTIONS(${target} PRIVATE ${HYP_LONG_DOUBLE_PRECISION_FLAGS})
		TARGET_COMPILE_DEFINITIONS(${target} PRIVATE HYPATIA_LONG_DOUBLE_PRECISION_FLOATS)
	elseif(precision STREQUAL "quad")
		TARGET_COMPILE_OPTIONS(${target} PRIVATE ${HYP_QUAD_PRECISION_FLAGS})
		TARGET_COMPILE_DEFINITIONS(${target} PRIVATE HYPATIA_QUAD_PRECISION_FLOATS)
	else()
		TARGET_COMPILE_OPTIONS(${target} PRIVATE ${HYP_DOUBLE_PRECISION_FLAGS})
	endif()
endfunction()

# a test program from the given sources, in each precision:
# <name>_single_precision, <name>_double_precision,
# <name>_long_double_precision and, where supported, <name>_quad_precision
function(hyp_add_test name)
	foreach(precision ${HYP_PRECISIONS})
		SET(target ${name}_${precision}_precision)
		ADD_EXECUTABLE(${target} ${ARGN})
		hyp_precision(${target} ${precision})
		TARGET_LINK_LIBRARIES(${target} hypatia::hypatia)
		ADD_TEST(NAME ${target} COMMAND ${target})
	endforeach()
endfunction()

hyp_add_test(test_hypatia main.c implementation.c)
hyp_add_test(test_random_default test_random_default.c)
hyp_add_test(test_depth test_depth.c)
hyp_add_test(test_hypatia_static test_static.c)

# HYP_NO_C_MATH: the program supplies the math functions
hyp_add_test(test_hypatia_no_c_math test_no_c_math.c no_c_math_functions.c)

# Hypatia built into a shared library, with the tests linked against it.  On
# Windows the functions are exported and imported by defining HYPAPI, as a user
# of a Hypatia DLL would.
if(WIN32)
	SET(HYP_SHARED_EXPORT "HYPAPI=__declspec(dllexport)" "TEST_RANDOM_API=__declspec(dllexport)")
	SET(HYP_SHARED_IMPORT "HYPAPI=__declspec(dllimport)" "TEST_RANDOM_API=__declspec(dllimport)")
endif()

hyp_add_test(test_hypatia_shared main.c)
foreach(precision ${HYP_PRECISIONS})
	SET(library hypatia_shared_${precision}_precision)
	ADD_LIBRARY(${library} SHARED implementation.c)
	hyp_precision(${library} ${precision})
	TARGET_COMPILE_DEFINITIONS(${library} PRIVATE ${HYP_SHARED_EXPORT})
	TARGET_LINK_LIBRARIES(${library} hypatia::hypatia)
	TARGET_COMPILE_DEFINITIONS(test_hypatia_shared_${precision}_precision PRIVATE ${HYP_SHARED_IMPORT})
	TARGET_LINK_LIBRARIES(test_hypatia_shared_${precision}_precision ${library})
endforeach()

# the precision gate: the errors of the functions, measured against _Float128
# references, compared with the baselines in precision/
OPTION(HYPATIA_PRECISION_CHECK "Build the precision measurements and check them against the baselines" OFF)
if(HYPATIA_PRECISION_CHECK)
	if(NOT HYP_HAS_QUAD_PRECISION)
		MESSAGE(FATAL_ERROR "HYPATIA_PRECISION_CHECK needs _Float128 and the f128 math functions")
	endif()
	foreach(precision single double)
		SET(target precision_${precision})
		ADD_EXECUTABLE(${target} precision/precision.c)
		hyp_precision(${target} ${precision})
		TARGET_LINK_LIBRARIES(${target} hypatia::hypatia)
		ADD_TEST(NAME precision_check_${precision}
			COMMAND ${target} --check ${CMAKE_CURRENT_SOURCE_DIR}/precision/baseline_${precision}.txt)
	endforeach()
endif()

# the speed gate: the instructions each function executes per call, counted by
# valgrind, compared with the baselines in speed/
OPTION(HYPATIA_SPEED_CHECK "Build the speed measurements and check them against the baselines" OFF)
if(HYPATIA_SPEED_CHECK)
	FIND_PROGRAM(HYP_VALGRIND valgrind)
	if(NOT HYP_VALGRIND OR NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
		MESSAGE(FATAL_ERROR "HYPATIA_SPEED_CHECK needs GCC and valgrind")
	endif()
	foreach(precision single double)
		SET(target speed_${precision})
		ADD_EXECUTABLE(${target} speed/speed.c)
		hyp_precision(${target} ${precision})
		TARGET_LINK_LIBRARIES(${target} hypatia::hypatia)
		ADD_TEST(NAME speed_check_${precision}
			COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed.sh $<TARGET_FILE:${target}>
				--check ${CMAKE_CURRENT_SOURCE_DIR}/speed/baseline_${precision}.txt)
	endforeach()
endif()
