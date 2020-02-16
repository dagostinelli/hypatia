

INCLUDE(../cmake/CMakeBuildFlags.cmake)


IF(HYPATIA_BUILD_SHARED)
	ADD_EXECUTABLE(test_hypatia_shared main.c)
	TARGET_INCLUDE_DIRECTORIES(test_hypatia_shared PUBLIC ${hypatia_SOURCE_DIR}/src)
	TARGET_LINK_LIBRARIES(test_hypatia_shared hypatia ${LIBM})
	ADD_TEST(NAME test_hypatia_shared COMMAND test_hypatia_shared)
	IF (MSVC)
		add_custom_command(TARGET test_hypatia_shared POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:hypatia>/$<TARGET_FILE_NAME:hypatia> $<TARGET_FILE_DIR:test_hypatia_shared>)
		IF (CMAKE_BUILD_TYPE==Debug)
			add_custom_command(TARGET test_hypatia_shared POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hypatia_BINARY_DIR}/hypatia*.pdb" $<TARGET_FILE_DIR:test_hypatia_shared>)
		ENDIF()
	ENDIF()
ENDIF()

IF(HYPATIA_BUILD_STATIC)
	ADD_EXECUTABLE(test_hypatia_static main.c)
	TARGET_INCLUDE_DIRECTORIES(test_hypatia_static PUBLIC ${hypatia_SOURCE_DIR}/src)
	TARGET_LINK_LIBRARIES(test_hypatia_static hypatia_static ${LIBM})
	ADD_TEST(NAME test_hypatia_static COMMAND test_hypatia_static)
ENDIF()

IF(HYPATIA_BUILD_TWOFILE_DISTRIBUTION)
	ADD_EXECUTABLE(test_hypatia_twofile main.c ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/hypatia.c)
	TARGET_INCLUDE_DIRECTORIES(test_hypatia_twofile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../dist/twofile/)
	TARGET_LINK_LIBRARIES(test_hypatia_twofile ${LIBM})
	ADD_TEST(NAME test_hypatia_twofile COMMAND test_hypatia_twofile)
ENDIF()

IF(HYPATIA_BUILD_SINGLE_FILE_HEADER)
	ADD_EXECUTABLE(test_hypatia_singlefile main.c implementation.c)
	TARGET_INCLUDE_DIRECTORIES(test_hypatia_singlefile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../dist/single/)
	TARGET_LINK_LIBRARIES(test_hypatia_singlefile ${LIBM})
	ADD_TEST(NAME test_hypatia_singlefile COMMAND test_hypatia_singlefile)
ENDIF()
