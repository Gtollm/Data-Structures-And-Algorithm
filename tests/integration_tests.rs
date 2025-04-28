use kd_tree::KdTree;
use kd_tree::Point2D;

#[test]
fn test_insert_and_nearest() {
  let mut tree = KdTree::new(2);

  tree.insert(Point2D { x: 1.0, y: 2.0 });
  tree.insert(Point2D { x: 3.0, y: 4.0 });
  tree.insert(Point2D { x: 5.0, y: 6.0 });

  let target = Point2D { x: 2.0, y: 3.0 };
  let nearest = tree.nearest_neighbor(&target).unwrap();
  assert_eq!(nearest.x, 1.0);
  assert_eq!(nearest.y, 2.0);
}

#[test]
fn test_empty_tree() {
  let tree: KdTree<Point2D> = KdTree::new(2);
  let target = Point2D { x: 1.0, y: 1.0 };
  assert!(tree.nearest_neighbor(&target).is_none());
}

#[test]
fn test_custom_point_type() {
  #[derive(Debug, Copy, Clone)]
  struct Point3D {
    x: f64,
    y: f64,
    z: f64,
  }

  impl kd_tree::KdPoint for Point3D {
    fn coordinate(&self, axis: usize) -> f64 {
      match axis {
        0 => self.x,
        1 => self.y,
        2 => self.z,
        _ => panic!("Point3D only has three axes (0, 1, and 2)"),
      }
    }
  }

  let mut tree = KdTree::new(3);
  tree.insert(Point3D {
    x: 1.0,
    y: 2.0,
    z: 3.0,
  });
  tree.insert(Point3D {
    x: 4.0,
    y: 5.0,
    z: 6.0,
  });

  let target = Point3D {
    x: 2.0,
    y: 3.0,
    z: 4.0,
  };
  let nearest = tree.nearest_neighbor(&target).unwrap();
  assert_eq!(nearest.x, 1.0);
  assert_eq!(nearest.y, 2.0);
  assert_eq!(nearest.z, 3.0);
}
