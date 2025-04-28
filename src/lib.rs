//! A Rust implementation of a k-dimensional tree data structure.
//!
//! This crate provides an efficient implementation of a kd-tree for
//! nearest neighbor searches in multi-dimensional spaces.
//!
//! # Examples
//!
//! ```rust
//! use kd_tree::KdTree;
//! use kd_tree::Point2D;
//!
//! let mut tree = KdTree::new(2);
//! tree.insert(Point2D { x: 1.0, y: 2.0 });
//! tree.insert(Point2D { x: 3.0, y: 4.0 });
//!
//! let target = Point2D { x: 2.0, y: 3.0 };
//! let nearest = tree.nearest_neighbor(&target);
//! ```

mod kd_tree;
pub use kd_tree::*;
