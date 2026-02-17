add_executable(cutscene_tests
    smoke_test.cpp
)

target_link_libraries(cutscene_tests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cutscene_tests)
