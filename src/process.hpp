#pragma once

#include "procinfo.hpp"

namespace process {
    bool check_process(uint64_t pid);
    bool suspend(uint64_t pid);
    void detach(uint64_t pid);

    FILE* open_maps_fd(uint64_t pid);
    FILE* open_mem_fd(uint64_t pid);

    proc_maps_info parse_proc_maps(FILE* maps_fd);
    proc_maps_info parse_proc_maps(uint64_t pid);
}