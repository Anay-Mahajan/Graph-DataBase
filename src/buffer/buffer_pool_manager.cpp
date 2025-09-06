#include "../../include/graph_db/buffer/buffer_pool_manager.h"
#include <stdexcept>

namespace graph_db {
namespace buffer {

BufferPoolManager::BufferPoolManager(size_t pool_size, storage::DiskManager* disk_manager)
    : disk_manager_(disk_manager), replacer_(pool_size) { // The 'pool_size_(pool_size)' part is removed here
    pages_ = new Page[pool_size];
    for (size_t i = 0; i < pool_size; ++i) {
        free_list_.push_back(&pages_[i]);
    }
}
BufferPoolManager::~BufferPoolManager() {
    flush_all_pages();
    delete[] pages_;
}

Page* BufferPoolManager::fetch_page(PageID page_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (page_table_.count(page_id)) {
        FrameID frame_id = page_table_[page_id];
        pages_[frame_id].pin_count_++;
        replacer_.pin(frame_id);
        return &pages_[frame_id];
    }

    FrameID frame_id;
    if (!free_list_.empty()) {
        frame_id = free_list_.front() - pages_;
        free_list_.pop_front();
    } else if (replacer_.victim(&frame_id)) {
        Page& victim = pages_[frame_id];
        if (victim.is_dirty_) {
            disk_manager_->write_page(victim.page_id_, victim.data_);
        }
        page_table_.erase(victim.page_id_);
    } else {
        return nullptr; // No frame available
    }

    Page& new_page = pages_[frame_id];
    new_page.page_id_ = page_id;
    new_page.pin_count_ = 1;
    new_page.is_dirty_ = false;
    disk_manager_->read_page(page_id, new_page.data_);
    page_table_[page_id] = frame_id;
    replacer_.pin(frame_id);
    return &new_page;
}

bool BufferPoolManager::unpin_page(PageID page_id, bool is_dirty) {
    std::lock_guard<std::mutex> lock(latch_);
    if (!page_table_.count(page_id)) {
        return false;
    }
    FrameID frame_id = page_table_[page_id];
    Page& page = pages_[frame_id];
    if (page.pin_count_ <= 0) {
        return false;
    }
    page.pin_count_--;
    if (is_dirty) {
        page.is_dirty_ = true;
    }
    if (page.pin_count_ == 0) {
        replacer_.unpin(frame_id);
    }
    return true;
}

bool BufferPoolManager::flush_page(PageID page_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (!page_table_.count(page_id)) {
        return false;
    }
    FrameID frame_id = page_table_[page_id];
    Page& page = pages_[frame_id];
    if (page.is_dirty_) {
        disk_manager_->write_page(page.page_id_, page.data_);
        page.is_dirty_ = false;
    }
    return true;
}

void BufferPoolManager::flush_all_pages() {
    std::lock_guard<std::mutex> lock(latch_);
    for (auto const& [page_id, frame_id] : page_table_) {
        flush_page(page_id);
    }
}

Page* BufferPoolManager::new_page(PageID* page_id) {
    // This is a simplified version. A real implementation would need to manage page allocation on disk.
    // For now, we'll just use a placeholder.
    static PageID next_page_id = 0;
    *page_id = next_page_id++;
    return fetch_page(*page_id);
}

bool BufferPoolManager::delete_page(PageID page_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (!page_table_.count(page_id)) {
        return true; // Page not in buffer pool
    }
    FrameID frame_id = page_table_[page_id];
    Page& page = pages_[frame_id];
    if (page.pin_count_ > 0) {
        return false; // Cannot delete pinned page
    }
    if (page.is_dirty_) {
        disk_manager_->write_page(page.page_id_, page.data_);
    }
    page_table_.erase(page_id);
    replacer_.pin(frame_id); // so it's not victimized
    free_list_.push_back(&page);
    return true;
}

} // namespace buffer
} // namespace graph_db