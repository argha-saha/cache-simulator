#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "CacheConfig.h"
#include "CacheStatistics.h"
#include "CacheSet.h"
#include "MemoryAccess.h"
#include "MemoryAddress.h"
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
     * @brief Gets the name of this cache level.
     * @return Name of the cache level.
     */
    const std::string& get_name() const;

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

    /**
     * @brief Returns a pointer to the next cache level.
     */
    CacheLevel* get_next_level() const;

    /**
     * @brief Sets the next cache level in the hierarchy.
     * @param level Pointer to the next CacheLevel instance, or nullptr if this is the last level.
     */
    void set_next_level(CacheLevel* level);

    /**
     * @brief Returns a pointer to the memory accessor.
     */
    MemoryAccessInterface* get_memory_accessor() const;

    /**
     * @brief Sets the memory accessor for this cache level.
     * @param accessor Pointer to the MemoryAccessInterface instance to be used for memory accesses.
     */
    void set_memory_accessor(MemoryAccessInterface* accessor);

    /**
     * @brief Returns the statistics for this cache level.
     * @return Reference to the CacheStatistics object.
     */
    CacheStatistics& get_statistics();

private:
    void handle_miss(uint64_t address, bool is_write);

    std::string name;
    uint64_t capacity;
    uint32_t block_size;
    uint32_t associativity;
    uint32_t num_sets;
    uint32_t block_offset_bits;
    uint32_t index_bits;
    std::vector<CacheSet> sets;

    std::unique_ptr<WritePolicyInterface> write_policy;
    std::unique_ptr<AllocationPolicyInterface> allocation_policy;

    CacheLevel* next_level;
    MemoryAccessInterface* memory_accessor;

    CacheStatistics statistics;
};