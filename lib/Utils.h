#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

namespace Utils
{
    template <typename... Args>
    void printChoices(Args... args) {
        int index = 1;
        ((cout << index++ << ". " << args << endl), ...); // what C++ wizardry is this?
    }
    string getCInput();
    string getCInput(string prompt, bool hideInput = false);
    int getIntCInput();
    int getIntCInput(string prompt);
    vector<std::string> splitString(const std::string& input, char delimiter);

    template <typename... Args>
    void ThrowFatalError(Args... args) {
        system("cls");
        ((std::cout << args << " "), ...) << endl;
        cin.get();
        exit(1);
    }

    string formatMoney(double amount);
    string formatGainLoss(double gain, double cost);

    string getCurrentTimestampStr();

    chrono::system_clock::time_point convertTimestampStrToChrono(const std::string& timestamp);
    string convertChronoToTimestampStr(std::chrono::system_clock::time_point timePoint);
};

