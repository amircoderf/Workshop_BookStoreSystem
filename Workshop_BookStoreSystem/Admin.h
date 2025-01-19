#ifndef ADMIN_H
#define ADMIN_H

//books and admins functions
#include "DBConnection.h"
#include "SharedFunc.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <sstream>
#include <regex>
#include <iomanip>
#include "SharedFunc.h"

using namespace std;

class Admin {
private:
	db_connection dbConn;
public:
	void AdminInterface(MYSQL* conn);
	void BookManagementMenu(MYSQL* conn);
	void AddBooks(MYSQL* conn);
	void UpdateBooks(MYSQL* conn,const string& bookid);
	void DeleteBooks(MYSQL* conn, const string& bookid);
	void SalesReport(MYSQL* conn);
	bool DisplayBooks(MYSQL* conn);
	void SearchBooks(MYSQL* conn);
	void MonthlySalesReport(MYSQL* conn);
	void BookSalesReport(MYSQL* conn);
	void BookSalesInMonth(MYSQL* conn);
	void CustomerManagementMenu(MYSQL* conn);
	void ViewCustomerInfo(MYSQL* conn);
	void ViewCustomerOrders(MYSQL* conn);
};


#endif 

