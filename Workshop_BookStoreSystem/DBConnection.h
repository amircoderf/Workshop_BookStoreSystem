#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <chrono>
#include <thread>
#include <mysql.h>
#include <iostream>

using namespace std;
using namespace std::chrono;

class db_connection {
public:
    MYSQL* conn;   // Database connection instance    
    MYSQL_ROW row; 
    MYSQL_RES* res; 

    // Function to initialize connection
    void ConnectionFunction();

    // Function to close the connection
    void CloseConnection();
};

#endif

