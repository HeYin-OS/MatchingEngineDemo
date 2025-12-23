//
// Created by 12544 on 2025/12/21.
//

#include <OrderBook.hpp>

std::vector<Trade> OrderBook::matchAgainstList(Order &incomingOrder, std::vector<Order> &bookOrders) {
    std::vector<Trade> trades;
    auto it = bookOrders.begin();

    while (it != bookOrders.end() && incomingOrder.quantity > 0) {
        Order &bookOrder = *it;
        // Get the minimum quantity
        Quantity tradeQty = std::min(incomingOrder.quantity, bookOrder.quantity);

        // Record the trade
        trades.push_back({bookOrder.price, tradeQty, bookOrder.id, incomingOrder.id});

        // Update remaining
        bookOrder.quantity -= tradeQty;
        incomingOrder.quantity -= tradeQty;

        // When current order quantity is zero, delete it from current vector.
        if (bookOrder.quantity == 0) {
            it = bookOrders.erase(it);
        } else {
            ++it;
        }
    }
    return trades;
}

std::vector<Trade> OrderBook::addOrder(Order order) {
    std::vector<Trade> allTrades;

    if (order.side == OrderSide::Buy) {
        // Buy order
        // Traverse all best ask orders
        auto& buyOrder = order;

        auto askIt = sellOrdersMap.begin();
        while (askIt != sellOrdersMap.end() && buyOrder.quantity > 0) {
            Price bestSellPrice = askIt->first;

            // Price does not match
            if (buyOrder.price < bestSellPrice) break;

            // Transaction
            std::vector<Order> &bestSellOrders = askIt->second;
            auto newTrades = matchAgainstList(buyOrder, bestSellOrders);
            allTrades.insert(allTrades.end(), newTrades.begin(), newTrades.end());

            // When there are no orders at this price, remove this price level in the map.
            if (bestSellOrders.empty()) {
                askIt = sellOrdersMap.erase(askIt);
            } else {
                ++askIt;
            }
        }

        // Buy order is still not empty, push it to bids map.
        if (buyOrder.quantity > 0) {
            buyOrdersMap[buyOrder.price].push_back(buyOrder);
        }
    } else {
        // Sell order
        // Traverse all best ask orders
        auto& sellOrder = order;

        auto bidIt = buyOrdersMap.begin();
        while (bidIt != buyOrdersMap.end() && sellOrder.quantity > 0) {
            Price bestBuyPrice = bidIt->first;

            // Price does not match
            if (sellOrder.price > bestBuyPrice) break;

            // Transaction
            std::vector<Order> &bestBuyOrders = bidIt->second;
            auto newTrades = matchAgainstList(sellOrder, bestBuyOrders);
            allTrades.insert(allTrades.end(), newTrades.begin(), newTrades.end());

            // When there are no orders at this price, remove this price level in the map.
            if (bestBuyOrders.empty()) {
                bidIt = buyOrdersMap.erase(bidIt);
            } else {
                ++bidIt;
            }
        }

        // Buy order is still not empty, push it to bids map.
        if (sellOrder.quantity > 0) {
            sellOrdersMap[sellOrder.price].push_back(sellOrder);
        }
    }

    return allTrades;
}

bool OrderBook::cancelOrder(OrderId orderId) {
    // brute search
    auto removeBySide = [&](auto& orderMap)->bool {
        for (auto mapIt = orderMap.begin(); mapIt != orderMap.end(); ++mapIt) {
            auto& currentOrderVector = mapIt->second;

            // find ID in vec
            for (auto vecIt = currentOrderVector.begin(); vecIt != currentOrderVector.end(); ++vecIt) {
                if (vecIt->id == orderId) {
                    currentOrderVector.erase(vecIt);

                    // if this price level is empty
                    if (currentOrderVector.empty()) {
                        orderMap.erase(mapIt);
                    }
                    return true;
                }
            }
        }
        return false;
    };

    if (removeBySide(buyOrdersMap))
        return true;

    if (removeBySide(sellOrdersMap))
        return true;

    return false;
}

void OrderBook::printBook(const std::string& symbol) const {
    std::cout << "\n+------------------------------------------------+" << std::endl;
    std::cout << "| ORDER BOOK: " << std::setw(35) << std::left << symbol << "|" << std::endl;
    std::cout << "+-------+-----------+----------------+-----------+" << std::endl;
    std::cout << "| TYPE  |   PRICE   |    QUANTITY    |   COUNT   |" << std::endl;
    std::cout << "+-------+-----------+----------------+-----------+" << std::endl;

    if (sellOrdersMap.empty()) {
    } else {
        for (auto it = sellOrdersMap.rbegin(); it != sellOrdersMap.rend(); ++it) {
            Price price = it->first;
            const auto& orders = it->second;

            Quantity totalQty = 0;
            for (const auto& o : orders) totalQty += o.quantity;

            std::cout << "| " << "\033[31mASK\033[0m"
                      << "   | " << std::setw(9) << price
                      << " | " << std::setw(14) << totalQty
                      << " | " << std::setw(9) << orders.size() << " |" << std::endl;
        }
    }

    std::cout << "+-------+-----------+----------------+-----------+" << std::endl;
    std::cout << "|              SPREAD (MARKET MID)               |" << std::endl;
    std::cout << "+-------+-----------+----------------+-----------+" << std::endl;

    if (buyOrdersMap.empty()) {
    } else {
        for (auto it = buyOrdersMap.begin(); it != buyOrdersMap.end(); ++it) {
            Price price = it->first;
            const auto& orders = it->second;

            Quantity totalQty = 0;
            for (const auto& o : orders) totalQty += o.quantity;

            std::cout << "| " << "\033[32mBID\033[0m"
                      << "   | " << std::setw(9) << price
                      << " | " << std::setw(14) << totalQty
                      << " | " << std::setw(9) << orders.size() << " |" << std::endl;
        }
    }
    std::cout << "+-------+-----------+----------------+-----------+\n" << std::endl;
}

