#include "process.hpp"

#include <zconf.h>
#include <sys/ptrace.h>
#include <wait.h>
#include <string>
#include <stdexcept>

bool process::check_process(uint64_t pid) {
    char mem_maps_path[64];
    sprintf(mem_maps_path, "/proc/%lu/maps", pid);

    char mem_path[64];
    sprintf(mem_path, "/proc/%lu/mem", pid);

    if (access(mem_maps_path, F_OK) == -1 || access(mem_path, F_OK) == -1) {
        return false;
    }

    return true;
}

bool process::attach(uint64_t pid, bool should_suspend) {
    if (ptrace(PTRACE_ATTACH, pid, 0, 0) == -1) {
        return false;
    }

    if (should_suspend) {
        waitpid(pid, nullptr, 0);
    }

    return true;
}

void process::detach(uint64_t pid) {
    ptrace(PTRACE_DETACH, pid, 0, 0);
}

FILE* process::open_maps_fd(uint64_t pid) {
    if (!check_process(pid)) {
        throw std::runtime_error("Process " + std::to_string(pid) + " doesn't exist\n");
    }

    std::string mem_maps_path = "/proc/" + std::to_string(pid) + "/maps";
    FILE* maps_fd = fopen(mem_maps_path.c_str(), "r");
    if (maps_fd == nullptr) {
        throw std::runtime_error("Failed to read " + std::to_string(pid) + " process memory maps.\nTry running \"melyze\" with sudo\n");
    }

    return maps_fd;
}

FILE* process::open_mem_fd(uint64_t pid) {
    if (!check_process(pid)) {
        throw std::runtime_error("Process " + std::to_string(pid) + " doesn't exist\n");
    }

    std::string mem_path = "/proc/" + std::to_string(pid) + "/mem";
    FILE* mem_fd = fopen(mem_path.c_str(), "r+");
    if (mem_fd == nullptr) {
        throw std::runtime_error("Failed to read " + std::to_string(pid) + " process memory\nTry running \"melyze\" with sudo\n");
    }

    return mem_fd;
}

ProcMapInfo process::parse_proc_maps(FILE* maps_fd) {
    ProcMapInfo proc_maps_info;

    size_t line_max_size = 256;
    char line[line_max_size];
    while (fgets(line, line_max_size, maps_fd) != nullptr) {
        MemoryRange range;
        sscanf(line, "%16lx-%16lx\n", &range.start, &range.end);
        proc_maps_info.ranges.push_back(range);
    }

    return proc_maps_info;
}

ProcMapInfo process::parse_proc_maps(uint64_t pid) {
    FILE* maps_fd = open_maps_fd(pid);
    ProcMapInfo info = parse_proc_maps(maps_fd);
    fclose(maps_fd);
    return info;
}
