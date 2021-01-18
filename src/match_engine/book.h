#ifndef BOOK_H
#define BOOK_H

#include <map>
#include <iostream>
#include <sstream>

#include "parser.h"
#include "side.h"
#include "types.h"
#include "util.h"
 
template <Side S>
struct OrderKey {
    Price price;
    TimeStamp timestamp;
 
    bool operator<(const OrderKey<S>& other) const {
        if (price == other.price)
            return timestamp < other.timestamp;
        return util::IsBetterOrEqual<S>(price, other.price);
    }
};
 
struct OrderInfo {
    Quantity size;
    OrderID orderid;
};
 
class OrderBook {

public:

    OrderBook(Instrument instrument);
    template <Side S>
    using RestingOrders = std::map<OrderKey<S>, OrderInfo>;
    template <Side S>
    using OrderIdMap = std::map<OrderID, typename RestingOrders<S>::iterator>;
   
    template<Side S>
    void addOrder(Price price, Quantity size, OrderID&& orderid);
    void printTrade(const OrderID& incoming_oid,
                    const OrderID& resting_oid,
                    Price resting_price,
                    Quantity size);

    template <Side S>
    const RestingOrders<S>& getRestingOrders() const;

private:

    RestingOrders<Side::Buy>  buyOrders;
    RestingOrders<Side::Sell> sellOrders;
    OrderIdMap<Side::Buy> buyOrderIdMap;
    OrderIdMap<Side::Sell> sellOrderIdMap;
    Instrument instrument;
    
    template <Side S>
    RestingOrders<S>& getRestingOrders();
    template <Side S>
    OrderIdMap<S>& getOrderIdMap();
    
    template <Side S>
    Quantity tradeMarketable(Price price, Quantity size, const OrderID& orderid);
   
    template <Side S>
    void addOrderToBook(Price price, Quantity size, OrderID&& orderid);
};

 
template <Side S>
void OrderBook::addOrder(Price price, Quantity size, OrderID&& orderid) {
    auto remainder = tradeMarketable<S>(price, size, orderid);
    addOrderToBook<S>(price, remainder, std::move(orderid));
}

OrderBook::OrderBook(Instrument instrument) : instrument(instrument) {}

template <Side S>
const OrderBook::RestingOrders<S>& 
OrderBook::getRestingOrders() const {
    if constexpr (S == Side::Buy) {
        return buyOrders;
    } else {
        return sellOrders;
    }
}

template <Side S>
OrderBook::RestingOrders<S>& 
OrderBook::getRestingOrders() {
    return const_cast<RestingOrders<S>&>(static_cast<const OrderBook&>(*this).getRestingOrders<S>());
}
 
template <Side S>
OrderBook::OrderIdMap<S>& OrderBook::getOrderIdMap() {
    if constexpr (S == Side::Buy) {
        return buyOrderIdMap;
    } else {
        return sellOrderIdMap;
    }
}

void OrderBook::printTrade(const OrderID& incoming_oid,
                           const OrderID& resting_oid, 
                           Price resting_price, 
                           Quantity size) {
    std::cout << "TRADE " 
              << instrument << " "
              << incoming_oid << " " 
              << resting_oid << " " 
              << size << " " 
              << resting_price 
              << '\n';
}
 
template <Side S>
Quantity OrderBook::tradeMarketable(Price price, Quantity size, const OrderID& orderid) {
    auto& otherSideOrders = getRestingOrders<OtherSide(S)>();
    auto& otherSideOrderIdMap = getOrderIdMap<OtherSide(S)>();
  
    while (size && !otherSideOrders.empty() && util::IsBetterOrEqual<S>(price, otherSideOrders.begin()->first.price)) {
        auto& kv = *otherSideOrders.begin();
        auto restingPrice = kv.first.price;
        auto restingOrderId = kv.second.orderid;
        auto& restingSize = kv.second.size;

        auto tradeSize = std::min(size, restingSize);
        printTrade(orderid, restingOrderId, restingPrice, tradeSize);
        size -= tradeSize;
        restingSize -= tradeSize;

        if (restingSize == 0) {
            otherSideOrderIdMap.erase(restingOrderId);
            otherSideOrders.erase(otherSideOrders.begin());
        }
    }
    return size;
}
 
template <Side S>
void OrderBook::addOrderToBook(Price price, Quantity size, OrderID&& orderid) {
    if (size == 0) {
        return;
    }
    OrderKey<S> new_key = {price, util::getTimestamp()};
    auto p = getRestingOrders<S>().emplace(new_key, OrderInfo{size, orderid});
    getOrderIdMap<S>()[std::move(orderid)] =  p.first;
}
 

#endif
