cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(GDMBuildAll)

option(COVERAGE "Enable coverage reporting" OFF)

add_subdirectory(lib)
add_subdirectory(examples)
if (COVERAGE)
    add_subdirectory(tests)
endif ()
