//
// Created by 12544 on 2025/12/21.
//

#pragma once
#include <unordered_map>
#include <OrderBook.hpp>

class MatchingEngine {
private:
    // hash table of various books
    std::unordered_map<std::string, OrderBook> orderBooks;

public:
    // add a new order to the system, it will be redirected to right book and be handled.
    std::vector<Trade> processOrder(const Order& order);

    // place a new cancel order with symbol, it will be redirected to right book and be handled.
    void cancelOrder(OrderId orderId, const std::string& symbol);

    // take a snapshot of the whole market
    void printGlobalSnapshot() const;
};