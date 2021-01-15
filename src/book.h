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
struct MapKey {
  Price price;
  TimeStamp timestamp;
 
  bool operator<(const MapKey<S>& other) const {
    if (price == other.price)
      return timestamp < other.timestamp;
    return util::IsBetterOrEqual<S>(price, other.price);
  }
};
 
struct MapValue {
  Quantity size;
  OrderID orderid;
};
 
class OrderBook {

public:

  OrderBook(Instrument instrument);
  template <Side S>
  using RestingOrders = std::map<MapKey<S>, MapValue>;
  template <Side S>
  using OrderIdMap = std::map<OrderID, typename RestingOrders<S>::iterator>;
 
  template<Side S>
  void addOrder(Price price, Quantity size, const OrderID& orderid);
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
  void addOrderToBook(Price price, Quantity size, OrderID orderid);
};

 
template <Side S>
void OrderBook::addOrder(Price price, Quantity size, const OrderID& orderid) {
    if constexpr(S == Side::Buy) {
        auto remainder = tradeMarketable<Side::Buy>(price, size, orderid);
        addOrderToBook<Side::Buy>(price, remainder, orderid);
    } else {
        auto remainder = tradeMarketable<Side::Sell>(price, size, orderid);
        addOrderToBook<Side::Sell>(price, remainder, orderid);
    }
}

OrderBook::OrderBook(Instrument instrument) : instrument(instrument) {}

template<Side S>
const OrderBook::RestingOrders<S>& 
OrderBook::getRestingOrders() const {
    if constexpr (S == Side::Buy) {
        return buyOrders;
    } else {
        return sellOrders;
    }
}

template <>
const OrderBook::RestingOrders<Side::Buy>&
OrderBook::getRestingOrders<Side::Buy>() const {
    return buyOrders;
}
 
 
template<Side S>
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
        auto kv = *otherSideOrders.begin();
        auto restingPrice     = kv.first.price;
        auto restingSize = kv.second.size;
        auto restingOrderId = kv.second.orderid;
    
        if (size >= restingSize) {
            otherSideOrderIdMap.erase(restingOrderId);
            printTrade(orderid, restingOrderId, restingPrice, restingSize);
            otherSideOrders.erase(otherSideOrders.begin());
            size -= restingSize;
        } else {
            printTrade(orderid, restingOrderId, restingPrice, size);
            otherSideOrders.begin()->second.size -= size;
            size = 0;
        }
    }
    return size;
}
 
template <Side S>
void OrderBook::addOrderToBook(Price price, Quantity size, OrderID orderid) {
    if (size != 0) {
        MapKey<S> new_key = {price, util::getTimestamp()};
        MapValue new_value = {size, orderid};
        auto p = getRestingOrders<S>().emplace(new_key, new_value);
        getOrderIdMap<S>()[orderid] =  p.first;
    }
}
 

#endif
