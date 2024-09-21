find_package(SFML 2.5 COMPONENTS graphics audio window system)
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(
        GDMBasics
        VERSION 0.0.1
        DESCRIPTION "2D Game development engine library"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build library
file(GLOB_RECURSE GDM_SOURCE_CODE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${GDM_SOURCE_CODE})

# Header files
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system)

if (COVERAGE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GDM_TESTING_ENABLED)
endif ()
