#pragma once 
#include"types.h"
#include"Index/index_manager.h"
#include<vector>
#include<memory>
#include<unordered_set>
#include<shared_mutex>
namespace graph_db{
    class Node{
        private:
            NodeID id_;
            std::unordered_set<EdgeID>Incoming_Edges_;
            std::unordered_set<EdgeID>Outgoing_Edges_;
            PropertyMap properties_;
            IndexManager* index_manager_ = nullptr;
            mutable std::shared_mutex mutex_;
        public:
            explicit Node(NodeID id) : id_(id) {}
            NodeID get_id()const {    return id_; }
             std::unordered_set<EdgeID> get_out_edges();
             std::unordered_set<EdgeID>get_in_edges();
            void add_outgoing_edge(EdgeID edge_id);
            void add_incoming_edge(EdgeID edge_id);
            void remove_outgoing_edge(EdgeID edge_id);
            void remove_incoming_edge(EdgeID edge_id);
            void set_property(std::string key,PropertyValue p);
            bool has_property(std::string s);
            void remove_property(std::string s);
            PropertyValue get_property(std::string s);
            void set_index_manager(IndexManager* manager) { index_manager_ = manager; }
    };
 }