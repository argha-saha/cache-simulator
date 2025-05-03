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
     */
    void initialize(uint32_t ways);

    /**
     * @brief Updates the counter of the accessed block.
     */
    void on_access(const CacheSet& set, uint32_t index) override;

private:
    uint32_t associativity = 0;
    std::vector<uint32_t> lru_stack;
    uint64_t counter;
};