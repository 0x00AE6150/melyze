#include "value.hpp"

#include <cstdlib>

ValueType value_type_from_string(const std::string& type) {
    if (type == "i8") {
        return ValueType::INT8;
    } else if (type == "u8") {
        return ValueType::UINT8;
    } else if (type == "i16") {
        return ValueType::INT16;
    } else if (type == "u16") {
        return ValueType::UINT16;
    } else if (type == "i32") {
        return ValueType::INT32;
    } else if (type == "u32") {
        return ValueType::UINT32;
    } else if (type == "i64") {
        return ValueType::INT64;
    } else if (type == "u64") {
        return ValueType::UINT64;
    } else if (type == "addr") {
        return ValueType::ADDR;
    } else if (type == "str") {
        return ValueType::STRING;
    }

    return ValueType::UNKNOWN;
}

Value value_from_string(ValueType type, const std::string& valStr) {
    Value val{ };
    val.type = type;
    switch (type) {
        case ValueType::INT8:
        case ValueType::UINT8:
        case ValueType::INT16:
        case ValueType::UINT16:
        case ValueType::INT32:
        case ValueType::UINT32:
        case ValueType::INT64:
        case ValueType::UINT64:
            val.data.uint64 = strtol(valStr.c_str(), nullptr, 10);
            break;
        case ValueType::ADDR:
            val.data.addr = strtol(valStr.c_str(), nullptr, 0);
            break;
        case ValueType::STRING:
        case ValueType::PATH:
            val.data.string = new std::string(valStr);
            val.data.string = new std::string(valStr);
            break;
        default:
            break;
    }

    return val;
}
