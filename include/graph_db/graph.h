#pragma once

#include "types.h"
#include "node.h"
#include "edge.h"

#include <unordered_map>
#include <memory>
#include <vector>
#include <shared_mutex>
#include <cstdint>

namespace graph_db {

class Graph {
public:
    Graph() = default;
    ~Graph() = default;

    // Node management
    NodeID create_node();
    bool remove_node(NodeID id);
    Node* get_node(NodeID id);
    const Node* get_node(NodeID id) const;
    bool has_node(NodeID id);

    // Edge management
    // Default value for label belongs in the declaration (header).
    EdgeID create_edge(NodeID from, NodeID to, const std::string& label = "");
    bool remove_edge(EdgeID id);
    Edge* get_edge(EdgeID id);
    bool has_edge(EdgeID id);

    // Utility
    std::vector<NodeID> get_neighbors(NodeID id);

private:
    std::unordered_map<NodeID, std::unique_ptr<Node>> Nodes_;
    std::unordered_map<EdgeID, std::unique_ptr<Edge>> Edges_;

    NodeID next_node_id_ = 1;
    EdgeID next_edge_id_ = 1;

    mutable std::shared_mutex mutex_;
};

} // namespace graph_db
