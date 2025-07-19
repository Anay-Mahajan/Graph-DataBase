#pragma once
#include<unordered_map>
#include <cstdint>
#include<variant>
using namespace std;

namespace graph_db{
    using NodeID= uint64_t;
    using EdgeID=uint64_t;
    using PropertyValue = variant<int64_t, double, std::string, bool>;
    using PropertyMap=unordered_map<string,PropertyValue>;
    
}