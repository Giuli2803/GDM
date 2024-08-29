add_executable(
        ${PROJECT_NAME}_InputActions
        test_InputActions.cpp
)

target_link_libraries(
        ${PROJECT_NAME}_InputActions
        GDMBasics
        gtest
        gtest_main
)

target_compile_definitions(${PROJECT_NAME}_InputActions PRIVATE GDM_TESTING_ENABLED)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_InputActions)
