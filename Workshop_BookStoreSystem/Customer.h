#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "DBConnection.h"
#include "SharedFunc.h"
#include <iomanip>
#include <conio.h>
#include <string>
using namespace std;

class Customer {
private:
    db_connection dbConn;
    string ic_no, phone_no, name, address, cus_username, cus_password;
    int userID;

public:
    void CustomerRegistration(MYSQL* conn);
    void CustomerInteface(MYSQL* conn,int userID);
    void ViewBooks(MYSQL* conn);
    void OrderCart(MYSQL* conn);
    void chooseBooksToOrder(MYSQL* conn);
    void searchBooks(MYSQL* conn);
    void sortBooks(MYSQL* conn);
    void confirmOrder(MYSQL* conn);
    void removeItemFromCart(MYSQL* conn);
    //update book quantity soon tm
};

#endif


