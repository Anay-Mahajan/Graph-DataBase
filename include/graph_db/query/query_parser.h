#pragma once

#include <string>
#include <vector>
#include <variant>
#include "../types.h"

namespace graph_db {
namespace query {

enum class QueryType {
    BFS,
    DFS,
    DIJKSTRA,
    UNKNOWN
};

struct ParsedQuery {
    QueryType type = QueryType::UNKNOWN;
    NodeID start_node;
    NodeID end_node; // For shortest path
};

class QueryParser {
public:
    ParsedQuery parse(const std::string& query);
};

}
}