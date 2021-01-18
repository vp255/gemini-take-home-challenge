#ifndef BOOK_LISTING_TESTS_H
#define BOOK_LISTING_TESTS_H

#include "book_listing.h"

namespace book_listing_tests {

void VerifyAddOrder() {
    BookListing bl;
    bl.addOrder<Side::Buy>("order_a", "instrument a", 5, 100);
    bl.addOrder<Side::Sell>("order_b", "instrument b", 6, 101);
    auto it = const_cast<const OrderBook*>(&bl.listing.at("instrument a"))->getRestingOrders<Side::Buy>().begin();
    assert(it->first.price == 100);
    assert(it->second.size == 5);
    assert(it->second.orderid == "order_a");
    auto it2 = const_cast<const OrderBook*>(&bl.listing.at("instrument b"))->getRestingOrders<Side::Sell>().begin();
    assert(it2->first.price == 101);
    assert(it2->second.size == 6);
    assert(it2->second.orderid == "order_b");
    bl.addOrder<Side::Buy>("order_c", "instrument a", 7, 101);
    bl.addOrder<Side::Sell>("order_d", "instrument b", 8, 100);
    auto it3 = const_cast<const OrderBook*>(&bl.listing.at("instrument a"))->getRestingOrders<Side::Buy>().begin();
    assert(it3->first.price == 101);
    assert(it3->second.size == 7);
    assert(it3->second.orderid == "order_c");
    auto it4 = const_cast<const OrderBook*>(&bl.listing.at("instrument b"))->getRestingOrders<Side::Sell>().begin();
    assert(it4->first.price == 100);
    assert(it4->second.size == 8);
    assert(it4->second.orderid == "order_d");
}

void Run() {
    std::cout << "Verifying book_listing.addOrder()" << '\n';
    VerifyAddOrder();
}

}


#endif
