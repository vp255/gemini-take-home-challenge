#include <iostream>
#include <stdio.h>
#include "book_listing.h"
#include "parser.h"

int main() {
  std::cerr << "====== Match Engine =====" << std::endl;
  std::cout << '\n'; 
  std::string line;
  BookListing listing;
  Parser<BookListing> parser(listing);

  while (getline(std::cin, line)) {
    parser.parse(line);
  }
  std::cout << '\n'; 
  listing.printRestingOrdersInOrder();
  return 0;
}
