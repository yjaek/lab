#pragma once

// VectorOrderMap.h
// ----------------
// Define an order book using vectors for the bids and asks.

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "lib/Order.h"

class VectorPriceLevel
{
    PriceT price;
    std::vector<Order> orders;

public:
    VectorPriceLevel(const PriceT price) : price(price) {}

    inline PriceT getPrice() const { return price; }

    std::vector<Order>::iterator add(const Order& order) { return orders.insert(orders.end(), order); }

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
                ++it;
                return {oid, price, size};
            }();
            fills.emplace_back(Order{order.oid, order.side, fillPrice, fillSize});
            fills.emplace_back(Order{fillOid, order.side, fillPrice, fillSize});
        }
        if (orders.begin() != it)
        {
            orders.erase(orders.begin(), it);
        }
        return fills;
    }

    void cancel(std::vector<Order>::iterator& it) { orders.erase(it); }

    bool empty() const { return orders.empty(); }

    bool invalid(const OrderIdT oid) const
    {
        return std::find_if(orders.begin(), orders.end(), [&](const Order& o) { return o.oid == oid; }) == orders.end();
    }

    size_t size() const { return orders.size(); }

    const std::vector<Order>& getOrders() const { return orders; }
};

class VectorOrderBook
{
    std::vector<VectorPriceLevel> bids;
    std::vector<VectorPriceLevel> asks;
    std::unordered_map<OrderIdT, std::vector<Order>::iterator> orders;

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
        auto& priceLevels = orderIt->side == Side::BID ? bids : asks;
        auto levelIt = std::find_if(priceLevels.begin(),
                                    priceLevels.end(),
                                    [&](const VectorPriceLevel& l) { return l.getPrice() == orderIt->price; });
        levelIt->cancel(orderIt);
        if (levelIt->empty())
        {
            priceLevels.erase(levelIt);
        }
        return true;
    }

    const auto& getBids() const { return bids; }

    const auto& getAsks() const { return asks; }

private:
    std::vector<Order> addBid(const OrderIdT oid, const PriceT price, const SizeT size)
    {
        Order order{oid, Side::BID, price, size};
        auto levelIt = asks.rbegin();
        std::vector<Order> fills;
        while (order.size > 0 && levelIt != asks.rend() && levelIt->getPrice() <= price)
        {
            auto levelFills = levelIt->match(order);
            for (size_t i = 1; i < levelFills.size(); i += 2)
            {
                const Order& fill = levelFills[i];
                if (levelIt->invalid(fill.oid))
                {
                    orders.erase(fill.oid);
                }
            }
            fills.insert(
                fills.end(), std::make_move_iterator(levelFills.begin()), std::make_move_iterator(levelFills.end()));
            if (levelIt->empty())
            {
                asks.erase(std::next(levelIt).base());
                levelIt = asks.rbegin();
            }
        }

        if (order.size > 0)
        {
            auto rit = bids.rbegin();
            while (rit != bids.rend() && rit->getPrice() > price)
            {
                ++rit;
            }
            if (rit == bids.rend() || rit->getPrice() < price)
            {
                auto it = bids.emplace(rit.base(), price);
                orders[oid] = it->add(order);
            }
            else
            {
                orders[oid] = rit->add(order);
            }
        }

        return fills;
    }

    std::vector<Order> addAsk(const OrderIdT oid, const PriceT price, const SizeT size)
    {
        Order order{oid, Side::ASK, price, size};
        auto levelIt = bids.rbegin();
        std::vector<Order> fills;
        while (order.size > 0 && levelIt != bids.rend() && levelIt->getPrice() >= price)
        {
            auto levelFills = levelIt->match(order);
            for (size_t i = 1; i < levelFills.size(); i += 2)
            {
                const Order& fill = levelFills[i];
                if (levelIt->invalid(fill.oid))
                {
                    orders.erase(fill.oid);
                }
            }
            fills.insert(
                fills.end(), std::make_move_iterator(levelFills.begin()), std::make_move_iterator(levelFills.end()));
            if (levelIt->empty())
            {
                bids.erase(std::next(levelIt).base());
                levelIt = bids.rbegin();
            }
        }

        if (order.size > 0)
        {
            auto rit = asks.rbegin();
            while (rit != asks.rend() && rit->getPrice() < price)
            {
                ++rit;
            }
            if (rit == asks.rend() || rit->getPrice() > price)
            {
                auto it = asks.emplace(rit.base(), price);
                orders[oid] = it->add(order);
            }
            else
            {
                orders[oid] = rit->add(order);
            }
        }

        return fills;
    }
};
