
INCLUDE(../cmake/CMakeBuildFlags.cmake)

ADD_EXECUTABLE(test_hypatia_single_precision main.c implementation.c)
TARGET_INCLUDE_DIRECTORIES(test_hypatia_single_precision PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
TARGET_LINK_LIBRARIES(test_hypatia_single_precision ${LIBM})
SET_TARGET_PROPERTIES(test_hypatia_single_precision PROPERTIES COMPILE_DEFINITIONS "HYPATIA_SINGLE_PRECISION_FLOATS")
ADD_TEST(NAME test_hypatia_single_precision COMMAND test_hypatia_single_precision)

ADD_EXECUTABLE(test_hypatia_double_precision main.c implementation.c)
TARGET_INCLUDE_DIRECTORIES(test_hypatia_double_precision PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
TARGET_LINK_LIBRARIES(test_hypatia_double_precision ${LIBM})
ADD_TEST(NAME test_hypatia_double_precision COMMAND test_hypatia_double_precision)
