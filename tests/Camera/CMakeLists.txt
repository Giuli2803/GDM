add_executable(
        ${PROJECT_NAME}_Camera
        test_Camera.cpp
)

target_link_libraries(
        ${PROJECT_NAME}_Camera
        GDMBasics
        gtest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Camera)