#include "../../include/graph_db/storage/serializer.h"
#include "../../include/graph_db/graph.h"
#include <variant>

namespace graph_db {
namespace storage {

Serializer::Serializer(Graph& graph) : graph_(graph) {}

bool Serializer::save_to_file(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        return false;
    }

    // Serialize nodes
    size_t node_count = graph_.node_count();
    out.write(reinterpret_cast<const char*>(&node_count), sizeof(node_count));
    for (const auto& [id, node_ptr] : graph_.get_all_nodes()) {
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        const auto& properties = node_ptr->get_properties();
        size_t prop_count = properties.size();
        out.write(reinterpret_cast<const char*>(&prop_count), sizeof(prop_count));
        for (const auto& [key, value] : properties) {
            size_t key_size = key.size();
            out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
            out.write(key.c_str(), key_size);
            write_property_value(out, value);
        }
    }

    // Serialize edges
    size_t edge_count = graph_.edge_count();
    out.write(reinterpret_cast<const char*>(&edge_count), sizeof(edge_count));
    for (const auto& [id, edge_ptr] : graph_.get_all_edges()) {
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        NodeID from = edge_ptr->from_node();
        NodeID to = edge_ptr->to_node();
        out.write(reinterpret_cast<const char*>(&from), sizeof(from));
        out.write(reinterpret_cast<const char*>(&to), sizeof(to));
        std::string label = edge_ptr->label();
        size_t label_size = label.size();
        out.write(reinterpret_cast<const char*>(&label_size), sizeof(label_size));
        out.write(label.c_str(), label_size);
        const auto& properties = edge_ptr->get_properties();
        size_t prop_count = properties.size();
        out.write(reinterpret_cast<const char*>(&prop_count), sizeof(prop_count));
        for (const auto& [key, value] : properties) {
            size_t key_size = key.size();
            out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
            out.write(key.c_str(), key_size);
            write_property_value(out, value);
        }
    }

    return true;
}

bool Serializer::load_from_file(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        return false;
    }

    // Deserialize nodes
    size_t node_count;
    in.read(reinterpret_cast<char*>(&node_count), sizeof(node_count));
    for (size_t i = 0; i < node_count; ++i) {
        NodeID id;
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        Node* node = graph_.create_node(id);
        size_t prop_count;
        in.read(reinterpret_cast<char*>(&prop_count), sizeof(prop_count));
        for (size_t j = 0; j < prop_count; ++j) {
            size_t key_size;
            in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
            std::string key(key_size, '\0');
            in.read(&key[0], key_size);
            node->set_property(key, read_property_value(in));
        }
    }

    // Deserialize edges
    size_t edge_count;
    in.read(reinterpret_cast<char*>(&edge_count), sizeof(edge_count));
    for (size_t i = 0; i < edge_count; ++i) {
        EdgeID id;
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        NodeID from, to;
        in.read(reinterpret_cast<char*>(&from), sizeof(from));
        in.read(reinterpret_cast<char*>(&to), sizeof(to));
        size_t label_size;
        in.read(reinterpret_cast<char*>(&label_size), sizeof(label_size));
        std::string label(label_size, '\0');
        in.read(&label[0], label_size);
        Edge* edge = graph_.create_edge(from, to, label, id);
        size_t prop_count;
        in.read(reinterpret_cast<char*>(&prop_count), sizeof(prop_count));
        for (size_t j = 0; j < prop_count; ++j) {
            size_t key_size;
            in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
            std::string key(key_size, '\0');
            in.read(&key[0], key_size);
            edge->set_property(key, read_property_value(in));
        }
    }

    return true;
}

void Serializer::write_property_value(std::ofstream& out, const PropertyValue& value) {
    uint8_t type = value.index();
    out.write(reinterpret_cast<const char*>(&type), sizeof(type));
    std::visit([&out](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
            size_t size = arg.size();
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
            out.write(arg.c_str(), size);
        } else {
            out.write(reinterpret_cast<const char*>(&arg), sizeof(T));
        }
    }, value);
}

PropertyValue Serializer::read_property_value(std::ifstream& in) {
    uint8_t type;
    in.read(reinterpret_cast<char*>(&type), sizeof(type));
    switch (type) {
        case 0: { // int64_t
            int64_t val;
            in.read(reinterpret_cast<char*>(&val), sizeof(val));
            return val;
        }
        case 1: { // double
            double val;
            in.read(reinterpret_cast<char*>(&val), sizeof(val));
            return val;
        }
        case 2: { // string
            size_t size;
            in.read(reinterpret_cast<char*>(&size), sizeof(size));
            std::string val(size, '\0');
            in.read(&val[0], size);
            return val;
        }
        case 3: { // bool
            bool val;
            in.read(reinterpret_cast<char*>(&val), sizeof(val));
            return val;
        }
        default:
            throw std::runtime_error("Unknown property type");
    }
}

} // namespace storage
} // namespace graph_db