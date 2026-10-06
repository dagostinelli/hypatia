cmake_minimum_required(VERSION 3.10)

# the version comes from HYPATIA_VERSION in hypatia.h
FILE(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/hypatia.h" HYPATIA_VERSION_LINE REGEX "^#define HYPATIA_VERSION ")
STRING(REGEX REPLACE "^#define HYPATIA_VERSION \"([0-9.]+)\".*$" "\\1" HYPATIA_VERSION "${HYPATIA_VERSION_LINE}")

PROJECT(hypatia
	VERSION ${HYPATIA_VERSION}
	LANGUAGES C)

# tests and docs are built by default only when hypatia is the top-level project,
# not when another project includes it with add_subdirectory
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
	SET(HYPATIA_IS_TOP_LEVEL ON)
else()
	SET(HYPATIA_IS_TOP_LEVEL OFF)
endif()

INCLUDE(CheckLibraryExists)
INCLUDE(GNUInstallDirs)
INCLUDE(CMakePackageConfigHelpers)

# hypatia::hypatia: the header, its include directory and the C math library
ADD_LIBRARY(hypatia INTERFACE)
ADD_LIBRARY(hypatia::hypatia ALIAS hypatia)
TARGET_INCLUDE_DIRECTORIES(hypatia INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
CHECK_LIBRARY_EXISTS(m sqrt "" HYPATIA_HAS_LIBM)
if(HYPATIA_HAS_LIBM)
	TARGET_LINK_LIBRARIES(hypatia INTERFACE m)
endif()

# install: the header and the CMake package files for find_package(hypatia)
SET(HYPATIA_CMAKE_DIR "${CMAKE_INSTALL_DATADIR}/cmake/hypatia")
INSTALL(FILES hypatia.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
INSTALL(TARGETS hypatia EXPORT hypatiaTargets)
INSTALL(EXPORT hypatiaTargets NAMESPACE hypatia:: DESTINATION ${HYPATIA_CMAKE_DIR})
FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/hypatiaConfig.cmake"
	"include(\"\${CMAKE_CURRENT_LIST_DIR}/hypatiaTargets.cmake\")\n")
if(NOT CMAKE_VERSION VERSION_LESS 3.14)
	SET(HYPATIA_ARCH_INDEPENDENT ARCH_INDEPENDENT)
endif()
WRITE_BASIC_PACKAGE_VERSION_FILE("${CMAKE_CURRENT_BINARY_DIR}/hypatiaConfigVersion.cmake"
	COMPATIBILITY SameMajorVersion ${HYPATIA_ARCH_INDEPENDENT})
INSTALL(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/hypatiaConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/hypatiaConfigVersion.cmake"
	DESTINATION ${HYPATIA_CMAKE_DIR})

OPTION(HYPATIA_BUILD_TESTS "Build unit tests" ${HYPATIA_IS_TOP_LEVEL})
IF(HYPATIA_BUILD_TESTS)
	enable_testing()
	ADD_SUBDIRECTORY("test")
ENDIF(HYPATIA_BUILD_TESTS)

OPTION(HYPATIA_BUILD_DOCS "Build help documents" ${HYPATIA_IS_TOP_LEVEL})
IF(HYPATIA_BUILD_DOCS)
	ADD_SUBDIRECTORY("help")
ENDIF(HYPATIA_BUILD_DOCS)
