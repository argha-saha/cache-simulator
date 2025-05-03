#pragma once

#include <random>

#include "CacheSet.h"
#include "CacheBlock.h"
#include "Policies.h"

class RandomPolicy : public EvictionPolicyInterface {
public:
    /**
     * @brief Constructs a new Random eviction policy.
     */
    RandomPolicy();

    /**
     * @brief Initializes the class member variables.
     * @param ways The associativity of the cache set.
     */
    void initialize(uint32_t ways) override;

private:
    uint32_t associativity = 0;
    std::mt19937 rng;
    std::uniform_int_distribution<uint32_t> uni_dist;
};