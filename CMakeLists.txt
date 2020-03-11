cmake_minimum_required(VERSION 3.0.8)
project(Segmenter VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(Segmenter_C segmenter.c packer.c)

add_library(Segmenter_CPP segmenter.cpp packer.c)

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
