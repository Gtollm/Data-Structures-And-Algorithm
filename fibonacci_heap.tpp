// Copyright 2024 Gregory Tolmachev

#pragma once
#include "fibonacci_heap.hpp"

namespace utils {

template <Containable K>
void FibonacciHeap<K>::AddAfter(NodePtr node, NodePtr obj) {
  obj->next = node->next;
  node->next->prev = obj;
  obj->prev = node;
  node->next = obj;
}

template <Containable K>
auto FibonacciHeap<K>::FindMin() -> NodePtr {
  if (!this->root_) {
    return nullptr;
  }
  if (this->root_->key < this->FindNewMin()->key) {
    return this->root_;
  }
  return this->FindNewMin();
}

template <Containable K>
auto FibonacciHeap<K>::FindNewMin() -> NodePtr {
  if (!this->root_) {
    return nullptr;
  }
  NodePtr current = this->root_;
  NodePtr min = nullptr;
  do {
    if (!min || current->key < min->key) {
      min = current;
    }
    current = current->next;
  } while (current != this->root_);
  return min;
}

template <Containable K>
auto FibonacciHeap<K>::CopySubtree(const NodePtr& node,
                                 std::unordered_map<NodePtr, NodePtr>& node_map,
                                 const NodePtr& parent) -> NodePtr {
  if (!node) return nullptr;

  if (node_map.find(node) != node_map.end()) {
    return node_map[node];
  }

  Node* new_node = new Node(node->key);
  new_node->marked = node->marked;
  new_node->rank = node->rank;

  node_map[node] = new_node;

  if (node->child) {
    new_node->child = CopySubtree(node->child, node_map, new_node);
    if (new_node->child) {
      new_node->child->parent = new_node;
    }
  }
  new_node->parent = const_cast<NodePtr>(parent);
  if (node->next != node) {
    new_node->next = CopySubtree(node->next, node_map, parent);
    if (new_node->next) {
      new_node->next->prev = new_node;
    }
  } else {
    new_node->next = new_node;
    new_node->prev = new_node;
  }

  return new_node;
}

template <Containable K>
void FibonacciHeap<K>::Link(NodePtr y, NodePtr x) {
  y->prev->next = y->next;
  y->next->prev = y->prev;

  y->marked = false;

  if (!x->child) {
    x->child = y;
    y->next = y;
    y->prev = y;
  } else {
    AddAfter(x->child, y);
  }
  y->parent = x;

  x->rank++;
}

template <Containable K>
void FibonacciHeap<K>::Consolidate() {
  if (!root_) return;

  std::unordered_map<std::size_t, NodePtr> rank_table;

  std::vector<NodePtr> roots;
  NodePtr current = this->root_;
  do {
    roots.push_back(current);
    current = current->next;
  } while (current != root_);

  for (auto& node : roots) {
    std::size_t d = node->rank;
    while (rank_table.count(d)) {
      NodePtr other = rank_table[d];
      if (node->key > other->key) {
        std::swap(node, other);
      }
      Link(other, node);
      rank_table.erase(d);
      d++;
    }
    rank_table[d] = node;
  }

  this->root_ = nullptr;
  for (auto& pair : rank_table) {
    NodePtr node = pair.second;
    if (!this->root_) {
      this->root_ = node;
      node->next = node;
      node->prev = node;
    } else {
      AddAfter(this->root_, node);
      if (node->key < this->root_->key) {
        this->root_ = node;
      }
    }
  }
}

template <Containable K>
auto FibonacciHeap<K>::FindElement(key_type key, NodePtr node) -> NodePtr {
  if (!node) return nullptr;

  NodePtr current = node;
  std::unordered_set<NodePtr> visited;

  do {
    if (current->key == key) {
      return current;
    }

    if (current->child) {
      NodePtr result = FindElement(key, current->child);
      if (result) {
        return result;
      }
    }

    visited.insert(current);
    current = current->next;
  } while (current != node && !visited.count(current));

  return nullptr;
}

template <Containable K>
auto FibonacciHeap<K>::FindElement(key_type key) -> NodePtr {
  return FindElement(key, this->root_);
}

template <Containable K>
void FibonacciHeap<K>::TransferToRoots(NodePtr node) {
  if (!node->parent) {
    return;
  }
  NodePtr node_parent = node->parent;
  if (node->next != node) {
    node_parent->child = node->next;
    node->next->prev = node->prev;
    node->prev->next = node->next;
  } else {
    node_parent->child = nullptr;
  }
  node->parent = nullptr;
  AddAfter(this->root_, node);
  if (node_parent->marked) {
    TransferToRoots(node_parent);
    node_parent->marked = false;
  } else {
    node_parent->marked = true;
  }
  node->marked = false;
  node_parent->rank--;
}

template <Containable K>
void FibonacciHeap<K>::CollectForDelete(NodePtr ptr, std::queue<NodePtr>& queue) {
  if (!ptr) return;

  NodePtr current = ptr;
  do {
    queue.push(current);
    current = current->next;
    if (current->child) {
      CollectForDelete(current->child, queue);
    }
  } while (current != ptr);
}

template <Containable K>
FibonacciHeap<K>::FibonacciHeap() : root_(nullptr), no_delete_(false) {}

template <Containable K>
FibonacciHeap<K>::FibonacciHeap(FibonacciHeap&& other) noexcept
    : root_(other.root_) {
  other.root_ = nullptr;
}

template <Containable K>
FibonacciHeap<K>::FibonacciHeap(const FibonacciHeap& other) : root_(nullptr), no_delete_(false) {
  std::unordered_map<NodePtr, NodePtr> node_map;

  if (other.root_) {
    this->root_ = CopySubtree(other.root_, node_map);

    if (this->root_->next == this->root_) {
      this->root_->prev = this->root_;
    } else {
      NodePtr current = this->root_;
      while (current->next != this->root_) {
        current = current->next;
      }
      current->next = this->root_;
      this->root_->prev = current;
    }
  }
}

template <Containable K>
FibonacciHeap<K>& FibonacciHeap<K>::operator=(FibonacciHeap other) {
  std::swap(root_, other.root_);
  std::swap(no_delete_, other.no_delete_);
  return *this;
}

template <Containable K>
FibonacciHeap<K>::~FibonacciHeap() {
  if (!this->root_ || this->no_delete_) return;
  std::queue<NodePtr> node_queue;
  this->CollectForDelete(this->root_, node_queue);
  std::size_t queue_size = node_queue.size();
  for (std::size_t i = 0; i < queue_size; ++i) {
    delete node_queue.front();
    node_queue.pop();
  }
  this->root_ = nullptr;
}

template <Containable K>
void FibonacciHeap<K>::Insert(key_type data) {
  NodePtr added = new Node(data);
  if (!this->root_) {
    this->root_ = added;
    added->next = added;
    added->prev = added;
  } else {
    AddAfter(this->root_, added);
    if (added->key < this->root_->key) {
      this->root_ = added;
    }
  }
}

template <Containable K>
std::optional<K> FibonacciHeap<K>::DeleteMin() {
  if (!this->root_) return std::nullopt;

  NodePtr old_root = this->root_;
  key_type old_root_key = this->root_->key;
  if (this->root_->child) {
    NodePtr child = this->root_->child;
    do {
      child->parent = nullptr;
      NodePtr next_child = child->next;
      AddAfter(this->root_, child);
      child = next_child;
    } while (child != old_root->child);
  }

  if (old_root == old_root->next) {
    delete this->root_;
    this->root_ = nullptr;
  } else {
    old_root->prev->next = old_root->next;
    old_root->next->prev = old_root->prev;
    this->root_ = old_root->next;
    Consolidate();
  }

  return old_root_key;
}

template <Containable K>
FibonacciHeap<K> FibonacciHeap<K>::Merge(const FibonacciHeap& other) {
  FibonacciHeap result(*this);
  FibonacciHeap copy(other);
  copy.no_delete_ = true;

  if (!result.root_) {
    return copy;
  }
  if (!copy.root_) {
    return result;
  }

  NodePtr result_root_prev = result.root_->prev;
  NodePtr copy_root_prev = copy.root_->prev;

  result_root_prev->next = copy.root_;
  copy.root_->prev = result_root_prev;

  copy_root_prev->next = result.root_;
  result.root_->prev = copy_root_prev;

  if (copy.root_->key < result.root_->key) {
    result.root_ = copy.root_;
  }

  return result;
}

template <Containable K>
void FibonacciHeap<K>::DecreaseKey(key_type x, key_type y) {
  NodePtr node = FindElement(x);
  if (!node) {
    return;
  }
  node->key = y;

  if (node->parent && y < node->parent->key) {
    TransferToRoots(node);
  } else if (node->child && y > node->child->key) {
    NodePtr cur = node->child;
    NodePtr next;
    do {
      next = cur->next;
      if (y > node->key) {
        TransferToRoots(cur);
      }
      cur = next;
    } while (cur != node->child);
  }
  if (y < this->root_->key) {
    this->root_ = node;
  }
}

template <Containable K>
void FibonacciHeap<K>::PrintTreeRoots() {
  if (!this->root_) return;
  NodePtr cur = this->root_;
  do {
    std::cout << cur->key << " ";
    cur = cur->next;
  } while (cur != this->root_);
  std::cout << std::endl;
}

template <Containable K>
void FibonacciHeap<K>::CheckIntegrity() {
  if (!this->root_) return;
  NodePtr current = this->root_;
  do {
    if (current->next->prev != current) {
      std::cerr << "Invalid prev pointer." << std::endl;
      exit(1);
    }
    if (current->prev->next != current) {
      std::cerr << "Invalid next pointer." << std::endl;
      exit(1);
    }
    current = current->next;
  } while (current != root_);
  current = this->root_;
  do {
    if (current->next->prev != current) {
      std::cerr << "Invalid prev pointer." << std::endl;
      exit(1);
    }
    if (current->prev->next != current) {
      std::cerr << "Invalid next pointer." << std::endl;
      exit(1);
    }
    current = current->prev;
  } while (current != root_);

  current = this->root_;
  do {
    if (current->child && current->child->parent != current) {
      std::cerr << "Invalid parent pointer." << std::endl;
      exit(1);
    }
    if (current->child && current->child->parent != current) {
      std::cerr << "Invalid parent pointer." << std::endl;
      exit(1);
    }
    current = current->child;
  } while (current != nullptr);
}

template <Containable K>
void FibonacciHeap<K>::PrintHeapASCII(NodePtr node, int depth, bool is_last) {
  if (!node) {
    if (!root_) {
      std::cout << "(empty heap)" << std::endl;
      return;
    }
    node = root_;
  }
  static std::unordered_set<NodePtr> visited;
  if (depth == 0) {
    visited.clear();
  }
  NodePtr current = node;
  do {
    if (visited.count(current)) {
      return;
    }
    visited.insert(current);
    for (int i = 0; i < depth; ++i) {
      std::cout << "          ";
    }
    std::cout << current->key << "[" << current->prev->key << ","
              << current->next->key << ",";
    std::cout << (current->child ? std::to_string(current->child->key)
                                : "~")
              << ",";
    std::cout << (current->parent ? std::to_string(current->parent->key)
                                  : "~")
              << ","
              << (current->marked ? "marked" : "unmarked") << "," << current->rank
              << "]" << std::endl;

    if (current->child) {
      PrintHeapASCII(current->child, depth + 1, false);
    }

    current = current->next;
  } while (current != node);
}

} // namespace utils 