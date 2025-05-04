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
};