#pragma once
#include "graph.h"
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <unordered_map>

namespace graph_db {

    // BFS from a source node
    std::vector<NodeID> bfs(Graph& g, NodeID start);

    std::vector<NodeID> bfs_level(Graph& g,NodeID start , int level);

    // DFS from a source node
    std::vector<NodeID> dfs(Graph& g, NodeID start);

    // Dijkstra shortest path
    std::unordered_map<NodeID, int64_t> dijkstra(Graph& g, NodeID start);

}
