# Fibonacci Heap

A C++ implementation of a Fibonacci Heap data structure with support for efficient priority queue operations.

## Features

- Efficient amortized time complexity for key operations:
  - Insert: O(1)
  - DeleteMin: O(log n)
  - DecreaseKey: O(1)
  - Merge: O(1)
- Modern C++ implementation using templates (C++20)
- Support for custom key types with proper comparison operators
- Memory-safe with proper resource management
- Comprehensive test suite with Google Test
- Debugging utilities for heap integrity checking

## Project Structure

- `fibonacci_heap.hpp` - Primary header with class declarations
- `fibonacci_heap.tpp` - Template implementation file
- `main.cpp` - Example usage and demonstration
- `console_app.cpp` - Command-line application
- `tests/` - Directory containing test files
- `CMakeLists.txt` - Build configuration

## Usage

```cpp
#include "fibonacci_heap.hpp"

int main() {
  // Create a heap
  utils::FibonacciHeap<int> heap;

  // Insert elements
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(7);

  // Get minimum element
  auto min = heap.DeleteMin(); // Returns std::optional containing 3

  // Decrease key
  heap.DecreaseKey(7, 2);

  // Merge two heaps
  utils::FibonacciHeap<int> other;
  other.Insert(4);
  auto merged = heap.Merge(other);
  
  return 0;
}
```

## API Reference

| Method | Description | Time Complexity |
|--------|-------------|-----------------|
| `Insert(key)` | Adds a new element to the heap | O(1) |
| `DeleteMin()` | Removes and returns the minimum element | O(log n) amortized |
| `DecreaseKey(old, new)` | Decreases the value of a key | O(1) amortized |
| `Merge(other)` | Merges another heap into this one | O(1) |
| `PrintHeapASCII()` | Prints the heap structure | O(n) |

## Building

The project uses CMake for building:

```bash
# Clone the repository
git clone https://github.com/yourusername/fibonacci-heap.git
cd fibonacci-heap

# Create a build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run the example
./fibonacci_heap_example

# Run the tests
ctest
```

## Notes

- The implementation uses modern C++ features including concepts and templates
- The heap maintains its structure through consolidation operations
- Debugging utilities are available for verifying heap integrity

## License

This project is licensed under the MIT License - see the LICENSE file for details. 