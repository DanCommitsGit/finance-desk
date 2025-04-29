#include "../lib/Utils.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;

string Utils::getCInput() {
    string input;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the newline character from the input buffer
    getline(cin, input);
    return input;
}

string Utils::getCInput(string prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

string Utils::getCInput(string prompt, bool hideInput = false) {
    cout << prompt;
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' = Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Erase character visually
            }
        }
        else {
            password += ch;
            cout << '*'; // Show asterisk instead of real character
        }
    }
    cout << endl;
    return password;
}

int Utils::getIntCInput() {
    string input;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the newline character from the input buffer
    getline(cin, input);
    return stoi(input);
}

int Utils::getIntCInput(string prompt) {
    cout << prompt;
    string input;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the newline character from the input buffer
    getline(cin, input);
    return stoi(input);
}

std::vector<std::string> Utils::splitString(const std::string& input, char delimiter) {
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> result;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

string Utils::getCurrentTimestampStr() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    struct tm timeinfo;
    gmtime_s(&timeinfo, &in_time_t);

    stringstream ss;
    ss << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

chrono::system_clock::time_point Utils::convertTimestampStrToChrono(const std::string& timestamp) {
    std::tm tm = {};
    std::istringstream ss(timestamp);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse timestamp: " + timestamp);
    }

    // Convert to time_t and then to time_point
    std::time_t time = _mkgmtime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

string Utils::convertChronoToTimestampStr(std::chrono::system_clock::time_point timePoint) {
    std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);
    std::tm tm = {};
    gmtime_s(&tm, &timeT);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
