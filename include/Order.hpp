#pragma once
#include <cstdint>
#include <chrono>
#include <utility>

enum class OrderSide : uint8_t {
    Buy = 0,
    Sell = 1
};

enum class OrderType : uint8_t {
    Limit,
    Market
};

using OrderId = uint64_t;
using Price = int64_t;
using Quantity = uint32_t;

struct Order {
    OrderId id; // ID of the order
    std::string symbol; // Goods type or symbol
    Price price; // Buy or sell price
    Quantity quantity; // Buy or sell quantity
    OrderSide side; // Buy side or sell side
    OrderType type; // Limit or market
    uint64_t timestamp; // Timestamp for order creating

    Order(OrderId _id, std::string _Symbol, Price _p, Quantity _q, OrderSide _s, OrderType _t)
        : id(_id), symbol(std::move(_Symbol)), price(_p), quantity(_q), side(_s), type(_t) {
        timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }
};

struct Trade {
    Price price; // Price on trade
    Quantity quantity; // Quantity on trade
    OrderId makerId;
    OrderId takerId;
};
