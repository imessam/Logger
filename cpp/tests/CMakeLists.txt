find_package(GTest REQUIRED)
add_executable(logger_test test_logger.cpp)


target_link_libraries(logger_test PRIVATE GTest::gtest_main logger)

# Resolve liblogger.so from the build tree rather than a stale system install.
# DT_RPATH (not DT_RUNPATH) is consulted before LD_LIBRARY_PATH, so it wins even
# when the environment points at /usr/local/lib. --disable-new-dtags forces RPATH.
target_link_options(logger_test PRIVATE "LINKER:--disable-new-dtags")
set_target_properties(logger_test PROPERTIES
  BUILD_RPATH "${CMAKE_BINARY_DIR}/src")

# include(GoogleTest)
# gtest_discover_tests(logger_test)
