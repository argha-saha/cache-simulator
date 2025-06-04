#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "CacheConfig.h"
#include "CacheStatistics.h"
#include "CacheSet.h"
#include "MemoryAccess.h"
#include "MemoryAddress.h"
#include "Policies.h"

class CacheLevel final {
public:
    /**
     * @brief Constructs a new CacheLevel instance.
     * @param config Configuration parameters for this cache level.
     * @param next_level Pointer to the next cache level in the hierarchy (nullptr if none).
     */
    CacheLevel(CacheConfig config, CacheLevel* next_level);

    // Default destructor
    ~CacheLevel() = default;

    // Prevent copying and moving
    CacheLevel(const CacheLevel&) = delete;
    CacheLevel(CacheLevel&&) = delete;
    CacheLevel& operator=(const CacheLevel&) = delete;
    CacheLevel& operator=(CacheLevel&&) = delete;

    /**
     * @brief Gets a reference to a specific CacheSet within this cache level.
     * @param index The index of the set to retrieve.
     * @return Reference to the CacheSet object.
     */
    CacheSet& get_set(uint32_t index);

    /**
     * @brief Simulates a read operation on this cache level.
     * @param address The memory address to read from.
     * @return True on a read hit, false on a read miss.
     */
    bool read(uint64_t address);

    /**
     * @brief Simulates a write operation on this cache level.
     * @param address The memory address to write to.
     * @return True on a write hit, false on a write miss.
     */
    bool write(uint64_t address);

    /**
     * @brief Fills a cache block with data from the next memory level.
     * @param address The starting address of the block to be filled into the cache.
     */
    void fill(uint64_t address);

    // Getters
    const std::string& get_name() const { return name; }
    const uint32_t get_capacity() const { return capacity; }
    const uint32_t get_block_size() const { return block_size; }
    const uint32_t get_num_sets() const { return num_sets; }
    CacheLevel* get_next_level() const { return next_level; }
    MemoryAccessInterface* get_memory_accessor() const { return memory_accessor; }
    CacheStatistics& get_statistics() { return statistics; }

    // Setters
    void set_next_level(CacheLevel* level) { next_level = level; }
    void set_memory_accessor(MemoryAccessInterface* accessor) { memory_accessor = accessor; }

private:
    /**
     * @brief Handles a cache miss.
     * @param address The memory address that caused the miss.
     * @param is_write True if this is a write operation, false for read.
     */
    void handle_miss(uint64_t address, bool is_write);

    /**
     * @brief Evicts a block from the cache and replaces it with a new block.
     * @param address The memory address of the block to be replaced.
     * @param set_index The index of the set from which to evict the block.
     */
    void evict_and_replace(const MemoryAddress& address, uint32_t set_index);

    std::string name;
    uint64_t capacity;
    uint32_t block_size;
    uint32_t associativity;
    uint32_t num_sets;
    uint32_t index_bits;
    uint32_t block_offset_bits;
    std::vector<CacheSet> sets;

    std::unique_ptr<WritePolicyInterface> write_policy;
    std::unique_ptr<AllocationPolicyInterface> allocation_policy;

    CacheLevel* next_level;
    MemoryAccessInterface* memory_accessor;

    CacheStatistics statistics;
};