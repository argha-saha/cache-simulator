#include <gtest/gtest.h>
#include <optional>

#include "AccessType.h"
#include "CacheConfig.h"
#include "CacheLevel.h"
#include "CacheStatistics.h"
#include "MemorySystem.h"
#include "Policies.h"

AccessType R = AccessType::R;
AccessType READ = AccessType::READ;
AccessType W = AccessType::W;
AccessType WRITE = AccessType::WRITE;

TEST(CacheTest, Constructor) {
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

TEST(CacheTest, ReadMiss) {
    MemorySystem simulator;

    CacheConfig l1_config(
        "L1",
        64 * 1024, // 64 KB
        64,      // 64 bytes block size
        4,    // 4-way set associative
        EvictionPolicyType::LRU,
        WritePolicyType::WRITE_BACK,
        AllocationPolicyType::WRITE_ALLOCATE
    );

    std::vector<CacheConfig> hierarchy_configs = {l1_config};
    simulator.configure_cache(hierarchy_configs);
    
    simulator.execute_access(R, 0x1000);
    simulator.execute_access(W, 0x1004);

    auto stats = simulator.get_cache_level_statistics("L1");
    ASSERT_TRUE(stats.has_value());
    EXPECT_EQ(stats->get_read_hits(), 0);
    EXPECT_EQ(stats->get_read_misses(), 1);
    EXPECT_EQ(stats->get_write_hits(), 1);
    EXPECT_EQ(stats->get_write_misses(), 0);
}