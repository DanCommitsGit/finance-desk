#include "../../lib/ui/ApplicationPages.h"
#include "../../lib/services/Authentication.h"
#include "../../lib/services/GameDatabaseManager.h"
#include "../../lib/models/Account.h"
#include "../../lib/models/Transaction.h"
#include "../../lib/models/Stock.h"
#include "../../lib/models/Loan.h"
#include "../../lib/models/Notification.h"
#include "../../lib/services/GameContext.h"
#include "../../lib/Utils.h"
#include "../../lib/ui/ANSI.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <optional>
#include "../../lib/StockMarketSimulator.h"

using namespace std;


// Options: 1. Login, 2. Signup
void ApplicationPages::StartPage() {
    while (true) {
        cout << "Welcome to the Bank!" << endl;
        Utils::printChoices("Login", "Signup");
        switch (Utils::getIntCInput()) {
        case 1: {
            ApplicationPages::LoginPage();
            break;
        }
        case 2: {
            ApplicationPages::SignupPage();
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
}

void ApplicationPages::LoginPage() {
    while (true) {
        string username = Utils::getCInput("Enter username:\n>>> ");
        if (username.empty()) {
            cout << "Username cannot be empty." << endl;
            continue;
        }

        cout << "Enter password: ";
        string password = Utils::getCInput("Enter password:\n>>> ", true);

        optional<Account> account = GameDatabaseManager::loginToAccount(username, password);
        if (account) {
            GameContext::currentLoggedInAccount = account;
            break;
        }

        cout << "Invalid username or password." << endl;
    }
    ApplicationPages::MainApplicationFlow();
}

void ApplicationPages::SignupPage() {
    string firstName = Utils::getCInput("Enter first name: \n>>> ");
    string lastName = Utils::getCInput("Enter last name: \n>>> ");
    string username = Utils::getCInput("Enter username: \n>>> ");
    string password = Utils::getCInput("Enter password: \n>>> ");
    if (GameDatabaseManager::createAccount(firstName, lastName, username, password)) {
        cout << "Account created successfully!" << endl;
    }
    else {
        cout << "Error creating account." << endl;
    }
}

void ApplicationPages::MainApplicationFlow() {
    while (GameContext::currentLoggedInAccount.has_value()) {
        cout << "Welcome, " << GameContext::currentLoggedInAccount->firstName << "!" << endl;
        cout << "Your balance is: $" << GameContext::currentLoggedInAccount->balance << endl;
        Utils::printChoices("View Transactions", "Pay Someone", "View Notifications", "Manage Loans", "Manage Stocks", "Logout", "Spawn Moneys");
        switch (Utils::getIntCInput()) {
        case 1:
            ApplicationPages::ViewTransactionsPage();
            break;
        case 2:
            ApplicationPages::PaySomeonePage();
            break;
        case 3:
            ApplicationPages::ViewNotificationsPage();
            break;
        case 4:
            ApplicationPages::ManageLoansPage();
            break;
        case 5:
            ApplicationPages::ManageStocksPage();
            break;
        case 6: {
            // are you sure?
            cout << "Are you sure you want to logout? (y/n)" << endl;
            string choice = Utils::getCInput();
            if (choice == "y") {
                GameContext::currentLoggedInAccount = nullopt;
                cout << "Logged out successfully!" << endl;
            }
            else {
                cout << "Logout cancelled." << endl;
            }
            break;
        }
        case 7:
            cout << "Spawned 1000 moneys!" << endl;
            GameDatabaseManager::TransferMoney(GameContext::bankMainAccount->accountNumber, GameContext::currentLoggedInAccount->accountNumber, 1000);
            break;
        }
    }
}

void ApplicationPages::ViewTransactionsPage() {
    vector<Transaction*> transactions = GameDatabaseManager::getTransactions(GameContext::currentLoggedInAccount->accountNumber);
    if (transactions.empty()) {
		cout << "No transactions found." << endl;
	}
    else {
		cout << "Transactions:" << endl;
        for (const auto& transaction : transactions) {
            Account* fromAccount = GameDatabaseManager::getAccount(transaction->fromAccountID);
            Account* toAccount = GameDatabaseManager::getAccount(transaction->toAccountID);
			cout << "Transaction ID: " << transaction->transactionID << ", From: " << fromAccount->firstName << ", To: " << toAccount->firstName
				 << ", Amount: $" << transaction->amount << ", Timestamp: " << transaction->timestamp << endl;
		}
	}
}

void ApplicationPages::PaySomeonePage() {
    while (true) {
        Account* destinationAccount = nullptr;
        Utils::printChoices("Lookup by username", "Lookup by account number");
        int choice = Utils::getIntCInput();
        switch (choice) {
            case 1: {
                string username = Utils::getCInput("Enter username:\n>>> ");
                destinationAccount = GameDatabaseManager::getAccount(username);
                break;
            }
            case 2: {
                int accountNumber = Utils::getIntCInput("Enter account number:\n>>> ");
                destinationAccount = GameDatabaseManager::getAccount(accountNumber);
                break;
            }
        }
        if (!destinationAccount) {
            cout << "User not found." << endl;
            continue;
        }
        cout << "User found: " << destinationAccount->firstName << " " << destinationAccount->lastName << endl;
        double amount = Utils::getIntCInput("Enter amount to pay:\n>>> ");
        if (amount <= 0) {
			cout << "Invalid amount." << endl;
			continue;
		}
        GameDatabaseManager::TransferMoney(GameContext::currentLoggedInAccount->accountNumber, destinationAccount->accountNumber, amount);
        cout << "Payment of $" << amount << " sent to " << destinationAccount->firstName << " " << destinationAccount->lastName << endl;
        break;
    }
}

void ApplicationPages::ViewNotificationsPage() {
    vector<Notification*> notifications = GameDatabaseManager::getNotifications(GameContext::currentLoggedInAccount->accountNumber);
    if (notifications.empty()) {
		cout << "No notifications found." << endl;
	}
    else {
		cout << "Notifications:" << endl;
        for (const auto& notification : notifications) {
			cout << "Notification ID: " << notification->id 
                << ", Message: " << notification->message 
                << ", Timestamp: " << notification->timestamp 
                << endl;
		}
	}
}

void ApplicationPages::ManageLoansPage() {
    while (true) {
        cout << "My Loans:" << endl; // TODO: show owned loans and total loan value
        Utils::printChoices("View Loans", "Take out loan", "back");
        int loanAction;
        cin >> loanAction;
        switch (loanAction) {
            case 1: {
                vector<Loan*> loans = GameDatabaseManager::getLoans(GameContext::currentLoggedInAccount->accountNumber);
                if (loans.empty()) {
					cout << "No loans found." << endl;
				}
                else {
					cout << "Loans:" << endl;
                    for (const auto& loan : loans) {
						cout << "Loan ID: " << loan->loanID 
							<< ", Amount: $" << loan->amount 
							<< ", Interest Rate: " << loan->interestRate 
							<< "%, Start Date: " << loan->startDate 
							<< ", End Date: " << loan->endDate 
							<< ", Status: " << loan->status 
							<< endl;
					}
				}
                break;
            }
            case 2: {
				double amount = Utils::getIntCInput("Enter loan amount:\n>>> ");
				double interestRate = Utils::getIntCInput("Enter interest rate:\n>>> ");
				string startDate = Utils::getCInput("Enter start date (YYYY-MM-DD):\n>>> ");
				string endDate = Utils::getCInput("Enter end date (YYYY-MM-DD):\n>>> ");
				/*Loan* newLoan = new Loan(0, GameContext::currentLoggedInAccount->accountNumber, amount, interestRate, startDate, endDate);
				GameDatabaseManager::saveLoan(*newLoan);
				cout << "Loan taken out successfully!" << endl;*/
				break;
			}
        }
    }
}

void ApplicationPages::ManageStocksPage() {
    while (true) {
        cout << "My Portfolio:" << endl; // TODO: show owned stocks and total portfolio value
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
        }
    }
}
