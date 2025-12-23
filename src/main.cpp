#include "MatchingEngine.hpp"

void printTrades(const std::vector<Trade> &trades) {
    if (trades.empty()) {
        std::cout << "  -> No trades executed (Order added to book)." << std::endl;
        return;
    }
    for (const auto &t: trades) {
        std::cout << "  -> [TRADE] " << t.quantity << " @ " << t.price
                << " (Maker: " << t.makerId << ", Taker: " << t.takerId << ")" << std::endl;
    }
}

int main() {
    MatchingEngine engine;

    std::cout << "==================================" << std::endl;
    std::cout << "      MATCHING ENGINE DEMO        " << std::endl;
    std::cout << "==================================" << std::endl;

    // ==========================================
    // Scene 1: Basic Matching (AAPL)
    // ==========================================
    std::cout << "\n[Test 1] Basic Matching (AAPL)" << std::endl;

    std::cout << "Order 1: Sell 100 AAPL @ 150 (Maker)" << std::endl;
    printTrades(engine.processOrder(Order(nextId(), "AAPL", 150, 100, OrderSide::Sell, OrderType::Limit)));

    std::cout << "Order 2: Buy 10 AAPL @ 150 (Taker)" << std::endl;
    printTrades(engine.processOrder(Order(nextId(), "AAPL", 150, 10, OrderSide::Buy, OrderType::Limit)));



    // ==========================================
    // Scene 2: No Match
    // ==========================================
    std::cout << "\n[Test 2] No Match (Price Gap)" << std::endl;

    std::cout << "Order 3: Sell 50 AAPL @ 160" << std::endl;
    printTrades(engine.processOrder(Order(nextId(), "AAPL", 160, 50, OrderSide::Sell, OrderType::Limit)));

    std::cout << "Order 4: Buy 50 AAPL @ 140" << std::endl;
    printTrades(engine.processOrder(Order(nextId(), "AAPL", 140, 50, OrderSide::Buy, OrderType::Limit)));


    // ==========================================
    // Scene 3: Sweep the Book
    // ==========================================
    std::cout << "\n[Test 3] Sweep the Book (Market Taker)" << std::endl;

    engine.processOrder(Order(nextId(), "TSLA", 100, 10, OrderSide::Sell, OrderType::Limit));
    engine.processOrder(Order(nextId(), "TSLA", 101, 20, OrderSide::Sell, OrderType::Limit));
    engine.processOrder(Order(nextId(), "TSLA", 102, 50, OrderSide::Sell, OrderType::Limit));
    std::cout << "Setup: TSLA Sell Orders placed: 10@100, 20@101, 50@102" << std::endl;

    std::cout << "Order 13: BIG BUY 40 TSLA @ 105" << std::endl;
    auto tradesSweep = engine.processOrder(Order(nextId(), "TSLA", 105, 40, OrderSide::Buy, OrderType::Limit));
    printTrades(tradesSweep);


    // ==========================================
    // Scene 4: Symbol Isolation (MSFT vs AAPL)
    // ==========================================
    std::cout << "\n[Test 4] Symbol Isolation" << std::endl;

    std::cout << "Order 20: Sell MSFT @ 200" << std::endl;
    engine.processOrder(Order(nextId(), "MSFT", 200, 10, OrderSide::Sell, OrderType::Limit));

    std::cout << "Order 21: Buy AAPL @ 200 (Should NOT match MSFT)" << std::endl;
    auto tradesIsolation = engine.processOrder(Order(nextId(), "AAPL", 200, 10, OrderSide::Buy, OrderType::Limit));
    printTrades(tradesIsolation);

    return 0;
}
