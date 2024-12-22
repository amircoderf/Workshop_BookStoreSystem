#include "SharedFunc.h"

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

    while ((ch = _getch()) != '\r') { // Read until Enter (\r) is pressed

        // Special keys like F1 generate a prefix (0 or -32), read and ignore the next character
        if (ch == '\0' || ch == -32) {
            _getch();
        }
        else if (ch == '\b') {
            if (!input.empty()) {
                cout << "\b \b"; // Remove character from output
                input.pop_back(); // Remove last character from string
            }
        }
        else if (isalnum(ch) || ispunct(ch)) { // Allow letters, numbers, and special characters
            input.push_back(ch);
            cout << '*';
        }
        // Ignore other invalid characters
    }
    cout << endl;
    return input;
}

void setConsoleTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}