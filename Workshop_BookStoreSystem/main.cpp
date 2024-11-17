#include <iostream>
#include <cstring>
#include <iomanip>
#include <conio.h>
#include "Util.h"
#include "Customer.h"
#include "DBConnection.h"

using namespace std;

//object decalaration
db_connection dbConn;
Customer customer;

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

        // Check if the username exists
        string query = "SELECT * FROM User WHERE username = '" + username + "'";
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);

        if (qstate) {
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            break;
        }

        dbConn.res = mysql_store_result(conn);
        if (!dbConn.res) {
            cerr << "Result Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            break;
        }

        if (mysql_num_rows(dbConn.res) == 0) {
            cout << "Username does not exist. Please try again." << endl;
            _getch();
            continue; 
        }

        // Username found, now check the password
        while (true) {
            cout << "Password: ";
            password = getHiddenInput();

            query = "SELECT * FROM User WHERE username = '" + username + "' AND password = '" + password + "'";
            const char* q = query.c_str();
            qstate = mysql_query(conn, q);

            if (qstate) {
                cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
                _getch();
                break;
            }

            dbConn.res = mysql_store_result(conn);
            if (!dbConn.res) {
                cerr << "Result Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
                _getch();
                break;
            }

            if (mysql_num_rows(dbConn.res) == 1) {
                loginSuccessful = true;
                break;
            }
            else {
                cout << "Password is wrong. Please try again." << endl;
                _getch();
            }
        }

        if (loginSuccessful)
            break; // Exit login loop
    }

    // Check role after successful login
    query = "SELECT Role FROM User WHERE username = '" + username + "'";  // Query for Role
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    if (qstate) {
        cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        return;
    }

    dbConn.res = mysql_store_result(conn);
    if (dbConn.res == NULL) {
        cerr << "Result Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        return;
    }

    dbConn.row = mysql_fetch_row(dbConn.res);
    if (dbConn.row) {
        string role = dbConn.row[0];  

        if (role == "admin") {
            cout << "Welcome, Admin " <<username<< endl;
        }
        else if (role == "customer") {
            cout << "Welcome,"<<username<< endl;
        }
        else {
            cout << "Unknown role." << endl;
            _getch();
            return;
        }
    }
    else {
        cout << "Username not found." << endl;
    }
}










