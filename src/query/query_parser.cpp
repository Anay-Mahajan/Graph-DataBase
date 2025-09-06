#include "../../include/graph_db/query/query_parser.h"
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

namespace graph_db {
namespace query {

ParsedQuery QueryParser::parse(const std::string& query) {
    ParsedQuery result;
    std::stringstream ss(query);
    std::string token;
    std::vector<std::string> tokens;
    while (ss >> token) {
        std::transform(token.begin(), token.end(), token.begin(), ::toupper);
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return result;
    }

    if (tokens[0] == "BFS" && tokens.size() == 3 && tokens[1] == "FROM") {
        result.type = QueryType::BFS;
        result.start_node = std::stoull(tokens[2]);
    } else if (tokens[0] == "DFS" && tokens.size() == 3 && tokens[1] == "FROM") {
        result.type = QueryType::DFS;
        result.start_node = std::stoull(tokens[2]);
    } else if (tokens[0] == "SHORTEST" && tokens.size() == 6 && tokens[1] == "PATH" && tokens[2] == "FROM" && tokens[4] == "TO") {
        result.type = QueryType::DIJKSTRA;
        result.start_node = std::stoull(tokens[3]);
        result.end_node = std::stoull(tokens[5]);
    }

    return result;
}

}
}