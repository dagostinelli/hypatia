# checksparse and checkpatch (experimental; CI does not run them): run the sparse
# and checkpatch.pl tools on the sources.  If a tool is not found, its target fails
# and says so.

FIND_PROGRAM(SPARSE sparse)
FIND_PROGRAM(CHECKPATCH checkpatch.pl
	PATHS /usr/src/kernels/${CMAKE_HOST_SYSTEM_VERSION}/scripts)

if(SPARSE)
	ADD_CUSTOM_TARGET(checksparse
		COMMAND ${SPARSE} hypatia.h
		COMMAND ${SPARSE} -I. test/implementation.c
		COMMAND ${SPARSE} -I. test/main.c
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		VERBATIM)
else()
	ADD_CUSTOM_TARGET(checksparse
		COMMAND ${CMAKE_COMMAND} -DTOOL=sparse -P ${CMAKE_CURRENT_LIST_DIR}/MissingTool.cmake)
endif()

if(CHECKPATCH)
	FILE(GLOB HYP_STYLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/hypatia.h
		${CMAKE_CURRENT_SOURCE_DIR}/test/*.[ch]
		${CMAKE_CURRENT_SOURCE_DIR}/test/package/*.[ch])
	ADD_CUSTOM_TARGET(checkpatch
		COMMAND ${CHECKPATCH}
			-f --no-tree --no-summary --terse --show-types --subjective
			--ignore BRACES,INLINE,COMPLEX_MACRO,LONG_LINE,OPEN_BRACE,ELSE_AFTER_BRACE,MACRO_WITH_FLOW_CONTROL,CAMELCASE,LINE_SPACING,SPACING,PREFER_KERNEL_TYPES,MACRO_ARG_REUSE,PARENTHESIS_ALIGNMENT,SPDX_LICENSE_TAG
			${HYP_STYLE_SOURCES}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		VERBATIM)
else()
	ADD_CUSTOM_TARGET(checkpatch
		COMMAND ${CMAKE_COMMAND} -DTOOL=checkpatch.pl -P ${CMAKE_CURRENT_LIST_DIR}/MissingTool.cmake)
endif()
