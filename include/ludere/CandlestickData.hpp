//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_CANDLESTICKDATA_HPP
#define LUDERE_CANDLESTICKDATA_HPP


#include <stdint.h>

namespace lud {

struct CandlestickCandle
{
    float open;
    float high;
    float low;
    float close;
    uint32_t volume;

    float timestamp;
};

}


#endif //LUDERE_CANDLESTICKDATA_HPP
