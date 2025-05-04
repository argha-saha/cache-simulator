#pragma once

#include "Policies.h"

class WriteAllocatePolicy : public AllocationPolicyInterface {
public:
    bool should_allocate_on_miss(CacheLevel& level, uint64_t address) override;
};

class NoWriteAllocatePolicy : public AllocationPolicyInterface {
public:
    bool should_allocate_on_miss(CacheLevel& level, uint64_t address) override;
};