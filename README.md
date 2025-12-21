# Simple Order Matching Engine

A simulation of a Limit Order Book (LOB) implementation in C++17. This project demonstrates core concepts of financial trading systems.

## Technology Stack

* **Language:** C++17 (STL heavily utilized)
* **Build System:** CMake (Cross-platform support)
* **Dependencies:** None (Standard Library only)

## Project Structure

```
MatchingEngineDemo/
├── CMakeLists.txt          # Build configuration
├── include/                # Header files (API definitions)
│   ├── MatchingEngine.hpp  # Engine that handles OrderBook(s)
│   ├── Order.hpp           # Data structures for Orders and Trades
│   └── OrderBook.hpp       # Core matching logic
├── src/                    # Source files (Implementation)
│   ├── MatchingEngine.cpp  # Implementation of the OrderBook Management
│   ├── OrderBook.cpp       # Implementation of the matching algorithm
│   └── main.cpp            # Entry point and test scenarios
└── README.md               # Documentation
```

## How to Build
Ensure you have a C++ compiler (GCC, Clang, or MSVC) and CMake installed.
Clone the repository (or navigate to the project directory), then:
```
bash
cd MatchingEngineDemo
mkdir build
cd build
cmake ..
cmake --build .

```

## How to Run
After compiling, run the executable from the build directory:
On Linux / macOS:
```
bash
./OrderMatchingEngine
```
On Windows:
```
powershell
./OrderMatchingEngine.exe
```

## Sample Output
```
==================================
      MATCHING ENGINE DEMO        
==================================

[Test 1] Basic Matching (AAPL)
Order 1: Sell 100 AAPL @ 150 (Maker)
  -> No trades executed (Order added to book).
Order 2: Buy 10 AAPL @ 150 (Taker)
  -> [TRADE] 10 @ 150 (Maker: 1, Taker: 2)

[Test 2] No Match (Price Gap)
Order 3: Sell 50 AAPL @ 160
  -> No trades executed (Order added to book).
Order 4: Buy 50 AAPL @ 140
  -> No trades executed (Order added to book).

[Test 3] Sweep the Book (Market Taker)
Setup: TSLA Sell Orders placed: 10@100, 20@101, 50@102
Order 13: BIG BUY 40 TSLA @ 105
  -> [TRADE] 10 @ 100 (Maker: 10, Taker: 13)
  -> [TRADE] 20 @ 101 (Maker: 11, Taker: 13)
  -> [TRADE] 10 @ 102 (Maker: 12, Taker: 13)

[Test 4] Symbol Isolation
Order 20: Sell MSFT @ 200
Order 21: Buy AAPL @ 200 (Should NOT match MSFT)
  -> [TRADE] 10 @ 150 (Maker: 1, Taker: 21)

```

## Design Decisions

* **Data Structures:**
    * `std::map`: Used for the *Order Book* (Bids and Asks). This guarantees that orders are **automatically sorted by price** (Red-Black Tree implementation), enabling efficient $O(\log N)$ insertion and deletion while maintaining strict Price-Time priority.
    * `std::unordered_map`: Used in the *Matching Engine* to map stock symbols (e.g., "AAPL") to their respective order books with **$O(1)$ average lookup time**.

* **Price Representation:**
    * `int64_t`: Prices are stored as integers (e.g., representing cents or micros) instead of `double`. This eliminates **floating-point precision errors** (IEEE 754), ensuring 100% accounting accuracy required for financial systems.
