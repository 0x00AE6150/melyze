#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>

enum class Permissions {
    READ    = 1 << 0,
    WRITE   = 1 << 1,
    EXECUTE = 1 << 2,
    PRIVATE = 1 << 3
};

struct MemoryRange {
    uint64_t start;
    uint64_t end;
    std::string path;
    Permissions perm;
};

struct ProcMapInfo {
    std::vector<MemoryRange> ranges;
};