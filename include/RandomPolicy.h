#pragma once

#include <random>

#include "CacheSet.h"
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

    /**
     * @brief No action upon access.
     * @param set The cache set containing the block.
     * @param index The index of the accessed block within the set.
     */
    void on_access(const CacheSet& set, uint32_t index) override;

    /**
     * @brief No action upon fill.
     * @param set The cache set containing the block.
     * @param index The index of the accessed block within the set.
     */
    void on_fill(const CacheSet& set, uint32_t index) override;

    uint32_t get_victim_index(const CacheSet& set) override;

    /**
     * @brief Clones the Random policy instance.
     * @return A unique pointer to a new instance of the Random policy.
     */
    std::unique_ptr<EvictionPolicyInterface> clone() const override;

private:
    uint32_t associativity = 0;
    std::mt19937 rng;
    std::uniform_int_distribution<uint32_t> uni_dist;
};