#include "../../lib/ui/ApplicationPages.h"
#include "../../lib/services/Authentication.h"
#include "../../lib/services/GameDatabaseManager.h"
#include "../../lib/models/Account.h"
#include "../../lib/models/Transaction.h"
#include "../../lib/models/Stock.h"
#include "../../lib/models/Notification.h"
#include "../../lib/services/GameContext.h"
#include "../../lib/Utils.h"
#include "../../lib/ui/ANSI.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <optional>

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
