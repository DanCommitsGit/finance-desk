#pragma once


#include <random>
#include <vector>
#include "models/Stock.h"
#include "../lib/models/Account.h"

using namespace std;

class StockMarketSimulator
{
public:
    static vector<Stock> stocks;

    static double simulateStockTick(double currentPrice);

    // will check how much time has passed and update the stock records accordingly
    static void updateStockHistory();

    static void PurchaseStockOnAccount(Account* account, int stockID, int amountOfShares);
    static void SellStockOnAccount(Account* account, int stockID, int amountOfShares);
};

