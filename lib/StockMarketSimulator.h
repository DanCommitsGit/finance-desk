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
    // returns false when the account does not own enough shares or the trade could not be saved
    static bool SellStockOnAccount(Account* account, int stockID, int amountOfShares);
};

