#include "Customer.h"
using namespace std;

void Customer::CustomerRegistration(MYSQL* conn) {
    system("cls");
    cout << "Customer Sign In" << endl;

    cin.ignore();
    cout << "Name: ";
    getline(cin, name);

    while (true) {
        cout << "IC no (12 digits): ";
        getline(cin, ic_no);
        if (ic_no.length() == 12 && isNumeric(ic_no)) {
            break;
        }
        cout << "Invalid input. Please enter exactly 12 digits for IC no." << endl;
    }

    while (true) {
        cout << "Phone no(Ex.:0123456789): ";
        getline(cin, phone_no);
        if (isNumeric(phone_no)) {
            break;
        }
        cout << "Invalid input. Please enter only numbers for Phone no." << endl;
    }

    cout << "Address: ";
    getline(cin, address);

    while (true) {
        cout << "Username: ";
        getline(cin, cus_username);

        if (cus_username.length() < 5) {
            cout << "Invalid input. Username must be at least 5 letters long." << endl;
            continue;
        }

        // Check if username already exists in the database
        string query = "SELECT COUNT(*) FROM user WHERE username = '" + cus_username + "'";
        const char* q = query.c_str();

        if (mysql_query(conn, q)) {
            cerr << "Error checking username. Error Code: " << mysql_errno(conn) << endl;
            continue; // Retry input
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "Error fetching result. Error Code: " << mysql_errno(conn) << endl;
            continue; // Retry input
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        int count = atoi(row[0]);
        mysql_free_result(res);

        if (count > 0) {
            cout << "This username is already taken. Please try a different one." << endl;
        }
        else {
            // Username is unique
            break;
        }
    }

    cout << "Password: ";
    getline(cin, cus_password);//check 2 times,adjust later

    string insert_query ="INSERT INTO USER (Name, IC_no, Phone_no, Address, username, password, Role) "
                         "VALUES ('" + name + "', '" + ic_no + "', '" + phone_no + "', '" + address +"', '" + cus_username + "', '" + cus_password + "', 'customer')";

    const char* q = insert_query.c_str();

    int qstate = mysql_query(conn, q);

    if (!qstate) {
        cout << endl << "Customer has been successfully added to the database." << endl;
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }
    _getch();
}

void Customer::CustomerInteface(MYSQL* conn) {
    int choice;

    do {
        system("cls");
        cout << "Welcome, ADMIN " << endl << endl;
        cout << "1. View Books\n2. View Order Cart\n0. Exit" << endl << endl;
        cout << "What would you like to do? ";

        // Input validation for menu choice
        while (true) {
            if (!(cin >> choice) || choice < 0 || choice > 3) {
                cout << "Invalid input. Please enter a number between 0 and 6: ";
                cin.clear();  // Clear the error flag
                cin.ignore(10000, '\n');  // Ignore any invalid input
            }
            else {
                break;  // Valid input, break the loop
            }
        }

        switch (choice) {
        case 0:
            return;  // Exit the program
        case 1:
            ViewBooks(conn);
            break;
        case 2:
            OrderCart(conn);
            break;
            break;
        default:
            break;
        }
    } while (choice != 0);
}

void Customer::ViewBooks(MYSQL* conn) {
    while (true) {
        system("cls");
        string query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book";
        const char* q = query.c_str();

        if (mysql_query(conn, q)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
        }

        dbConn.res = mysql_store_result(conn);

        if (dbConn.res == nullptr) {
            cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
            _getch();
        }

        // Check if there are any rows in the result set
        if (mysql_num_rows(dbConn.res) == 0) {
            cout << "There are no books in the database." << endl;
            mysql_free_result(dbConn.res);
        }

        cout << "===================================================================================" << endl;
        cout << "                              Books in the Database                                " << endl;
        cout << "===================================================================================" << endl << endl;

        int count = 1; // Counter for numbering books
        while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
            cout << "-----------------------------------------------------------------------------------" << endl;
            setConsoleTextColor(14);
            cout << " BOOK " << count++ << endl;
            setConsoleTextColor(7);
            cout << " BookID        : " << dbConn.row[0] << endl;
            cout << " ISBN          : " << dbConn.row[1] << endl;
            cout << " Title         : " << dbConn.row[2] << endl;
            cout << " Price         : RM" << dbConn.row[3] << endl;
            cout << " Author        : " << dbConn.row[4] << endl;
            cout << " Publisher     : " << dbConn.row[5] << endl;
            cout << " Published Year: " << dbConn.row[6] << endl;
            cout << "-----------------------------------------------------------------------------------" << endl;
            cout << "\n\n";
        }

        mysql_free_result(dbConn.res);



    }
}


void Customer::OrderCart(MYSQL* conn) {

}








