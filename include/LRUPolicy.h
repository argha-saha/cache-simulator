#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include "CacheSet.h"
#include "CacheBlock.h"
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
    void initialize(uint32_t ways) override;

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

    /**
     * @brief Returns the index of the victim block to be evicted.
     * @param set The cache set from which to select the victim.
     * @return The index of the victim block.
     */
    uint32_t get_victim_index(const CacheSet& set) override;

    /**
     * @brief Clones the LRU policy instance.
     * @return A unique pointer to a new instance of the LRU policy.
     */
    std::unique_ptr<EvictionPolicyInterface> clone() const override;

private:
    uint32_t associativity = 0;
    std::vector<uint32_t> lru_stack;
    uint64_t counter;
};