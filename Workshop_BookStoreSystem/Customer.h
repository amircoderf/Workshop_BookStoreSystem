#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "DBConnection.h"
#include "SharedFunc.h"
#include <iomanip>
#include <conio.h>
#include <string>
#include <sstream>
#include <vector>
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
    void adjustItemQuantity(MYSQL* conn);
    void addOrUpdateBookInOrder(MYSQL* conn, int orderID, int bookID, int quantity, double price);
    void myProfile(MYSQL* conn);
    void viewPastOrder(MYSQL* conn);
    //update book quantity soon tm
};

#endif


