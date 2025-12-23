# Simple Order Matching Engine

A simulation of a Limit Order Book (LOB) implementation in C++20. This project demonstrates core concepts of financial trading systems.

## Technology Stack

* **Language:** C++20 (STL heavily utilized)
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
Select your favorite C++ toolchain on your preferred platform.
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
==================================================
             GLOBAL MARKET SNAPSHOT               
==================================================
  (No OrderBooks Active)
==================================================


[Test 1] Basic Matching (AAPL)
Order 1: Sell 100 AAPL @ 150 (Maker)
  -> No trades executed (Order added to book).
Order 2: Buy 10 AAPL @ 150 (Taker)
  -> [TRADE] 10 @ 150 (Maker: 1, Taker: 2)
==================================================
             GLOBAL MARKET SNAPSHOT               
==================================================

+------------------------------------------------+
| ORDER BOOK: AAPL                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 150       | 90             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
+-------+-----------+----------------+-----------+

==================================================


[Test 2] No Match (AAPL)
Order 3: Sell 50 AAPL @ 160
  -> No trades executed (Order added to book).
Order 4: Buy 50 AAPL @ 140
  -> No trades executed (Order added to book).
==================================================
             GLOBAL MARKET SNAPSHOT               
==================================================

+------------------------------------------------+
| ORDER BOOK: AAPL                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 160       | 50             | 1         |
| ASK   | 150       | 90             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
| BID   | 140       | 50             | 1         |
+-------+-----------+----------------+-----------+

==================================================


[Test 3] Sweep the Book (TSLA)
Setup: TSLA Sell Orders (5, 6, 7) placed: 10@100, 20@101, 50@102
Order 8: BIG BUY 40 TSLA @ 105
  -> [TRADE] 10 @ 100 (Maker: 5, Taker: 8)
  -> [TRADE] 20 @ 101 (Maker: 6, Taker: 8)
  -> [TRADE] 10 @ 102 (Maker: 7, Taker: 8)
==================================================
             GLOBAL MARKET SNAPSHOT               
==================================================

+------------------------------------------------+
| ORDER BOOK: AAPL                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 160       | 50             | 1         |
| ASK   | 150       | 90             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
| BID   | 140       | 50             | 1         |
+-------+-----------+----------------+-----------+


+------------------------------------------------+
| ORDER BOOK: TSLA                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 102       | 40             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
+-------+-----------+----------------+-----------+

==================================================


[Test 4] Symbol Isolation
Order 20: Sell 10 MSFT @ 200
Order 21: Buy 10 AAPL @ 200 (Should NOT match MSFT, but match Order 1)
  -> [TRADE] 10 @ 150 (Maker: 1, Taker: 21)
==================================================
             GLOBAL MARKET SNAPSHOT               
==================================================

+------------------------------------------------+
| ORDER BOOK: MSFT                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 200       | 10             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
+-------+-----------+----------------+-----------+


+------------------------------------------------+
| ORDER BOOK: AAPL                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 160       | 50             | 1         |
| ASK   | 150       | 80             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
| BID   | 140       | 50             | 1         |
+-------+-----------+----------------+-----------+


+------------------------------------------------+
| ORDER BOOK: TSLA                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 102       | 40             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
+-------+-----------+----------------+-----------+

==================================================


[Test 5] Cancel Order
Order 99: Sell 100 AAPL @ 200 (Will be cancelled)
Cancelling Order 99...
  -> Order 99 Cancelled Successfully.
Order 100: Buy 50 AAPL @ 200 (Should NOT match Order 99, but match Order 1)
  -> [TRADE] 50 @ 150 (Maker: 1, Taker: 100)
==================================================
             GLOBAL MARKET SNAPSHOT               
==================================================

+------------------------------------------------+
| ORDER BOOK: MSFT                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 200       | 10             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
+-------+-----------+----------------+-----------+


+------------------------------------------------+
| ORDER BOOK: AAPL                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 160       | 50             | 1         |
| ASK   | 150       | 30             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
| BID   | 140       | 50             | 1         |
+-------+-----------+----------------+-----------+


+------------------------------------------------+
| ORDER BOOK: TSLA                               |
+-------+-----------+----------------+-----------+
| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |
+-------+-----------+----------------+-----------+
| ASK   | 102       | 40             | 1         |
+-------+-----------+----------------+-----------+
|              SPREAD (MARKET MID)               |
+-------+-----------+----------------+-----------+
+-------+-----------+----------------+-----------+

==================================================


```

## Design Decisions & Architecture

### 1. Data Structures: The "Price-Time" Priority
* **Order Book (`std::map` + `std::vector`):**
  * We use `std::map<Price, std::vector<Order>>` to store orders.
  * **Price Priority:** `std::map` (implemented as a Red-Black Tree) automatically keeps price levels sorted ($O(\log N)$). Bids use `std::greater<>` to keep the highest price at the top.
  * **Time Priority:** `std::vector<Order>` handles orders at the *same* price level. This naturally enforces **FIFO (First-In-First-Out)** execution—earlier orders are at the front of the vector.
* **Symbol Routing (`std::unordered_map`):**
  * The `MatchingEngine` uses a Hash Map to route orders to their specific `OrderBook` (e.g., AAPL, TSLA) with **$O(1)$ average time complexity**.

### 2. Low-Latency Optimizations (Hardware Sympathy)
* **POD (Plain Old Data) over Objects:**
  * The `Order` struct replaces `std::string` with a fixed-size `char symbol[8]`.
  * **Why?** This avoids heap allocation overhead and pointer indirection associated with `std::string`. It ensures the `Order` struct is a contiguous block of memory, significantly improving **CPU Cache Locality** and serialization performance.
* **Memory Management:**
  * Iterators are carefully managed during `vector::erase` operations to avoid invalidation while maintaining high throughput in the matching loop.

### 3. Financial Precision
* **Integer Arithmetic:**
  * `Price` is defined as `int64_t` (e.g., representing micros or cents) rather than `double`.
  * **Why?** Floating-point arithmetic (IEEE 754) introduces precision errors (e.g., `0.1 + 0.2 != 0.3`) which are unacceptable in financial accounting.

### 4. Modern C++ Standards (C++20)
* **Aggregate Initialization:**
  * We utilize C++20 designated initializers (e.g., `.id = nextId(), .symbol = "AAPL"`). This removes the need for boilerplate constructors and improves code readability while maintaining the POD nature of the `Order` struct.
* **Type Safety:**
  * `enum class` is used for `OrderSide` and `OrderType` to prevent implicit type conversions and ensure stricter type safety compared to legacy C-style enums.