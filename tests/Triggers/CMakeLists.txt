add_executable(
        ${PROJECT_NAME}_Triggers
        test_Triggers.cpp
)

target_link_libraries(
        ${PROJECT_NAME}_Triggers
        GDMBasics
        gtest
        gtest_main
)

target_compile_definitions(${PROJECT_NAME}_Triggers PRIVATE GDM_TESTING_ENABLED)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Triggers)
