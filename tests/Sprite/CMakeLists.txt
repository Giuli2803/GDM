add_executable(
        ${PROJECT_NAME}_Sprite
        test_Sprite.cpp
)

target_link_libraries(
        ${PROJECT_NAME}_Sprite
        GDMBasics
        gtest
        gtest_main
)

target_compile_definitions(${PROJECT_NAME}_Sprite PRIVATE GDM_TESTING_ENABLED)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Sprite)
