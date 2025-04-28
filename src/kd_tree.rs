#[derive(Debug, Copy, Clone)]
pub struct Point2D {
  pub x: f64,
  pub y: f64,
}
pub trait KdPoint: Copy {
  fn coordinate(&self, axis: usize) -> f64;

  fn distance_squared(&self, other: &Self, dimensions: usize) -> f64 {
    let mut sum = 0.0;
    for axis in 0..dimensions {
      let diff = self.coordinate(axis) - other.coordinate(axis);
      sum += diff * diff;
    }
    sum
  }
  fn distance(&self, other: &Self, dimensions: usize) -> f64 {
    self.distance_squared(other, dimensions).sqrt()
  }
}

impl KdPoint for Point2D {
  fn coordinate(&self, axis: usize) -> f64 {
    match axis {
      0 => self.x,
      1 => self.y,
      _ => panic!("Point2D only has two axes (0 and 1)"),
    }
  }
}
#[derive(Debug)]
struct Node<P: KdPoint> {
  point: P,
  left: Option<Box<Node<P>>>,
  right: Option<Box<Node<P>>>,
}

impl<P: KdPoint> Node<P> {
  fn new(point: P) -> Self {
    Node {
      point,
      left: None,
      right: None,
    }
  }
  fn insert(&mut self, new_node: P, depth: usize, dimensions: usize) {
    let axis = depth % dimensions;
    let current_value = self.point.coordinate(axis);
    let new_value = new_node.coordinate(axis);

    if new_value < current_value {
      if let Some(ref mut left) = self.left {
        left.insert(new_node, depth + 1usize, dimensions);
      } else {
        self.left = Some(Box::new(Node::new(new_node)));
      }
    } else if let Some(ref mut right) = self.right {
      right.insert(new_node, depth + 1usize, dimensions);
    } else {
      self.right = Some(Box::new(Node::new(new_node)))
    }
  }
  fn nearest_update<'a>(
    &'a self, target: &P, depth: usize, dimensions: usize, best: (&'a P, f64),
  ) -> (&'a P, f64) {
    let current_distance = self.point.distance_squared(target, dimensions);
    let mut current_best = if current_distance < best.1 {
      (&self.point, current_distance)
    } else {
      best
    };

    let axis = depth % dimensions;
    let current_coord = self.point.coordinate(axis);
    let target_coord = target.coordinate(axis);

    let (first, second) = if target_coord < current_coord {
      (&self.left, &self.right)
    } else {
      (&self.right, &self.left)
    };

    if let Some(first_node) = first {
      current_best =
        first_node.nearest_update(target, depth + 1, dimensions, current_best);
    }

    let diff = target_coord - current_coord;

    if diff.powf(2f64) < current_best.1 {
      if let Some(second_node) = second {
        current_best = second_node.nearest_update(
          target,
          depth + 1,
          dimensions,
          current_best,
        );
      }
    }

    current_best
  }
}

#[derive(Debug)]
pub struct KdTree<P: KdPoint> {
  root: Option<Box<Node<P>>>,
  dimensions: usize,
}

impl<P: KdPoint> KdTree<P> {
  pub fn new(dimensions: usize) -> Self {
    KdTree {
      root: None,
      dimensions,
    }
  }
  pub fn insert(&mut self, point: P) {
    if let Some(ref mut node) = self.root {
      node.insert(point, 0, self.dimensions);
    } else {
      self.root = Some(Box::new(Node::new(point)));
    }
  }
  pub fn nearest_neighbor(&self, target: &P) -> Option<&P> {
    self.root.as_ref().map(|node| {
      let initial_distance =
        node.point.distance_squared(target, self.dimensions);
      let initial_best = (&node.point, initial_distance);
      let best = node.nearest_update(target, 0, self.dimensions, initial_best);
      best.0
    })
  }
}
