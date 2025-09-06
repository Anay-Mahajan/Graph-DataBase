#pragma once

#include "../types.h"
#include "index.h"
#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace graph_db {

class IndexManager {
public:
    void create_index(const std::string& property_key);
    Index* get_index(const std::string& property_key);

private:
    std::unordered_map<std::string, std::unique_ptr<Index>> indexes_;
    mutable std::shared_mutex mutex_;
};

}