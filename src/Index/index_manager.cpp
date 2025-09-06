#include "../../include/graph_db/Index/index_manager.h"

namespace graph_db {

void IndexManager::create_index(const std::string& property_key) {
    std::unique_lock lock(mutex_);
    if (indexes_.find(property_key) == indexes_.end()) {
        indexes_[property_key] = std::make_unique<Index>();
    }
}

Index* IndexManager::get_index(const std::string& property_key) {
    std::shared_lock lock(mutex_);
    auto it = indexes_.find(property_key);
    if (it != indexes_.end()) {
        return it->second.get();
    }
    return nullptr;
}

}