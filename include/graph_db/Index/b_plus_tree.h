#pragma once

#include "graph_db/types.h"
#include "External/b_plus_tree.hpp"
#include <vector>

namespace graph_db {

class BPlusTree {
public:
    void insert(const PropertyValue& key, NodeID value);
    std::vector<NodeID> find(const PropertyValue& key) const;
    void remove(const PropertyValue& key, NodeID value);

private:
    mutable bplustree::BPlusTree<PropertyValue, NodeID> tree_;
};

}