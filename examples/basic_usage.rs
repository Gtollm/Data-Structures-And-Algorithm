use kd_tree::KdTree;
use kd_tree::Point2D;

fn main() {
  let mut tree = KdTree::new(2);

  let points = vec![
    Point2D { x: 1.0, y: 2.0 },
    Point2D { x: 3.0, y: 4.0 },
    Point2D { x: 5.0, y: 6.0 },
    Point2D { x: 7.0, y: 8.0 },
  ];

  for point in points {
    tree.insert(point);
  }

  let target = Point2D { x: 2.0, y: 3.0 };
  if let Some(nearest) = tree.nearest_neighbor(&target) {
    println!(
      "Nearest neighbor to ({}, {}) is ({}, {})",
      target.x, target.y, nearest.x, nearest.y
    );
  } else {
    println!("Tree is empty");
  }
}
