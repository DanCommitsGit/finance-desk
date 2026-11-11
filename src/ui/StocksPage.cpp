#include "../../lib/ui/ApplicationPages.h"
#include "../../lib/models/Account.h"
#include "../../lib/models/Stock.h"
#include "../../lib/services/GameContext.h"
#include "../../lib/StockMarketSimulator.h"
#include "../../lib/Utils.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static void printPortfolio() {
    const vector<Holding>& portfolio = GameContext::currentLoggedInAccount->portfolio;
    if (portfolio.empty()) {
        cout << "You don't own any stocks." << endl;
        return;
    }

    ostringstream table;
    table << left << setw(8) << "Symbol" << left << setw(8) << "Shares" << setw(12) << "Avg cost"
          << setw(12) << "Price" << setw(14) << "Value" << setw(22) << "Gain/Loss" << "\n";
    double totalValue = 0;
    double totalCost = 0;
    for (const Holding& holding : portfolio) {
        auto stock = find_if(StockMarketSimulator::stocks.begin(), StockMarketSimulator::stocks.end(), [&holding](const Stock& candidate) {
            return candidate.id == holding.stockID;
        });
        if (stock == StockMarketSimulator::stocks.end()) {
            continue;
        }
        double price = stock->history.back().price;
        double value = holding.quantity * price;
        double cost = holding.quantity * holding.averageCost;
        totalValue += value;
        totalCost += cost;
        table << left << setw(8) << stock->symbol << left << setw(8) << holding.quantity << setw(12) << Utils::formatMoney(holding.averageCost)
              << setw(12) << Utils::formatMoney(price) << setw(14) << Utils::formatMoney(value) << setw(22) << Utils::formatGainLoss(value - cost, cost) << "\n";
    }
    table << "Total value: " << Utils::formatMoney(totalValue) << "   Gain/Loss: " << Utils::formatGainLoss(totalValue - totalCost, totalCost) << "\n";
    cout << table.str();
}

void ApplicationPages::ManageStocksPage() {
    while (true) {
        StockMarketSimulator::updateStockHistory();
        printPortfolio();
        Utils::printChoices("View All Stocks", "back");
        int stockAction = Utils::getIntCInput();
        switch (stockAction) {
            case 1: {
                StockMarketSimulator::updateStockHistory();
			    vector<Stock> stocks = StockMarketSimulator::stocks;
			    if (stocks.empty()) {
				    cout << "No stocks found." << endl;
			    } else {
				    cout << "Stocks:" << endl;
				    for (const auto& stock : stocks) {
					    cout << "Stock ID: " << stock.id
						     << ", Name: " << stock.name
						     << ", Price: $" << stock.history.back().price
						     << endl;
				    }
                    cout << "Enter stock ID to buy or sell, or 0 to go back: ";
                    int stockID = Utils::getIntCInput();
                    if (stockID == 0) {
					    break;
				    }
                    // Find the stock by ID
                    auto it = find_if(stocks.begin(), stocks.end(), [stockID](const Stock& stock) {
						return stock.id == stockID;
					});
                    // if stock not found
                    if (it == stocks.end()) {
						cout << "Stock not found." << endl;
						continue;
					}
					Stock selectedStock = *it;
					cout << "Selected stock: " << selectedStock.name << " at $" << selectedStock.history.back().price << endl;
					cout << "Do you want to buy or sell? (b/s): ";
					char action = Utils::getCInput()[0];
					if (action == 'b') {
						int quantity = Utils::getIntCInput("Enter quantity to buy:\n>>> ");
						if (quantity <= 0) {
							cout << "Invalid quantity." << endl;
							continue;
						}
						double totalCost = selectedStock.history.back().price * quantity;
						if (GameContext::currentLoggedInAccount->balance < totalCost) {
							cout << "Insufficient balance." << endl;
							continue;
						}
                        StockMarketSimulator::PurchaseStockOnAccount(&GameContext::currentLoggedInAccount.value(), selectedStock.id, quantity);
						cout << "Bought " << quantity << " shares of " << selectedStock.name << "." << endl;
					} else if (action == 's') {
						int quantity = Utils::getIntCInput("Enter quantity to sell:\n>>> ");
						if (quantity <= 0) {
							cout << "Invalid quantity." << endl;
							continue;
						}
                        if (StockMarketSimulator::SellStockOnAccount(&GameContext::currentLoggedInAccount.value(), selectedStock.id, quantity)) {
							cout << "Sold " << quantity << " shares of " << selectedStock.name << "." << endl;
						}
						else {
							cout << "Failed to sell. You don't own that many shares." << endl;
						}
					} else {
					    cout << "Invalid action." << endl;
				    }
			    }
			    break;
            }
            case 2:
                return;
        }
    }
}
