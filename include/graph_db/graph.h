#pragma once

#include "types.h"
#include "node.h"
#include "edge.h"
#include "Index/index_manager.h"
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
    bool has_node(NodeID id);
    size_t node_count() const { 
        std::shared_lock lock(mutex_);
        return Nodes_.size(); 
    }
    size_t edge_count() const { 
        std::shared_lock lock(mutex_);
        return Edges_.size(); 
    }
    EdgeID create_edge(NodeID from, NodeID to, const std::string& label = "");
    bool remove_edge(EdgeID id);
    Edge* get_edge(EdgeID id);
    bool has_edge(EdgeID id);
    Node* get_node_unlocked(NodeID id);
    std::vector<NodeID> get_neighbors(NodeID id);
    std::unordered_map<NodeID, std::unique_ptr<Node>>& get_all_nodes() { return Nodes_; }
    std::unordered_map<EdgeID, std::unique_ptr<Edge>>& get_all_edges() { return Edges_; }
    void create_index(const std::string& property_key) {
        index_manager_.create_index(property_key);
    }
    std::vector<NodeID> find_nodes(const std::string& property_key, const PropertyValue& value) {
        Index* index = index_manager_.get_index(property_key);
        return index ? index->find(value) : std::vector<NodeID>{};
    }

    private:
    
    std::unordered_map<NodeID, std::unique_ptr<Node>> Nodes_;
    std::unordered_map<EdgeID, std::unique_ptr<Edge>> Edges_;
    NodeID next_node_id_ = 1;
    EdgeID next_edge_id_ = 1;
    IndexManager index_manager_;


    mutable std::shared_mutex mutex_;
};

} 