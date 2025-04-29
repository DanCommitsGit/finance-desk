#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "../SQLiteDB.h"
#include "Share.h"

using namespace std;

class Account
{
public:
	int accountNumber;
	string username;
	string firstName;
	string lastName;
	double balance;
	std::vector<Share> portfolio;

	Account(int accountNumber, const string& firstName, const string& lastName, const string& username, double balance) : accountNumber(accountNumber), firstName(firstName), lastName(lastName), username(username), balance(balance) {}

	void deposit(double amount);
};

