#pragma once

#include <list>
#include <mutex>
#include <unordered_map>
#include "../types.h"

namespace graph_db {
namespace buffer {

class LRUReplacer {
public:
    LRUReplacer(size_t num_pages);
    ~LRUReplacer() = default;

    bool victim(FrameID* frame_id);
    void pin(FrameID frame_id);
    void unpin(FrameID frame_id);
    size_t size();

private:
    std::list<FrameID> lru_list_;
    std::unordered_map<FrameID, std::list<FrameID>::iterator> lru_map_;
    std::mutex latch_;
    size_t capacity_;
};

} // namespace buffer
} // namespace graph_db