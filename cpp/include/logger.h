#ifndef LOGGER_H
#define LOGGER_H

#include <condition_variable>
#include <deque>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace logger {

enum class Level { DEBUG, INFO, WARN, ERR };

struct LogEntry {
  Level level;
  std::string sender;
  std::string message;
  std::chrono::system_clock::time_point timestamp;
};

class Logger {
public:
  static Logger &getInstance();

  void log(Level level, std::string_view sender, std::string_view message);

  // Helper methods for direct logging
  void debug(std::string_view sender, std::string_view message) {
    log(Level::DEBUG, sender, message);
  }
  void info(std::string_view sender, std::string_view message) {
    log(Level::INFO, sender, message);
  }
  void warn(std::string_view sender, std::string_view message) {
    log(Level::WARN, sender, message);
  }
  void err(std::string_view sender, std::string_view message) {
    log(Level::ERR, sender, message);
  }

  void setFile(std::string_view path);
  void stop();

private:
  Logger();
  ~Logger();
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  void workerLoop();
  std::string levelToString(Level level);

  std::string _file_path;
  std::ofstream _file_stream;
  bool _file_enabled{false};

  std::deque<LogEntry> _queue;
  std::mutex _mutex;
  std::condition_variable _cv;
  std::thread _worker;
  bool _stop{false};
};

// Helper class for stream-like syntax: Logger::getInstance().info("Sender") <<
// "Msg";
class LogStream {
public:
  LogStream(Level level, const std::string_view &sender)
      : _level(level), _sender(sender) {}
  ~LogStream() { Logger::getInstance().log(_level, _sender, _oss.str()); }

  template <typename T> LogStream &operator<<(const T &val) {
    _oss << val;
    return *this;
  }

  // Support for std::endl and other manipulators
  LogStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
    _oss << manip;
    return *this;
  }

private:
  Level _level;
  std::string _sender;
  std::ostringstream _oss;
};

// Convenience functions to start a stream
inline LogStream debug(std::string_view sender) {
  return LogStream(Level::DEBUG, sender);
}
inline LogStream info(std::string_view sender) {
  return LogStream(Level::INFO, sender);
}
inline LogStream warn(std::string_view sender) {
  return LogStream(Level::WARN, sender);
}
inline LogStream err(std::string_view sender) {
  return LogStream(Level::ERR, sender);
}

} // namespace logger

#endif // LOGGER_H
