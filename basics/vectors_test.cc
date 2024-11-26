#include "gtest/gtest.h"

#include <algorithm>
#include <vector>

TEST(Vectors, Basic)
{
    std::vector<int> vec = {1, 2, 3, 4};
    EXPECT_EQ(vec.size(), 4);

    vec.resize(10);
    EXPECT_EQ(vec.capacity(), 10);

    vec.at(0) = 5;
    EXPECT_EQ(vec[0], 5);
}
