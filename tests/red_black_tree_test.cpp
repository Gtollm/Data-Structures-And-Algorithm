// Copyright 2024 Gregory Tolmachev

#include <gtest/gtest.h>
#include <stdexcept>

#include "red_black_tree.hpp"

TEST(RedBlackTree, EmptyTree) {
  utils::RBTree<int> tree;

  EXPECT_EQ(tree.GetRoot(), nullptr);
  EXPECT_EQ(nullptr, tree.Search(5));
}

TEST(RedBlackTree, InsertAndSearch) {
  utils::RBTree<int> tree;

  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(2);
  tree.Insert(4);
  tree.Insert(6);
  tree.Insert(8);

  EXPECT_NE(nullptr, tree.Search(5));
  EXPECT_NE(nullptr, tree.Search(3));
  EXPECT_NE(nullptr, tree.Search(7));
  EXPECT_NE(nullptr, tree.Search(2));
  EXPECT_NE(nullptr, tree.Search(4));
  EXPECT_NE(nullptr, tree.Search(6));
  EXPECT_NE(nullptr, tree.Search(8));

  EXPECT_EQ(nullptr, tree.Search(1));
  EXPECT_EQ(nullptr, tree.Search(9));
}

TEST(RedBlackTree, MinMaxThrowsOnEmpty) {
  utils::RBTree<int> tree;

  try {
    [[maybe_unused]] int min = tree.Minimum();
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error&) {
  }

  try {
    [[maybe_unused]] int max = tree.Maximum();
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error&) {
  }
}

TEST(RedBlackTree, MinMax) {
  utils::RBTree<int> tree;

  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(2);
  tree.Insert(4);
  tree.Insert(6);
  tree.Insert(8);

  EXPECT_EQ(2, tree.Minimum());
  EXPECT_EQ(8, tree.Maximum());

  tree.Insert(1);
  tree.Insert(10);

  EXPECT_EQ(1, tree.Minimum());
  EXPECT_EQ(10, tree.Maximum());
}

TEST(RedBlackTree, LeastUpperBound) {
  utils::RBTree<int> tree;

  EXPECT_EQ(nullptr, tree.LeastUpperBound(5));

  tree.Insert(10);
  tree.Insert(5);
  tree.Insert(15);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(12);
  tree.Insert(20);

  auto result = tree.LeastUpperBound(6);
  ASSERT_NE(nullptr, result);
  EXPECT_EQ(7, result->key_);

  result = tree.LeastUpperBound(7);
  ASSERT_NE(nullptr, result);
  EXPECT_EQ(10, result->key_);

  result = tree.LeastUpperBound(21);
  EXPECT_EQ(nullptr, result);
}

TEST(RedBlackTree, GreatestLowerBound) {
  utils::RBTree<int> tree;

  EXPECT_EQ(nullptr, tree.GreatestLowerBound(5));

  tree.Insert(10);
  tree.Insert(5);
  tree.Insert(15);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(12);
  tree.Insert(20);

  auto result = tree.GreatestLowerBound(6);
  ASSERT_NE(nullptr, result);
  EXPECT_EQ(5, result->key_);

  result = tree.GreatestLowerBound(7);
  ASSERT_NE(nullptr, result);
  EXPECT_EQ(5, result->key_);

  result = tree.GreatestLowerBound(1);
  EXPECT_EQ(nullptr, result);
}

TEST(RedBlackTree, DeleteFromEmpty) {
  utils::RBTree<int> tree;

  EXPECT_NO_THROW(tree.Delete(5));
}

TEST(RedBlackTree, DeleteExistingNodes) {
  utils::RBTree<int> tree;

  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(2);
  tree.Insert(4);
  tree.Insert(6);
  tree.Insert(8);

  tree.Delete(2);
  EXPECT_EQ(nullptr, tree.Search(2));
  EXPECT_EQ(3, tree.Minimum());

  tree.Delete(3);
  EXPECT_EQ(nullptr, tree.Search(3));
  EXPECT_EQ(4, tree.Minimum());

  tree.Delete(7);
  EXPECT_EQ(nullptr, tree.Search(7));
  EXPECT_EQ(8, tree.Maximum());

  tree.Delete(5);
  EXPECT_EQ(nullptr, tree.Search(5));

  EXPECT_NE(nullptr, tree.Search(4));
  EXPECT_NE(nullptr, tree.Search(6));
  EXPECT_NE(nullptr, tree.Search(8));
}

TEST(RedBlackTree, EmptyTreeIterator) {
  utils::RBTree<int> tree;

  auto it = tree.begin();
  EXPECT_EQ(it, tree.end());
}

TEST(RedBlackTree, IteratorTraversal) {
  utils::RBTree<int> tree;

  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(2);
  tree.Insert(4);
  tree.Insert(6);
  tree.Insert(8);

  int expected[] = {2, 3, 4, 5, 6, 7, 8};
  int i = 0;

  for (const auto& value : tree) {
    EXPECT_EQ(value, expected[i++]);
  }

  EXPECT_EQ(i, 7);
}

TEST(RedBlackTree, TreeHeight) {
  utils::RBTree<int> tree;

  EXPECT_EQ(0, tree.Height());

  tree.Insert(5);
  EXPECT_GE(tree.Height(), 1);

  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(2);
  tree.Insert(4);
  tree.Insert(6);
  tree.Insert(8);

  EXPECT_LE(tree.Height(), 6);
}

TEST(RedBlackTree, Clear) {
  utils::RBTree<int> tree;

  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);

  tree.Clear();

  EXPECT_EQ(nullptr, tree.GetRoot());
  EXPECT_EQ(nullptr, tree.Search(5));
  EXPECT_EQ(nullptr, tree.Search(3));
  EXPECT_EQ(nullptr, tree.Search(7));

  try {
    [[maybe_unused]] int min = tree.Minimum();
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error&) {
  }

  try {
    [[maybe_unused]] int max = tree.Maximum();
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error&) {
  }

  EXPECT_EQ(tree.begin(), tree.end());
}

TEST(RedBlackTree, StringType) {
  utils::RBTree<std::string> tree;

  tree.Insert("banana");
  tree.Insert("apple");
  tree.Insert("cherry");
  tree.Insert("date");

  EXPECT_NE(nullptr, tree.Search("apple"));
  EXPECT_NE(nullptr, tree.Search("banana"));
  EXPECT_NE(nullptr, tree.Search("cherry"));
  EXPECT_NE(nullptr, tree.Search("date"));
  EXPECT_EQ(nullptr, tree.Search("elderberry"));

  EXPECT_EQ("apple", tree.Minimum());
  EXPECT_EQ("date", tree.Maximum());

  tree.Delete("banana");
  EXPECT_EQ(nullptr, tree.Search("banana"));

  std::string expected[] = {"apple", "cherry", "date"};
  int i = 0;

  for (const auto& value : tree) {
    EXPECT_EQ(value, expected[i++]);
  }
}

struct TestPoint {
  int x, y;

  bool operator<(const TestPoint& other) const {
    if (x == other.x) return y < other.y;
    return x < other.x;
  }

  bool operator>(const TestPoint& other) const { return other < *this; }

  bool operator==(const TestPoint& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const TestPoint& other) const { return !(*this == other); }
};

TEST(RedBlackTree, CustomType) {
  utils::RBTree<TestPoint> tree;

  tree.Insert({1, 2});
  tree.Insert({3, 4});
  tree.Insert({5, 6});

  EXPECT_NE(nullptr, tree.Search({1, 2}));
  EXPECT_NE(nullptr, tree.Search({3, 4}));
  EXPECT_NE(nullptr, tree.Search({5, 6}));
  EXPECT_EQ(nullptr, tree.Search({7, 8}));

  TestPoint min = tree.Minimum();
  TestPoint max = tree.Maximum();

  EXPECT_EQ(1, min.x);
  EXPECT_EQ(2, min.y);
  EXPECT_EQ(5, max.x);
  EXPECT_EQ(6, max.y);

  tree.Delete({3, 4});
  EXPECT_EQ(nullptr, tree.Search({3, 4}));
}
