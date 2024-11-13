#include <iostream>
#include <cstring>
#include <iomanip>
#include <conio.h>
#include "Customer.h"
#include "DBConnection.h"

using namespace std;

int main() {
    db_connection dbConn;
    int choice;
    Customer customer;

    dbConn.ConnectionFunction();

    do {
        system("cls");

        cout << "1. Customer\n2. Admin\n3. Exit" << endl << endl;
        cout << "Please enter your choice: ";

        while (true) {
            if (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number between 1 and 3: ";
                cin.clear();
                cin.ignore(10000,'\n');
            }
            else if (choice < 1 || choice > 3) {
                cout << "Invalid choice. Please enter a number between 1 and 3: ";
            }
            else {
                break;
            }
        }

        switch (choice) {
        case 1:
            customer.CustomerInterface(dbConn.GetConnection());
            break;
        case 2:
            customer.View_Customer(dbConn.GetConnection());
            break;
        case 3:
            cout << "Exiting..." << endl;
            break; 
        default:
            break;//alreacy check choice
        }
    } while (choice != 3); // Repeat until the user chooses to exit

    dbConn.CloseConnection();
    return 0;
}






