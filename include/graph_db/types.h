#pragma once
#include<unordered_map>
#include <cstdint>
#include<variant>
namespace graph_db{
    using NodeID= std::uint64_t;
    using EdgeID=std::uint64_t;
    using PropertyValue = std::variant<std::int64_t, double, std::string, bool>;
    using PropertyMap=std::unordered_map<std::string,PropertyValue>;
}