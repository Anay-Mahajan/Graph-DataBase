#include "../../include/graph_db/buffer/lru_replacer.h"

namespace graph_db {
namespace buffer {

LRUReplacer::LRUReplacer(size_t num_pages) : capacity_(num_pages) {}

bool LRUReplacer::victim(FrameID* frame_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (lru_list_.empty()) {
        return false;
    }
    *frame_id = lru_list_.back();
    lru_list_.pop_back();
    lru_map_.erase(*frame_id);
    return true;
}

void LRUReplacer::pin(FrameID frame_id) {
    std::lock_guard<std::mutex> lock(latch_);
    auto it = lru_map_.find(frame_id);
    if (it != lru_map_.end()) {
        lru_list_.erase(it->second);
        lru_map_.erase(it);
    }
}

void LRUReplacer::unpin(FrameID frame_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (lru_map_.find(frame_id) == lru_map_.end()) {
        if (lru_list_.size() >= capacity_) {
            // This shouldn't happen if managed correctly
            return;
        }
        lru_list_.push_front(frame_id);
        lru_map_[frame_id] = lru_list_.begin();
    }
}

size_t LRUReplacer::size() {
    std::lock_guard<std::mutex> lock(latch_);
    return lru_list_.size();
}

} // namespace buffer
} // namespace graph_db