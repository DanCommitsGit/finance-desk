#pragma once
#include <string>
#include "../SQLiteDB.h"
#include "../models/Transaction.h"
#include "../models/Account.h"
#include "../../include/sqlite3.h"
#include <optional>
#include "../models/Loan.h"
#include "../models/Notification.h"
#include "../models/Stock.h"
#include "../models/Holding.h"

using namespace std;

namespace GameDatabaseManager
{
    extern SQLiteDB* db;

    void initializeDatabase();
    optional<Account> loginToAccount(const string& username, const string& password);
    Account* getAccount(const string &username);
    Account* getAccount(int accountNumber);
    void saveAccount(Account account);
    bool createAccount(const string& firstName, const string& lastName, const string& username, const string& password);
    Transaction* TransferMoney(int fromAccountID, int toAccountID, double amount);
    vector<Transaction*> getTransactions(int accountID);
    vector<Loan*> getLoans(int accountID);
    vector<Notification*> getNotifications(int accountID);
    vector<Holding> getHoldings(int accountID);
    bool saveTrade(Account& account, int stockID, double balanceChange);

    vector<Stock> getStocks();
    void createStock(const Stock& stock);
    void addStockPrice(int stockID, const StockEntry& entry);

    void UpdateLastUpdateStockTimestamp(const string& timestamp);
    void UpdateLastSaveTimestamp(const string& timestamp);
    string GetLastUpdateStockTimestampStr();
    string GetLastSaveTimestamp();
    
}
