# Asynchronous Logger Library

A high-performance, asynchronous logging library for C++ and Python. Designed to prevent logging I/O from blocking your application's main execution flow.

## Features
- **Asynchronous Design**: Logging operations happen in a background thread.
- **Micro-Free C++ API**: Modern C++ streaming syntax (`logger::info("Sender") << "Message"`).
- **Python Integration**: Clean, idiomatic Python API.
- **Optional File Output**: Easily enable logging to a file at runtime.
- **Thread-Safe**: Safe to use from multiple threads simultaneously.

## C++ Usage

### Basic Logging
```cpp
#include "logger.h"

// Stream-like syntax
logger::info("MyClass") << "Successfully initialized model " << modelPath;
logger::err("MyClass") << "Failed to process frame " << frameId;
```

### Enabling File Output
```cpp
logger::Logger::getInstance().setFile("app.log");
```

## Python Usage

## Installation

### Python
You can install the logger for the current user or system-wide.

**Option 1: System-wide (Recommended for shared use)**
```bash
sudo pip install ./python
```
*Note: If you are on a modern Linux distribution (e.g., Ubuntu 23.04+) and see an "externally-managed-environment" error, use:*
`sudo pip install ./python --break-system-packages`

**Option 2: User-only**
```bash
pip install --user ./python
```

**Option 3: Editable mode (For development)**
```bash
pip install -e ./python
```

### C++
The library uses CMake for building and installation.

#### Build and Install
```bash
mkdir build && cd build
cmake ../cpp
make
sudo make install
```

#### Linking in other projects
Once installed, you can find and link the library in your `CMakeLists.txt`:
```cmake
find_package(Logger REQUIRED)
target_link_libraries(your_app PRIVATE Logger::logger)
```

## Basic Usage

### C++
```cpp
#include "logger.h"

// Stream-like syntax
logger::info("MyClass") << "Successfully initialized model " << modelPath;
```

### Python
```python
import logger

# Convenience functions
logger.info("MyScript", "Starting processing...")
```

## Documentation

Full documentation is available at [docs/index.md](docs/index.md) or can be generated using MkDocs:

```bash
mkdocs serve
```