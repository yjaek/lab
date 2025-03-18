#include "lib/MapOrderBook.h"

int main()
{
    MapOrderBook book;
    book.add(1, "B", 1, 11);
    book.add(2, "B", 2, 12);
    book.add(3, "B", 3, 13);
    book.add(4, "A", 4, 14);
    book.add(5, "A", 5, 15);
    book.add(6, "A", 6, 16);

    const auto& asks = book.getAsks();
    for (auto it = asks.crbegin(); it != asks.crend(); ++it)
    {
        const auto& orders = it->second.getOrders();
        const size_t size =
            std::accumulate(orders.begin(), orders.end(), 0, [](size_t curr, const Order& o) { return curr + o.size; });
        std::cout << "Ask $" << it->first << " for " << size << std::endl;
    }

    const auto& bids = book.getBids();
    for (const auto& [price, level] : bids)
    {
        const auto& orders = level.getOrders();
        const size_t size =
            std::accumulate(orders.begin(), orders.end(), 0, [](size_t curr, const Order& o) { return curr + o.size; });
        std::cout << "Bid $" << price << " for " << size << std::endl;
    }
}