cmake_minimum_required(VERSION 2.8)

PROJECT(hypatia_master)

enable_testing()

ADD_SUBDIRECTORY("src")
ADD_SUBDIRECTORY("test")

OPTION(HYPATIA_BUILD_DOCS "Build help documents" TRUE)
IF(HYPATIA_BUILD_DOCS)
	ADD_SUBDIRECTORY("doc")
ENDIF(HYPATIA_BUILD_DOCS)
