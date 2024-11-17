#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "DBConnection.h"
#include "SharedFunc.cpp"
#include <iomanip>
#include <conio.h>
#include <string>
using namespace std;

class Customer {
private:
    db_connection dbConn;
    string ic_no, phone_no, name, address, cus_username, cus_password;

public:
    void Customer_SignIn(MYSQL* conn);
    void View_Customer(MYSQL* conn);//this is for admin,test it on customer header file for now(change later)
};

#endif


