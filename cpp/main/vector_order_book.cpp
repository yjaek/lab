#include <iostream>
#include <numeric>

#include "lib/VectorOrderBook.h"

int main()
{
    VectorOrderBook book;
    book.add(1, "B", 1, 11);
    book.add(2, "B", 2, 12);
    book.add(3, "B", 3, 13);
    book.add(4, "A", 4, 14);
    book.add(5, "A", 5, 15);
    book.add(6, "A", 6, 16);

    const auto& asks = book.getAsks();
    for (auto it = asks.begin(); it != asks.end(); ++it)
    {
        const auto& orders = it->getOrders();
        const size_t size =
            std::accumulate(orders.begin(), orders.end(), 0, [](size_t curr, const Order& o) { return curr + o.size; });
        std::cout << "Ask $" << it->getPrice() << " for " << size << std::endl;
    }

    const auto& bids = book.getBids();
    for (auto it = bids.rbegin(); it != bids.rend(); ++it)
    {
        const auto& orders = it->getOrders();
        const size_t size =
            std::accumulate(orders.begin(), orders.end(), 0, [](size_t curr, const Order& o) { return curr + o.size; });
        std::cout << "Bid $" << it->getPrice() << " for " << size << std::endl;
    }
}