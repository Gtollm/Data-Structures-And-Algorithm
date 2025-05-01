cmake_minimum_required(VERSION 3.14)
project(RedBlackTree VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(red_black_tree INTERFACE)
target_include_directories(red_black_tree INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(red_black_tree_example
  main.cpp
)

target_link_libraries(red_black_tree_example PRIVATE red_black_tree)

enable_testing()

add_subdirectory(tests)

install(FILES 
  red_black_tree.hpp 
  red_black_tree.tpp
  DESTINATION include/red_black_tree
) 
