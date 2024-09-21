cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(
        GDMTests
)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

file(GLOB_RECURSE TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(Basics)
add_subdirectory(Camera)
add_subdirectory(Sprite)
add_subdirectory(Triggers)
add_subdirectory(InputActions)
