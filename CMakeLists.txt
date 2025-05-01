cmake_minimum_required(VERSION 3.14)

project(
    FibonacciHeap
    VERSION 1.0.0
    DESCRIPTION "Fibonacci heap"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(fibonacci_heap INTERFACE)
target_include_directories(fibonacci_heap INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(fibonacci_heap_example
  main.cpp
)

target_link_libraries(fibonacci_heap_example PRIVATE fibonacci_heap)

enable_testing()

add_subdirectory(tests)

install(TARGETS fibonacci_heap_example
  RUNTIME DESTINATION bin
)

install(FILES 
  fibonacci_heap.hpp 
  fibonacci_heap.tpp
  DESTINATION include/fibonacci_heap
)

