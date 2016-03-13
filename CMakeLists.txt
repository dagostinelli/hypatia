cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0054 NEW)

PROJECT(hypatia C)

enable_testing()
INCLUDE(CPack)

SET(HYPATIA_VERSION 1.0)
SET(HYPATIA_SOVERSION 1) # Increment this if/when we break backwards compatibility.

ADD_SUBDIRECTORY("src")
ADD_SUBDIRECTORY("test")

OPTION(HYPATIA_BUILD_DOCS "Build help documents" TRUE)
IF(HYPATIA_BUILD_DOCS)
	ADD_SUBDIRECTORY("doc")
ENDIF(HYPATIA_BUILD_DOCS)

#get_cmake_property(_variableNames VARIABLES)
#	foreach (_variableName ${_variableNames})
#		message(STATUS "${_variableName}=${${_variableName}}")
#	endforeach()
