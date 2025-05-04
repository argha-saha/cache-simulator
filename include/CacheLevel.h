#pragma once

#include <cstdint>
#include <memory>

#include "CacheConfig.h"
#include "CacheStatistics.h"
#include "CacheSet.h"
#include "Policies.h"

class CacheLevel {
public:
    /**
     * @brief Constructs a new CacheLevel instance.
     */
    CacheLevel(CacheConfig config, CacheLevel* next_level);

    /**
     * @brief Default destructor
     */
    virtual ~CacheLevel() = default;

    /**
     * @brief Copy constructor
     */
    CacheLevel(const CacheLevel&) = delete;

    /**
     * @brief Move constructor
     */
    CacheLevel(CacheLevel&&) = delete;

    /**
     * @brief Copy assignment
     */
    CacheLevel& operator=(const CacheLevel&) = delete;

    /**
     * @brief Move assignment
     */
    CacheLevel& operator=(CacheLevel&&) = delete;

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

    /**
     * @brief Sets the next cache level in the hierarchy.
     * @param level Pointer to the next CacheLevel instance, or nullptr if this is the last level.
     */
    void set_next_level(CacheLevel* level);

private:
    std::string name;
    uint64_t capacity;
    uint32_t block_size;
    uint32_t associativity;
    uint32_t num_sets;
    uint32_t index_bits;
    uint32_t block_offset_bits;

    CacheLevel* next_level;

    std::unique_ptr<WritePolicyInterface> write_policy;
    std::unique_ptr<AllocationPolicyInterface> allocation_policy;

    CacheStatistics statistics;
};