#include "CacheConfig.h"
#include "Policies.h"
#include "CacheLevel.h"
#include <iostream>

int main() {
    CacheConfig config;
    config.name = "L1";
    config.capacity = 1024; // 1KB
    config.block_size = 64; // 64B
    config.associativity = 4; // 4-way set associative
    config.eviction_policy = EvictionPolicyType::LRU;
    config.write_policy = WritePolicyType::WRITE_BACK;
    config.allocation_policy = AllocationPolicyType::WRITE_ALLOCATE;

    CacheLevel cache_level(config, nullptr);
    
    return 0;
}