#pragma once 
#include"types.h"
#include<vector>
 using namespace std;
 namespace graph_db{
    class Node{
        private:
            NodeID id_;
            vector<EdgeID>Incoming_Edges_;
            vector<EdgeID>Outgoing_Edges_;
            PropertyMap properties_;
        public:
            explicit  Node(NodeID id){ id_=id;}
            NodeID get_id(){    return id_; }
            vector<EdgeID> get_out_edges(){ return Outgoing_Edges_;}
            vector<EdgeID> get_in_edges(){  return Incoming_Edges_;}
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