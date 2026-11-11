#include "../lib/StockMarketSimulator.h"
#include "../lib/Utils.h"
#include "../lib/models/Account.h"
#include "../lib/models/Stock.h"
#include "../lib/services/GameDatabaseManager.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

std::vector<Stock> StockMarketSimulator::stocks;

double StockMarketSimulator::simulateStockTick(double currentPrice) {
    // Create a RNG and a uniform distribution
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<> dis(-1.0, 1.0);

    // Generate a random percentage change between -1% and +1%
    double change = dis(gen);
    double newPrice = currentPrice * (1 + change / 100);  // Apply change to the current price
    return newPrice;
}

void StockMarketSimulator::updateStockHistory() {
	cout << "Updating stock history..." << endl;

	if (stocks.empty()) {
		stocks.push_back(Stock(1, "Apple Inc.", "AAPL"));
		stocks.push_back(Stock(2, "Microsoft Corporation", "MSFT"));
		stocks.push_back(Stock(3, "Alphabet Inc.", "GOOGL"));
		stocks.push_back(Stock(4, "Amazon.com Inc.", "AMZN"));
		stocks.push_back(Stock(5, "Tesla Inc.", "TSLA"));

		for (auto& stock : stocks) {
			stock.history.push_back({ Utils::getCurrentTimestampStr(), 100, 0 });
		}
		return;
	}
	// Get the last update timestamp and the current timestamp
	string lastUpdateTimestampStr = GameDatabaseManager::GetLastUpdateStockTimestampStr();
	auto lastUpdateTimestamp = Utils::convertTimestampStrToChrono(lastUpdateTimestampStr);
	auto currentTimestamp = Utils::convertTimestampStrToChrono(Utils::getCurrentTimestampStr());

	// Calculate time difference in seconds
	auto timeDiff = chrono::duration_cast<chrono::seconds>(currentTimestamp - lastUpdateTimestamp).count();

	cout << "Last update timestamp: " << lastUpdateTimestampStr << endl;
	cout << "Updating last update timestamp to current time: " << Utils::convertChronoToTimestampStr(currentTimestamp) << endl;
	cout << "Time since last update: " << timeDiff << " seconds." << endl;

	// Simulate stock ticks for the time difference
	for (auto& stock : stocks) {
		for (int i = 0; i < timeDiff; ++i) {
			double newPrice = simulateStockTick(stock.history.back().price);
			stock.history.push_back({ Utils::convertChronoToTimestampStr(currentTimestamp), newPrice, 0 });
		}
	}

	GameDatabaseManager::UpdateLastUpdateStockTimestamp(Utils::convertChronoToTimestampStr(currentTimestamp));
}

void StockMarketSimulator::PurchaseStockOnAccount(Account* account, int stockID, int amountOfShares) {
    // find the stock
	auto it = std::find_if(stocks.begin(), stocks.end(), [stockID](const Stock& stock) {
		return stock.id == stockID;
	});
    // if doesnt exist, return
	if (it == stocks.end()) {
		Utils::ThrowFatalError("Stock with ID " + std::to_string(stockID) + " does not exist.");
	}

	// check if the account has enough money
	double totalCost = it->history.back().price * amountOfShares;
	if (account->balance < totalCost) {
		Utils::ThrowFatalError("Insufficient funds in account to purchase stock.");
	}

	account->balance -= totalCost;

	double price = it->history.back().price;
	auto holding = std::find_if(account->portfolio.begin(), account->portfolio.end(),
		[stockID](const Holding& share) { return share.stockID == stockID; });
	if (holding == account->portfolio.end()) {
		account->portfolio.push_back(Holding { stockID, amountOfShares, price });
	} else {
		int totalQuantity = holding->quantity + amountOfShares;
		holding->averageCost = (holding->quantity * holding->averageCost + amountOfShares * price) / totalQuantity;
		holding->quantity = totalQuantity;
	}
}

void StockMarketSimulator::SellStockOnAccount(Account* account, int stockID, int amountOfShares) {
// find the stock
	auto it = std::find_if(stocks.begin(), stocks.end(), [stockID](const Stock& stock) {
		return stock.id == stockID;
	});
	// if doesnt exist, return
	if (it == stocks.end()) {
		Utils::ThrowFatalError("Stock with ID " + std::to_string(stockID) + " does not exist.");
	}
}
