#include "value.hpp"

#include <cstdlib>

value_type value_type_from_string(const std::string& type) {
    if (type == "i8") {
        return int8;
    } else if (type == "u8") {
        return uint8;
    } else if (type == "i16") {
        return int16;
    } else if (type == "u16") {
        return uint16;
    } else if (type == "i32") {
        return int32;
    } else if (type == "u32") {
        return uint32;
    } else if (type == "i64") {
        return int64;
    } else if (type == "u64") {
        return uint64;
    } else if (type == "addr") {
        return addr;
    } else if (type == "str") {
        return string;
    }

    return unknown;
}

value value_from_string(value_type type, const std::string& valStr) {
    value val;
    val.type = type;
    switch (type) {
        case int8:
        case uint8:
        case int16:
        case uint16:
        case int32:
        case uint32:
        case int64:
        case uint64:
            val.data.uint64 = strtol(valStr.c_str(), nullptr, 10);
            break;
        case addr:
            val.data.addr = strtol(valStr.c_str(), nullptr, 0);
            break;
        case string:
        case path:
            val.data.string = new std::string(valStr);
            val.data.string = new std::string(valStr);
            break;
        default:
            break;
    }

    return val;
}
