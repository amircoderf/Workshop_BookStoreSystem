#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Function to check if a string contains only digits(Used in Customer and login)
bool isNumeric(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

#endif 

