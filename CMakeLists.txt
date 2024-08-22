cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(GDMBuildAll)

option(COVERAGE "Enable coverage reporting" ON)
if (COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

add_subdirectory(lib)
add_subdirectory(examples)
if (COVERAGE)
    add_subdirectory(tests)
endif ()
