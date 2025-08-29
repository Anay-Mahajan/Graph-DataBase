# src/CMakeLists.txt
add_library(graphdb
    core/graph.cpp
    core/node.cpp
    core/edge.cpp
)

target_include_directories(graphdb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
