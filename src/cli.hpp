#pragma once

#include "value.hpp"

static uint64_t s_pid = 0;

typedef enum command_type {
    UNKNOWN = 0,
    PROCESSES,      // (melyze) p
    ATTACH,         // (melyze) a <pid>
    DUMP_MEM,       // (melyze) d <file-path>
    SEEK_ADDR_OF,   // (melyze) s <value type> <val>
    WRITE_AT_ADDR,  // (melyze) w <addr> <value type> <val>
    DUMP_LIB_SYMS   // (melyze) l <library name>
} command_type;

typedef struct command {
    command_type type;
    value operand_1;
    value operand_2;
} command;

typedef enum cli_color {
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
} cli_color;

void set_cli_color(cli_color color);

void execute_cmd(const command& cmd);

bool melyze_cli_run();
