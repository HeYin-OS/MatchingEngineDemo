#include <map>
#include <vector>
#include <iostream>
#include "Order.hpp"

class OrderBook {
private:
    // ascending list of sell orders
    std::map<Price, std::vector<Order>> sellOrdersMap;

    // descending list of buy orders
    std::map<Price, std::vector<Order>, std::greater<>> buyOrdersMap;

    // matching core, use incoming order to take actions in target order vector
    static std::vector<Trade> matchAgainstList(Order& incomingOrder, std::vector<Order>& bookOrders);

public:
    // add a new order for matching to the system
    std::vector<Trade> addOrder(Order order);

    // cancel an order through ID
    bool cancelOrder(OrderId orderId);

    // take a snapshot of this book
    void printBook(const std::string& symbol) const;
};