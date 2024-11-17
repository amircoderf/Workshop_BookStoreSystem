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

    string insert_query = "INSERT INTO CUSTOMER(Name, IC_no, Phone_no, Address, cus_username, cus_password,Role) ""VALUES ('" + name + "', '" + ic_no + "', '" + phone_no + "', '" + address + "', '" + cus_username + "', '" + cus_password + "')";
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




