#include "Customer.h"
#include "Util.h"
#include <iomanip>
#include <conio.h>
using namespace std;


void Customer::Customer_SignIn(MYSQL* conn) {
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
        if (cus_username.length() >= 5) {
            break;
        }
        cout << "Invalid input.Username must be at least 5 letters long:";
    }

    cout << "Password: ";
    getline(cin, cus_password);//check 2 times,adjust later

    string insert_query = "INSERT INTO CUSTOMER(Name, IC_no, Phone_no, Address, cus_username, cus_password) ""VALUES ('" + name + "', '" + ic_no + "', '" + phone_no + "', '" + address + "', '" + cus_username + "', '" + cus_password + "')";
    const char* q = insert_query.c_str();

    int qstate = mysql_query(conn, q);

    if (!qstate) {
        cout << endl << "Customer has been successfully added to the database." << endl;
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }
    _getch();
    CustomerInterface(conn);
}

void Customer::View_Customer(MYSQL* conn) {
    cout << "\t\t\t\t--- List of All Customer ---" << endl;

    int qstate = mysql_query(conn, "SELECT CustomerID, Name,IC_no,Phone_no,Address,cus_username FROM CUSTOMER");

    if (!qstate)
    {
        cout << setw(6) << "CustomerID" << setw(7) << "Name" << setw(20) << "IC_No" << setw(15) << "Phone_No" << setw(15) << "Address" <<setw(10)<<"Customer Username"<< endl;

        dbConn.res = mysql_store_result(conn);
        while (dbConn.row = mysql_fetch_row(dbConn.res))
        {
            cout << setw(6) << dbConn.row[0] << setw(7) << dbConn.row[1] << setw(20) << dbConn.row[2] << setw(15) << dbConn.row[3] << setw(15) << dbConn.row[4]<<setw(10)<<dbConn.row[5] << endl;
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }

    cout << "Press Enter To Go Back...";
    _getch();
}

void Customer::Customer_LogIn(MYSQL* conn) {
    while (true) {
        cout << "Username: ";
        cin >> cus_username;


        string query = "SELECT * FROM CUSTOMER WHERE cus_username = '" + cus_username + "'";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (qstate) {
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        //store the result in res
        dbConn.res = mysql_store_result(conn);
        if (!dbConn.res) {
            cerr << "Query Execution Problem!Error Code:  " << mysql_error(conn) <<endl;
            return;
        }

        if (mysql_num_rows(dbConn.res) == 1) {
            break;
        }
        else {
            cout << "Username does not exist. Please try again.";
            _getch();
            system("cls");
        }        
    }

    while (true) {
        system("cls");
        cout << "Username: " << cus_username << endl;
        cout << "Password: ";
        cin >> cus_password;


        string query = "SELECT * FROM CUSTOMER WHERE cus_username = '" + cus_username + "' AND cus_password = '" + cus_password + "'";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (qstate) {
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        //store the result in res
        dbConn.res = mysql_store_result(conn);
        if (!dbConn.res) {
            cerr << "Query Execution Problem!Error Code:  " << mysql_error(conn) << endl;
            return;
        }

        if (mysql_num_rows(dbConn.res) == 1) {
            break;
        }
        else {
            cout << "Password is wrong. Please try again." << std::endl;
            _getch();
        }

        cout << "In prgoress.......";
        _getch();
        return;
    }
}

void Customer::CustomerInterface(MYSQL* conn) {
    system("cls");
    int choice;
    cout << "1.Sign in\n2.Log in\n3.Back" << endl;
    cout << "Please enter your choice:";

    while (true) {
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter a number between 1 and 3: ";
        }
        else {
            break;
        }
    }

    if (choice == 1) {
        system("cls");
        Customer_SignIn(conn);
    }
    else if (choice == 2) {
        system("cls");
        Customer_LogIn(conn);
    }
    else {
        return;
    }
}

