# Python API Reference

## Functions

### `logger.get_logger()`
Returns the singleton `Logger` instance.

### `logger.debug(sender, message)`
Helper to log at DEBUG level.

### `logger.info(sender, message)`
Helper to log at INFO level.

### `logger.warn(sender, message)`
Helper to log at WARN level.

### `logger.err(sender, message)`
Helper to log at ERR level.

## Classes

### `logger.Logger`
- `set_file(path)`: Enable logging to the specified file.
- `stop()`: Shutdown the background thread and close files.

## Enums

### `logger.Level`
- `DEBUG`
- `INFO`
- `WARN`
- `ERR`
