#pragma once

#include <string>

using namespace std;

class StockEntry
{
public:
    string timestamp;
    double price;
    int volume;

    StockEntry(const std::string& ts, double p, int v)
		: timestamp(ts), price(p), volume(v) {}
};

