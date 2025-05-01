// Copyright 2024 Gregory Tolmachev

#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace utils {

template <typename T>
concept Containable = std::is_same_v<std::remove_cvref_t<T>, T>;

template <Containable K>
class FibonacciHeap final {
 private:
  using key_type = K;
  struct Node {
    Node* next;
    Node* prev;
    Node* parent;
    Node* child;
    key_type key;
    bool marked;
    std::uint32_t rank;

    explicit Node(const key_type& data)
        : next(),
          prev(),
          parent(),
          child(nullptr),
          key(data),
          marked(false),
          rank(0) {}
  };
  using NodePtr = Node*;

  NodePtr root_;
  bool no_delete_ = false;

  void AddAfter(NodePtr node, NodePtr obj);
  NodePtr FindMin();
  NodePtr FindNewMin();
  NodePtr CopySubtree(const NodePtr& node,
                      std::unordered_map<NodePtr, NodePtr>& node_map,
                      const NodePtr& parent = nullptr);
  void Link(NodePtr y, NodePtr x);
  void Consolidate();
  NodePtr FindElement(key_type key, NodePtr node);
  NodePtr FindElement(key_type key);
  void TransferToRoots(NodePtr node);
  void CollectForDelete(NodePtr ptr, std::queue<NodePtr>& queue);

 public:
  FibonacciHeap();
  FibonacciHeap(FibonacciHeap&& other) noexcept;
  FibonacciHeap(const FibonacciHeap& other);
  FibonacciHeap& operator=(FibonacciHeap other);
  ~FibonacciHeap();

  void Insert(key_type data);
  std::optional<key_type> DeleteMin();
  FibonacciHeap Merge(const FibonacciHeap& other);
  void DecreaseKey(key_type x, key_type y);
  
  void PrintTreeRoots();
  void CheckIntegrity();
  void PrintHeapASCII(NodePtr node = nullptr, int depth = 0, bool is_last = true);
};

} // namespace utils

#include "fibonacci_heap.tpp" 