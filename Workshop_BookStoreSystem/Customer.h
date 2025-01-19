#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "DBConnection.h"
#include "SharedFunc.h"
#include <iomanip>
#include <conio.h>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include "bcrypt/BCrypt.hpp"
using namespace std;

class Customer {
private:
    MYSQL* conn;
    string ic_no, phone_no, name, address, cus_username, cus_password;
    int user_id;

public:    
    void SetUserId(int user_id);
    void SetDBConnection(MYSQL* conn);
    void CustomerRegistration(MYSQL* conn);
    void CustomerInterface();
    void ViewBooks();
    void OrderCart();
    void ChooseBooksToOrder();
    void SearchBooks();
    void SortBooks();
    void ConfirmOrder();
    void AdjustItemQuantity();
    void AddOrUpdateBookInOrder(int order_id, int book_id, int quantity, double price);
    void UpdateBookStock(int book_id, int quantity);
    void MyProfile();
    void ViewPastOrder();
    void EditProfile();
    bool IsUsernameExists(const string& username);
};

#endif


