#pragma once

#include <cstdint>

struct MemoryAddress final {
    uint64_t address = 0;
    uint32_t block_offset = 0;
    uint64_t index = 0;
    uint64_t tag = 0;

    /**
     * @brief Constructs a MemoryAddress from a raw address.
     * @param addr The raw memory address.
     * @param block_size The size of a cache block in bytes.
     * @param num_sets The number of sets in the cache.
     */
    MemoryAddress(uint64_t addr, uint32_t block_size, uint32_t num_sets);

    // Default destructor
    ~MemoryAddress() = default;
};