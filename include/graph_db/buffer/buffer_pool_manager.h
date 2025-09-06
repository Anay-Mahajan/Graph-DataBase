#pragma once

#include <list>
#include <mutex>
#include <unordered_map>
#include "../storage/disk_manager.h"
#include "lru_replacer.h"

namespace graph_db {
namespace buffer {

class BufferPoolManager {
public:
    BufferPoolManager(size_t pool_size, storage::DiskManager* disk_manager);
    ~BufferPoolManager();

    Page* fetch_page(PageID page_id);
    bool unpin_page(PageID page_id, bool is_dirty);
    bool flush_page(PageID page_id);
    void flush_all_pages();
    Page* new_page(PageID* page_id);
    bool delete_page(PageID page_id);

private:
    // size_t pool_size_; // This line is removed
    storage::DiskManager* disk_manager_;
    LRUReplacer replacer_;
    std::list<Page*> free_list_;
    std::unordered_map<PageID, FrameID> page_table_;
    Page* pages_;
    std::mutex latch_;
};

} // namespace buffer
} // namespace graph_db