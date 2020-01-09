#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>

enum permissions {
    READ    = 1 << 0,
    WRITE   = 1 << 1,
    EXECUTE = 1 << 2,
    PRIVATE = 1 << 3
};

typedef struct mem_range {
    uint64_t start;
    uint64_t end;
    std::string path;
    permissions perm;
} mem_range;

typedef struct proc_maps_info {
    std::vector<mem_range> ranges;
} proc_maps_info;