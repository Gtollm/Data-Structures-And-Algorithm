// Copyright 2024 Gregory Tolmachev

#pragma once
#include "red_black_tree.hpp"

namespace utils {

template <Containable T>
void RBTree<T>::LeftRotate(RbNodePtr x) {
  RbNodeUPtr y = std::move(x->right_);
  x->right_ = std::move(y->left_);
  if (x->right_) {
    x->right_->parent_ = x;
  }
  y->parent_ = x->parent_;
  RbNodeUPtr x_holder;
  RbNodePtr y_ptr = nullptr;
  if (!x->parent_) {
    x_holder = std::move(this->root_);
    this->root_ = std::move(y);
    y_ptr = this->root_.get();
  } else if (x == x->parent_->left_.get()) {
    x_holder = std::move(x->parent_->left_);
    x->parent_->left_ = std::move(y);
    y_ptr = x->parent_->left_.get();
  } else {
    x_holder = std::move(x->parent_->right_);
    x->parent_->right_ = std::move(y);
    y_ptr = x->parent_->right_.get();
  }
  y_ptr->left_ = std::move(x_holder);
  y_ptr->left_->parent_ = y_ptr;
}

template <Containable T>
void RBTree<T>::RightRotate(RbNodePtr x) {
  RbNodeUPtr y = std::move(x->left_);
  x->left_ = std::move(y->right_);
  if (x->left_) {
    x->left_->parent_ = x;
  }
  y->parent_ = x->parent_;
  RbNodeUPtr x_holder;
  RbNodePtr y_ptr = nullptr;
  if (!x->parent_) {
    x_holder = std::move(this->root_);
    this->root_ = std::move(y);
    y_ptr = this->root_.get();
  } else if (x == x->parent_->left_.get()) {
    x_holder = std::move(x->parent_->left_);
    x->parent_->left_ = std::move(y);
    y_ptr = x->parent_->left_.get();
  } else {
    x_holder = std::move(x->parent_->right_);
    x->parent_->right_ = std::move(y);
    y_ptr = x->parent_->right_.get();
  }
  y_ptr->right_ = std::move(x_holder);
  y_ptr->right_->parent_ = y_ptr;
}

template <Containable T>
void RBTree<T>::InsertFixUp(RbNodePtr z) {
  while (z && z->parent_ && z->parent_->color_ == RbColor::RED) {
    if (z->parent_ == z->parent_->parent_->left_.get()) {
      RbNodePtr uncle_right = z->parent_->parent_->right_.get();
      if (uncle_right && uncle_right->color_ == RbColor::RED) {
        // case 1
        z->parent_->color_ = RbColor::BLACK;
        uncle_right->color_ = RbColor::BLACK;
        z->parent_->parent_->color_ = RbColor::RED;
        z = z->parent_->parent_;
      } else {
        // case 2
        if (z == z->parent_->right_.get()) {
          z = z->parent_;
          LeftRotate(z);
        }
        // case 3
        z->parent_->color_ = RbColor::BLACK;
        z->parent_->parent_->color_ = RbColor::RED;
        RightRotate(z->parent_->parent_);
      }
    } else {
      RbNodePtr uncle_left = z->parent_->parent_->left_.get();
      if (uncle_left && uncle_left->color_ == RbColor::RED) {
        // case 1
        z->parent_->color_ = RbColor::BLACK;
        uncle_left->color_ = RbColor::BLACK;
        z->parent_->parent_->color_ = RbColor::RED;
        z = z->parent_->parent_;
      } else {
        // case 2
        if (z == z->parent_->left_.get()) {
          z = z->parent_;
          RightRotate(z);
        }
        // case 3
        z->parent_->color_ = RbColor::BLACK;
        z->parent_->parent_->color_ = RbColor::RED;
        LeftRotate(z->parent_->parent_);
      }
    }
  }
  if (this->root_->color_ == RbColor::RED) {
    this->root_->color_ = RbColor::BLACK;
  }
}

template <Containable T>
void RBTree<T>::Transplant(RbNodePtr u, RbNodeUPtr v) {
  if (!u) {
    throw std::invalid_argument("Node u cannot be null.");
  }

  RbNodePtr u_parent = u->parent_;
  if (!u_parent) {
    this->root_ = std::move(v);
    if (this->root_) {
      this->root_->parent_ = nullptr;
    }
  } else if (u == u_parent->left_.get()) {
    u_parent->left_ = std::move(v);
    if (u_parent->left_) {
      u_parent->left_->parent_ = u_parent;
    }
  } else {
    u_parent->right_ = std::move(v);
    if (u_parent->right_) {
      u_parent->right_->parent_ = u_parent;
    }
  }
}

template <Containable T>
void RBTree<T>::Delete(RbNodePtr z) {
  if (!z) {
    return;
  }

  RbColor orig_color = z->color_;

  RbNodePtr x = nullptr;
  RbNodePtr x_parent = nullptr;
  if (!z->left_) {
    x = z->right_.get();
    x_parent = z->parent_;
    Transplant(z, std::move(z->right_));
  } else if (!z->right_) {
    x = z->left_.get();
    x_parent = z->parent_;
    Transplant(z, std::move(z->left_));
  } else {
    RbNodePtr y = Minimum(z->right_.get());
    orig_color = y->color_;
    x = y->right_.get();
    RbNodeUPtr y_holder;
    if (y != z->right_.get()) {
      RbNodePtr y_parent = y->parent_;
      y_holder = std::move(y_parent->left_);
      y_parent->left_ = std::move(y->right_);
      if (y_parent->left_) {
        y_parent->left_->parent_ = y_parent;
      }
      y = y_holder.get();
      y->right_ = std::move(z->right_);
      x_parent = y_parent;
    } else {
      x_parent = y;
    }
    RbNodeUPtr z_left_holder = std::move(z->left_);
    RbNodePtr y_parent = y->parent_;
    if (!y_holder) {
      y_holder = std::move(y_parent->right_);
    }
    bool z_black = z->color_ == RbColor::BLACK;
    RbNodePtr z_parent = z->parent_;
    if (!z_parent) {
      this->root_ = std::move(y_holder);
      this->root_->parent_ = nullptr;
      y = this->root_.get();
    } else if (z == z_parent->left_.get()) {
      z_parent->left_ = std::move(y_holder);
      z_parent->left_->parent_ = z_parent;
      y = z_parent->left_.get();
    } else {
      z_parent->right_ = std::move(y_holder);
      z_parent->right_->parent_ = z_parent;
      y = z_parent->right_.get();
    }
    y->left_ = std::move(z_left_holder);
    y->left_->parent_ = y;
    if (y->right_) {
      y->right_->parent_ = y;
    }

    y->color_ = z_black ? RbColor::BLACK : RbColor::RED;
  }
  if (orig_color == RbColor::BLACK) {
    DeleteFixUp(x, x_parent);
  }
}

template <Containable T>
void RBTree<T>::DeleteFixUp(RbNodePtr x, RbNodePtr x_parent) {
  while (x != this->root_.get() && (!x || x->color_ == RbColor::BLACK)) {
    if (x == x_parent->left_.get()) {
      RbNodePtr sibling = x_parent->right_.get();
      if (sibling->color_ == RbColor::RED) {
        // Case 1
        sibling->color_ = RbColor::BLACK;
        x_parent->color_ = RbColor::RED;
        LeftRotate(x_parent);
        sibling = x_parent->right_.get();
      }
      if ((!sibling->left_ || sibling->left_->color_ == RbColor::BLACK) &&
          (!sibling->right_ || sibling->right_->color_ == RbColor::BLACK)) {
        // Case 2
        sibling->color_ = RbColor::RED;
        x = x_parent;
        x_parent = x_parent->parent_;
      } else {
        if (!sibling->right_ || sibling->right_->color_ == RbColor::BLACK) {
          // Case 3
          sibling->left_->color_ = RbColor::BLACK;
          sibling->color_ = RbColor::RED;
          RightRotate(sibling);
          sibling = x_parent->right_.get();
        }
        // Case 4
        sibling->color_ = x_parent->color_;
        x_parent->color_ = RbColor::BLACK;
        sibling->right_->color_ = RbColor::BLACK;
        LeftRotate(x_parent);
        x = this->root_.get();
      }
    } else {
      RbNodePtr sibling = x_parent->left_.get();
      if (sibling->color_ == RbColor::RED) {
        // Case 1
        sibling->color_ = RbColor::BLACK;
        x_parent->color_ = RbColor::RED;
        RightRotate(x_parent);
        sibling = x_parent->left_.get();
      }
      if ((!sibling->left_ || sibling->left_->color_ == RbColor::BLACK) &&
          (!sibling->right_ || sibling->right_->color_ == RbColor::BLACK)) {
        // Case 2
        sibling->color_ = RbColor::RED;
        x = x_parent;
        x_parent = x_parent->parent_;
      } else {
        if (!sibling->left_ || sibling->left_->color_ == RbColor::BLACK) {
          // Case 3
          sibling->right_->color_ = RbColor::BLACK;
          sibling->color_ = RbColor::RED;
          LeftRotate(sibling);
          sibling = x_parent->left_.get();
        }
        // Case 4
        sibling->color_ = x_parent->color_;
        x_parent->color_ = RbColor::BLACK;
        sibling->left_->color_ = RbColor::BLACK;
        RightRotate(x_parent);
        x = this->root_.get();
      }
    }
  }
  if (x) {
    x->color_ = RbColor::BLACK;
  }
}

template <Containable T>
auto RBTree<T>::Minimum(RbNodePtr node) const -> RbNodePtr {
  if (!node) return nullptr;
  while (node->left_) node = node->left_.get();
  return node;
}

template <Containable T>
auto RBTree<T>::Maximum(RbNodePtr node) const -> RbNodePtr {
  if (!node) return nullptr;
  while (node->right_) node = node->right_.get();
  return node;
}

template <Containable T>
void RBTree<T>::PrintIndent(int level) {
  for (int i = 0; i < level; i++) {
    std::cout << "    ";
  }
}

template <Containable T>
void RBTree<T>::PrintTree(RbNodePtr root, int level, bool isLeft) {
  if (!root) {
    return;
  }

  PrintTree(root->right_.get(), level + 1, false);

  PrintIndent(level);
  std::cout << (isLeft ? "└──" : "┌──");
  std::cout << root->key_ << "("
            << (root->color_ == RbColor::RED ? "R" : "B") << ")"
            << std::endl;

  PrintTree(root->left_.get(), level + 1, true);
}

template <Containable T>
void RBTree<T>::Insert(const T &data) {
  RbNodeUPtr z = std::make_unique<RbNode<T>>(data);
  RbNodePtr y = nullptr;
  RbNodePtr x = this->root_.get();

  while (x != nullptr) {
    y = x;
    if (z->key_ < x->key_) {
      x = x->left_.get();
    } else if (z->key_ > x->key_) {
      x = x->right_.get();
    } else {
      return;  // Duplicate key, do not insert
    }
  }

  z->parent_ = y;
  if (y == nullptr) {
    this->root_ = std::move(z);
    this->root_->color_ = RbColor::BLACK;
  } else if (z->key_ < y->key_) {
    y->left_ = std::move(z);
    InsertFixUp(y->left_.get());
  } else {
    y->right_ = std::move(z);
    InsertFixUp(y->right_.get());
  }
}

template <Containable T>
void RBTree<T>::Delete(const T &data) {
  RbNodePtr z = Search(data);
  if (z) {
    Delete(z);
  }
}

template <Containable T>
auto RBTree<T>::Search(const T &data) const -> RbNodePtr {
  RbNodePtr x = this->root_.get();
  while (x != nullptr && x->key_ != data) {
    if (data < x->key_) {
      x = x->left_.get();
    } else {
      x = x->right_.get();
    }
  }
  return x;
}

template <Containable T>
void RBTree<T>::PrintTree() {
  PrintTree(this->root_.get());
}

template <Containable T>
std::ostream &operator<<(std::ostream &out, const RBTree<T> &tree) {
  tree.PrintTree();
  return out;
}

template <Containable T>
T RBTree<T>::Minimum() const {
  RbNodePtr min_node = Minimum(this->root_.get());
  if (!min_node) {
    throw std::runtime_error("Tree is empty");
  }
  return min_node->key_;
}

template <Containable T>
T RBTree<T>::Maximum() const {
  RbNodePtr max_node = Maximum(this->root_.get());
  if (!max_node) {
    throw std::runtime_error("Tree is empty");
  }
  return max_node->key_;
}

template <Containable T>
auto RBTree<T>::LeastUpperBound(const T &key) const -> RbNodePtr {
  auto x = this->root_.get();
  RbNodePtr res = nullptr;
  while (x) {
    if (x->key_ > key) {
      res = x;
      x = x->left_.get();
    } else {
      x = x->right_.get();
    }
  }
  return res;
}

template <Containable T>
auto RBTree<T>::GreatestLowerBound(const T &key) const -> RbNodePtr {
  RbNodePtr x = this->root_.get();
  RbNodePtr res = nullptr;

  while (x) {
    if (x->key_ < key) {
      res = x;
      x = x->right_.get();
    } else {
      x = x->left_.get();
    }
  }
  return res;
}

template <Containable T>
template <bool Const>
auto RBTree<T>::Iterator<Const>::operator++() -> Iterator & {
  if (!node_) {
    return *this;
  }

  if (node_->right_) {
    node_ = node_->right_.get();
    while (node_->left_) {
      node_ = node_->left_.get();
    }
  } else {
    RbNodePtr parent = node_->parent_;
    while (parent && node_ == parent->right_.get()) {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template <Containable T>
template <bool Const>
auto RBTree<T>::Iterator<Const>::operator++(int) -> Iterator {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template <Containable T>
template <bool Const>
auto RBTree<T>::Iterator<Const>::operator--() -> Iterator & {
  if (!node_) {
    return *this;
  }

  if (node_->left_) {
    node_ = node_->left_.get();
    while (node_->right_) {
      node_ = node_->right_.get();
    }
  } else {
    RbNodePtr parent = node_->parent_;
    while (parent && node_ == parent->left_.get()) {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template <Containable T>
template <bool Const>
auto RBTree<T>::Iterator<Const>::operator--(int) -> Iterator {
  Iterator temp = *this;
  --(*this);
  return temp;
}

template <Containable T>
int RBTree<T>::Height(RbNodePtr root) {
  if (!root) {
    return 0;
  }
  int left_height = Height(root->left_.get());
  int right_height = Height(root->right_.get());
  return std::max(left_height, right_height) + 1;
}

template <Containable T>
auto RBTree<T>::begin() const -> iterator_type {
  return iterator_type(Minimum(root_.get()));
}

template <Containable T>
auto RBTree<T>::begin() -> const_iterator_type {
  return const_iterator_type(Minimum(root_.get()));
}

template <Containable T>
auto RBTree<T>::cbegin() const -> const_iterator_type {
  return const_iterator_type(Minimum(root_.get()));
}

template <Containable T>
auto RBTree<T>::end() -> const_iterator_type {
  return const_iterator_type(nullptr);
}

template <Containable T>
auto RBTree<T>::cend() const -> const_iterator_type {
  return const_iterator_type(nullptr);
}

}  // namespace utils
