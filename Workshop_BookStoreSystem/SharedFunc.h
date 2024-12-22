#ifndef SHAREDFUNC_H
#define SHAREDFUNC_H

#include <iostream>
#include <conio.h>
#include <cctype>
#include <Windows.h>

using namespace std;

// Function declaration
bool isNumeric(const string& str);
string getHiddenInput();
void setConsoleTextColor(int color);

#endif