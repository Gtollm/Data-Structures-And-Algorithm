# Kd-Tree

A Rust implementation of a k-dimensional tree data structure for efficient nearest neighbor searches in multi-dimensional spaces.

## Features

- Generic implementation supporting any number of dimensions
- Efficient nearest neighbor search with O(log n) average case complexity
- Type-safe implementation using Rust's trait system
- Built-in support for 2D points with extensible design for custom point types
- Memory efficient with optional boxed nodes

## Usage

Add this to your `Cargo.toml`:

```toml
[dependencies]
kd_tree = "0.1.0"
```

Then in your code:

```rust
use kd_tree::{KdTree, Point2D};

// Create a new kd-tree for 2D points
let mut tree = KdTree::new(2);

// Insert points
tree.insert(Point2D { x: 1.0, y: 2.0 });
tree.insert(Point2D { x: 3.0, y: 4.0 });
tree.insert(Point2D { x: 5.0, y: 6.0 });

// Find nearest neighbor
let target = Point2D { x: 2.0, y: 3.0 };
let nearest = tree.nearest_neighbor(&target);
```

## Custom Point Types

You can implement your own point types by implementing the `KdPoint` trait:

```rust
use kd_tree::KdPoint;

struct Point3D {
    x: f64,
    y: f64,
    z: f64,
}

impl KdPoint for Point3D {
    fn coordinate(&self, axis: usize) -> f64 {
        match axis {
            0 => self.x,
            1 => self.y,
            2 => self.z,
            _ => panic!("Point3D only has three axes (0, 1, and 2)"),
        }
    }
}
```

## API Reference

| Method | Description | Time Complexity |
|--------|-------------|-----------------|
| `new(dimensions)` | Creates a new kd-tree | O(1) |
| `insert(point)` | Inserts a point into the tree | O(log n) average |
| `nearest_neighbor(target)` | Finds the nearest neighbor | O(log n) average |

## Examples

Run the example:

```bash
cargo run --example basic_usage
```

## Tests

Run the tests:

```bash
cargo test
```

## Notes

- The implementation uses Rust's ownership system for memory safety
- Points are stored by value for efficiency
- The tree structure is automatically balanced during insertion
- Nearest neighbor search uses squared distance for optimization

## License

This project is licensed under the MIT License - see the LICENSE file for details. 
