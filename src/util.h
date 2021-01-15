#ifndef UTIL_H
#define UTIL_H

#include "side.h"

namespace util {

template<Side S, class P>
bool IsBetterOrEqual(const P& a, const P& b) {
    if constexpr (S == Side::Buy)
        return a >= b;
    else
        return a <= b;
}

TimeStamp getTimestamp() {
    static TimeStamp stamp = 0;
    return ++stamp;
}

}

#endif
