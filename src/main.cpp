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

    std::cout << "Order: Sell 100 AAPL @ 150 (Maker)" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 150,
        .quantity = 100,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    }));

    std::cout << "Order: Buy 10 AAPL @ 150 (Taker)" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 150,
        .quantity = 10,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    }));

    // ==========================================
    // Scene 2: No Match (Price Gap)
    // ==========================================
    std::cout << "\n[Test 2] No Match (AAPL)" << std::endl;

    std::cout << "Order: Sell 50 AAPL @ 160" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 160,
        .quantity = 50,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    }));

    std::cout << "Order: Buy 50 AAPL @ 140" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 140,
        .quantity = 50,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    }));


    // ==========================================
    // Scene 3: Sweep (TSLA)
    // ==========================================
    std::cout << "\n[Test 3] Sweep the Book (TSLA)" << std::endl;

    std::cout << "Setup: TSLA Sell Orders placed: 10@100, 20@101, 50@102" << std::endl;
    engine.processOrder({
        .id = nextId(),
        .symbol = "TSLA",
        .price = 100,
        .quantity = 10,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    });
    engine.processOrder({
        .id = nextId(),
        .symbol = "TSLA",
        .price = 101,
        .quantity = 20,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    });
    engine.processOrder({
        .id = nextId(),
        .symbol = "TSLA",
        .price = 102,
        .quantity = 50,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    });

    std::cout << "Order: BIG BUY 40 TSLA @ 105" << std::endl;
    auto tradesSweep = engine.processOrder({
        .id = nextId(),
        .symbol = "TSLA",
        .price = 105,
        .quantity = 40,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    });
    printTrades(tradesSweep);


    // ==========================================
    // Scene 4: Symbol Isolation (MSFT vs AAPL)
    // ==========================================
    std::cout << "\n[Test 4] Symbol Isolation" << std::endl;

    std::cout << "Order 20: Sell MSFT @ 200" << std::endl;
    engine.processOrder({
        .id = nextId(),
        .symbol = "MSFT",
        .price = 200,
        .quantity = 10,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    });

    std::cout << "Order 21: Buy AAPL @ 200 (Should NOT match MSFT)" << std::endl;
    auto tradesIsolation = engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 200,
        .quantity = 10,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    });
    printTrades(tradesIsolation);

    return 0;
}
