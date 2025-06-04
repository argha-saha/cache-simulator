#pragma once

#include "Policies.h"

/**
 * @brief Implements the write-allocate policy.
 */
class WriteAllocatePolicy final : public AllocationPolicyInterface {
public:
    bool should_allocate_on_miss(CacheLevel& level, uint64_t address) override;
};

/**
 * @brief Implements the no-write-allocate policy.
 */
class NoWriteAllocatePolicy final : public AllocationPolicyInterface {
public:
    bool should_allocate_on_miss(CacheLevel& level, uint64_t address) override;
};