#include "skip_list.hpp"
#include <gtest/gtest.h>

TEST(SkipListTest, InsertAndFind) {
    SkipList<int> list;
    list.insert(10);
    list.insert(20);
    list.insert(15);
    EXPECT_TRUE(list.find(10));
    EXPECT_TRUE(list.find(20));
    EXPECT_TRUE(list.find(15));
    EXPECT_FALSE(list.find(5));
}

TEST(SkipListTest, Erase) {
    SkipList<int> list;
    list.insert(30);
    list.insert(40);
    list.erase(30);
    EXPECT_FALSE(list.find(30));
    EXPECT_TRUE(list.find(40));
}

TEST(SkipListTest, EqualityOperators) {
    SkipList<int> a, b;
    a.insert(1); a.insert(2); a.insert(3);
    b.insert(1); b.insert(2); b.insert(3);
    EXPECT_TRUE(a == b);
    b.insert(4);
    EXPECT_TRUE(a != b);
}

TEST(SkipListTest, Iterators) {
    SkipList<int> list;
    list.insert(1);
    list.insert(2);
    list.insert(3);

    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it, ++expected) {
        EXPECT_EQ(*it, expected);
    }
}