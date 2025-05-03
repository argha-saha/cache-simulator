#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include "Policies.h"

class LRUPolicy : public EvictionPolicyInterface {
public:
    /**
     * @brief Constructs a new LRU eviction policy.
     */
    LRUPolicy() = default;

    /** 
     * @brief Initializes the class member variables.
     * @param ways The associativity of the cache set.
     */
    void initialize(uint32_t ways);

    /**
     * @brief Updates the counter of the accessed block.
     * @param set The cache set containing the block.
     * @param index The index of the accessed block within the set.
     */
    void on_access(const CacheSet& set, uint32_t index) override;

    /**
     * @brief Treats a fill the same as an access.
     * @param set The cache set containing the block.
     * @param index The index of the filled block within the set.
     */
    void on_fill(const CacheSet& set, uint32_t index) override;

private:
    uint32_t associativity = 0;
    std::vector<uint32_t> lru_stack;
    uint64_t counter;
};