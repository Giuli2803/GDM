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

target_compile_definitions(${PROJECT_NAME}_Camera PRIVATE GDM_TESTING_ENABLED)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Camera)
