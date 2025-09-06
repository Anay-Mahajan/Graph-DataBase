#pragma once

#include "../types.h"
#include "b_plus_tree.h"

namespace graph_db {

class Index {
public:
    void insert(const PropertyValue& key, NodeID value) {
        tree_.insert(key, value);
    }

    std::vector<NodeID> find(const PropertyValue& key) const {
        return tree_.find(key);
    }

    void remove(const PropertyValue& key, NodeID value) {
        tree_.remove(key, value);
    }

private:
    BPlusTree tree_;
};

}