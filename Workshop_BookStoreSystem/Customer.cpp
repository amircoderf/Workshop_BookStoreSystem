#include "Customer.h"
using namespace std;


void Customer::Customer_SignIn(MYSQL* conn) {
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
        if (cus_username.length() >= 5) {
            break;
        }
        cout << "Invalid input.Username must be at least 5 letters long:";
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





