
INCLUDE(../cmake/CMakeBuildFlags.cmake)

# the precision flags and definitions for a target; precision is single or double
function(hyp_precision target precision)
	if(precision STREQUAL "single")
		TARGET_COMPILE_OPTIONS(${target} PRIVATE ${HYP_SINGLE_PRECISION_FLAGS})
		TARGET_COMPILE_DEFINITIONS(${target} PRIVATE HYPATIA_SINGLE_PRECISION_FLOATS)
	else()
		TARGET_COMPILE_OPTIONS(${target} PRIVATE ${HYP_DOUBLE_PRECISION_FLAGS})
	endif()
endfunction()

# a test program from the given sources, in both precisions:
# <name>_single_precision and <name>_double_precision
function(hyp_add_test name)
	foreach(precision single double)
		SET(target ${name}_${precision}_precision)
		ADD_EXECUTABLE(${target} ${ARGN})
		hyp_precision(${target} ${precision})
		TARGET_LINK_LIBRARIES(${target} hypatia::hypatia)
		ADD_TEST(NAME ${target} COMMAND ${target})
	endforeach()
endfunction()

hyp_add_test(test_hypatia main.c implementation.c)
hyp_add_test(test_random_default test_random_default.c)
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
foreach(precision single double)
	SET(library hypatia_shared_${precision}_precision)
	ADD_LIBRARY(${library} SHARED implementation.c)
	hyp_precision(${library} ${precision})
	TARGET_COMPILE_DEFINITIONS(${library} PRIVATE ${HYP_SHARED_EXPORT})
	TARGET_LINK_LIBRARIES(${library} hypatia::hypatia)
	TARGET_COMPILE_DEFINITIONS(test_hypatia_shared_${precision}_precision PRIVATE ${HYP_SHARED_IMPORT})
	TARGET_LINK_LIBRARIES(test_hypatia_shared_${precision}_precision ${library})
endforeach()
