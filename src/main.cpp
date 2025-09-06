#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include "graph_db/graph.h"
#include "graph_db/graph_algo.h"
#include "graph_db/query/query_parser.h"

// Helper to convert string to uppercase for case-insensitive commands
void to_upper(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
}

// Helper to parse a string into a PropertyValue
graph_db::PropertyValue parse_property_value(const std::string& val_str) {
    // Check for boolean
    if (val_str == "true" || val_str == "TRUE") return true;
    if (val_str == "false" || val_str == "FALSE") return false;

    // Check for integer
    try {
        size_t pos;
        long long ll_val = std::stoll(val_str, &pos);
        if (pos == val_str.length()) {
            return static_cast<int64_t>(ll_val);
        }
    } catch (...) {}

    // Check for double
    try {
        size_t pos;
        double d_val = std::stod(val_str, &pos);
        if (pos == val_str.length()) {
            return d_val;
        }
    } catch (...) {}

    // Default to string
    return val_str;
}

void print_help() {
    std::cout << "\n--- GraphDB Command-Line Interface ---\n"
              << "Available Commands:\n"
              << "  CREATE NODE\n"
              << "  CREATE EDGE FROM <from_id> TO <to_id> LABEL <label> [WEIGHT <weight>]\n"
              << "  CREATE INDEX ON <property_key>\n"
              << "  SET PROPERTY ON NODE <id> KEY <key> VALUE <value>\n"
              << "  SET PROPERTY ON EDGE <id> KEY <key> VALUE <value>\n"
              << "  GET NODE <id>\n"
              << "  GET EDGE <id>\n"
              << "  REMOVE NODE <id>\n"
              << "  REMOVE EDGE <id>\n"
              << "  PRINT GRAPH\n"
              << "  SAVE <filename>\n"
              << "  LOAD <filename>\n"
              << "  -- Traversal Queries --\n"
              << "  BFS FROM <start_node_id>\n"
              << "  DFS FROM <start_node_id>\n"
              << "  SHORTEST PATH FROM <start_node_id> TO <end_node_id>\n"
              << "  -- Other --\n"
              << "  HELP\n"
              << "  EXIT\n"
              << "---------------------------------------\n";
}

// Function to print node properties
void print_properties(const graph_db::PropertyMap& props) {
    if (props.empty()) {
        std::cout << "    Properties: None\n";
        return;
    }
    std::cout << "    Properties:\n";
    for (const auto& [key, val] : props) {
        std::cout << "      - " << key << ": ";
        std::visit([](auto&& arg) {
            std::cout << arg;
        }, val);
        std::cout << "\n";
    }
}


int main() {
    graph_db::Graph g;
    graph_db::query::QueryParser traversal_parser;
    std::string line;

    print_help();

    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            std::cout << "> ";
            continue;
        }

        std::stringstream ss(line);
        std::string command;
        ss >> command;
        to_upper(command);

        try {
            if (command == "CREATE") {
                std::string type;
                ss >> type;
                to_upper(type);
                if (type == "NODE") {
                    graph_db::NodeID id = g.create_node();
                    std::cout << "Created node with ID: " << id << std::endl;
                } else if (type == "EDGE") {
                    std::string token;
                    graph_db::NodeID from, to;
                    std::string label;
                    int64_t weight = 1;
                    ss >> token; // FROM
                    ss >> from;
                    ss >> token; // TO
                    ss >> to;
                    ss >> token; // LABEL
                    ss >> label;
                    if (ss >> token && token == "WEIGHT") {
                        ss >> weight;
                    }
                    graph_db::EdgeID id = g.create_edge(from, to, label);
                    g.get_edge(id)->set_weight(weight);
                    std::cout << "Created edge with ID: " << id << " from " << from << " to " << to << std::endl;
                } else if (type == "INDEX") {
                    std::string on_token, key;
                    ss >> on_token >> key;
                    g.create_index(key);
                    std::cout << "Created index on property: " << key << std::endl;
                } else {
                    std::cerr << "Unknown CREATE type. Use NODE, EDGE, or INDEX." << std::endl;
                }
            } else if (command == "SET") {
                 std::string prop, on, type, key_token, val_token;
                 graph_db::NodeID id;
                 std::string key, val_str;
                 ss >> prop >> on >> type >> id >> key_token >> key >> val_token;
                 ss >> val_str;
                 to_upper(type);
                 if(key_token != "KEY" || val_token != "VALUE") throw std::runtime_error("Invalid SET syntax.");
                 
                 graph_db::PropertyValue value = parse_property_value(val_str);

                 if(type == "NODE") {
                    auto* node = g.get_node(id);
                    if(node) {
                        node->set_property(key, value);
                        std::cout << "Property set on node " << id << std::endl;
                    } else {
                        std::cerr << "Node " << id << " not found." << std::endl;
                    }
                 } else if (type == "EDGE") {
                     auto* edge = g.get_edge(id);
                     if(edge) {
                         edge->set_property(key, value);
                         std::cout << "Property set on edge " << id << std::endl;
                     } else {
                        std::cerr << "Edge " << id << " not found." << std::endl;
                     }
                 } else {
                     std::cerr << "Unknown SET type. Use NODE or EDGE." << std::endl;
                 }
            } else if (command == "GET") {
                std::string type;
                graph_db::NodeID id;
                ss >> type >> id;
                to_upper(type);
                if (type == "NODE") {
                    auto* node = g.get_node(id);
                    if (node) {
                        std::cout << "Node ID: " << node->get_id() << std::endl;
                        print_properties(node->get_properties());
                    } else {
                        std::cerr << "Node " << id << " not found." << std::endl;
                    }
                } else if (type == "EDGE") {
                     auto* edge = g.get_edge(id);
                     if (edge) {
                        std::cout << "Edge ID: " << edge->id() << "\n"
                                  << "  From: " << edge->from_node() << "\n"
                                  << "  To: " << edge->to_node() << "\n"
                                  << "  Label: " << edge->label() << "\n"
                                  << "  Weight: " << edge->get_weight() << "\n";
                        print_properties(edge->get_properties());
                     } else {
                        std::cerr << "Edge " << id << " not found." << std::endl;
                     }
                }
            } else if (command == "REMOVE") {
                std::string type;
                graph_db::NodeID id;
                ss >> type >> id;
                to_upper(type);
                if(type == "NODE"){
                    if(g.remove_node(id)) std::cout << "Removed node " << id << std::endl;
                    else std::cerr << "Node " << id << " not found." << std::endl;
                } else if (type == "EDGE"){
                    if(g.remove_edge(id)) std::cout << "Removed edge " << id << std::endl;
                    else std::cerr << "Edge " << id << " not found." << std::endl;
                }
            } else if (command == "PRINT" && ss.str().find("GRAPH") != std::string::npos) {
                 std::cout << "--- Current Graph State ---\n"
                           << "Nodes (" << g.node_count() << "):\n";
                 for(const auto& [id, node] : g.get_all_nodes()) {
                     std::cout << "  - Node " << id << "\n";
                 }
                 std::cout << "Edges (" << g.edge_count() << "):\n";
                 for(const auto& [id, edge] : g.get_all_edges()) {
                     std::cout << "  - Edge " << id << " (" << edge->from_node() 
                               << " -> " << edge->to_node() << ")\n";
                 }
                 std::cout << "---------------------------\n";
            } else if (command == "SAVE") {
                std::string filename;
                ss >> filename;
                if(g.save_to_file(filename)) std::cout << "Graph saved to " << filename << std::endl;
                else std::cerr << "Failed to save graph to " << filename << std::endl;
            } else if (command == "LOAD") {
                std::string filename;
                ss >> filename;
                if(g.load_from_file(filename)) std::cout << "Graph loaded from " << filename << std::endl;
                else std::cerr << "Failed to load graph from " << filename << std::endl;
            } else if (command == "BFS" || command == "DFS" || command == "SHORTEST") {
                auto parsed_query = traversal_parser.parse(line);
                 switch (parsed_query.type) {
                    case graph_db::query::QueryType::BFS: {
                        auto results = graph_db::bfs(g, parsed_query.start_node);
                        std::cout << "BFS Result: ";
                        for(auto n : results) std::cout << n << " ";
                        std::cout << std::endl;
                        break;
                    }
                    case graph_db::query::QueryType::DFS: {
                        auto results = graph_db::dfs(g, parsed_query.start_node);
                        std::cout << "DFS Result: ";
                        for(auto n : results) std::cout << n << " ";
                        std::cout << std::endl;
                        break;
                    }
                    case graph_db::query::QueryType::DIJKSTRA: {
                        auto results = graph_db::dijkstra(g, parsed_query.start_node);
                        std::cout << "Shortest distance from " << parsed_query.start_node << " to " << parsed_query.end_node 
                                  << " is: " << results[parsed_query.end_node] << std::endl;
                        break;
                    }
                    default:
                        std::cerr << "Unknown or malformed traversal query." << std::endl;
                }
            } else if (command == "HELP") {
                print_help();
            } else if (command == "EXIT") {
                break;
            } else {
                std::cerr << "Unknown command: " << command << ". Type HELP for a list of commands." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::cout << "> ";
    }

    std::cout << "Exiting." << std::endl;
    return 0;
}
