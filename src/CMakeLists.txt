# src/CMakeLists.txt
add_library(graphdb
    core/graph.cpp
    core/node.cpp
    core/edge.cpp
    core/graph_algo.cpp
    Index/index_manager.cpp
    Index/b_plus_tree.cpp
)

target_include_directories(graphdb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
