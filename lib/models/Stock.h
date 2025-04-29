#pragma once

#include <string>
#include <vector>
#include "StockEntry.h"

using namespace std;

class Stock
{
public:
	int id;
	std::string name;
	std::string symbol;
	vector<StockEntry> history;

	Stock(int id, const std::string& name, const std::string& symbol)
		: id(id), name(name), symbol(symbol) {}

	void addHistoryEntry(const StockEntry& entry) {
		history.push_back(entry);
	}
};

