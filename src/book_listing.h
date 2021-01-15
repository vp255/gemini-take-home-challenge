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
    void addOrder(const OrderID& orderId,
             const Instrument& instrument,
             Quantity quantity,
             Price price);

    ListingType listing;

    void printRestingOrdersInOrder();
};

template <Side S>
void BookListing::addOrder(const OrderID& orderId,
                        const Instrument& instrument,
                        Quantity quantity,
                        Price price) {
    auto iter = listing.find(instrument);

    if (iter == listing.end()) {
        iter = listing.insert(decltype(listing)::value_type(instrument, OrderBook(instrument))).first;
    }

    iter->second.addOrder<S>(price, quantity, orderId);
}

void BookListing::printRestingOrdersInOrder() {
  
    std::vector<RestingOrder> restingSellOrders;
    for (const auto& [instrument, book] : listing) {
        for (const auto& [key, value] : book.getRestingOrders<Side::Sell>())
            restingSellOrders.push_back({key.timestamp,
                                         value.orderid,
                                         instrument,
                                         value.size,
                                         key.price});
    }
    std::sort(restingSellOrders.begin(), restingSellOrders.end());
  
    std::vector<RestingOrder> restingBuyOrders;
    for (const auto& [instrument, book] : listing) {
        for (const auto& [key, value] : book.getRestingOrders<Side::Buy>())
            restingBuyOrders.push_back({key.timestamp,
                                        value.orderid,
                                        instrument,
                                        value.size,
                                        key.price});
    }
    std::sort(restingBuyOrders.begin(), restingBuyOrders.end());
  
    for (const auto& order : restingSellOrders) {
        std::cout << order.orderId << " "
                  << "SELL "
                  << order.instrument << " "
                  << order.size << " "
                  << order.price << '\n';
    }
  
    for (const auto& order : restingBuyOrders) {
        std::cout << order.orderId << " "
                  << "BUY "
                  << order.instrument << " "
                  << order.size << " "
                  << order.price << '\n';
    }

}


#endif
