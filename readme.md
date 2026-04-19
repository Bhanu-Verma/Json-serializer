# FastJsonLibrary

A high-performance, C++20 JSON serialization and parsing library designed for speed and low memory overhead.

---

## Project Structure
```text
/my_json_library
├── include/      # Public API headers
├── src/          # Source code for library implementation
├── tests/        # Unit tests and performance benchmarks
├── examples/     # Usage examples
└── README.md
```



---

## Getting Started

### Prerequisites
* A C++20 compliant compiler (GCC 10+, Clang 10+, or MSVC 19.29+).
* CMake 3.15 or higher.

### Building the Library
```bash
# Clone the repository
git clone <your-repo-url>
cd FastJsonLibrary

# Build the project
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
mingw32-make  # Or 'make' on Linux/macOS
```

---

## Usage

Simply link the library in your `CMakeLists.txt`:

```cmake
find_package(json_lib REQUIRED)
target_link_libraries(your_app PRIVATE json::json_lib)
```

### Code Example
```cpp
#include <json/json.hpp>
#include <iostream>

int main() {
    json::Json doc;
    doc["name"] = "FastJson";
    doc["version"] = 1.0;
    
    std::cout << doc.serialize() << std::endl;
    return 0;
}
```

---

## Performance
This library is optimized for high-throughput JSON processing. Using the provided `Benchmarker` utility, you can verify parsing performance. 



---

## Contributing
Contributions are welcome! Please ensure that any Pull Request includes:
1.  Updated unit tests in `tests/`.
2.  Verification of zero-regression on performance benchmarks.


