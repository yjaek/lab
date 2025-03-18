#pragma once

// MapOrderBook.h
// --------------
// Define an order book using maps for the bids and asks.

#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <unordered_map>

using OrderIdT = uint32_t;
using SizeT = uint16_t;
using PriceT = double;

enum class Side
{
    BID,
    ASK
};

struct Order
{
    OrderIdT oid;
    Side side;
    PriceT price;
    SizeT size;
};

class ListPriceLevel
{
    std::list<Order> orders;
    std::map<OrderIdT, std::list<Order>::iterator> orderIts;

public:
    std::list<Order>::iterator add(const Order& order)
    {
        auto it = orders.insert(orders.end(), order);
        orderIts[order.oid] = it;
        return it;
    }

    std::vector<Order> match(Order& order)
    {
        std::vector<Order> fills;
        auto it = orders.begin();
        while (order.size > 0 && it != orders.end())
        {
            const auto [fillOid, fillPrice, fillSize] = [&]() -> std::tuple<OrderIdT, PriceT, SizeT>
            {
                if (it->size > order.size)
                {
                    it->size -= order.size;
                    const SizeT size = order.size;
                    order.size = 0;
                    return {it->oid, it->price, size};
                }
                order.size -= it->size;
                const OrderIdT oid = it->oid;
                const PriceT price = it->price;
                const SizeT size = it->size;
                it->size = 0;
                orderIts.erase(it->oid);
                orders.erase(it);
                it = orders.begin();
                return {oid, price, size};
            }();
            fills.emplace_back(Order{order.oid, order.side, fillPrice, fillSize});
            fills.emplace_back(Order{fillOid, order.side, fillPrice, fillSize});
        }
        return fills;
    }

    void cancel(std::list<Order>::iterator& it)
    {
        orderIts.erase(it->oid);
        orders.erase(it);
    }

    bool empty() const { return orders.empty(); }

    bool invalid(const OrderIdT oid) const { return orderIts.find(oid) == orderIts.end(); }

    size_t size() const { return orders.size(); }

    const std::list<Order>& getOrders() const { return orders; }
};

class MapOrderBook
{
    std::map<PriceT, ListPriceLevel, std::greater<PriceT>> bids;
    std::map<PriceT, ListPriceLevel, std::less<PriceT>> asks;
    std::unordered_map<OrderIdT, std::list<Order>::iterator> orders;

public:
    std::vector<Order> add(const OrderIdT oid, const std::string& side, const PriceT price, const SizeT size)
    {
        if (orders.find(oid) != orders.end())
        {
            throw std::runtime_error("Duplicate order id, " + std::to_string(oid));
        }

        if (side == "B")
        {
            return addBid(oid, price, size);
        }
        else if (side == "A")
        {
            return addAsk(oid, price, size);
        }
        else
        {
            throw std::runtime_error("Unknown side, " + side);
        }
    }

    bool cancel(const OrderIdT oid)
    {
        auto it = orders.find(oid);
        if (it == orders.end())
        {
            return false;
        }
        auto orderIt = it->second;
        orders.erase(it);
        if (orderIt->side == Side::BID)
        {
            auto levelIt = bids.find(orderIt->price);
            ListPriceLevel& level = levelIt->second;
            level.cancel(orderIt);
            if (level.empty())
            {
                bids.erase(levelIt);
            }
        }
        else
        {
            auto levelIt = asks.find(orderIt->price);
            ListPriceLevel& level = levelIt->second;
            level.cancel(orderIt);
            if (level.empty())
            {
                asks.erase(levelIt);
            }
        }
        return true;
    }

    const auto& getBids() const { return bids; }

    const auto& getAsks() const { return asks; }

private:
    std::vector<Order> addBid(const OrderIdT oid, const PriceT price, const SizeT size)
    {
        Order order{oid, Side::BID, price, size};
        auto askIt = asks.begin();
        std::vector<Order> fills;
        while (order.size > 0 && askIt != asks.end() && askIt->first <= price)
        {
            auto levelFills = askIt->second.match(order);
            for (size_t i = 1; i < levelFills.size(); i += 2)
            {
                const Order& fill = levelFills[i];
                if (askIt->second.invalid(fill.oid))
                {
                    orders.erase(fill.oid);
                }
            }
            fills.insert(
                fills.end(), std::make_move_iterator(levelFills.begin()), std::make_move_iterator(levelFills.end()));
            if (askIt->second.size() == 0)
            {
                asks.erase(askIt);
                askIt = asks.begin();
            }
        }

        if (order.size > 0)
        {
            orders[oid] = bids[price].add(order);
        }

        return fills;
    }

    std::vector<Order> addAsk(const OrderIdT oid, const PriceT price, const SizeT size)
    {
        Order order{oid, Side::ASK, price, size};
        auto bidIt = bids.begin();
        std::vector<Order> fills;
        while (order.size > 0 && bidIt != bids.end() && bidIt->first >= price)
        {
            auto levelFills = bidIt->second.match(order);
            for (size_t i = 1; i < levelFills.size(); i += 2)
            {
                const Order& fill = levelFills[i];
                if (bidIt->second.invalid(fill.oid))
                {
                    orders.erase(fill.oid);
                }
            }
            fills.insert(
                fills.end(), std::make_move_iterator(levelFills.begin()), std::make_move_iterator(levelFills.end()));
            if (bidIt->second.size() == 0)
            {
                bids.erase(bidIt);
                bidIt = bids.begin();
            }
        }

        if (order.size > 0)
        {
            orders[oid] = asks[price].add(order);
        }

        return fills;
    }
};
