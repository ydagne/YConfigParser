#include <iostream>

#define USE_ORDERED_MAP

#include "YConfParser.hpp"


int main()
{
    ylibs::yconfparser::configList config = ylibs::yconfparser::parseFile("config-sample.txt");
    
    ylibs::yconfparser::printConfiguration(config);
    
    return 0;
}
