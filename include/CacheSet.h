#pragma once

#include <memory>
#include <vector>

#include "CacheBlock.h"
#include "Policies.h"

class CacheSet {
public:
    CacheSet(uint32_t associativity);

private:
    uint32_t associativity;
    std::vector<CacheBlock> blocks;
    std::unique_ptr<EvictionPolicyInterface> eviction_policy;
};