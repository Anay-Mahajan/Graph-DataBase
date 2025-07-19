#pragma once
#include"types.h"
#include<string>
using namespace std;
namespace graph_db{
    class Edge{
        private:
            EdgeID id_;
            NodeID from_node_;
            NodeID to_node_;
            string label_;
            PropertyMap properties_;
        public:
            explicit Edge(EdgeID id,NodeID from,NodeID to,const string& label=" "){
                id_=id;
                from_node_=from;
                to_node_=to;
                label_=label;
            }
            EdgeID id()  { return id_; }
            NodeID from_node()  { return from_node_; }
            NodeID to_node()  { return to_node_; }
            string label()  { return label_; }
            void set_property(string key,PropertyValue p);
            bool has_property(string s);
            void remove_property(string s);
            PropertyValue get_property(string s);

    };
} 

