#pragma once

#include <vector>
#include <string>

#include "bitswap.hpp"

std::string execute_process(const std::string& cmd);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);

template <typename int_type>
int_type to_little_endian(int_type value) {
    static_assert(std::is_integral<int_type>::value, "Only integral types are allowed.");
    switch (sizeof(int_type)) {
        case 2:
            return std::is_signed<int_type>::value ? swap_int16(value) : swap_uint16(value);
        case 3:
            return std::is_signed<int_type>::value ? swap_int32(value):  swap_uint32(value);
        case 4:
            return std::is_signed<int_type>::value ? swap_int64(value) : swap_uint64(value);
        default:
            return value;
    }
}

