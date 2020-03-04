#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>

#include "utils.hpp"

struct MemoryRange {
    explicit MemoryRange(const std::string& procMapsEntry) {
        std::vector<std::string> tokens = split(procMapsEntry, " ");

        std::vector<std::string> addr_range = split(tokens[0], "-");
        if (addr_range.size() != 2) {
            throw std::runtime_error("Failed to parse proc maps entry");
        }

        start = std::strtol(addr_range[0].c_str(), nullptr, 16);
        end = std::strtol(addr_range[1].c_str(), nullptr, 16);

        const std::string& perms = tokens[1];
        if (perms.size() != 4) {
            throw std::runtime_error("Failed to parse proc maps entry");
        }

        read = perms[0] == 'r';
        write = perms[1] == 'w';
        exec = perms[2] == 'x';
        shared = perms[0] != 'p';

        offset = std::strtol(tokens[2].c_str(), nullptr, 16);
        dev = tokens[3];
        inode = std::strtol(tokens[4].c_str(), nullptr, 10);

        if (tokens.size() == 6) {
            pathname = tokens[5];
        } else {
            for (uint64_t idx = 5; idx < tokens.size(); ++idx) {
                pathname += tokens[idx];
                if (idx + 1 != tokens.size()) {
                    pathname += " ";
                }
            }
        }
    }

    uint64_t start;
    uint64_t end;
    bool read;
    bool write;
    bool exec;
    bool shared;
    uint64_t offset;
    std::string dev;
    uint64_t inode;
    std::string pathname;

};

struct ProcMapInfo {
    std::vector<MemoryRange> ranges;
};