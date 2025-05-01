// Copyright 2024 Gregory Tolmachev

#pragma once

#include <concepts>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace utils {

template <typename T>
concept Containable = std::is_same_v<std::remove_cvref_t<T>, T>;

enum class RbColor { RED, BLACK };

template <Containable T>
struct RbNode {
  T key_;
  RbColor color_;
  std::unique_ptr<RbNode<T>> left_;
  std::unique_ptr<RbNode<T>> right_;
  RbNode *parent_;

  explicit RbNode(const T &data)
      : key_(data),
        color_(RbColor::RED),
        left_(nullptr),
        right_(nullptr),
        parent_(nullptr) {}
};

template <Containable T>
class RBTree {
 private:
  using RbNodePtr = RbNode<T> *;
  using RbNodeUPtr = std::unique_ptr<RbNode<T>>;

  RbNodeUPtr root_;

  void LeftRotate(RbNodePtr x);
  void RightRotate(RbNodePtr x);
  void InsertFixUp(RbNodePtr z);
  void Transplant(RbNodePtr u, RbNodeUPtr v);
  void Delete(RbNodePtr z);
  void DeleteFixUp(RbNodePtr x, RbNodePtr x_parent);
  [[nodiscard]] RbNodePtr Minimum(RbNodePtr node) const;
  [[nodiscard]] RbNodePtr Maximum(RbNodePtr node) const;
  void PrintIndent(int level);
  void PrintTree(RbNodePtr root, int level = 0, bool isLeft = false);
  [[nodiscard]] static int Height(RbNodePtr root);

 public:
  RBTree() : root_(nullptr) {}

  void Insert(const T &data);
  void Delete(const T &data);
  [[nodiscard]] RbNodePtr Search(const T &data) const;
  void PrintTree();
  friend std::ostream &operator<<(std::ostream &out, const RBTree &tree);
  [[nodiscard]] T Minimum() const;
  [[nodiscard]] T Maximum() const;
  [[nodiscard]] RbNodePtr LeastUpperBound(const T &key) const;
  [[nodiscard]] RbNodePtr GreatestLowerBound(const T &key) const;

  template <bool Const>
  class Iterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t<Const, const T, T>;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = std::bidirectional_iterator_tag;

    RbNodePtr node_ = nullptr;

    Iterator() = default;
    explicit Iterator(RbNodePtr node) : node_{node} {}
    Iterator(const Iterator<!Const> &other) : node_{other.node_} {}
    Iterator(const Iterator &other) = default;
    Iterator(Iterator &&other) = default;

    Iterator &operator=(const Iterator &other) = default;
    Iterator &operator=(Iterator &&other) = default;

    reference operator*() const { return node_->key_; }
    pointer operator->() const { return &(node_->key_); }

    Iterator &operator++();
    Iterator operator++(int);
    Iterator &operator--();
    Iterator operator--(int);

    bool operator==(const Iterator &other) const {
      return node_ == other.node_;
    }
    bool operator!=(const Iterator &other) const {
      return node_ != other.node_;
    }
  };

  using iterator_type = Iterator<false>;
  using const_iterator_type = Iterator<true>;

  [[nodiscard]] int Height() const { return Height(root_.get()); }

  [[nodiscard]] iterator_type begin() const;
  [[nodiscard]] const_iterator_type begin();
  [[nodiscard]] const_iterator_type cbegin() const;
  [[nodiscard]] iterator_type end() const { return iterator_type(nullptr); }
  [[nodiscard]] const_iterator_type end();
  [[nodiscard]] const_iterator_type cend() const;

  void Clear() { this->root_ = nullptr; }
  [[nodiscard]] RbNodePtr GetRoot() { return root_.get(); }
  [[nodiscard]] const RbNodePtr GetRoot() const { return root_.get(); }
};

}  // namespace utils

#include "red_black_tree.tpp"
