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

target_compile_definitions(${PROJECT_NAME} PRIVATE GDM_TESTING_ENABLED)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
