INCLUDE_DIRECTORIES(.)

OPTION(HYPATIA_SINGLE_PRECISION_FLOATS "Use single precision floating point values" BOOL:FALSE)
IF(HYPATIA_SINGLE_PRECISION_FLOATS)
	MESSAGE(STATUS "Using single-precision floating point values")
ELSE()
	MESSAGE(STATUS "Using double-precision floating point values")
ENDIF()

SET( HYPATIA_HEADERS
	hypatia.hpp
	hypatia.h
	vector2.h
	vector3.h
	vector4.h
	matrix3.h
	matrix4.h
	quaternion.h
	experimental.h
)

SET( Hypatia_SRCS
	scalar.c
	vector2.c
	vector3.c
	vector4.c
	matrix3.c
	matrix4.c
	quaternion.c
	experimental.c
)
SOURCE_GROUP(Hypatia FILES ${Hypatia_SRCS})

SET( Hypatia_Tests_SRCS
	tests/main.c
)
SOURCE_GROUP(Tests FILES ${Hypatia_Tests_SRCS})

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
	MESSAGE(STATUS "No CMAKE_BUILD_TYPE specified, using default RELEASE")
ENDIF()
MESSAGE(STATUS "Built type is set to ${CMAKE_BUILD_TYPE}")

IF(CMAKE_COMPILER_IS_WINDOWS)
	SET(CMAKE_DEBUG_POSTFIX d)
ENDIF()

INCLUDE(../cmake/CMakeBuildFlags.cmake)
INCLUDE(../cmake/CMakeFunctions.cmake)

# unit tests do not use strict-prototypes, so this is here
IF(CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_CLANG)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes")
ENDIF()

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

OPTION(HYPATIA_BUILD_STATIC "Build static library" ON)
IF(HYPATIA_BUILD_STATIC)
	MESSAGE(STATUS "Building static library")

	ADD_LIBRARY(hypatia_static STATIC ${Hypatia_SRCS})
	SET_TARGET_PROPERTIES(hypatia_static PROPERTIES LINKER_LANGUAGE C)
	if(CMAKE_COMPILER_IS_GNUC)
		SET_TARGET_PROPERTIES(hypatia_static PROPERTIES OUTPUT_NAME "hypatia")
	endif()
	list(APPEND HYPATIA_LIB_TARGET hypatia_static)
	SET_TARGET_PROPERTIES(hypatia_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	IF(APPLE)
		SET_TARGET_PROPERTIES(hypatia_static PROPERTIES MACOSX_RPATH ON)
	ENDIF()
	IF(HYPATIA_SINGLE_PRECISION_FLOATS)
		target_compile_definitions(hypatia_static PRIVATE HYPATIA_SINGLE_PRECISION_FLOATS)
	ENDIF()
ENDIF()


OPTION(HYPATIA_BUILD_SHARED "Build shared library" ON)
IF(HYPATIA_BUILD_SHARED)
	MESSAGE(STATUS "Building shared library")

	ADD_LIBRARY(hypatia SHARED ${Hypatia_SRCS})
	SET_TARGET_PROPERTIES(hypatia PROPERTIES LINKER_LANGUAGE C)
	SET_TARGET_PROPERTIES(hypatia PROPERTIES VERSION ${PROJECT_VERSION})
	#SET_TARGET_PROPERTIES(hypatia PROPERTIES SOVERSION 1)
	list(APPEND HYPATIA_LIB_TARGET hypatia)
	SET_TARGET_PROPERTIES(hypatia PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	IF(LINUX)
		# shared library should link to libm
		FIND_LIBRARY(M_LIB m)
		TARGET_LINK_LIBRARIES(hypatia ${M_LIB})
	ENDIF()
	IF(APPLE)
		SET_TARGET_PROPERTIES(hypatia PROPERTIES MACOSX_RPATH ON)
	ENDIF()
	#IF(CMAKE_COMPILER_IS_WINDOWS)
	#	SET_TARGET_PROPERTIES(hypatia PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
	#ENDIF()
	IF(HYPATIA_SINGLE_PRECISION_FLOATS)
		target_compile_definitions(hypatia PRIVATE HYPATIA_SINGLE_PRECISION_FLOATS)
	ENDIF()
ENDIF()


OPTION(HYPATIA_BUILD_SINGLE_FILE_HEADER "Build single file header distribution" ON)
IF(HYPATIA_BUILD_SINGLE_FILE_HEADER)
	MESSAGE(STATUS "Building single file header distribution")

	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "")

	# Compose the .h file portion
	cat(hypatia.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(vector2.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(vector3.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(vector4.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(matrix3.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(matrix4.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(quaternion.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(experimental.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)

	# Compose the .c file portion
	file(READ single-file-lib-begin.c CONTENTS_BEGIN)
	file(READ single-file-lib-end.c CONTENTS_END)

	file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "${CONTENTS_BEGIN}")

	cat(scalar.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(vector2.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(vector3.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(vector4.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(matrix3.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(matrix4.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(quaternion.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)
	cat(experimental.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h)

	file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "${CONTENTS_END}")

	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"hypatia.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"config.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"vector2.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"vector3.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"vector4.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"matrix3.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"matrix4.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"quaternion.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/hypatia.h "#include \"experimental.h\"")

ENDIF()


OPTION(HYPATIA_BUILD_TWOFILE_DISTRIBUTION "Build two file source distribution" ON)
IF(HYPATIA_BUILD_TWOFILE_DISTRIBUTION)
	MESSAGE(STATUS "Building two file source distribution")

	# Compose the .h file
	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "")

	cat(hypatia.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)
	cat(vector2.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)
	cat(vector3.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)
	cat(vector4.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)
	cat(matrix3.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)
	cat(matrix4.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)
	cat(quaternion.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)
	cat(experimental.h ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h)

	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"hypatia.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"config.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"vector2.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"vector3.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"vector4.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"matrix3.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"matrix4.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"quaternion.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.h "#include \"experimental.h\"")

	# Compose the .c file
	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "")
	cat(scalar.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	cat(vector2.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	cat(vector3.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	cat(vector4.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	cat(matrix3.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	cat(matrix4.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	cat(quaternion.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	cat(experimental.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)

	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"hypatia.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"config.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"vector2.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"vector3.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"vector4.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"matrix3.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"matrix4.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"quaternion.h\"")
	deleteinplace(${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#include \"experimental.h\"")

	file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c CONTENTS)
	file(READ single-file-lib-begin.c CONTENTS_BEGIN)
	file(READ single-file-lib-end.c CONTENTS_END)

	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "")
	file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "#define HYPATIA_IMPLEMENTATION")
	file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "${CONTENTS_BEGIN}")
	file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "${CONTENTS}")
	file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c "${CONTENTS_END}")
ENDIF()


IF(NOT HYPATIA_BUILD_STATIC AND NOT HYPATIA_BUILD_SHARED)
	MESSAGE(FATAL_ERROR "Both shared and static libraries are disabled. Please specify one or the other or both.")
ENDIF(NOT HYPATIA_BUILD_STATIC AND NOT HYPATIA_BUILD_SHARED)

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET(LIB_SUFFIX "64")
ENDIF()

INSTALL(TARGETS ${HYPATIA_LIB_TARGET}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib${LIB_SUFFIX}
		ARCHIVE DESTINATION lib${LIB_SUFFIX}
		)
INSTALL(FILES ${HYPATIA_HEADERS} DESTINATION include/hypatia)

#IF(CMAKE_COMPILER_IS_WINDOWS AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/hypatia.pdb DESTINATION bin)
#ENDIF()
