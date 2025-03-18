#include "gtest/gtest.h"
#include "lib/MapOrderBook.h"

class MapOrderBookTest : public testing::Test
{
protected:
    void SetUp() override
    {
        book.add(1, "B", 1, 11);
        book.add(2, "B", 2, 12);
        book.add(3, "B", 3, 13);
        book.add(4, "A", 4, 14);
        book.add(5, "A", 5, 15);
        book.add(6, "A", 6, 16);
    }

    MapOrderBook book;
};

TEST_F(MapOrderBookTest, Bid)
{
    const auto& bids = book.getBids();
    EXPECT_EQ(bids.size(), 3);
    auto it = bids.begin();
    EXPECT_EQ(it->first, 3);
    ++it;
    EXPECT_EQ(it->first, 2);
    ++it;
    EXPECT_EQ(it->first, 1);
}

TEST_F(MapOrderBookTest, Ask)
{
    const auto& asks = book.getAsks();
    EXPECT_EQ(asks.size(), 3);
    auto it = asks.begin();
    EXPECT_EQ(it->first, 4);
    ++it;
    EXPECT_EQ(it->first, 5);
    ++it;
    EXPECT_EQ(it->first, 6);
}

TEST_F(MapOrderBookTest, CancelBid)
{
    const auto& bids = book.getBids();
    EXPECT_EQ(bids.size(), 3);
    EXPECT_FALSE(book.cancel(7));
    // Cancel best bid
    EXPECT_TRUE(book.cancel(3));
    EXPECT_EQ(bids.size(), 2);
    EXPECT_EQ(bids.begin()->first, 2);
    // Cancel best bid
    EXPECT_TRUE(book.cancel(2));
    EXPECT_EQ(bids.size(), 1);
    EXPECT_EQ(bids.begin()->first, 1);
    // Cancel best bid
    EXPECT_TRUE(book.cancel(1));
    EXPECT_EQ(bids.size(), 0);
}

TEST_F(MapOrderBookTest, CancelAsk)
{
    const auto& bids = book.getAsks();
    EXPECT_EQ(bids.size(), 3);
    EXPECT_FALSE(book.cancel(7));
    // Cancel best ask
    EXPECT_TRUE(book.cancel(4));
    EXPECT_EQ(bids.size(), 2);
    EXPECT_EQ(bids.begin()->first, 5);
    // Cancel best ask
    EXPECT_TRUE(book.cancel(5));
    EXPECT_EQ(bids.size(), 1);
    EXPECT_EQ(bids.begin()->first, 6);
    // Cancel best ask
    EXPECT_TRUE(book.cancel(6));
    EXPECT_EQ(bids.size(), 0);
}

TEST_F(MapOrderBookTest, MatchBid)
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

TEST_F(MapOrderBookTest, MatchAsk)
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