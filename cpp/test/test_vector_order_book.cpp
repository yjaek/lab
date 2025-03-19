#include "gtest/gtest.h"

#include "lib/VectorOrderBook.h"

class VectorOrderBookTest : public testing::Test
{
protected:
    void SetUp() override
    {
        const auto& fills1 = book.add(1, "B", 1, 11);
        EXPECT_TRUE(fills1.empty());
        const auto& fills2 = book.add(2, "B", 2, 12);
        EXPECT_TRUE(fills2.empty());
        const auto& fills3 = book.add(3, "B", 3, 13);
        EXPECT_TRUE(fills3.empty());
        const auto& fills4 = book.add(4, "A", 4, 14);
        EXPECT_TRUE(fills4.empty());
        const auto& fills5 = book.add(5, "A", 5, 15);
        EXPECT_TRUE(fills5.empty());
        const auto& fills6 = book.add(6, "A", 6, 16);
    }

    VectorOrderBook book;
};

TEST_F(VectorOrderBookTest, CheckBids)
{
    const auto& bids = book.getBids();
    EXPECT_EQ(bids.size(), 3);
    auto it = bids.rbegin();
    EXPECT_EQ(it->getPrice(), 3);
    ++it;
    EXPECT_EQ(it->getPrice(), 2);
    ++it;
    EXPECT_EQ(it->getPrice(), 1);
}

TEST_F(VectorOrderBookTest, CheckAsks)
{
    const auto& asks = book.getAsks();
    EXPECT_EQ(asks.size(), 3);
    auto it = asks.rbegin();
    EXPECT_EQ(it->getPrice(), 4);
    ++it;
    EXPECT_EQ(it->getPrice(), 5);
    ++it;
    EXPECT_EQ(it->getPrice(), 6);
}

TEST_F(VectorOrderBookTest, AddBid)
{
    const auto& bids = book.getBids();
    EXPECT_EQ(bids.size(), 3);

    const auto& fills = book.add(7, "B", 3.5, 10);
    EXPECT_TRUE(fills.empty());
    EXPECT_EQ(bids.size(), 4);
    EXPECT_EQ(bids.rbegin()->getPrice(), 3.5);
}

TEST_F(VectorOrderBookTest, AddAsk)
{
    const auto& asks = book.getAsks();
    EXPECT_EQ(asks.size(), 3);

    const auto& fills = book.add(7, "A", 3.5, 10);
    EXPECT_TRUE(fills.empty());
    EXPECT_EQ(asks.size(), 4);
    EXPECT_EQ(asks.rbegin()->getPrice(), 3.5);
}

TEST_F(VectorOrderBookTest, CancelBid)
{
    const auto& bids = book.getBids();
    EXPECT_EQ(bids.size(), 3);
    EXPECT_EQ(bids.rbegin()->getPrice(), 3);
    EXPECT_FALSE(book.cancel(7));
    // Cancel best bid
    EXPECT_TRUE(book.cancel(3));
    EXPECT_EQ(bids.size(), 2);
    EXPECT_EQ(bids.rbegin()->getPrice(), 2);
    // Cancel best bid
    EXPECT_TRUE(book.cancel(2));
    EXPECT_EQ(bids.size(), 1);
    EXPECT_EQ(bids.rbegin()->getPrice(), 1);
    // Cancel best bid
    EXPECT_TRUE(book.cancel(1));
    EXPECT_EQ(bids.size(), 0);
}

TEST_F(VectorOrderBookTest, CancelAsk)
{
    const auto& bids = book.getAsks();
    EXPECT_EQ(bids.size(), 3);
    EXPECT_EQ(bids.rbegin()->getPrice(), 4);
    EXPECT_FALSE(book.cancel(7));
    // Cancel best ask
    EXPECT_TRUE(book.cancel(4));
    EXPECT_EQ(bids.size(), 2);
    EXPECT_EQ(bids.rbegin()->getPrice(), 5);
    // Cancel best ask
    EXPECT_TRUE(book.cancel(5));
    EXPECT_EQ(bids.size(), 1);
    EXPECT_EQ(bids.rbegin()->getPrice(), 6);
    // Cancel best ask
    EXPECT_TRUE(book.cancel(6));
    EXPECT_EQ(bids.size(), 0);
}

TEST_F(VectorOrderBookTest, MatchBid)
{
    const auto& bids = book.getBids();
    EXPECT_EQ(bids.size(), 3);

    const auto& orders = book.add(7, "A", 2, 20);
    EXPECT_EQ(orders.size(), 4);

    const Order& o0 = orders.at(0);
    EXPECT_EQ(o0.oid, 7);
    EXPECT_EQ(o0.price, 3);
    EXPECT_EQ(o0.size, 13);

    const Order& o1 = orders.at(1);
    EXPECT_EQ(o1.oid, 3);
    EXPECT_EQ(o1.price, 3);
    EXPECT_EQ(o1.size, 13);

    const Order& o2 = orders.at(2);
    EXPECT_EQ(o2.oid, 7);
    EXPECT_EQ(o2.price, 2);
    EXPECT_EQ(o2.size, 7);

    const Order& o3 = orders.at(3);
    EXPECT_EQ(o3.oid, 2);
    EXPECT_EQ(o3.price, 2);
    EXPECT_EQ(o3.size, 7);
}

TEST_F(VectorOrderBookTest, MatchAsk)
{
    const auto& asks = book.getAsks();
    EXPECT_EQ(asks.size(), 3);

    const auto& orders = book.add(7, "B", 5, 20);
    EXPECT_EQ(orders.size(), 4);

    const Order& o0 = orders.at(0);
    EXPECT_EQ(o0.oid, 7);
    EXPECT_EQ(o0.price, 4);
    EXPECT_EQ(o0.size, 14);

    const Order& o1 = orders.at(1);
    EXPECT_EQ(o1.oid, 4);
    EXPECT_EQ(o1.price, 4);
    EXPECT_EQ(o1.size, 14);

    const Order& o2 = orders.at(2);
    EXPECT_EQ(o2.oid, 7);
    EXPECT_EQ(o2.price, 5);
    EXPECT_EQ(o2.size, 6);

    const Order& o3 = orders.at(3);
    EXPECT_EQ(o3.oid, 5);
    EXPECT_EQ(o3.price, 5);
    EXPECT_EQ(o3.size, 6);
}