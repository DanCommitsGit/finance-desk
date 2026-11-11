#include "../../lib/ui/ApplicationPages.h"
#include "../../lib/models/Account.h"
#include "../../lib/models/Loan.h"
#include "../../lib/services/GameContext.h"
#include "../../lib/services/GameDatabaseManager.h"
#include "../../lib/Utils.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
