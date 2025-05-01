// Copyright 2024 Gregory Tolmachev


#include "fibonacci_heap.hpp"
#include <gtest/gtest.h>
#include <stdexcept>


TEST(FibonacciHeap, EmptyHeap) {
  utils::FibonacciHeap<int> heap;
  
 auto min = heap.DeleteMin();
  EXPECT_FALSE(min.has_value());
}


TEST(FibonacciHeap, InsertAndDeleteMin) {
  utils::FibonacciHeap<int> heap;
  
  
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(7);
  heap.Insert(2);
  heap.Insert(4);
  
  
  EXPECT_EQ(2, *heap.DeleteMin());
  EXPECT_EQ(3, *heap.DeleteMin());
  EXPECT_EQ(4, *heap.DeleteMin());
  EXPECT_EQ(5, *heap.DeleteMin());
  EXPECT_EQ(7, *heap.DeleteMin());
  
  
  auto min = heap.DeleteMin();
  EXPECT_FALSE(min.has_value());
}


TEST(FibonacciHeap, DecreaseKey) {
  utils::FibonacciHeap<int> heap;
  
  
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(7);
  
  
  heap.DecreaseKey(7, 1);
  
  
  EXPECT_EQ(1, *heap.DeleteMin());
  EXPECT_EQ(3, *heap.DeleteMin());
  EXPECT_EQ(5, *heap.DeleteMin());
}


TEST(FibonacciHeap, Merge) {
  utils::FibonacciHeap<int> heap1;
  utils::FibonacciHeap<int> heap2;
  
  
  heap1.Insert(5);
  heap1.Insert(3);
  heap1.Insert(7);
  
  
  heap2.Insert(2);
  heap2.Insert(4);
  heap2.Insert(6);
  
  
  utils::FibonacciHeap<int> merged = heap1.Merge(heap2);
  
  
  EXPECT_EQ(2, *merged.DeleteMin());
  EXPECT_EQ(3, *merged.DeleteMin());
  EXPECT_EQ(4, *merged.DeleteMin());
  EXPECT_EQ(5, *merged.DeleteMin());
  EXPECT_EQ(6, *merged.DeleteMin());
  EXPECT_EQ(7, *merged.DeleteMin());
  
  
  auto min = merged.DeleteMin();
  EXPECT_FALSE(min.has_value());
}


TEST(FibonacciHeap, StringType) {
  utils::FibonacciHeap<std::string> heap;
  
  
  heap.Insert("banana");
  heap.Insert("apple");
  heap.Insert("cherry");
  heap.Insert("date");
  
  
  EXPECT_EQ("apple", *heap.DeleteMin());
  EXPECT_EQ("banana", *heap.DeleteMin());
  EXPECT_EQ("cherry", *heap.DeleteMin());
  EXPECT_EQ("date", *heap.DeleteMin());
}


struct TestPoint {
  int x, y;
  
  explicit TestPoint(int x = 0, int y = 0) : x(x), y(y) {}
  
  bool operator<(const TestPoint& other) const {
    if (x == other.x) return y < other.y;
    return x < other.x;
  }
  
  bool operator>(const TestPoint& other) const { 
    return other < *this; 
  }
  
  bool operator==(const TestPoint& other) const {
    return x == other.x && y == other.y;
  }
  
  bool operator!=(const TestPoint& other) const { 
    return !(*this == other); 
  }
};

TEST(FibonacciHeap, CustomType) {
  utils::FibonacciHeap<TestPoint> heap;
  
  
  heap.Insert(TestPoint(5, 5));
  heap.Insert(TestPoint(3, 3));
  heap.Insert(TestPoint(4, 4));
  heap.Insert(TestPoint(1, 1));
  heap.Insert(TestPoint(2, 2));
  
  
  TestPoint min = *heap.DeleteMin();
  EXPECT_EQ(1, min.x);
  EXPECT_EQ(1, min.y);
  
  min = *heap.DeleteMin();
  EXPECT_EQ(2, min.x);
  EXPECT_EQ(2, min.y);
  
  min = *heap.DeleteMin();
  EXPECT_EQ(3, min.x);
  EXPECT_EQ(3, min.y);
  
  min = *heap.DeleteMin();
  EXPECT_EQ(4, min.x);
  EXPECT_EQ(4, min.y);
  
  min = *heap.DeleteMin();
  EXPECT_EQ(5, min.x);
  EXPECT_EQ(5, min.y);
}


TEST(FibonacciHeap, LargeHeap) {
  utils::FibonacciHeap<int> heap;
  
  
  const int n = 1000;
  for (int i = n; i > 0; i--) {
    heap.Insert(i);
  }
  
  
  for (int i = 1; i <= n; i++) {
    EXPECT_EQ(i, *heap.DeleteMin());
  }
  
  
  auto min = heap.DeleteMin();
  EXPECT_FALSE(min.has_value());
} 