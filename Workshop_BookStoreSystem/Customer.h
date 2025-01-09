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
    MYSQL* conn;
    string ic_no, phone_no, name, address, cus_username, cus_password;
    int userID;

public:
    void setUserId(int userID);
    void setDBConnection(MYSQL* conn);
    void CustomerRegistration(MYSQL* conn);
    void CustomerInteface();
    void ViewBooks();
    void OrderCart();
    void chooseBooksToOrder();
    void searchBooks();
    void sortBooks();
    void confirmOrder();
    void adjustItemQuantity();
    void addOrUpdateBookInOrder(int orderID, int bookID, int quantity, double price);
    void updateBookStock(int bookID, int quantity);
    void myProfile();
    void viewPastOrder();
    void EditProfile();
    //update book quantity soon tm
};

#endif


