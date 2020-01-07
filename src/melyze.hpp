#pragma once

#include <cstdint>

#include "value.hpp"

void list_processes();

void dump_proc_mem(uint64_t pid);

void seek_addr(uint64_t pid, const value& val, bool should_suspend);

void write_val_at_addr(uint64_t pid, const value& address, const value& val);

void dump_library_symbols(uint64_t pid, const value& val);
