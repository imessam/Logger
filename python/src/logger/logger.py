from typing import Optional, Union, Any, Type, Dict
import threading
import queue
import sys
from enum import Enum
from datetime import datetime
import os

class Level(Enum):
    """Log levels for the logger library."""
    DEBUG = 0
    INFO = 1
    WARN = 2
    ERR = 3

class LogEntry:
    """represents a single log message with metadata."""
    def __init__(self, level: Level, sender: str, message: str, timestamp: str):
        self.level: Level = level
        self.sender: str = sender
        self.message: str = message
        self.timestamp: str = timestamp

class Logger:
    """
    Asynchronous logger that writes messages to console and optionally to a file.
    
    This class follows the Singleton pattern to ensure a single background thread
    manages all logging operations.
    """
    _instance: Optional['Logger'] = None
    _lock: threading.Lock = threading.Lock()

    def __new__(cls: Type['Logger']) -> 'Logger':
        with cls._lock:
            if cls._instance is None:
                cls._instance = super(Logger, cls).__new__(cls)
                cls._instance._initialized = False
            return cls._instance

    def __init__(self) -> None:
        if self._initialized:
            return
        
        self._queue: queue.Queue[Optional[LogEntry]] = queue.Queue()
        self._stop_event: threading.Event = threading.Event()
        self._file_path: Optional[str] = None
        self._file_handle: Optional[Any] = None
        self._file_lock: threading.Lock = threading.Lock()
        
        self._worker_thread: threading.Thread = threading.Thread(target=self._worker_loop, daemon=True)
        self._worker_thread.start()
        self._initialized: bool = True

    def set_file(self, path: Optional[str]) -> None:
        """
        Sets the output file for the logger.
        
        Args:
            path: Path to the log file. If None, file logging is disabled.
        """
        with self._file_lock:
            if self._file_handle:
                self._file_handle.close()
            self._file_path = path
            if path:
                try:
                    self._file_handle = open(path, 'a')
                except Exception as e:
                    print(f"[Logger][ERROR] Failed to open log file {path}: {e}")
                    self._file_path = None
                    self._file_handle = None

    def log(self, level: Level, sender: str, message: str) -> None:
        """
        Adds a message to the logging queue.
        
        Args:
            level: The severity level of the log.
            sender: The name of the component sending the log.
            message: The message content.
        """
        if self._stop_event.is_set():
            return
        
        timestamp: str = datetime.now().strftime("%H:%M:%S")
        entry = LogEntry(level, sender, message, timestamp)
        self._queue.put(entry)

    def debug(self, sender: str, message: str) -> None:
        """Logs a DEBUG level message."""
        self.log(Level.DEBUG, sender, message)

    def info(self, sender: str, message: str) -> None:
        """Logs an INFO level message."""
        self.log(Level.INFO, sender, message)

    def warn(self, sender: str, message: str) -> None:
        """Logs a WARN level message."""
        self.log(Level.WARN, sender, message)

    def err(self, sender: str, message: str) -> None:
        """Logs an ERR level message."""
        self.log(Level.ERR, sender, message)

    def stop(self) -> None:
        """Stops the background worker thread and closes any open files."""
        self._stop_event.set()
        self._queue.put(None) # Signal worker to exit
        if self._worker_thread.is_alive():
            self._worker_thread.join(timeout=1.0)
        with self._file_lock:
            if self._file_handle:
                self._file_handle.close()
                self._file_handle = None

    def _worker_loop(self) -> None:
        """Main loop for the background worker thread."""
        while True:
            entry = self._queue.get()
            if entry is None: # Exit signal
                break
            
            level_str: str = entry.level.name
            formatted: str = f"[{entry.sender}][{entry.timestamp}][{level_str}] {entry.message}"
            
            # Print to console
            if entry.level in (Level.ERR, Level.WARN):
                print(formatted, file=sys.stderr)
            else:
                print(formatted)
            
            # Write to file
            with self._file_lock:
                if self._file_handle:
                    self._file_handle.write(formatted + "\n")
                    self._file_handle.flush()
            
            self._queue.task_done()

def get_logger() -> Logger:
    """Returns the singleton Logger instance."""
    return Logger()

def debug(sender: str, message: str) -> None:
    """Global convenience function for debug logging."""
    get_logger().debug(sender, message)

def info(sender: str, message: str) -> None:
    """Global convenience function for info logging."""
    get_logger().info(sender, message)

def warn(sender: str, message: str) -> None:
    """Global convenience function for warn logging."""
    get_logger().warn(sender, message)

def err(sender: str, message: str) -> None:
    """Global convenience function for error logging."""
    get_logger().err(sender, message)
