#include "utils.hpp"

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>

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

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;

    size_t start_pos = 0;
    size_t delim_pos = 0;
    std::string token;
    while ((delim_pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(start_pos, delim_pos);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        start_pos = delim_pos + delimiter.size();
    }

    return tokens;
}
