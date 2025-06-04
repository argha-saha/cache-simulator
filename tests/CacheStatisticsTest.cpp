#include <gtest/gtest.h>

#include "CacheStatistics.h"

TEST(CacheStatisticsTest, TotalAccesses) {
    CacheStatistics stats;
    stats.read_hits = 10;
    stats.read_misses = 6;
    stats.write_hits = 8;
    stats.write_misses = 4;

    EXPECT_EQ(stats.total_accesses(), 28);
}

// TEST(CacheStatisticsTest, ReadHitRate) {
//     CacheStatistics stats;
//     stats.read_hits = 24;
//     stats.read_misses = 12;

//     EXPECT_DOUBLE_EQ(stats.read_hit_rate(), 24.0 / 36.0);
// }

// TEST(CacheStatisticsTest, WriteHitRate) {
//     CacheStatistics stats;
//     stats.write_hits = 16;
//     stats.write_misses = 16;

//     EXPECT_DOUBLE_EQ(stats.write_hit_rate(), 16.0 / 16.0);
// }