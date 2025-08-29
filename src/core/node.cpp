#include "../../include/graph_db/node.h"
#include<algorithm>
#include<stdexcept>
#include<unordered_set>
namespace graph_db{
    void Node::add_outgoing_edge(EdgeID edge_id){
        Outgoing_Edges_.insert(edge_id);
    }
    void Node::add_incoming_edge(EdgeID edge_id){
        Incoming_Edges_.insert(edge_id);
    }
    void Node::remove_incoming_edge(EdgeID edge_id){
        auto it=Incoming_Edges_.find(edge_id);
        if(it!=Incoming_Edges_.end()){
            Incoming_Edges_.erase(it);
        }
    }
    void Node::remove_outgoing_edge(EdgeID edge_id){
        auto it =Outgoing_Edges_.find(edge_id);
        if(it!=Outgoing_Edges_.end()){
            Outgoing_Edges_.erase(it);
        }
    }
    void Node::set_property(std::string key,PropertyValue p){
        properties_[key]=p;
    }
    bool Node:: has_property(std::string s){
        if(properties_.find(s)!=properties_.end()){
            return true;
        }
        return false;
    }
    void Node::remove_property(std::string s){
        properties_.erase(s);
    }
    PropertyValue Node:: get_property(std::string s){
        auto it = properties_.find(s);
        if (it == properties_.end()) {
            throw std::runtime_error("Property not found: " + s);
        }
        return it->second;
    }
    std::unordered_set<EdgeID> Node:: get_out_edges(){
          return Outgoing_Edges_;
    }
    std::unordered_set<EdgeID> Node:: get_in_edges(){
        return Incoming_Edges_;
    }
}