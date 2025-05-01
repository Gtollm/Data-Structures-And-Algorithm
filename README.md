# Red-Black Tree

A C++ implementation of a self-balancing binary search tree that maintains balance through color properties, ensuring efficient operations with guaranteed logarithmic time complexity.

## Features

- Self-balancing binary search tree with guaranteed O(log n) operations
- Efficient time complexity for key operations:
  - Insert: O(log n)
  - Delete: O(log n)
  - Search: O(log n)
  - FindMin/FindMax: O(log n)
- Modern C++ implementation using templates (C++20)
- Support for custom key types with proper comparison operators
- Memory-safe with proper resource management (using smart pointers)
- Bidirectional iterators for convenient tree traversal
- ASCII tree visualization for debugging

## Project Structure

- `red_black_tree.hpp` - Primary header with class declarations
- `red_black_tree.tpp` - Template implementation file
- `main.cpp` - Example usage and demonstration
- `CMakeLists.txt` - Build configuration

## Usage

```cpp
#include "red_black_tree.hpp"

int main() {
  // Create a tree
  utils::RBTree<int> tree;

  // Insert elements
  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);

  // Search for elements
  bool exists = tree.Search(3); // Returns true

  // Delete elements
  tree.Delete(5);

  // Find minimum and maximum
  auto min = tree.Minimum(); // Returns 3
  auto max = tree.Maximum(); // Returns 7
  
  // Iterate through the tree
  for (const auto& element : tree) {
    std::cout << element << " ";
  }
  
  return 0;
}
```

## API Reference

| Method | Description | Time Complexity |
|--------|-------------|-----------------|
| `Insert(key)` | Adds a new element to the tree | O(log n) |
| `Delete(key)` | Removes an element from the tree | O(log n) |
| `Search(key)` | Checks if an element exists | O(log n) |
| `Minimum()` | Returns the minimum element | O(log n) |
| `Maximum()` | Returns the maximum element | O(log n) |
| `Height()` | Returns the height of the tree | O(n) |
| `PrintTree()` | Prints the tree structure | O(n) |
| `begin()`, `end()` | Iterator methods for range-based for loops | O(log n) |
| `cbegin()`, `cend()` | Const iterator methods | O(log n) |

## Building

The project uses CMake for building:

```bash
# Clone the repository
git clone https://github.com/yourusername/red-black-tree.git
cd red-black-tree

# Create a build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run the example
./red_black_tree_example
```

## Red-Black Tree Properties

The implementation maintains the following red-black tree properties:

1. Every node is either red or black
2. The root is black
3. All leaves (NIL) are black
4. If a node is red, then both its children are black
5. Every path from root to leaves contains the same number of black nodes

These properties ensure that the tree remains balanced after insertions and deletions.

## License

This project is licensed under the MIT License - see the LICENSE file for details. 