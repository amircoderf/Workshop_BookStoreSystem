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

public:
    void CustomerRegistration(MYSQL* conn);
};

#endif


