#include "logger.h"
#include <ctime>
#include <iomanip>
#include <iostream>

namespace logger {

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

Logger::Logger() : _worker(&Logger::workerLoop, this) {}

Logger::~Logger() { stop(); }

void Logger::log(Level level, const std::string &sender,
                 const std::string &message) {
  std::unique_lock<std::mutex> lock(_mutex);
  if (_stop)
    return;

  LogEntry entry;
  entry.level = level;
  entry.sender = sender;
  entry.message = message;
  entry.timestamp = std::chrono::system_clock::now();

  _queue.push_back(std::move(entry));
  _cv.notify_one();
}

void Logger::setFile(const std::string &path) {
  std::unique_lock<std::mutex> lock(_mutex);
  _file_path = path;
  if (_file_stream.is_open()) {
    _file_stream.close();
  }
  _file_stream.open(_file_path, std::ios::app);
  _file_enabled = _file_stream.is_open();
}

void Logger::stop() {
  {
    std::unique_lock<std::mutex> lock(_mutex);
    if (_stop)
      return;
    _stop = true;
  }
  _cv.notify_all();
  if (_worker.joinable()) {
    _worker.join();
  }
  if (_file_stream.is_open()) {
    _file_stream.close();
  }
}

std::string Logger::levelToString(Level level) {
  switch (level) {
  case Level::DEBUG:
    return "DEBUG";
  case Level::INFO:
    return "INFO";
  case Level::WARN:
    return "WARN";
  case Level::ERR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

void Logger::workerLoop() {
  while (true) {
    LogEntry entry;
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _cv.wait(lock, [this] { return _stop || !_queue.empty(); });

      if (_stop && _queue.empty()) {
        break;
      }

      entry = std::move(_queue.front());
      _queue.pop_front();
    }

    std::string level_str = levelToString(entry.level);

    // Convert timestamp to string in the worker thread
    std::time_t t = std::chrono::system_clock::to_time_t(entry.timestamp);
    std::tm tm_buf{};
    localtime_r(&t, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%H:%M:%S");
    std::string timestamp_str = oss.str();

    std::string formatted = "[" + entry.sender + "][" + timestamp_str + "][" +
                            level_str + "] " + entry.message + "\n";

    // Output to console
    if (entry.level == Level::ERR || entry.level == Level::WARN) {
      std::cerr << formatted << std::flush;
    } else {
      std::cout << formatted << std::flush;
    }

    // Output to file
    if (_file_enabled) {
      _file_stream << formatted;
      _file_stream.flush();
    }
  }
}

} // namespace logger
