#include <cstdlib>

#include "cli.hpp"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        s_pid = (uint64_t)strtol(argv[1], nullptr, 10);
    }

    bool should_run = true;
    while(should_run) {
        should_run = melyze_cli_run();
    }

    return 0;
}