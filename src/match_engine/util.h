#ifndef UTIL_H
#define UTIL_H

#include "side.h"

namespace util {

template<Side S, class P>
bool IsBetterOrEqual(P a, P b) {
    if constexpr (S == Side::Buy)
        return a >= b;
    else
        return a <= b;
}

template<Side S, class P>
P Improve(P base, P delta) {
  if constexpr (S == Side::Buy) {
      return base + delta;
  } else {
      return base - delta;
  }
}

uint32_t getTimestamp() {
    static uint32_t stamp = 0;
    return ++stamp;
}

}

#endif
