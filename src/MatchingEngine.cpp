//
// Created by 12544 on 2025/12/21.
//

#include <MatchingEngine.hpp>

std::vector<Trade> MatchingEngine::processOrder(const Order& order) {
    OrderBook &book = orderBooks[order.symbol];
    return book.addOrder(order);
}
