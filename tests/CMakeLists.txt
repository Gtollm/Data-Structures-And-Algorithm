include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  red_black_tree_test
  red_black_tree_test.cpp
)

target_link_libraries(
  red_black_tree_test
  red_black_tree
  GTest::gtest_main
)

target_include_directories(red_black_tree_test PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(red_black_tree_test) 
