#ifndef ADMIN_H
#define ADMIN_H

#include "DBConnection.h"
#include <iostream>
#include <iomanip>
#include <conio.h>

class Admin {
private:
	db_connection dbConn;
public:
	void AdminInterface(MYSQL* conn,string username);
	void AddBooks(MYSQL* conn);
	void UpdateBooks(MYSQL* conn);
	void DeleteBooks(MYSQL* conn);
	void ViewCustomer(MYSQL* conn);
	void SalesReport(MYSQL* conn);
};


#endif 

