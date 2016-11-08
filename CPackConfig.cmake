# Variable definitions are here:
# https://cmake.org/Wiki/CMake:Packaging_With_CPack
# https://cmake.org/cmake/help/v3.0/module/CPack.html

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A pure-C math library for 2D/3D graphics (matrix, vector, quaternion) Seeks to be close to C89/C90 compliant for portability.")
SET(CPACK_PACKAGE_VENDOR "Darryl T. Agostinelli")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

SET(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
