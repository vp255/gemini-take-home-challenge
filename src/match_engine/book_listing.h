#ifndef BOOK_LISTING_H
#define BOOK_LISTING_H

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "book.h"
#include "types.h"

namespace {

struct RestingOrder {
    TimeStamp timestamp;
    OrderID orderId;
    Instrument instrument;
    Quantity size;
    Price price;

    bool operator<(const RestingOrder& other) const {
        return timestamp < other.timestamp;
    }

};

}

class BookListing {
public:

    BookListing() = default;

    typedef std::unordered_map<Instrument, OrderBook> ListingType;

    template <Side S>
    void addOrder(OrderID&& orderId,
                  const Instrument& instrument,
                  Quantity quantity,
                  Price price);

    ListingType listing;

    void printRestingOrdersInOrder() const;

private:

    template <Side S>
    void printOrder(const RestingOrder& restingOrder) const;
};

template <Side S>
void BookListing::addOrder(OrderID&& orderId,
                           const Instrument& instrument,
                           Quantity quantity,
                           Price price) {
    auto iter = listing.find(instrument);

    if (iter == listing.end()) {
        iter = listing.emplace(instrument, OrderBook(instrument)).first;
    }
    iter->second.addOrder<S>(price, quantity, std::move(orderId));
}

void BookListing::printRestingOrdersInOrder() const {
    std::vector<Instrument> instruments;
    for (auto& [instrument, _] : listing) {
        instruments.push_back(instrument);
    }
    std::sort(instruments.begin(), instruments.end());

    for (const auto& instrument : instruments) {
        const auto& book = listing.at(instrument);
        std::vector<RestingOrder> restingSellOrders;
        for (const auto& [key, value] : book.getRestingOrders<Side::Sell>()) {
            restingSellOrders.push_back({key.timestamp,
                                       value.orderid,
                                       instrument,
                                       value.size,
                                       key.price});
        }
        std::sort(restingSellOrders.begin(), restingSellOrders.end());
        for (const auto& order : restingSellOrders) {
            printOrder<Side::Sell>(order);
        }
    }
    for (const auto& instrument : instruments) {
        const auto& book = listing.at(instrument);
        std::vector<RestingOrder> restingBuyOrders;
        for (const auto& [key, value] : book.getRestingOrders<Side::Buy>()) {
            restingBuyOrders.push_back({key.timestamp,
                                       value.orderid,
                                       instrument,
                                       value.size,
                                       key.price});
        }
        std::sort(restingBuyOrders.begin(), restingBuyOrders.end());
        for (const auto& order : restingBuyOrders) {
            printOrder<Side::Buy>(order);
        }
    }
}

template <Side S>
void BookListing::printOrder(const RestingOrder& order) const {
    std::cout << order.orderId << " "
              << S << " "
              << order.instrument << " "
              << order.size << " "
              << order.price << '\n';
}

#endif
