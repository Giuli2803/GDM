cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(
        GDMateCamExample
        VERSION 0.0.1
        DESCRIPTION "GDMate example project for better explanation of the different Camera::ScaleType"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source file/s
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC GDMBasics)

# Copy the resources directory to the build directory
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
