INCLUDE_DIRECTORIES(.)


OPTION(HYPATIA_USE_SINGLE "Use single precision floating point values" TRUE)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
	${CMAKE_CURRENT_SOURCE_DIR}/config.h
)


SET( HYPATIA_HEADERS
	hypatia.hpp
	hypatia.h
	config.h
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
	SET(CMAKE_BUILD_TYPE DEBUG)
	MESSAGE(STATUS "No CMAKE_BUILD_TYPE specified, using default DEBUG")
ENDIF()


if(MSVC)
	SET(CMAKE_DEBUG_POSTFIX d)
	#SET(LINKER_FLAGS "${LINKER_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	#ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS=1)
	#ADD_DEFINITIONS(-D_WIN32)
	#ADD_DEFINITIONS(-DWIN32)
	#ADD_DEFINITIONS(_WIN32)
	#ADD_DEFINITIONS(WIN32)
	ADD_DEFINITIONS(-DHYP_EXPORTS_ON)
	# VC2010 fixes
	#OPTION( VC10_STDINT_FIX "Fix for VC10 Compiler regarding pstdint.h redefinition errors" OFF )
	#if( VC10_STDINT_FIX )
	#	ADD_DEFINITIONS( -D_STDINT )
	#endif( VC10_STDINT_FIX )
endif()


if("${CMAKE_C_COMPILER}" MATCHES "clang$" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
	SET(CMAKE_COMPILER_IS_GNUCC 1)
endif()


if(CMAKE_COMPILER_IS_GNUCC)
	FIND_LIBRARY(LIBM m)

	#-Wtraditional -Wfloat-equal -Wdouble-promotion -Wpedantic -Wtrampolines -Wtraditional-conversion -Wunsafe-loop-optimizations -Wfloat-conversion -Wconversion 
	SET(CMAKE_C_FLAGS_DEBUG "-ggdb -std=c90 -Wextra -Wmissing-prototypes -Wall -Wstrict-prototypes -Wold-style-definition -Wdeclaration-after-statement -Wundef -Wpointer-arith")
	SET(CMAKE_C_FLAGS_RELEASE "-std=c90 -Wextra -Wmissing-prototypes -Wall -Wstrict-prototypes -Wold-style-definition")
	
	IF (HYPATIA_USE_SINGLE)
		# Cannot make warnings as errors anymore when using single-precision floats because -Wconversion will complain about not casting sqrt()
	ELSE()
		SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Werror")
		SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Werror")
	ENDIF(HYPATIA_USE_SINGLE)

endif()


OPTION(HYPATIA_BUILD_STATIC "Build static library" TRUE)
IF(HYPATIA_BUILD_STATIC)
	ADD_LIBRARY(hypatia_static STATIC ${Hypatia_SRCS})
	SET_TARGET_PROPERTIES(hypatia_static PROPERTIES LINKER_LANGUAGE C)
	if(CMAKE_COMPILER_IS_GNUCC)
		SET_TARGET_PROPERTIES(hypatia_static PROPERTIES OUTPUT_NAME "hypatia")
	endif()
	list(APPEND HYPATIA_LIB_TARGET hypatia_static)
	SET_TARGET_PROPERTIES(hypatia_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	IF(APPLE)
		SET_TARGET_PROPERTIES(hypatia_static PROPERTIES MACOSX_RPATH ON)
	ENDIF()
ENDIF(HYPATIA_BUILD_STATIC)


OPTION(HYPATIA_BUILD_SHARED "Build shared library" TRUE)
IF(HYPATIA_BUILD_SHARED)
	ADD_LIBRARY(hypatia SHARED ${Hypatia_SRCS})
	SET_TARGET_PROPERTIES(hypatia PROPERTIES LINKER_LANGUAGE C)
	SET_TARGET_PROPERTIES(hypatia PROPERTIES VERSION ${HYPATIA_VERSION})
	SET_TARGET_PROPERTIES(hypatia PROPERTIES SOVERSION ${HYPATIA_SOVERSION})
	list(APPEND HYPATIA_LIB_TARGET hypatia)
	SET_TARGET_PROPERTIES(hypatia PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	IF(APPLE)
		SET_TARGET_PROPERTIES(hypatia PROPERTIES MACOSX_RPATH ON)
	ENDIF()
ENDIF(HYPATIA_BUILD_SHARED)


IF(NOT HYPATIA_BUILD_STATIC AND NOT HYPATIA_BUILD_SHARED)
	MESSAGE(FATAL_ERROR "Both shared and static libraries are disabled. Please specify one or the other or both.")
ENDIF(NOT HYPATIA_BUILD_STATIC AND NOT HYPATIA_BUILD_SHARED)


#IF(HYPATIA_BUILD_STATIC AND HYPATIA_BUILD_SHARED)
#	MESSAGE(FATAL_ERROR "Please do not specity both shared and static libraries at the same time")
#ENDIF()


INSTALL(TARGETS ${HYPATIA_LIB_TARGET}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		)
INSTALL(FILES ${HYPATIA_HEADERS} DESTINATION include/hypatia)


IF ((MSVC) AND (CMAKE_BUILD_TYPE==Debug))
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/hypatiad.pdb DESTINATION bin)
ENDIF()


#get_cmake_property(_variableNames VARIABLES)
#	foreach (_variableName ${_variableNames})
#		message(STATUS "${_variableName}=${${_variableName}}")
#	endforeach()
