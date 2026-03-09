# Getting Started

## Installation

### C++ Integration
The logger can be installed on the system and found using CMake:

```cmake
find_package(Logger REQUIRED)
target_link_libraries(your_app PRIVATE Logger::logger)
```

Alternatively, you can include it as a subdirectory:
```cmake
add_subdirectory(libs/Logger/cpp)
target_link_libraries(your_app PRIVATE Logger::logger)
```

### Python Integration
To install the logger once for the entire system:

```bash
sudo pip install ./python
```

*Note: If you are on a restricted environment (like recent Ubuntu versions), you may need to add `--break-system-packages` or install it within a user environment using `pip install --user ./python`.*

## Basic Usage

### C++
```cpp
#include "logger.h"

int main() {
    logger::info("Main") << "Application started!";
    return 0;
}
```

### Python
```python
import logger

logger.info("Main", "Application started!")
```

> [!NOTE]
> Ensure the `python/src` directory is in your `PYTHONPATH`.
