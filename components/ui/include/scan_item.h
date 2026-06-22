#pragma once
#include <string>
struct ScanItem {
    std::string path;
    std::string name;
    int depth;
    bool is_dir;
};