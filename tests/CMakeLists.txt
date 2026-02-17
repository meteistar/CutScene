add_executable(cutscene_tests
    test_main.cpp
    drop_widget_test.cpp
)

target_link_libraries(cutscene_tests
    PRIVATE
        GTest::gtest
        Qt6::Widgets
)

include(GoogleTest)
gtest_discover_tests(cutscene_tests)
