#include <gtest/gtest.h>
#include "skip_list.hpp"

// Basic insertion and find test
TEST(SkipListTest, InsertAndFind) {
    SkipList<int> list;
    list.insert(10);
    list.insert(20);
    list.insert(30);

    EXPECT_TRUE(list.find(10));
    EXPECT_TRUE(list.find(20));
    EXPECT_TRUE(list.find(30));
    EXPECT_FALSE(list.find(40));
}

// Erase elements
TEST(SkipListTest, Erase) {
    SkipList<int> list;
    list.insert(10);
    list.insert(20);
    list.insert(30);

    list.erase(20);
    EXPECT_FALSE(list.find(20));
    EXPECT_TRUE(list.find(10));
    EXPECT_TRUE(list.find(30));
}

// Size and empty
TEST(SkipListTest, SizeAndEmpty) {
    SkipList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);

    list.insert(5);
    list.insert(15);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 2);
}

// Operator []
TEST(SkipListTest, IndexAccess) {
    SkipList<int> list;
    list.insert(3);
    list.insert(1);
    list.insert(2);

    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);
}

// Clear
TEST(SkipListTest, Clear) {
    SkipList<int> list;
    list.insert(7);
    list.insert(8);
    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_FALSE(list.find(7));
}

// Equality operators
TEST(SkipListTest, Equality) {
    SkipList<int> list1;
    SkipList<int> list2;

    list1.insert(1);
    list1.insert(2);

    list2.insert(1);
    list2.insert(2);

    EXPECT_TRUE(list1 == list2);

    list2.insert(3);
    EXPECT_FALSE(list1 == list2);
    EXPECT_TRUE(list1 != list2);
}

// Swap
TEST(SkipListTest, Swap) {
    SkipList<int> a;
    SkipList<int> b;

    a.insert(1);
    a.insert(2);

    b.insert(10);
    b.insert(20);

    a.swap(b);

    EXPECT_TRUE(a.find(10));
    EXPECT_TRUE(b.find(1));
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(b.size(), 2);
}