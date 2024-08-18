cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(
        GDMTests
)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        ${PROJECT_NAME}
        test_Basics.cpp
)

target_link_libraries(
        ${PROJECT_NAME}
        GDMBasics
        gtest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
