#include "logger.h"
#include <chrono>
#include <fstream>
#include <gtest/gtest.h>
#include <thread>

using namespace logger;

TEST(LoggerTest, AsyncLogging) {
  std::string my_name = "TestObject";

  // Log messages
  info(my_name) << "Hello async info";
  err(my_name) << "Hello async error";

  // Give background thread room to process
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST(LoggerTest, FileLogging) {
  std::string log_file = "/tmp/test_logger.log";
  std::string my_name = "FileLoggerTest";

  Logger::getInstance().setFile(log_file);
  info(my_name) << "This should be in the file";

  // Ensure the message is flushed
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::ifstream ifs(log_file);
  std::string line;
  bool found = false;
  while (std::getline(ifs, line)) {
    if (line.find("This should be in the file") != std::string::npos) {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);

  // Cleanup
  std::remove(log_file.c_str());
}
