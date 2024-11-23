#include <gtest/gtest.h>
#include "simple_list.h"

TEST(SimpleListTest, PushFrontAndGet)
{
    SimpleList<int> list;
    list.push_front(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.head(), 1);
    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.head(), std::out_of_range);
}

TEST(SimpleListTest, OutOfRange)
{
    SimpleList<int> list;
    EXPECT_THROW(list.head(), std::out_of_range);
}

TEST(SimpleListTest, Size)
{
    SimpleList<int> list;
    EXPECT_EQ(list.size(), 0);
}

TEST(SimpleListTest, MoveConstructorAndAssignmentOperator)
{
    SimpleList<int> list;
    list.push_front(1);
    SimpleList<int> list2 = std::move(list);

    // After move, the original list is empty
    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.pop_front(), std::out_of_range);
    EXPECT_THROW(list.head(), std::out_of_range);

    // The moved-to list is not empty
    EXPECT_EQ(list2.size(), 1);
    EXPECT_EQ(list2.pop_front(), 1);
}

TEST(SimpleListTest, Iterator)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    int i = 3;
    for (auto &it : list)
    {
        EXPECT_EQ(it, i--);
    }
}

TEST(SimpleListTest, Contains)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    EXPECT_TRUE(list.contains(1));
    EXPECT_TRUE(list.contains(2));
    EXPECT_TRUE(list.contains(3));
    EXPECT_FALSE(list.contains(4));
}

TEST(SimpleListTest, Clear)
{
    SimpleList<int> list;
    list.push_front(1);
    EXPECT_EQ(list.size(), 1);
    list.clear();
    EXPECT_TRUE(list.empty());
}