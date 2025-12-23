//
// Created by 12544 on 2025/12/21.
//

#pragma once
#include <unordered_map>
#include <OrderBook.hpp>

class MatchingEngine {
private:
    std::unordered_map<std::string, OrderBook> orderBooks;

public:
    std::vector<Trade> processOrder(const Order& order);

    void cancelOrder(OrderId orderId, const std::string& symbol);

    void printGlobalSnapshot() const;
};