#include <gtest/gtest.h>

#include "Utils.h"

TEST(UtilsTest, IsPowerOfTwo) {
    EXPECT_TRUE(is_power_of_two(1));
    EXPECT_TRUE(is_power_of_two(2));
    EXPECT_TRUE(is_power_of_two(4));
    EXPECT_TRUE(is_power_of_two(8));
    EXPECT_TRUE(is_power_of_two(16));

    EXPECT_FALSE(is_power_of_two(3));
    EXPECT_FALSE(is_power_of_two(5));
    EXPECT_FALSE(is_power_of_two(6));
    EXPECT_FALSE(is_power_of_two(7));
    EXPECT_FALSE(is_power_of_two(9));
}

TEST(UtilsTest, Log2) {
    EXPECT_EQ(log2(1), 0);
    EXPECT_EQ(log2(2), 1);
    EXPECT_EQ(log2(4), 2);
    EXPECT_EQ(log2(8), 3);
    EXPECT_EQ(log2(16), 4);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}