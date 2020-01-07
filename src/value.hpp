#pragma once

#include <cstdint>
#include <string>

typedef enum value_type {
    unknown,
    int8,
    uint8,
    int16,
    uint16,
    int32,
    uint32,
    int64,
    uint64,
    string,
    path
} value_type;

typedef union value_data {
    int8_t int8;
    uint8_t uint8;
    int16_t int16;
    uint16_t uint16;
    int32_t int32;
    uint32_t uint32;
    int64_t int64;
    uint64_t uint64;
    std::string* string;
    std::string* path;
} value_data;

typedef struct value {
    value_type type;
    value_data data;

    ~value() {
        if (type == value_type::string || type == value_type::path) {
            delete data.string;
        }
    }
} value;

value_type value_type_from_string(const std::string& type);

value value_from_string(value_type type, const std::string& valStr);