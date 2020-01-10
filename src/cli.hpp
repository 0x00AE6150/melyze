#pragma once

#include "value.hpp"

static uint64_t s_pid = 0;

enum class CommandType {
    UNKNOWN = 0,
    PROCESSES,      // (melyze) p
    ATTACH,         // (melyze) a <pid>
    DUMP_MEM,       // (melyze) d <file-path>
    SEEK_ADDR_OF,   // (melyze) s <value type> <val>
    WRITE_AT_ADDR,  // (melyze) w <addr> <value type> <val>
    DUMP_LIB_SYMS   // (melyze) l <library name>
};

struct Command {
    CommandType type;
    Value operand_1;
    Value operand_2;
};

enum class CliColor {
    RED,
    RED_BOLD,
    GREEN,
    GREEN_BOLD,
    YELLOW,
    YELLOW_BOLD,
    BLUE,
    BLUE_BOLD,
    MAGENTA,
    MAGENTA_BOLD,
    CYAN,
    CYAN_BOLD,
    RESET
};

void set_cli_color(CliColor color);

void execute_cmd(const Command& cmd);

bool melyze_cli_run();
