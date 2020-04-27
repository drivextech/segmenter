cmake_minimum_required(VERSION 3.1.8)
project(Segmenter VERSION 0.1.0)

option(SELF_TEST_ENABLE "If is standalone project? (not part of some project)" OFF)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(SELF_TEST_ENABLE)
    execute_process(
        COMMAND git clone https://github.com/drivextech/dxt_common
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    add_subdirectory(dxt_common)
endif()
unset(SELF_TEST_ENABLE CACHE)

include_directories(${PROJECT_SOURCE_DIR})

add_library(Segmenter_C STATIC segmenter.c)
add_dependencies(Segmenter_C dxt_common)
target_link_libraries(Segmenter_C dxt_common)

add_library(Segmenter_CPP STATIC segmenter.cpp)
add_dependencies(Segmenter_CPP dxt_common)
target_link_libraries(Segmenter_CPP dxt_common)

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
