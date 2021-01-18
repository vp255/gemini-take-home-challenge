#ifndef BOOK_TEST_H
#define BOOK_TEST_H

#include "book.h"
#include "util.h"

namespace book_tests {

template <Side S>
void VerifyTimeTradePriority() {
    OrderBook book("testing_instrument");
    book.addOrder<S>(100, 5, "order_a");
    book.addOrder<S>(100, 5, "order_b");
    book.addOrder<OtherSide(S)>(util::Improve<OtherSide(S)>(100,5), 5, "order c");
    assert(const_cast<const OrderBook*>(&book)->getRestingOrders<S>().begin()->second.orderid == "order_b");
}

template <Side S>
void VerifyPricePriority() {
    OrderBook book("testing_instrument");
    book.addOrder<S>(100, 5, "order_a");
    book.addOrder<S>(100, 5, "order_b");
    assert(const_cast<const OrderBook*>(&book)->getRestingOrders<S>().begin()->second.orderid == "order_a");
}

template <Side S>
void VerifyPriceTradePriority() {
    OrderBook book("testing_instrument");
    book.addOrder<S>(100, 5, "order_a");
    book.addOrder<S>(util::Improve<S>(100, 2), 5, "order_b");
    book.addOrder<OtherSide(S)>(util::Improve<OtherSide(S)>(100,5), 5, "order c");
    assert(const_cast<const OrderBook*>(&book)->getRestingOrders<S>().begin()->second.orderid == "order_a");
}

void Run() {
    std::cout << "Verifying OrderBook Price priority" << '\n';
    VerifyPricePriority<Side::Buy>();
    VerifyPricePriority<Side::Sell>();
    std::cout << "Verifying OrderBook Time Trade priority" << '\n';
    VerifyTimeTradePriority<Side::Buy>();
    VerifyTimeTradePriority<Side::Sell>();
    std::cout << "Verifying OrderBook Price Trade priority" << '\n';
    VerifyPriceTradePriority<Side::Buy>();
    VerifyPriceTradePriority<Side::Sell>();
}

}

#endif

