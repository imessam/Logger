# C++ API Reference

The C++ API lives in the `logger` namespace.

## Classes

### `logger::Logger`
The main singleton class managing the background thread and sinks.

- `static Logger& getInstance()`: Get the singleton instance.
- `void setFile(const std::string& path)`: Enable logging to a file.
- `void log(Level level, const std::string& sender, const std::string& message)`: Low-level log function.

### `logger::LogStream`
Helper class for streaming syntax.

- `operator<<`: Template operator to append data to the message.

## Helper Functions
These functions return a temporary `LogStream` and are the preferred way to log:

- `logger::debug(sender)`
- `logger::info(sender)`
- `logger::warn(sender)`
- `logger::err(sender)`

## Enums

### `logger::Level`
- `DEBUG`
- `INFO`
- `WARN`
- `ERR`
