#include "../../include/Index/b_plus_tree.h"
#include <algorithm>

namespace graph_db {

void BPlusTree::insert(const PropertyValue& key, NodeID value) {
    tree_.insert(key, value);
}

std::vector<NodeID> BPlusTree::find(const PropertyValue& key) const {
    return tree_.find(key);
}

void BPlusTree::remove(const PropertyValue& key, NodeID value) {
    tree_.remove(key, value);
}

}