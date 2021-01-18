#include <cassert>
#include <iostream>

#include "book_listing_tests.h"
#include "book_tests.h"
#include "parser_tests.h"
#include "util_tests.h"

int main() {
  std::cout << "   Running Unit Tests" << "\n\n";

  util_tests::Run();
  book_tests::Run();
  book_listing_tests::Run();
  book_listing_tests::Run();
  parser_tests::Run();

  std::cout << "PASSED ALL TESTS" << '\n';
}
