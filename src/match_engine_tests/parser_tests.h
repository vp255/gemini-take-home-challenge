#ifndef PARSER_TESTS_H
#define PARSER_TESTS_H

namespace parser_tests {

void VerifyCorrectParsing() {

    BookListing listing;
    Parser<BookListing> parser(listing);
    parser.parse("12345 BUY BTCUSD 5 10000");
    parser.parse("zod42 SELL BTCUSD 2 10001");

    auto it = const_cast<const OrderBook*>(&listing.listing.at("BTCUSD"))->getRestingOrders<Side::Buy>().begin();
    assert(it->first.price == 10000);
    assert(it->second.size == 5);
    assert(it->second.orderid == "12345");
    auto it2 = const_cast<const OrderBook*>(&listing.listing.at("BTCUSD"))->getRestingOrders<Side::Sell>().begin();
    assert(it2->first.price == 10001);
    assert(it2->second.size == 2);
    assert(it2->second.orderid == "zod42");
}

void Run() {
    std::cout << "Verifying Parser.parse()" << '\n';
    VerifyCorrectParsing();
}


}

#endif
