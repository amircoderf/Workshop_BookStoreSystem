#include <iostream>
#include <cstring>
#include <iomanip>
#include <conio.h>
#include "SharedFunc.h"
#include "Customer.h"
#include "Admin.h"
#include "DBConnection.h"


using namespace std;

//object decalaration
db_connection dbConn;
Customer customer;
Admin admin;

void LogIn(MYSQL* conn);

int main() {
    dbConn.ConnectionFunction(); 
    int choice;

    do {
        system("cls");
        setConsoleTextColor(9);  // Blue for main menu
        cout << "======================================\n";
        cout << "      WELCOME TO OUR BOOKSTORE\n";
        cout << "======================================\n\n";
        setConsoleTextColor(7);  // Default color
        cout << "1. Registration\n2. Log in\n3. Exit" << endl<<endl;
        cout << "Enter your choice: ";

        while (true) {
            if (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number between 1 and 3: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else if (choice < 1 || choice > 3) {
                cout << "Invalid choice. Please enter a number between 1 and 3: ";
            }
            else {
                break;
            }
        }

        if (choice == 1) {
            customer.CustomerRegistration(dbConn.conn);
        }

        else if (choice == 2) {
            LogIn(dbConn.conn);
        }
        else if (choice == 3) {
            cout << "Exiting..." << endl;
        }
    } while (choice != 3);

    dbConn.CloseConnection();
    return 0;
}

void LogIn(MYSQL* conn) {
    string username, password;
    while (true) {
        system("cls");
        setConsoleTextColor(14);  // Yellow color for header
        cout << "=============================\n";
        cout << "  WELCOME TO THE LOGIN PAGE\n";
        cout << "=============================\n\n";
        setConsoleTextColor(7);  // Default color
        // Prompt for username
        cout << "Username: ";
        cin >> username;

        // Query to check username and retrieve password and role
        string query = "SELECT password, Role, UserID FROM User WHERE username = '" + username + "'";
        if (mysql_query(conn, query.c_str())) {
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        dbConn.res = mysql_store_result(conn);
        if (!dbConn.res) {
            cerr << "Error fetching results. Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        string correctPassword, role;
        int userId = -1;
        bool userExists = false;

        if (mysql_num_rows(dbConn.res) > 0) {
            dbConn.row = mysql_fetch_row(dbConn.res);
            correctPassword = dbConn.row[0]; // Retrieve password
            role = dbConn.row[1];           // Retrieve role
            userId = atoi(dbConn.row[2]);   // Retrieve UserID,atoi string->integer
            userExists = true;
        }

        mysql_free_result(dbConn.res);

        // Prompt for password
        cout << "Password: ";
        password = getHiddenInput();
        bool userPassword = BCrypt::validatePassword(password, correctPassword);

        // Validate login credentials
        if (userExists && userPassword) {
            cout << "\nLogin successful!" << endl;

            if (role == "admin") {
                admin.AdminInterface(conn);
            }
            else if (role == "customer") {
                customer.setDBConnection(conn);
                customer.setUserId(userId);
                customer.CustomerInterface(); // Pass UserID for customer actions
            }
            else {
                cout << "Unknown role. Please contact the administrator." << endl;
            }

            break; // Exit the loop on successful login
        }
        else {
            setConsoleTextColor(12); // Red text for error message
            cout << "\nInvalid username or password. Please try again." << endl;
            setConsoleTextColor(7); // Reset to default text color
            _getch();
        }
    }
}











