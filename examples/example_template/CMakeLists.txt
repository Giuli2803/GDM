cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(
        GDMateExample
        VERSION 0.0.1
        DESCRIPTION "GDMate example"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source file/s
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC GDMBasics)
