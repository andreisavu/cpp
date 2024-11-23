#include <gtest/gtest.h>
#include "simple_list.h"

TEST(SimpleListTest, AddAndGet)
{
    SimpleList<int> list;
    list.add(1);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get_size(), 1);
}

TEST(SimpleListTest, OutOfRange)
{
    SimpleList<int> list;
    EXPECT_THROW(list.get(0), std::out_of_range);
}

TEST(SimpleListTest, Size)
{
    SimpleList<int> list;
    EXPECT_EQ(list.get_size(), 0);
}
