#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "DBConnection.h"
using namespace std;

class Customer {
private:
    db_connection dbConn;
    string ic_no, phone_no, name, address, cus_username, cus_password;

public:
    void CustomerInterface(MYSQL* conn);
    void Customer_LogIn(MYSQL* conn);
    void Customer_SignIn(MYSQL* conn);
    void View_Customer(MYSQL* conn);//this is for admin,test it on customer header file for now(change later)
};

#endif


