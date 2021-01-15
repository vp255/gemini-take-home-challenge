#ifndef PARSER_H
#define PARSER_H

#include "types.h"
#include "side.h"

template <class Listing>
class Parser {
    
public:
 
    Parser(Listing& listing)
       : listing(listing) {
    }
       
    void parse(const std::string& line) {
        iss.clear();
        iss.str(line);
        OrderID orderid;
        Side side;
        Instrument instrument;
        Quantity size;
        Price price;
        iss >> orderid
            >> side
            >> instrument
            >> size
            >> price;
        if (side == Side::Buy)
            listing.template addOrder<Side::Buy>(orderid, instrument, size, price);
        else 
            listing.template addOrder<Side::Sell>(orderid, instrument, size, price);
    }
private:
    Listing& listing;
    std::istringstream iss;
};

#endif
