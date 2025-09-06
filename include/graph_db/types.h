#pragma once
#include<unordered_map>
#include <cstdint>
#include<variant>
#include<string>
namespace graph_db{
    using NodeID= std::uint64_t;
    using EdgeID=std::uint64_t;
    using PropertyValue = std::variant<std::int64_t, double, std::string, bool>;
    using PropertyMap=std::unordered_map<std::string,PropertyValue>;
    using PageID = int32_t;
    using FrameID = int32_t;

    class Page {
    public:
        char data_[4096]{};
        PageID page_id_{};
        int pin_count_{0};
        bool is_dirty_{false};
    };
}