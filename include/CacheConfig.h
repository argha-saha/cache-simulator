#pragma once

#include <string>

#include "Policies.h"

/** 
 * @brief Configuration struct holding all parameters to define
 * the properties of a single cache level within the hierarchy.
 */
struct CacheConfig {
    // Cache identification (e.g., L1, L2, L3)
    std::string name = "";

    // Cache size properties in bytes
    uint64_t capacity = 0;
    uint32_t block_size = 0;

    /**
     * @brief Number of blocks in each set.
     * - Set to 1 for direct-mapped cache
     * - Set to n for n-way set associative cache
     * - Set to total number of blocks for fully associative cache
     */
    uint32_t associativity = 0;

    /** Cache policies */
    EvictionPolicyType eviction_policy = EvictionPolicyType::LRU;
    WritePolicyType write_policy = WritePolicyType::WRITE_BACK;
    AllocationPolicyType allocation_policy = AllocationPolicyType::WRITE_ALLOCATE;

    /**
     * @brief Constructs a CacheConfig with specified parameters.
     * @param name Cache name (e.g., "L1", "L2")
     * @param capacity Cache size in bytes
     * @param block_size Size of each cache block in bytes
     * @param associativity Number of blocks per set
     * @param eviction_policy Eviction policy type
     * @param write_policy Write policy type
     * @param allocation_policy Allocation policy type
     */
    CacheConfig(const std::string& name, uint64_t capacity, uint32_t block_size,
                uint32_t associativity, EvictionPolicyType eviction_policy,
                WritePolicyType write_policy, AllocationPolicyType allocation_policy)
        : name(name), capacity(capacity), block_size(block_size),
          associativity(associativity), eviction_policy(eviction_policy),
          write_policy(write_policy), allocation_policy(allocation_policy) {}
};