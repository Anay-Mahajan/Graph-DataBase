#pragma once
#include"types.h"
#include<string>
#include<shared_mutex>
namespace graph_db{
    class Edge{
        private:
            EdgeID id_;
            NodeID from_node_;
            NodeID to_node_;
            std::string label_;
            int64_t weight_=1;
            PropertyMap properties_;
            mutable std::shared_mutex mutex_;
        public:
            explicit Edge(EdgeID id,NodeID from,NodeID to,const std::string& label=" ",int64_t weight=1){
                id_=id;
                from_node_=from;
                to_node_=to;
                label_=label;
                weight_=weight;
            }
            EdgeID id()  { return id_; }
            NodeID from_node()  { return from_node_; }
            NodeID to_node()  { return to_node_; }
            std::string label()  { return label_; }
            void set_property(std::string key,PropertyValue p);
            bool has_property(std::string s);
            void remove_property(std::string s);
            PropertyValue get_property(std::string s);
    };
} 

