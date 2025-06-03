#include <gtest/gtest.h>

#include "CacheBlock.h"

TEST(CacheBlockTest, Constructor) {
    CacheBlock block;
    EXPECT_EQ(block.get_tag(), 0);
    EXPECT_FALSE(block.is_dirty());
    EXPECT_FALSE(block.is_valid());
}