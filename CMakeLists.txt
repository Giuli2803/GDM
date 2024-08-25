cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(GDMBuildAll)

option(COVERAGE "Enable coverage reporting" OFF)

if (COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    add_subdirectory(lib)
    add_subdirectory(examples)
    add_subdirectory(tests)
else ()
    add_subdirectory(lib)
    add_subdirectory(examples)
endif ()
