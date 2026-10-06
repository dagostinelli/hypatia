cmake_minimum_required(VERSION 3.10)

# A program that uses hypatia the way a user's project would: through
# find_package (installed) or add_subdirectory (HYPATIA_SOURCE_DIR set).
PROJECT(hypatia_package_test LANGUAGES C)

if(HYPATIA_SOURCE_DIR)
	ADD_SUBDIRECTORY(${HYPATIA_SOURCE_DIR} hypatia)
else()
	FIND_PACKAGE(hypatia 2.0 REQUIRED)
endif()

ADD_EXECUTABLE(package_test main.c)
TARGET_LINK_LIBRARIES(package_test PRIVATE hypatia::hypatia)

enable_testing()
ADD_TEST(NAME package_test COMMAND package_test)
