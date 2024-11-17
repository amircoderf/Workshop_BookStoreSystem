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

    while ((ch = _getch()) != '\r') { // Read until Enter(\r) is pressed
        if (ch == '\b') { 
            if (!input.empty()) {
                cout << "\b \b"; // Remove character from output
                input.pop_back(); // Remove last character from string
            }
        }
        else {
            input.push_back(ch);
            cout << '*'; 
        }
    }
    cout << endl;
    return input;
}
