#include "../../include/graph_db/graph.h"
using namespace std;
namespace graph_db{
    NodeID Graph::create_node(){
       std::unique_lock lock(mutex_);
       NodeID id= next_node_id_++;
       Nodes_[id]=std::make_unique<Node>(id);
       return id;
    }
    bool Graph::remove_node(NodeID id) {
        std::unique_lock lock(mutex_);
        auto it = Nodes_.find(id);
        if (it == Nodes_.end()) return false;

        // Collect all edge IDs connected to this node so we can remove them safely.
        std::vector<EdgeID> edges_to_remove;
        for (auto const & [eid, edge_ptr] : Edges_) {
            if (!edge_ptr) continue;
            if (edge_ptr->from_node() == id || edge_ptr->to_node() == id) {
                edges_to_remove.push_back(eid);
            }
        }

        // Remove edges (this will also update the other node's in/out sets).
        for (EdgeID eid : edges_to_remove) {
            // reuse remove_edge which expects the lock already held
            // but be careful: remove_edge will try to lock again if it locks internally.
            // Our remove_edge implementation assumes caller holds the graph lock, so it's safe.
            remove_edge(eid);
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
    bool   Graph::has_node(NodeID id){
        return Nodes_.find(id)!=Nodes_.end();
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
    bool Graph::remove_edge(EdgeID id){
        std::unique_lock lock(mutex_);
        if(!has_edge(id)){
            return 0;
        }
        Edge *E=get_edge(id);
        Node *from =get_node(E->from_node());
        Node *to= get_node(E->to_node());
        from->remove_outgoing_edge(id);
        to->remove_incoming_edge(id);
        Edges_.erase(id);
        return true;
    }
    vector<NodeID> Graph::get_neighbors(NodeID id){
        std::shared_lock lock (mutex_);
        vector<NodeID>neighbors;
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