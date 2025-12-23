//
// Created by 12544 on 2025/12/21.
//

#include <MatchingEngine.hpp>

#include <iostream>

std::vector<Trade> MatchingEngine::processOrder(const Order &order) {
    OrderBook &book = orderBooks[order.symbol];
    return book.addOrder(order);
}

void MatchingEngine::cancelOrder(OrderId orderId, const std::string &symbol) {
    if (orderBooks.find(symbol) != orderBooks.end()) {
        bool success = orderBooks[symbol].cancelOrder(orderId);
        if (success) {
            std::cout << "  -> Order " << orderId << " Cancelled Successfully." << std::endl;
        } else {
            std::cout << "  -> Order " << orderId << " Cancel Failed (Not Found)." << std::endl;
        }
    } else {
        std::cout << "  -> OrderBook for " << symbol << " not found." << std::endl;
    }
}
