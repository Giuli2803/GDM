cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(
        GDMateExamples
        VERSION 0.0.1
        DESCRIPTION "GDMate libraries examples"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# libraries
add_subdirectory(lib/GDMBasics)

# Example code todo: move into an examples folder
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC lib/GDMBasics/include)

# Libraries to use on examples
target_link_libraries(${PROJECT_NAME} PUBLIC GDMBasics)

# Unit tests
enable_testing()
add_subdirectory(tests)
