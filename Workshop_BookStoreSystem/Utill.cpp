#include "Util.h"
#include <iostream>
#include <conio.h>
#include <cctype>

using namespace std;

bool isNumeric(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

string getHiddenInput() {
    string input;
    char ch;

    while ((ch = _getch()) != '\r') { // Read until Enter is pressed
        if (ch == '\b') { // Handle backspace
            if (!input.empty()) {
                cout << "\b \b"; // Remove character from console
                input.pop_back(); // Remove last character from string
            }
        }
        else {
            input.push_back(ch);
            cout << '*'; // Display * for each character
        }
    }
    cout << endl;
    return input;
}
