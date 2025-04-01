#include "gtest/gtest.h"

#include "lib/LinearProbingHashSet.h"

class LinearProbingHashSetTest : public testing::Test
{
protected:
    void SetUp() override
    {
        EXPECT_TRUE(hs.insert(1));
        EXPECT_TRUE(hs.contains(1));
        EXPECT_TRUE(hs.insert(2));
        EXPECT_TRUE(hs.contains(2));
        EXPECT_TRUE(hs.insert(3));
        EXPECT_TRUE(hs.contains(3));
        EXPECT_TRUE(hs.contains(1));
        EXPECT_TRUE(hs.contains(2));
        EXPECT_TRUE(hs.contains(3));
        EXPECT_FALSE(hs.insert(3));
        EXPECT_FALSE(hs.contains(4));
        EXPECT_EQ(hs.size(), 3);
    }

    LinearProbingHashSet<int> hs{10};
};

TEST_F(LinearProbingHashSetTest, Insert)
{
    EXPECT_TRUE(hs.insert(4));
    EXPECT_TRUE(hs.contains(4));
    EXPECT_EQ(hs.size(), 4);
}

TEST_F(LinearProbingHashSetTest, Remove)
{
    EXPECT_FALSE(hs.remove(4));
    EXPECT_FALSE(hs.contains(4));
    EXPECT_EQ(hs.size(), 3);
    EXPECT_TRUE(hs.remove(1));
    EXPECT_FALSE(hs.contains(1));
    EXPECT_EQ(hs.size(), 2);
    EXPECT_TRUE(hs.remove(2));
    EXPECT_FALSE(hs.contains(2));
    EXPECT_EQ(hs.size(), 1);
    EXPECT_TRUE(hs.remove(3));
    EXPECT_FALSE(hs.contains(3));
    EXPECT_EQ(hs.size(), 0);
}

TEST_F(LinearProbingHashSetTest, Resize)
{
    hs.resize(20);
    EXPECT_EQ(hs.size(), 3);
    EXPECT_TRUE(hs.contains(1));
    EXPECT_TRUE(hs.contains(2));
    EXPECT_TRUE(hs.contains(3));
}