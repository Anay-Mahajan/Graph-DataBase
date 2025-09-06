#include "../../include/graph_db/storage/disk_manager.h"
#include <stdexcept>
#include <sys/stat.h>


namespace graph_db {
namespace storage {

constexpr size_t PAGE_SIZE = 4096;

DiskManager::DiskManager(const std::string& db_file) : file_name_(db_file) {
    db_io_.open(db_file, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    if (!db_io_.is_open()) {
        db_io_.open(db_file, std::ios::out | std::ios::binary | std::ios::app);
        db_io_.close();
        db_io_.open(db_file, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    }
}

DiskManager::~DiskManager() {
    if (db_io_.is_open()) {
        db_io_.close();
    }
}

void DiskManager::write_page(PageID page_id, const char* page_data) {
    std::lock_guard<std::mutex> lock(db_io_latch_);
    db_io_.seekp(page_id * PAGE_SIZE);
    db_io_.write(page_data, PAGE_SIZE);
    if (db_io_.bad()) {
        throw std::runtime_error("Error writing to file");
    }
}

void DiskManager::read_page(PageID page_id, char* page_data) {
    std::lock_guard<std::mutex> lock(db_io_latch_);
    db_io_.seekg(page_id * PAGE_SIZE);
    db_io_.read(page_data, PAGE_SIZE);
    if (db_io_.bad()) {
        throw std::runtime_error("Error reading from file");
    }
}

} // namespace storage
} // namespace graph_db