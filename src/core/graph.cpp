#include "../../include/graph_db/graph.h"
namespace graph_db{
    NodeID Graph::create_node(){
       std::unique_lock lock(mutex_);
       NodeID id= next_node_id_++;
       auto node = std::make_unique<Node>(id);
       node->set_index_manager(&index_manager_);
       Nodes_[id]=std::move(node);
       return id;
    }
    bool Graph::remove_node(NodeID id) {
        std::unique_lock lock(mutex_);
        auto it = Nodes_.find(id);
        if (it == Nodes_.end()) return false;

        // Collect edges connected to the node
        std::vector<EdgeID> edges_to_remove;
        for (auto const & [eid, edge_ptr] : Edges_) {
            if (!edge_ptr) continue;
            if (edge_ptr->from_node() == id || edge_ptr->to_node() == id) {
                edges_to_remove.push_back(eid);
            }
        }

        // Remove edges directly while holding the unique_lock (avoid calling remove_edge())
        for (EdgeID eid : edges_to_remove) {
            auto eit = Edges_.find(eid);
            if (eit == Edges_.end()) continue;
            Edge* e = eit->second.get();
            // remove references from neighbor nodes if they still exist
            auto fromIt = Nodes_.find(e->from_node());
            if (fromIt != Nodes_.end()) fromIt->second->remove_outgoing_edge(eid);
            auto toIt = Nodes_.find(e->to_node());
            if (toIt != Nodes_.end()) toIt->second->remove_incoming_edge(eid);
            Edges_.erase(eit);
        }

        // Erase the node
        Nodes_.erase(it);
        return true;
    }
    Node* Graph::get_node(NodeID id){
        std::shared_lock lock(mutex_);
        if(!has_node(id)){
            return nullptr;
        }
        return Nodes_[id].get();
    }
    bool  Graph::has_node(NodeID id){
        auto it=Nodes_.find(id);
        return it!=Nodes_.end();
    }
    Node* Graph::get_node_unlocked(NodeID id) {
    auto it = Nodes_.find(id);
    return (it != Nodes_.end()) ? it->second.get() : nullptr;
    }

    bool Graph::has_edge(EdgeID id){
        auto it=Edges_.find(id);
        return it!=Edges_.end();
    }
    Edge* Graph::get_edge(EdgeID id){
        std::shared_lock lock(mutex_);
        if(!has_edge(id)){
            return nullptr;
        }
        return Edges_[id].get();
    }
    EdgeID Graph::create_edge(NodeID from, NodeID to, const std::string& label) {
        std::unique_lock lock(mutex_);

        // Validate nodes exist
        if (Nodes_.find(from) == Nodes_.end()) {
            throw std::runtime_error("create_edge: from node does not exist");
        }
        if (Nodes_.find(to) == Nodes_.end()) {
            throw std::runtime_error("create_edge: to node does not exist");
        }

        EdgeID id = next_edge_id_++;
        Edges_[id] = std::make_unique<Edge>(id, from, to, label);

        // Update nodes' edge lists
        Nodes_[from]->add_outgoing_edge(id);
        Nodes_[to]->add_incoming_edge(id);

        return id;
    }
    bool Graph::remove_edge(EdgeID id) {
        std::unique_lock lock(mutex_);   // lock graph

        auto it = Edges_.find(id);
        if (it == Edges_.end()) {
            return false;   // edge not found
        }

        Edge* E = it->second.get();
        Node* from = get_node_unlocked(E->from_node());  // these should still lock node’s mutex, not graph’s
        Node* to   = get_node_unlocked(E->to_node());

        if (from) {
            from->remove_outgoing_edge(id);
        }
        if (to) {
            to->remove_incoming_edge(id);
        }

        Edges_.erase(it);   // finally erase edge
        return true;
    }
    std::vector<NodeID> Graph::get_neighbors(NodeID id){
        std::shared_lock lock (mutex_);
        std::vector<NodeID>neighbors;
        if(!has_node(id)){
            return neighbors;
        }
        Node * N=get_node(id);
        for( EdgeID edge: N->get_out_edges()){
            Edge *E=get_edge(edge);
            neighbors.push_back(E->to_node());
        }
        return neighbors;
    }
}