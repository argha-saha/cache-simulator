#include <gtest/gtest.h>

#include "CacheSet.h"
#include "LRUPolicy.h"

TEST(CacheSetTest, Constructor) {
    CacheSet set0(2, LRUPolicy());
    EXPECT_EQ(set0.get_associativity(), 2);
}

TEST(CacheSetTest, FindBlock) {
    CacheSet set0(2, LRUPolicy());

    CacheBlock& block0 = set0.get_block(0);
    block0.set_tag(0x1234);
    block0.set_valid(true);

    CacheBlock& block1 = set0.get_block(1);
    block1.set_tag(0x5678);
    block1.set_valid(true);

    EXPECT_EQ(set0.find_block(0x1234).value(), 0);
    EXPECT_EQ(set0.find_block(0x5678).value(), 1);
    EXPECT_FALSE(set0.find_block(0xABCD).has_value());
}

TEST(CacheSetTest, GetVictimIndexLRU) {
    CacheSet set0(2, LRUPolicy());

    CacheBlock& block0 = set0.get_block(0);
    block0.set_tag(0x1234);
    block0.set_valid(true);

    CacheBlock& block1 = set0.get_block(1);
    block1.set_tag(0x5678);
    block1.set_valid(true);

    // Both blocks are valid so block0 should be the victim
    EXPECT_EQ(set0.get_victim_index(), 0);

    // Access block1 to update LRU state
    set0.access_block(1);

    // Block0 should now be the least recently used
    EXPECT_EQ(set0.get_victim_index(), 0);
}