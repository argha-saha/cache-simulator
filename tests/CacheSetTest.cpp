#include <gtest/gtest.h>

#include "CacheSet.h"
#include "LRUPolicy.h"

TEST(CacheSetTest, Constructor) {
    CacheSet set0(2, LRUPolicy());
    EXPECT_EQ(set0.get_associativity(), 2);
}