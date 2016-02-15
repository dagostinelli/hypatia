cmake_minimum_required(VERSION 2.8)

PROJECT(hypatia_tests)

INCLUDE_DIRECTORIES(${hypatia_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCC)
	FIND_LIBRARY(LIBM m)
	SET(CMAKE_C_FLAGS_DEBUG "-ggdb -std=c90 -Wextra -Wmissing-prototypes -Wall -Wstrict-prototypes -Wold-style-definition -Werror")
	SET(CMAKE_C_FLAGS_RELEASE "-std=c90 -Wextra -Wmissing-prototypes -Wall -Wstrict-prototypes -Wold-style-definition -Werror")
endif()

IF(HYPATIA_BUILD_SHARED)
	ADD_EXECUTABLE(test_hypatia_shared main.c)
	TARGET_LINK_LIBRARIES(test_hypatia_shared hypatia ${LIBM})
	ADD_TEST(NAME test_hypatia_shared COMMAND test_hypatia_shared)
ENDIF()

IF(HYPATIA_BUILD_STATIC)
	ADD_EXECUTABLE(test_hypatia_static main.c)
	TARGET_LINK_LIBRARIES(test_hypatia_static hypatia_static ${LIBM})
	ADD_TEST(NAME test_hypatia_static COMMAND test_hypatia_static)
ENDIF()

