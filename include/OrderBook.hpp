#include <map>
#include <vector>
#include <iostream>
#include "Order.hpp"

class OrderBook {
private:
    std::map<Price, std::vector<Order>> sellOrdersMap;

    std::map<Price, std::vector<Order>, std::greater<>> buyOrdersMap;

    static std::vector<Trade> matchAgainstList(Order& incomingOrder, std::vector<Order>& bookOrders);

public:
    std::vector<Trade> addOrder(Order order);

    bool cancelOrder(OrderId orderId);
};