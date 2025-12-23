#pragma once
#include <cstdint>
#include <chrono>
#include <cstring>
#include <string>

// two types of order: buy and sell
enum class OrderSide : uint8_t {
    Buy = 0,
    Sell = 1
};

// types of order: limit and market
enum class OrderType : uint8_t {
    Limit,
    Market
};

using OrderId = uint64_t;
using Price = int64_t;
using Quantity = uint32_t;

// An ID generator that takes auto-increment strategy
inline OrderId nextId() {
    static OrderId counter = 0;
    return ++counter;
}

struct Order {
    OrderId id; // ID of the order
    char symbol[8]{}; // Goods type or symbol
    Price price; // Buy or sell price
    Quantity quantity; // Buy or sell quantity
    OrderSide side; // Buy side or sell side
    OrderType type; // Limit or market
    uint64_t timestamp = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()); // Timestamp for order creating
};

struct Trade {
    Price price; // Price on trade
    Quantity quantity; // Quantity on trade
    OrderId makerId; // Originally from which order
    OrderId takerId; // Eventually towards which order
};
