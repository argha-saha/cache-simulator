#pragma once

#include <string>

struct CacheConfig {
    std::string name;
    uint64_t capacity;      // In bytes
    uint32_t block_size;    // In bytes
    uint32_t associativity; // 1 for direct-mapped, >1 for n-way set associative
};