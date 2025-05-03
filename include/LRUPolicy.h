#pragma once

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

private:
    uint32_t associativity = 0;
    std::vector<uint32_t> lru_stack;
    uint64_t counter;
};