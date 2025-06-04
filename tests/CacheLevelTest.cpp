#include <gtest/gtest.h>

#include "CacheLevel.h"
#include "Policies.h"

TEST(CacheLevelTest, Constructor) {
    CacheConfig config;
    config.name = "L1";
    config.capacity = 1024; // 1KB
    config.block_size = 64; // 64B
    config.associativity = 4; // 4-way set associative
    config.eviction_policy = EvictionPolicyType::LRU;
    config.write_policy = WritePolicyType::WRITE_BACK;
    config.allocation_policy = AllocationPolicyType::WRITE_ALLOCATE;

    CacheLevel cache_level(config, nullptr);
    EXPECT_EQ(cache_level.get_capacity(), 1024);
    EXPECT_EQ(cache_level.get_block_size(), 64);
    EXPECT_EQ(cache_level.get_num_sets(), 4);
}