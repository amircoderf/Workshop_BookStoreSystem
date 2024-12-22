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
        cout << "1. Registration\n2. Log in\n3. Exit" << endl;
        cout << "Please enter your choice: ";

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
    string username, password,query;
    int qstate;

    while (true) {
        system("cls");
        cout << "Log In" << endl;
        cout << "Username: ";
        cin >> username;

        // Check username and retrieve password and role
        string query = "SELECT password, Role FROM User WHERE username = '" + username + "'";
        if (mysql_query(conn, query.c_str())) {
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        dbConn.res = mysql_store_result(conn);

        string correctPassword, role;
        bool userExists = false;

        if (dbConn.res && mysql_num_rows(dbConn.res) > 0) {
            dbConn.row = mysql_fetch_row(dbConn.res);
            correctPassword = dbConn.row[0]; // Retrieve password
            role = dbConn.row[1];           // Retrieve role
            userExists = true;
        }

        // Always ask for the password regardless of username existence
        cout << "Password: ";
        password = getHiddenInput();

        // Validate login credentials
        if (userExists && password == correctPassword) {

            // Redirect user based on role
            if (role == "admin") {
                admin.AdminInterface(conn);
            }
            else if (role == "customer") {
                cout << "Welcome, " << username << "!" << endl;
            }
            else {
                cout << "Unknown role. Please contact the administrator." << endl;
            }

            break;//break the while loop without speciying whether it's true or nor
        }

        else {
            setConsoleTextColor(12); // 12 is the color code for red text
            cout << "\nYou have entered an invalid username or password." << endl;
            setConsoleTextColor(7); // Reset to default (7 is the standard console text color)
            _getch();
        }
    }

}











