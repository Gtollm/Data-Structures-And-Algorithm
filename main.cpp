// Copyright 2024 Gregory Tolmachev

#include <iostream>

#include "red_black_tree.hpp"

int main() {
  utils::RBTree<int> tree;

  std::cout << "Inserting elements: 10, 5, 15, 3, 7, 12, 20" << std::endl;
  tree.Insert(10);
  tree.Insert(5);
  tree.Insert(15);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(12);
  tree.Insert(20);

  std::cout << "\nTree structure:" << std::endl;
  tree.PrintTree();

  std::cout << "\nSearching for elements:" << std::endl;
  std::cout << "7 exists: " << (tree.Search(7) ? "true" : "false")
            << std::endl;
  std::cout << "9 exists: " << (tree.Search(9) ? "true" : "false")
            << std::endl;

  std::cout << "\nMinimum: " << tree.Minimum() << std::endl;
  std::cout << "Maximum: " << tree.Maximum() << std::endl;

  std::cout << "\nDeleting element: 15" << std::endl;
  tree.Delete(15);

  std::cout << "\nTree structure after deletion:" << std::endl;
  tree.PrintTree();

  std::cout << "\nIterating through the tree: ";
  for (const auto& element : tree) {
    std::cout << element << " ";
  }
  std::cout << std::endl;

  std::cout << "\nTree height: " << tree.Height() << std::endl;

  utils::RBTree<std::string> str_tree;

  std::cout << "\nCreating a tree of strings" << std::endl;
  str_tree.Insert("banana");
  str_tree.Insert("apple");
  str_tree.Insert("cherry");
  str_tree.Insert("date");

  std::cout << "\nString tree structure:" << std::endl;
  str_tree.PrintTree();

  return 0;
}
