# src/CMakeLists.txt
add_subdirectory(query)


add_library(graphdb
    core/graph.cpp
    core/node.cpp
    core/edge.cpp
    core/graph_algo.cpp
    Index/index_manager.cpp
    Index/b_plus_tree.cpp
    storage/serializer.cpp
    storage/disk_manager.cpp
    buffer/lru_replacer.cpp
    buffer/buffer_pool_manager.cpp
)
add_executable(graph_cli main.cpp)

# Link the CLI against the graphdb and query libraries
target_link_libraries(graph_cli PRIVATE graphdb query)


target_include_directories(graphdb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
