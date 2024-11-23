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

TEST(SimpleListTest, Reverse)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    EXPECT_EQ(list.size(), 3);
    list.reverse();
    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.pop_front(), 2);
    EXPECT_EQ(list.pop_front(), 3);
    EXPECT_TRUE(list.empty());
}

TEST(SimpleListTest, Sort)
{
    SimpleList<int> list;
    list.push_front(3);
    list.push_front(1);
    list.push_front(2);
    list.sort();
    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.pop_front(), 2);
    EXPECT_EQ(list.pop_front(), 3);
}

TEST(SimpleListTest, Transform)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    list.transform([](int x)
                   { return x * 2; });

    EXPECT_EQ(list.pop_front(), 6);
    EXPECT_EQ(list.pop_front(), 4);
    EXPECT_EQ(list.pop_front(), 2);
}

TEST(SimpleListTest, Filter)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    list.filter([](int x)
                { return x % 2 == 0; });

    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.pop_front(), 2);
}

TEST(SimpleListTest, SortedStateAfterOperations)
{
    SimpleList<int> list;
    EXPECT_TRUE(list.sorted());

    list.push_front(3);
    EXPECT_TRUE(list.sorted());

    list.push_front(2);
    list.push_front(1);
    EXPECT_TRUE(list.sorted());

    list.push_front(5);
    EXPECT_FALSE(list.sorted());
    list.transform([](int x)
                   { if (x == 5) { return 0; }
                     return x; });
    list.transform([](int x)
                   { return x * 2; });
    EXPECT_TRUE(list.sorted());

    list.filter([](int x)
                { return x % 2 == 0; });
    EXPECT_TRUE(list.sorted());

    list.push_front(10);
    EXPECT_FALSE(list.sorted());

    list.filter([](int x)
                { return x != 10; });
    EXPECT_TRUE(list.sorted());

    list.push_front(15);
    EXPECT_FALSE(list.sorted());

    // Popping the front element always makes the list unsorted
    // because we there is no way to know if the list was sorted before popping
    list.pop_front();
    EXPECT_FALSE(list.sorted());

    list.clear();
    EXPECT_TRUE(list.sorted());
}

TEST(SimpleListTest, ReverseTwice)
{
    SimpleList<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    EXPECT_TRUE(list.sorted());

    list.reverse();
    EXPECT_FALSE(list.sorted());

    list.reverse();
    EXPECT_TRUE(list.sorted());

    list.push_front(4);
    EXPECT_FALSE(list.sorted());

    list.reverse();
    EXPECT_FALSE(list.sorted());
}
