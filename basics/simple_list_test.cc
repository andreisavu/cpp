#include <gtest/gtest.h>
#include "simple_list.h"

TEST(SimpleListTest, PushFrontAndGet)
{
    SimpleList<int> list;
    list.pushFront(1);
    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.popFront(), 1);
    EXPECT_EQ(list.count(), 0);
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST(SimpleListTest, PushBack)
{
    SimpleList<int> list;
    list.pushBack(1);
    list.pushBack(2);

    EXPECT_TRUE(list.isSortedAscending());
    EXPECT_EQ(list.count(), 2);
    EXPECT_EQ(list.front(), 1);

    EXPECT_EQ(list.popFront(), 1);
    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.popFront(), 2);

    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST(SimpleListTest, Tail)
{
    SimpleList<int> list;
    list.pushBack(1);
    EXPECT_EQ(list.back(), 1);
    list.pushBack(2);
    EXPECT_EQ(list.back(), 2);
    list.pushBack(3);
    EXPECT_EQ(list.back(), 3);
    list.popFront();
    EXPECT_EQ(list.back(), 3);
    list.popFront();
    EXPECT_EQ(list.back(), 3);
    list.popFront();
    EXPECT_THROW(list.back(), std::out_of_range);
}

TEST(SimpleListTest, Remove)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.remove(1);
    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.popFront(), 2);
    EXPECT_TRUE(list.empty());
}

TEST(SimpleListTest, OutOfRange)
{
    SimpleList<int> list;
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST(SimpleListTest, Size)
{
    SimpleList<int> list;
    EXPECT_EQ(list.count(), 0);
}

TEST(SimpleListTest, MoveConstructorAndAssignmentOperator)
{
    SimpleList<int> list;
    list.pushFront(1);
    SimpleList<int> list2 = std::move(list);

    // After move, the original list is empty
    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.popFront(), std::out_of_range);
    EXPECT_THROW(list.front(), std::out_of_range);

    // The moved-to list is not empty
    EXPECT_EQ(list2.count(), 1);
    EXPECT_EQ(list2.popFront(), 1);
}

TEST(SimpleListTest, Iterator)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);

    int i = 3;
    for (auto &it : list)
    {
        EXPECT_EQ(it, i--);
    }
}

TEST(SimpleListTest, Contains)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);
    EXPECT_TRUE(list.contains(1));
    EXPECT_TRUE(list.contains(2));
    EXPECT_TRUE(list.contains(3));
    EXPECT_FALSE(list.contains(4));
}

TEST(SimpleListTest, Clear)
{
    SimpleList<int> list;
    list.pushFront(1);
    EXPECT_EQ(list.count(), 1);
    list.clear();
    EXPECT_TRUE(list.empty());
}

TEST(SimpleListTest, Reverse)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);
    EXPECT_EQ(list.count(), 3);
    list.reverse();
    EXPECT_EQ(list.popFront(), 1);
    EXPECT_EQ(list.popFront(), 2);
    EXPECT_EQ(list.popFront(), 3);
    EXPECT_TRUE(list.empty());
}

TEST(SimpleListTest, Sort)
{
    SimpleList<int> list;
    list.pushFront(3);
    list.pushFront(1);
    list.pushFront(2);
    list.sort();
    EXPECT_EQ(list.popFront(), 1);
    EXPECT_EQ(list.popFront(), 2);
    EXPECT_EQ(list.popFront(), 3);
}

TEST(SimpleListTest, Transform)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);

    list.transform([](int x)
                   { return x * 2; });

    EXPECT_EQ(list.popFront(), 6);
    EXPECT_EQ(list.popFront(), 4);
    EXPECT_EQ(list.popFront(), 2);
}

TEST(SimpleListTest, KeepIf)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);

    list.keepIf([](int x)
                 { return x % 2 == 0; });

    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.popFront(), 2);
}

TEST(SimpleListTest, SortedStateAfterOperations)
{
    SimpleList<int> list;
    EXPECT_TRUE(list.isSortedAscending());

    list.pushFront(3);
    EXPECT_TRUE(list.isSortedAscending());

    list.pushFront(2);
    list.pushFront(1);
    EXPECT_TRUE(list.isSortedAscending());

    list.pushFront(5);
    EXPECT_FALSE(list.isSortedAscending());
    list.transform([](int x)
                   { if (x == 5) { return 0; }
                     return x; });
    list.transform([](int x)
                   { return x * 2; });
    EXPECT_TRUE(list.isSortedAscending());

    list.keepIf([](int x)
                 { return x % 2 == 0; });
    EXPECT_TRUE(list.isSortedAscending());

    list.pushFront(10);
    EXPECT_FALSE(list.isSortedAscending());

    list.keepIf([](int x)
                 { return x != 10; });
    EXPECT_TRUE(list.isSortedAscending());

    list.pushFront(15);
    EXPECT_FALSE(list.isSortedAscending());

    // Popping the front element always makes the list unsorted
    // because we there is no way to know if the list is isSortedAscending after popping
    // the front element

    list.popFront();
    EXPECT_FALSE(list.isSortedAscending());

    list.clear();
    EXPECT_TRUE(list.isSortedAscending());
}

TEST(SimpleListTest, ReverseTwice)
{
    SimpleList<int> list;
    list.pushFront(3);
    list.pushFront(2);
    list.pushFront(1);
    EXPECT_TRUE(list.isSortedAscending());

    list.reverse();
    EXPECT_FALSE(list.isSortedAscending());

    list.reverse();
    EXPECT_TRUE(list.isSortedAscending());

    list.pushFront(4);
    EXPECT_FALSE(list.isSortedAscending());

    list.reverse();
    EXPECT_FALSE(list.isSortedAscending());
}

TEST(SimpleListTest, Count)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);
    list.pushBack(1);
    EXPECT_EQ(list.count(1), 2);
}

TEST(SimpleListTest, UniqueWithInsertSorted)
{
    SimpleList<int> list;
    list.insertSorted(1);
    list.insertSorted(2);
    list.insertSorted(3);
    list.insertSorted(2);
    list.insertSorted(1);
    list.insertSorted(4);

    EXPECT_TRUE(list.isSortedAscending());
    list.unique();

    EXPECT_EQ(list.count(), 4);
    EXPECT_EQ(list.popFront(), 1);
    EXPECT_EQ(list.popFront(), 2);
    EXPECT_EQ(list.popFront(), 3);
    EXPECT_EQ(list.popFront(), 4);
}

TEST(SimpleListTest, Merge)
{
    SimpleList<int> list;
    list.pushFront(1);
    list.pushFront(2);

    SimpleList<int> list2;
    list2.pushFront(3);
    list2.pushFront(4);

    list.merge(list2);

    EXPECT_EQ(list.count(), 4);
    EXPECT_EQ(list.popFront(), 2);
    EXPECT_EQ(list.popFront(), 1);
    EXPECT_EQ(list.popFront(), 4);
    EXPECT_EQ(list.popFront(), 3);

    EXPECT_TRUE(list2.empty());
}

TEST(SimpleListTest, MergeSorted)
{
    SimpleList<int> list;
    list.insertSorted(2);
    list.insertSorted(1);
    EXPECT_TRUE(list.isSortedAscending());

    SimpleList<int> list2;
    list2.insertSorted(4);
    list2.insertSorted(3);
    list2.insertSorted(5);
    EXPECT_TRUE(list2.isSortedAscending());

    list.merge(list2);
    EXPECT_TRUE(list.isSortedAscending());

    EXPECT_EQ(list.count(), 5);
    EXPECT_EQ(list.popFront(), 1);
    EXPECT_EQ(list.popFront(), 2);
    EXPECT_EQ(list.popFront(), 3);
    EXPECT_EQ(list.popFront(), 4);
    EXPECT_EQ(list.popFront(), 5);

    EXPECT_TRUE(list2.empty());
}