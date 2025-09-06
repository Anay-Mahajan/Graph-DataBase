#pragma once
#include "../types.h" 
#include <fstream>
#include <string>

// Forward declare Graph to avoid circular dependencies
namespace graph_db {
    class Graph;
}

namespace graph_db {
namespace storage {

class Serializer {
public:
    explicit Serializer(Graph& graph);

    bool save_to_file(const std::string& filename);
    bool load_from_file(const std::string& filename);

private:
    Graph& graph_;

    void write_property_value(std::ofstream& out, const PropertyValue& value);
    PropertyValue read_property_value(std::ifstream& in);
};

} // namespace storage
} // namespace graph_db