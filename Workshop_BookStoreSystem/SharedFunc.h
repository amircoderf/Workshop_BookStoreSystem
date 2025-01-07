#ifndef SHAREDFUNC_H
#define SHAREDFUNC_H

#include <iostream>
#include <conio.h>
#include <cctype>
#include <tabulate/table.hpp>
#include <Windows.h>

using namespace tabulate;
using namespace std;

// Function declaration
bool isNumeric(const string& str);
string getHiddenInput();
void setConsoleTextColor(int color);
void customerTableFormat(Table table);//view customer table
void reportTableFormat(Table table);
void cartTableFormat(Table table);

#endif