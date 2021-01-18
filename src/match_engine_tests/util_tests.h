#ifndef UTIL_TESTS_H
#define UTIL_TESTS_H

#include "util.h"
#include <cassert>

namespace util_tests {

void VerifyImprove() {
  assert(util::Improve<Side::Buy>(100, 5) == 105);
  assert(util::Improve<Side::Sell>(100, 5) ==  95);
  assert(util::Improve<Side::Buy>(100, -5) == 95);
  assert(util::Improve<Side::Sell>(100, -5) == 105);

}

void VerifyIsBetterOrEqual() {
    assert(util::IsBetterOrEqual<Side::Buy>(105, 100));
    assert(util::IsBetterOrEqual<Side::Buy>(100, 100));
    assert(!util::IsBetterOrEqual<Side::Buy>(95, 100));
    assert(util::IsBetterOrEqual<Side::Sell>(95, 100));
    assert(util::IsBetterOrEqual<Side::Sell>(100, 100));
    assert(!util::IsBetterOrEqual<Side::Sell>(105, 100));
}

void Run() {
  std::cout << "Verifying utility functions" << '\n';
  VerifyImprove();
  VerifyIsBetterOrEqual();
}

}

#endif
