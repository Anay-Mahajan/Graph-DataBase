#pragma once 
#include"types.h"
#include<vector>
#include<memory>
#include<unordered_set>
 using namespace std;
 namespace graph_db{
    class Node{
        private:
            NodeID id_;
            unordered_set<EdgeID>Incoming_Edges_;
            unordered_set<EdgeID>Outgoing_Edges_;
            PropertyMap properties_;
        public:
            explicit  Node(NodeID id){ id_=id;}
            const NodeID get_id()const {    return id_; }
            const unordered_set<EdgeID>& get_out_edges() const { return Outgoing_Edges_;}
            const unordered_set<EdgeID>& get_in_edges() const {  return Incoming_Edges_;}
            void add_outgoing_edge(EdgeID edge_id);
            void add_incoming_edge(EdgeID edge_id);
            void remove_outgoing_edge(EdgeID edge_id);
            void remove_incoming_edge(EdgeID edge_id);
            void set_property(string key,PropertyValue p);
            bool has_property(string s);
            void remove_property(string s);
            PropertyValue get_property(string s);
    };
 }