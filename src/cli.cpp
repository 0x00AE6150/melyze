#include <cassert>
#include <string>
#include <array>
#include <cstring>

#include "cli.hpp"
#include "melyze.hpp"
#include "process.hpp"

#define MAX_INPUT_SIZE 256

void empty_stdin() {
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

void print_help() {
    // TODO
}

void set_cli_color(cli_color color) {
    switch (color) {
        case RED:
            printf("\033[0;31m");
            break;
        case RED_BOLD:
            printf("\033[1;31m");
            break;
        case GREEN:
            printf("\033[0;32m");
            break;
        case GREEN_BOLD:
            printf("\033[1;32m");
            break;
        case YELLOW:
            printf("\033[0;33m");
            break;
        case YELLOW_BOLD:
            printf("\033[1;33m");
            break;
        case BLUE:
            printf("\033[0;34m");
            break;
        case BLUE_BOLD:
            printf("\033[1;34m");
            break;
        case MAGENTA:
            printf("\033[0;35m");
            break;
        case MAGENTA_BOLD:
            printf("\033[1;35m");
            break;
        case CYAN:
            printf("\033[0;36m");
            break;
        case CYAN_BOLD:
            printf("\033[1;36m");
            break;
        default:
            printf("\033[0m");
            break;
    }
}

void execute_cmd(const command& cmd) {
    try {
        switch (cmd.type) {
            case PROCESSES:
                list_processes();
                break;
            case ATTACH: {
                uint64_t pid = cmd.operand_1.data.uint64;
                if (process::check_process(pid)) {
                    s_pid = pid;
                }
                break;
            } case DUMP_MEM:
                dump_proc_mem(s_pid);
                break;
            case SEEK_ADDR_OF:
                seek_addr(s_pid, cmd.operand_1, true);
                break;
            case WRITE_AT_ADDR:
                write_val_at_addr(s_pid, cmd.operand_1, cmd.operand_2);
                break;
            case DUMP_LIB_SYMS:

                break;
            default:
                assert(false);
        }
    } catch (std::exception& e) {
        printf("%s\n", e.what());
    }
}

bool melyze_cli_run() {
    set_cli_color(CYAN_BOLD);
    printf("(melyze) ");
    set_cli_color(RESET);

    char buffer[MAX_INPUT_SIZE];
    fgets(buffer, MAX_INPUT_SIZE, stdin);

    size_t input_len = strlen(buffer);
    if (input_len <= 1 /* \n */) {
        return true;
    }

    if (input_len >= MAX_INPUT_SIZE - 1) {
        empty_stdin();
        return true;
    }

    strtok(buffer, "\n");

    command cmd;
    cmd.type = UNKNOWN;

    const uint8_t allowed_word_count = 4;
    std::array<std::string, allowed_word_count> words;

    uint8_t word_count = 0;

    char delim[] = " ";
    char* word = strtok(buffer, delim);
    words[word_count] = word;

    while(word != nullptr && word_count < allowed_word_count) {
        word = strtok(nullptr, delim);
        ++word_count;
        if (word != nullptr) {
            words[word_count] = word;
        }
    }

    if (words[0].size() != 1) {
        printf("Unknown command\n");
        print_help();
        return true;
    } else if (words[0] ==  "p") {
        if (word_count != 1) {
            printf("Incorrect command operands.\nUsage:\n\tp\n");
            return true;
        }

        cmd.type = PROCESSES;
    } else if (words[0] == "a") {
        if (word_count != 2) {
            printf("Incorrect command operands.\nUsage:\n\ta <pid>\n");
            return true;
        }

        cmd.type = ATTACH;
        cmd.operand_1 = value_from_string(uint64, words[1]);
    } else if (words[0] == "d") {
        if (word_count != 2) {
            printf("Incorrect command operands.\nUsage:\n\td <file-path>\n");
            return true;
        }

        if (s_pid == 0) {
            printf("Attach to the process first.\nUsage:\n\ta <pid>\n");
            return true;
        }

        cmd.type = DUMP_MEM;
        cmd.operand_1 = value_from_string(path, words[1]);
        // TODO implement dump_mem
        printf("Unimplemented command\n");
    } else if (words[0] == "s") {
        if (word_count != 3) {
            printf("Incorrect command operands.\nUsage:\n\ts <value-type> <value>\n");
            return true;
        }

        if (s_pid == 0) {
            printf("Attach to the process first.\nUsage:\n\ta <pid>\n");
            return true;
        }

        cmd.type = SEEK_ADDR_OF;
        cmd.operand_1 = value_from_string(value_type_from_string(words[1]), words[2]);
        if (cmd.operand_1.type == unknown) {
            printf("Incorrect command operands.\nUnknown value type \"%s\"\n", words[1].c_str());
            printf("Possible options:\n"
                   "    i8\n"
                   "    u8\n"
                   "    i6\n"
                   "    u16\n"
                   "    i32\n"
                   "    u32\n"
                   "    i64\n"
                   "    u64\n"
                   "    str\n"
            );
            return true;
        }
    } else if (words[0] == "w") {
        if (word_count != 4) {
            printf("Incorrect command operands.\nUsage:\n\tw <address> <value-type> <value>\n");
            return true;
        }

        if (s_pid == 0) {
            printf("Attach to the process first.\nUsage:\n\ta <pid>\n");
            return true;
        }

        cmd.type = WRITE_AT_ADDR;
        cmd.operand_1 = value_from_string(uint64, words[1]);
        if (cmd.operand_1.data.uint64 == 0) {
            printf("Incorrect command operands. Failed to parse %s address\n", words[1].c_str());
            return true;
        }

        cmd.operand_2 = value_from_string(value_type_from_string(words[2]), words[3]);
        if (cmd.operand_2.type == unknown) {
            printf("Incorrect command operands.\nUnknown value type \"%s\"\n", words[1].c_str());
            printf("Possible options:\n"
                   "    i8\n"
                   "    u8\n"
                   "    i6\n"
                   "    u16\n"
                   "    i32\n"
                   "    u32\n"
                   "    i64\n"
                   "    u64\n"
                   "    str\n"
            );
            return true;
        }
    } else if (words[0] == "l"){
        if (word_count != 2) {
            printf("Incorrect command operands.\nUsage:\n\tl <library name>.so\n");
            return true;
        }

        if (s_pid == 0) {
            printf("Attach to the process first.\nUsage:\n\ta <pid>\n");
            return true;
        }

        cmd.type = DUMP_LIB_SYMS;
        cmd.operand_1 = value_from_string(string, words[1]);
        if (cmd.operand_1.data.string->empty()) {
            printf("Incorrect command operands. Failed to parse library name %s\n", words[1].c_str());
            return true;
        }
    } else if (words[0] == "q") {
        return false;
    } else {
        printf("Unknown command\n");
        print_help();
        return true;
    }

    execute_cmd(cmd);

    return true;
}
