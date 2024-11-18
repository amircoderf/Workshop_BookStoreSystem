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
            customer.Customer_SignIn(dbConn.conn);
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
    bool loginSuccessful = false;

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
            continue;
        }

        dbConn.res = mysql_store_result(conn);
        if (!dbConn.res || mysql_num_rows(dbConn.res) == 0) {
            cout << "Username does not exist. Please try again." << endl;
            _getch();
            continue;
        }

        dbConn.row = mysql_fetch_row(dbConn.res);
        string correctPassword = dbConn.row[0];
        string role = dbConn.row[1];

        while (true) {
            cout << "Password: ";
            password = getHiddenInput();

            if (password == correctPassword) {
                loginSuccessful = true;
                break;
            }
            else {
                cout << "Password is wrong. Please try again." << endl;
            }
        }

        if (loginSuccessful) {
            if (role == "admin") {
                admin.AdminInterface(conn);
            }
            else if (role == "customer") {
                cout << "Welcome, " << username << "!" << endl;
            }
            else {
                cout << "Unknown role. Please contact the administrator." << endl;
            }
            break;
        }
    }

}










