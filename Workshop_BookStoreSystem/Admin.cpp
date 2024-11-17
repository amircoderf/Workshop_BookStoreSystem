#include "Admin.h"

using namespace std;

void Admin::AdminInterface(MYSQL* conn,string username) {
    system("cls");
	int choice;
	cout << "Welcome, Admin " << username <<endl<<endl;
	cout << "1.Add Books\n2.Update Books\n3.Delete Books\n4.View Customer Information\n5.View Sales Report" << endl<<endl;
	cout << "What would you like to do? ";

    while (true) {
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else if (choice < 1 || choice > 5) {
            cout << "Invalid input. Please enter a number between 1 and 5: ";
        }
        else {
            break;
        }
    }

    switch (choice) {
    case 1:
        AddBooks(conn);
        break;
    case 2:
        UpdateBooks(conn);
        break;
    case 3:
        DeleteBooks(conn);
        break;
    case 4:
        ViewCustomer(conn);
        break;
    case 5:
        SalesReport(conn);
        break;
    deafult:
        break;
    }
}

void Admin::AddBooks(MYSQL* conn) {
    cout << "soon";
}

void Admin::UpdateBooks(MYSQL* conn) {
    cout << "soon";
}

void Admin::DeleteBooks(MYSQL* conn) {
    cout << "soon";
}

void Admin::SalesReport(MYSQL* conn) {
    cout << "soon";
}

void Admin::ViewCustomer(MYSQL* conn) {
    system("cls");
    cout << "\n\t\t\t\t--- List of All Customers ---\n" << endl;

    int qstate = mysql_query(conn, "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER where Role='customer'");

    if (!qstate) {
        // Print table header
        cout << "+-----------------------------------------------------------------------------------------------------------------------+" << endl;
        cout << "| UserID | Name                 | IC No           | Phone No        | Address                         | Username        |" << endl;
        cout << "+-----------------------------------------------------------------------------------------------------------------------+" << endl;

  
        dbConn.res = mysql_store_result(conn);
        while (dbConn.row = mysql_fetch_row(dbConn.res)) {
            cout << "| " << setw(6) << left << dbConn.row[0] 
                << " | " << setw(20) << left << dbConn.row[1] 
                << " | " << setw(15) << left << dbConn.row[2] 
                << " | " << setw(15) << left << dbConn.row[3] 
                << " | " << setw(25) << left << dbConn.row[4] 
                << " | " << setw(15) << left << dbConn.row[5] 
                << " |" << endl;
        }

 
        cout << "+-----------------------------------------------------------------------------------------------------------------------+" << endl;
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }

    cout << "\nPress Enter To Go Back...";
    _getch();
}