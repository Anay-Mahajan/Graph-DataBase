#pragma once
#include "node.h"
#include "edge.h"
#include <unordered_map>
#include <memory>
#include <optional>
using namespace std;
namespace graph_db{
    class Graph{
        private:
                unordered_map<NodeID,unique_ptr<Node>>Nodes_;
                unordered_map<EdgeID,unique_ptr<Edge>>Edges_;
                NodeID next_node_id_;
                EdgeID next_edge_id_;
        public:
            Graph(){
                next_node_id_=1;
                next_edge_id_=1;
            }
            NodeID create_node();
            bool remove_node(NodeID node_id);
            Node* get_node(NodeID node_id);
            bool has_node(NodeID node_id) const;
            EdgeID create_edge(NodeID from, NodeID to, const string& label = "");
            bool remove_edge(EdgeID edge_id);
            Edge* get_edge(EdgeID edge_id);
            const Edge* get_edge(EdgeID edge_id) const;
            bool has_edge(EdgeID edge_id) const;
            
            size_t node_count() const { return Nodes_.size(); }
            size_t edge_count() const { return Edges_.size(); }
            
            vector<NodeID> get_neighbors(NodeID node_id) const;
            vector<EdgeID> get_edges_between(NodeID from, NodeID to) const;
    };
}