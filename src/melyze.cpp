#include "melyze.hpp"

#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <vector>
#include <cstring>
#include <iterator>

#include "process.hpp"
#include "bitswap.hpp"
#include "utils.hpp"

template <typename int_type>
void print_value_address(const ProcMapInfo& proc_map_info, FILE* mem_fd, int_type value) {
    static_assert(std::is_integral<int_type>::value, "Only integral types are allowed.");

    int_type little_endian_value = to_little_endian(value);

    size_t buffer_size = 0;
    for (const MemoryRange& range : proc_map_info.ranges) {
        size_t range_size = range.end - range.start;

        if (range_size <= 0) {
            continue;
        }

        if (buffer_size < range_size) {
            buffer_size = range_size;
        }
    }

    uint8_t* buffer = (uint8_t*)malloc(buffer_size * sizeof(uint8_t));

    for (const MemoryRange& range : proc_map_info.ranges) {
        size_t range_size = range.end - range.start;
        if (range_size <= 0) {
            continue;
        }

        //printf("Scanning range 0x%lx - 0x%lx \n", ange.start, range.end);

        if (fseeko(mem_fd, range.start, SEEK_SET) != 0) {
            continue;
        }

        fread(buffer, sizeof(uint8_t), range_size, mem_fd);

        for (size_t byte_offset = 0; byte_offset + sizeof(int_type) <= range_size; ++byte_offset) {
            int_type* value_in_buffer = reinterpret_cast<int_type*>(buffer + byte_offset);
            if (*value_in_buffer == value || *value_in_buffer == little_endian_value) {
                printf("0x%llx\n", range.start + byte_offset);
            }
        }
    }

    free(buffer);
}

void print_byte_array_value_address(const ProcMapInfo& proc_map_info, FILE* mem_fd, const uint8_t* array, size_t size) {
    size_t buffer_size = 0;
    for (const MemoryRange& range : proc_map_info.ranges) {
        size_t range_size = range.end - range.start;

        if (range_size <= 0) {
            continue;
        }

        if (buffer_size < range_size) {
            buffer_size = range_size;
        }
    }

    uint8_t* buffer = (uint8_t*)malloc(buffer_size * sizeof(uint8_t));

    for (const MemoryRange& range : proc_map_info.ranges) {
        size_t range_size = range.end - range.start;
        if (range_size <= 0) {
            continue;
        }

        //printf("Scanning range 0x%llx - 0x%llx \n", (long) (range.start), (long) (range.end));

        fseeko(mem_fd, range.start, SEEK_SET);
        fread(buffer, sizeof(uint8_t), range_size, mem_fd);

        for (size_t byte_offset = 0; byte_offset + sizeof(uint8_t) <= range_size; ++byte_offset) {
            uint8_t* addr = buffer + byte_offset;
            if (memcmp(addr, array, size) == 0) {
                printf("0x%llx\n", range.start + byte_offset);
            }
        }
    }

    free(buffer);
}

void list_processes() {
    std::string output = execute_process("/bin/ps -A");
    std::stringstream output_stream(output);

    std::vector<std::string> words {
        std::istream_iterator<std::string>{ output_stream },
        std::istream_iterator<std::string>{ }
    };

    size_t wordIdx = 0;
    std::string proc_name;
    for (const std::string& word : words) {
        if (word == "PID" || word == "TTY" || word == "TIME" || word ==  "CMD") {
            continue;
        }

        if (wordIdx >= 3 && std::atoi(word.c_str()) != 0) {
            printf("%s\n", proc_name.c_str());
            proc_name = "";
            wordIdx = 0;
        }

        if (wordIdx == 0) {
            printf("%s ", word.c_str());
        } else if (wordIdx >= 3) {
            proc_name += word;
        }

        ++wordIdx;
    }

    printf("\n");
}

void dump_proc_mem(uint64_t pid) {

}

void seek_addr(uint64_t pid, const Value& val, bool should_suspend) {
    ProcMapInfo proc_map_info = process::parse_proc_maps(pid);
    FILE* mem_fd = process::open_mem_fd(pid);

    if (!process::attach(pid, should_suspend)) {
        printf("Failed to attach to the process\n");
        return;
    }

    switch (val.type) {
        case ValueType::UINT64:
            print_value_address(proc_map_info, mem_fd, val.data.uint64);
            break;
        case ValueType::INT64:
            print_value_address(proc_map_info, mem_fd, val.data.int64);
            break;
        case ValueType::UINT32:
            print_value_address(proc_map_info, mem_fd, val.data.uint32);
            break;
        case ValueType::INT32:
            print_value_address(proc_map_info, mem_fd, val.data.int32);
            break;
        case ValueType::UINT16:
            print_value_address(proc_map_info, mem_fd, val.data.uint16);
            break;
        case ValueType::INT16:
            print_value_address(proc_map_info, mem_fd, val.data.int16);
            break;
        case ValueType::UINT8:
            print_value_address(proc_map_info, mem_fd, val.data.uint8);
            break;
        case ValueType::INT8:
            print_value_address(proc_map_info, mem_fd, val.data.int8);
            break;
        case ValueType::STRING:
            print_byte_array_value_address(
                    proc_map_info,
                    mem_fd,
                    reinterpret_cast<const uint8_t*>(val.data.string->data()),
                    val.data.string->size()
            );
            break;
        default:
            assert(false);
    }

    fclose(mem_fd);

    if (should_suspend) {
        process::detach(pid);
    }
}

void write_val_at_addr(uint64_t pid, const Value& address, const Value& val) {
    FILE* mem_fd = process::open_mem_fd(pid);

    if (!process::attach(pid)) {
        printf("Failed to attach to the process\n");
        return;
    }

    if (fseeko(mem_fd, address.data.addr, SEEK_SET) != 0) {
        printf("Failed to seek to address %llx\n", address.data.addr);
        return;
    }

    switch (val.type) {
        case ValueType::UINT64:
            fwrite(&val.data.uint64, sizeof(uint8_t), sizeof(uint64_t), mem_fd);
            break;
        case ValueType::INT64:
            fwrite(&val.data.int64, sizeof(uint8_t), sizeof(int64_t), mem_fd);
            break;
        case ValueType::UINT32:
            fwrite(&val.data.uint32, sizeof(uint8_t), sizeof(uint32_t), mem_fd);
            break;
        case ValueType::INT32:
            fwrite(&val.data.int32, sizeof(uint8_t), sizeof(int32_t), mem_fd);
            break;
        case ValueType::UINT16:
            fwrite(&val.data.uint16, sizeof(uint8_t), sizeof(uint16_t), mem_fd);
            break;
        case ValueType::INT16:
            fwrite(&val.data.int16, sizeof(uint8_t), sizeof(int16_t), mem_fd);
            break;
        case ValueType::UINT8:
            fwrite(&val.data.uint8, sizeof(uint8_t), sizeof(uint8_t), mem_fd);
            break;
        case ValueType::INT8:
            fwrite(&val.data.int8, sizeof(uint8_t), sizeof(int8_t), mem_fd);
            break;
        case ValueType::STRING:
            fwrite(val.data.string->data(), sizeof(uint8_t), val.data.string->size(), mem_fd);
            break;
        default:
            assert(false);
    }

    fclose(mem_fd);

    process::detach(pid);
}

void dump_library_symbols(uint64_t pid, const Value& val) {
    ProcMapInfo proc_map_info = process::parse_proc_maps(pid);




    FILE* mem_fd = process::open_mem_fd(pid);



    fclose(mem_fd);
}

