#ifndef ADMIN_H
#define ADMIN_H

//books and admins functions
#include "DBConnection.h"
#include "SharedFunc.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>

using namespace std;

class Admin {
private:
	db_connection dbConn;
public:
	void AdminInterface(MYSQL* conn);
	void AddBooks(MYSQL* conn);
	void UpdateBooks(MYSQL* conn);
	void DeleteBooks(MYSQL* conn);
	void ViewCustomer(MYSQL* conn);
	void SalesReport(MYSQL* conn);
	void DisplayBooks(MYSQL* conn);
};


#endif 

