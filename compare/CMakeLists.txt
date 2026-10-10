# Compares hypatia with GLM, cglm, Eigen and LAPACK.  Experimental; not part of
# the library build.  Run ./fetch.sh first.
cmake_minimum_required(VERSION 3.16)
project(hypatia_compare C CXX)

set(EXT ${CMAKE_CURRENT_SOURCE_DIR}/ext)
set(HYPATIA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.. CACHE PATH "the hypatia.h to compare")
# extra definitions for every program, e.g. PRECISION_ONLY;HYP_MASTER
set(COMPARE_DEFS "" CACHE STRING "extra compile definitions")
find_library(LAPACK_LIB NAMES lapack liblapack.so.3 REQUIRED)

# one program per configuration: precision and depth range
function(compare_program name defs)
	add_library(${name}_hyp STATIC hyp_impl.c)
	target_include_directories(${name}_hyp PRIVATE ${HYPATIA_DIR})
	target_compile_definitions(${name}_hyp PRIVATE ${defs} ${COMPARE_DEFS})
	set_property(TARGET ${name}_hyp PROPERTY C_STANDARD 90)

	add_executable(${name} compare.cpp)
	target_include_directories(${name} PRIVATE ${HYPATIA_DIR} ${EXT}/glm ${EXT}/cglm/include ${EXT}/eigen)
	target_compile_definitions(${name} PRIVATE ${defs} ${COMPARE_DEFS})
	set_property(TARGET ${name} PROPERTY CXX_STANDARD 17)
	target_link_libraries(${name} ${name}_hyp ${LAPACK_LIB} m)
endfunction()

compare_program(compare_double "HYPATIA_DOUBLE_PRECISION_FLOATS")
compare_program(compare_single "HYPATIA_SINGLE_PRECISION_FLOATS")
compare_program(compare_double_depth_no "HYPATIA_DOUBLE_PRECISION_FLOATS;HYP_DEPTH_MINUS_ONE_TO_ONE")
