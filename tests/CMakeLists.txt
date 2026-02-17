add_executable(cutscene_tests
    test_main.cpp
    drop_widget_test.cpp
    ../src/DropWidget.h
)

set_target_properties(cutscene_tests PROPERTIES
    AUTOMOC ON
)

target_include_directories(cutscene_tests
    PRIVATE
        ../src
)

target_link_libraries(cutscene_tests
    PRIVATE
        GTest::gtest
        Qt6::Core
        Qt6::Widgets
)

include(GoogleTest)
gtest_add_tests(TARGET cutscene_tests)
