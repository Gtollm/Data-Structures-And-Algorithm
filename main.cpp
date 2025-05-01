// Copyright 2024 Gregory Tolmachev

#include <iostream>
#include "fibonacci_heap.hpp"

int main() {
  utils::FibonacciHeap<int> heap;
  
  std::cout << "Inserting elements..." << std::endl;
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(7);
  heap.Insert(2);
  heap.Insert(4);
  heap.Insert(6);
  heap.Insert(8);
  
  std::cout << "\nHeap structure:" << std::endl;
  heap.PrintHeapASCII();
  
  std::cout << "\nExtracting minimum elements:" << std::endl;
  while (auto min = heap.DeleteMin()) {
    std::cout << "Min: " << *min << std::endl;
  }
  
  std::cout << "\nTesting merge operation..." << std::endl;
  utils::FibonacciHeap<int> heap1;
  utils::FibonacciHeap<int> heap2;
  
  heap1.Insert(5);
  heap1.Insert(3);
  heap1.Insert(7);
  
  heap2.Insert(2);
  heap2.Insert(4);
  heap2.Insert(6);
  
  utils::FibonacciHeap<int> merged = heap1.Merge(heap2);
  
  std::cout << "Merged heap structure:" << std::endl;
  merged.PrintHeapASCII();
  
  std::cout << "\nExtracting minimum elements from merged heap:" << std::endl;
  while (auto min = merged.DeleteMin()) {
    std::cout << "Min: " << *min << std::endl;
  }
  
  std::cout << "\nTesting decrease key operation..." << std::endl;
  utils::FibonacciHeap<int> heap3;
  
  heap3.Insert(5);
  heap3.Insert(10);
  heap3.Insert(7);
  heap3.Insert(15);
  
  std::cout << "Initial heap structure:" << std::endl;
  heap3.PrintHeapASCII();
  
  std::cout << "\nDecreasing key 15 to 1..." << std::endl;
  heap3.DecreaseKey(15, 1);
  
  std::cout << "Heap structure after decrease key:" << std::endl;
  heap3.PrintHeapASCII();
  
  std::cout << "\nExtracting all elements:" << std::endl;
  while (auto min = heap3.DeleteMin()) {
    std::cout << "Min: " << *min << std::endl;
  }
  
  return 0;
} 