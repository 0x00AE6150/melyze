#include "utils.hpp"

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string execute_process(const std::string& cmd) {
    std::array<char, 1024> buffer{ };
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to execute \"" + cmd + "\" command");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
