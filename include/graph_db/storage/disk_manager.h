#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include "../types.h"

namespace graph_db {
namespace storage {

class DiskManager {
public:
    DiskManager(const std::string& db_file);
    ~DiskManager();

    void write_page(PageID page_id, const char* page_data);
    void read_page(PageID page_id, char* page_data);

private:
    std::fstream db_io_;
    std::string file_name_;
    std::mutex db_io_latch_;
};

} // namespace storage
} // namespace graph_db