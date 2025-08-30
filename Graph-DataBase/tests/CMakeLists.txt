cmake_minimum_required(VERSION 3.10)
project(GraphDataBaseTests)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_graph test_graph.cpp)
target_link_libraries(test_graph GTest::GTest GTest::Main)

add_executable(test_node test_node.cpp)
target_link_libraries(test_node GTest::GTest GTest::Main)

add_executable(test_edge test_edge.cpp)
target_link_libraries(test_edge GTest::GTest GTest::Main)

add_test(NAME GraphTests COMMAND test_graph)
add_test(NAME NodeTests COMMAND test_node)
add_test(NAME EdgeTests COMMAND test_edge)