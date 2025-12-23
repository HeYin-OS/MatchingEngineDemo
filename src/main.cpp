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

    // snapshot on startup
    engine.printGlobalSnapshot();

    // ==========================================
    // Scene 1: Basic Matching (AAPL)
    // ==========================================
    std::cout << "\n[Test 1] Basic Matching (AAPL)" << std::endl;

    std::cout << "Order 1: Sell 100 AAPL @ 150 (Maker)" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 150,
        .quantity = 100,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    }));

    std::cout << "Order 2: Buy 10 AAPL @ 150 (Taker)" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 150,
        .quantity = 10,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    }));

    engine.printGlobalSnapshot();

    // ==========================================
    // Scene 2: No Match (Price Gap)
    // ==========================================
    std::cout << "\n[Test 2] No Match (AAPL)" << std::endl;

    std::cout << "Order 3: Sell 50 AAPL @ 160" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 160,
        .quantity = 50,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    }));

    std::cout << "Order 4: Buy 50 AAPL @ 140" << std::endl;
    printTrades(engine.processOrder({
        .id = nextId(),
        .symbol = "AAPL",
        .price = 140,
        .quantity = 50,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    }));

    engine.printGlobalSnapshot();

    // ==========================================
    // Scene 3: Sweep (TSLA)
    // ==========================================
    std::cout << "\n[Test 3] Sweep the Book (TSLA)" << std::endl;

    std::cout << "Setup: TSLA Sell Orders (5, 6, 7) placed: 10@100, 20@101, 50@102" << std::endl;
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

    std::cout << "Order 8: BIG BUY 40 TSLA @ 105" << std::endl;
    auto tradesSweep = engine.processOrder({
        .id = nextId(),
        .symbol = "TSLA",
        .price = 105,
        .quantity = 40,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    });
    printTrades(tradesSweep);

    engine.printGlobalSnapshot();

    // ==========================================
    // Scene 4: Symbol Isolation (MSFT vs AAPL)
    // ==========================================
    std::cout << "\n[Test 4] Symbol Isolation" << std::endl;

    std::cout << "Order 20: Sell 10 MSFT @ 200" << std::endl;
    engine.processOrder({
        .id = 20,
        .symbol = "MSFT",
        .price = 200,
        .quantity = 10,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    });

    std::cout << "Order 21: Buy 10 AAPL @ 200 (Should NOT match MSFT, but match Order 1)" << std::endl;
    auto tradesIsolation = engine.processOrder({
        .id = 21,
        .symbol = "AAPL",
        .price = 200,
        .quantity = 10,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    });
    printTrades(tradesIsolation);

    engine.printGlobalSnapshot();

    // ==========================================
    // Scene 5: Cancel Order
    // ==========================================
    std::cout << "\n[Test 5] Cancel Order" << std::endl;

    std::cout << "Order 99: Sell 100 AAPL @ 200 (Will be cancelled)" << std::endl;
    engine.processOrder({
        .id = 99,
        .symbol = "AAPL",
        .price = 200,
        .quantity = 100,
        .side = OrderSide::Sell,
        .type = OrderType::Limit
    });

    std::cout << "Cancelling Order 99..." << std::endl;
    engine.cancelOrder(99, "AAPL");

    std::cout << "Order 100: Buy 50 AAPL @ 200 (Should NOT match Order 99, but match Order 1)" << std::endl;
    printTrades(engine.processOrder({
        .id = 100,
        .symbol = "AAPL",
        .price = 200,
        .quantity = 50,
        .side = OrderSide::Buy,
        .type = OrderType::Limit
    }));

    engine.printGlobalSnapshot();

    // pause
    std::cin.get();

    return 0;
}
