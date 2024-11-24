#include <gtest/gtest.h>
#include "simple_list.h"

TEST(SimpleListTest, PushFrontAndGet)
{
    SimpleList<int> list;
    list.push_front(1);
    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.count(), 0);
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST(SimpleListTest, PushBack)
{
    SimpleList<int> list;
    list.push_back(1);
    list.push_back(2);

    EXPECT_TRUE(list.sorted_ascending());
    EXPECT_EQ(list.count(), 2);
    EXPECT_EQ(list.front(), 1);

    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.pop_front(), 2);

    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST(SimpleListTest, Tail)
{
    SimpleList<int> list;
    list.push_back(1);
    EXPECT_EQ(list.back(), 1);
    list.push_back(2);
    EXPECT_EQ(list.back(), 2);
    list.push_back(3);
    EXPECT_EQ(list.back(), 3);
    list.pop_front();
    EXPECT_EQ(list.back(), 3);
    list.pop_front();
    EXPECT_EQ(list.back(), 3);
    list.pop_front();
    EXPECT_THROW(list.back(), std::out_of_range);
}

TEST(SimpleListTest, Remove)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.remove(1);
    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.pop_front(), 2);
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
    list.push_front(1);
    SimpleList<int> list2 = std::move(list);

    // After move, the original list is empty
    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.pop_front(), std::out_of_range);
    EXPECT_THROW(list.front(), std::out_of_range);

    // The moved-to list is not empty
    EXPECT_EQ(list2.count(), 1);
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
    EXPECT_EQ(list.count(), 1);
    list.clear();
    EXPECT_TRUE(list.empty());
}

TEST(SimpleListTest, Reverse)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    EXPECT_EQ(list.count(), 3);
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

TEST(SimpleListTest, KeepIf)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    list.keep_if([](int x)
                 { return x % 2 == 0; });

    EXPECT_EQ(list.count(), 1);
    EXPECT_EQ(list.pop_front(), 2);
}

TEST(SimpleListTest, SortedStateAfterOperations)
{
    SimpleList<int> list;
    EXPECT_TRUE(list.sorted_ascending());

    list.push_front(3);
    EXPECT_TRUE(list.sorted_ascending());

    list.push_front(2);
    list.push_front(1);
    EXPECT_TRUE(list.sorted_ascending());

    list.push_front(5);
    EXPECT_FALSE(list.sorted_ascending());
    list.transform([](int x)
                   { if (x == 5) { return 0; }
                     return x; });
    list.transform([](int x)
                   { return x * 2; });
    EXPECT_TRUE(list.sorted_ascending());

    list.keep_if([](int x)
                 { return x % 2 == 0; });
    EXPECT_TRUE(list.sorted_ascending());

    list.push_front(10);
    EXPECT_FALSE(list.sorted_ascending());

    list.keep_if([](int x)
                 { return x != 10; });
    EXPECT_TRUE(list.sorted_ascending());

    list.push_front(15);
    EXPECT_FALSE(list.sorted_ascending());

    // Popping the front element always makes the list unsorted
    // because we there is no way to know if the list is sorted_ascending after popping
    // the front element

    list.pop_front();
    EXPECT_FALSE(list.sorted_ascending());

    list.clear();
    EXPECT_TRUE(list.sorted_ascending());
}

TEST(SimpleListTest, ReverseTwice)
{
    SimpleList<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    EXPECT_TRUE(list.sorted_ascending());

    list.reverse();
    EXPECT_FALSE(list.sorted_ascending());

    list.reverse();
    EXPECT_TRUE(list.sorted_ascending());

    list.push_front(4);
    EXPECT_FALSE(list.sorted_ascending());

    list.reverse();
    EXPECT_FALSE(list.sorted_ascending());
}

TEST(SimpleListTest, Count)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.push_back(1);
    EXPECT_EQ(list.count(1), 2);
}

TEST(SimpleListTest, UniqueWithInsertSorted)
{
    SimpleList<int> list;
    list.insert_sorted(1);
    list.insert_sorted(2);
    list.insert_sorted(3);
    list.insert_sorted(2);
    list.insert_sorted(1);
    list.insert_sorted(4);

    EXPECT_TRUE(list.sorted_ascending());
    list.unique();

    EXPECT_EQ(list.count(), 4);
    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.pop_front(), 2);
    EXPECT_EQ(list.pop_front(), 3);
    EXPECT_EQ(list.pop_front(), 4);
}

TEST(SimpleListTest, Merge)
{
    SimpleList<int> list;
    list.push_front(1);
    list.push_front(2);

    SimpleList<int> list2;
    list2.push_front(3);
    list2.push_front(4);

    list.merge(list2);

    EXPECT_EQ(list.count(), 4);
    EXPECT_EQ(list.pop_front(), 2);
    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.pop_front(), 4);
    EXPECT_EQ(list.pop_front(), 3);

    EXPECT_TRUE(list2.empty());
}

TEST(SimpleListTest, MergeSorted)
{
    SimpleList<int> list;
    list.insert_sorted(2);
    list.insert_sorted(1);
    EXPECT_TRUE(list.sorted_ascending());

    SimpleList<int> list2;
    list2.insert_sorted(4);
    list2.insert_sorted(3);
    list2.insert_sorted(5);
    EXPECT_TRUE(list2.sorted_ascending());

    list.merge(list2);
    EXPECT_TRUE(list.sorted_ascending());

    EXPECT_EQ(list.count(), 5);
    EXPECT_EQ(list.pop_front(), 1);
    EXPECT_EQ(list.pop_front(), 2);
    EXPECT_EQ(list.pop_front(), 3);
    EXPECT_EQ(list.pop_front(), 4);
    EXPECT_EQ(list.pop_front(), 5);

    EXPECT_TRUE(list2.empty());
}