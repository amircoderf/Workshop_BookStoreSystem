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
        else if (ch == '\b') { // Backspace handling
            if (!input.empty()) {
                cout << "\b \b"; // Remove character from output
                input.pop_back(); // Remove last character from string
            }
        }
        else if (isalnum(ch) || ispunct(ch) || ch == ' ') { // Allow letters, numbers, special characters, and spaces
            input.push_back(ch);
            cout << '*'; // Mask input with asterisks
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

//view customer table
void customerTableFormat(Table table)
{
    size_t index = 0;

    table[0].format()
        .font_color(Color::grey)
        .font_background_color(Color::yellow)
        .padding_top(0.5)
        .padding_bottom(0.5);

    for (auto& row : table) {
        row.format()
            .font_style({ FontStyle::bold })
            .font_align(FontAlign::center)
            .border_top(" ")
            .border_bottom(" ")
            .border_left(" ")
            .border_right(" ")
            .corner(" ");

        // Set blue background color for alternate rows
        if (index > 0 && index % 2 == 0) {
            for (auto& cell : row) {
                cell.format()
                    .font_background_color(Color::blue);
            }
        }
        index += 1;
    }
    // End of table format
}

void reportTableFormat(Table table) {
    size_t index = 0;

    // Header formatting
    table[0].format()
        .font_color(Color::white)
        .font_background_color(Color::yellow)  // Blue background for the header
        .font_style({ FontStyle::bold })     // Bold text
        .font_align(FontAlign::center)      // Center alignment for the header
        .padding_top(1.0)
        .padding_bottom(1.0);

    // Row formatting for data rows
    size_t rowIndex = 0;
    for (auto& row : table) {
        if (rowIndex > 0) {  // Skip the header row
            row.format()
                .border_top("-")
                .border_bottom("-")
                .border_left("|")
                .border_right("|")
                .corner("+");

            for (auto& cell : row) {
                cell.format()
                    .font_background_color(Color::grey)  // Yellow background for data cells
                    .font_color(Color::white)              // Grey text color
                    .font_align(FontAlign::left)          // Left-align text
                    .padding_left(2.0)
                    .padding_right(2.0)
                    .padding_top(0.5)
                    .padding_bottom(0.5);
            }
        }
        rowIndex++;
    }
}


void cartTableFormat(Table table) {
    table.format()
        .font_style({ FontStyle::bold })
        .font_align(FontAlign::center)
        .border_top("-")
        .border_bottom("-")
        .border_left("|")
        .border_right("|")
        .corner("+");
    table[0].format().font_color(Color::white).font_background_color(Color::blue);
}

