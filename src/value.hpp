#pragma once

#include <cstdint>
#include <string>

enum class ValueType {
    UNKNOWN,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,
    ADDR,
    STRING,
    PATH
};

union ValueData {
    int8_t int8;
    uint8_t uint8;
    int16_t int16;
    uint16_t uint16;
    int32_t int32;
    uint32_t uint32;
    int64_t int64;
    uint64_t uint64;
    uint64_t addr;
    std::string* string;
    std::string* path;
};

struct Value {
    ValueType type;
    ValueData data;

    ~Value() {
        if (type == ValueType::STRING || type == ValueType::PATH) {
            delete data.string;
        }
    }
};

ValueType value_type_from_string(const std::string& type);

Value value_from_string(ValueType type, const std::string& valStr);