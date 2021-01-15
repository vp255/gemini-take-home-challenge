#ifndef SIDE_H
#define SIDE_H

#include <iostream>
#include <string>

enum class Side : bool {
  Buy,
  Sell
};

template <Side S>
Side OtherSide() {
  if constexpr(S == Side::Buy)
    return Side::Sell;
  else
    return Side::Buy;
}

constexpr Side OtherSide(Side S) {
  if (S == Side::Buy)
    return Side::Sell;
  else
    return Side::Buy;
}
 
std::ostream& operator<<(std::ostream& oss, Side side) {
  if (side == Side::Buy)
    oss << "BUY";
  else
    oss << "SELL";
  return oss;
}

std::istream& operator>>(std::istream& iss, Side& side) {
  std::string token;
  iss >> token;
  if (token == "BUY")
    side =  Side::Buy;
  else if (token == "SELL")
    side =  Side::Sell;
  return iss;
}

#endif
 
