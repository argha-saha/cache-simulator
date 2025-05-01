#pragma once

#include <cstdint>

struct MemoryAddress {
    uint64_t address = 0;
    uint64_t tag = 0;
    uint64_t index = 0;
    uint32_t block_offset = 0;

    MemoryAddress(uint64_t addr, uint32_t block_size, uint32_t num_sets);
};