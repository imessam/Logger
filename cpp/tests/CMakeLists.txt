find_package(GTest REQUIRED)
add_executable(logger_test test_logger.cpp)


target_link_libraries(logger_test PRIVATE GTest::gtest_main logger)

# include(GoogleTest)
# gtest_discover_tests(logger_test)
