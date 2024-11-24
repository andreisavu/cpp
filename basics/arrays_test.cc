#include "gtest/gtest.h"

#include <algorithm>
#include <array>


TEST(Arrays, NativeArray)
{
    int arr[10] = {1, 2, 3, 4};
    EXPECT_EQ(std::size(arr), 10);
    arr[0] = 5;
    EXPECT_EQ(arr[0], 5);

    int sum = 0;
    for (auto v : arr)
    {
        sum += v;
    }
    EXPECT_EQ(sum, 14);
}

TEST(Arrays, StdArray)
{
    std::array<int, 10> arr = {1, 2, 3, 4};
    EXPECT_EQ(arr.size(), 10);

    arr.at(0) = 5;
    EXPECT_EQ(arr[0], 5);

    std::sort(arr.begin(), arr.end());

    int sum = 0;
    for (auto v : arr)
    {
        sum += v;
    }
    EXPECT_EQ(sum, 14);

    arr.fill(0);
    EXPECT_EQ(arr.size(), 10);
    EXPECT_EQ(arr.front(), 0);
    EXPECT_EQ(arr.back(), 0);
}
