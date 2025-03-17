#include "gtest/gtest.h"
#include "lib/SPSCQueue.h"

TEST(SPSCQueueTest, PushPop)
{
    SPSCQueue<int> q(10);
    for (int i = 0; i < 10; ++i)
    {
        q.push(i);
    }

    for (int i = 0; i < 10; ++i)
    {
        int* j = q.front();
        EXPECT_EQ(i, *j);
        q.pop();
    }
}
