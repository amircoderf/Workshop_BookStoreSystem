#include "Admin.h"

void Admin::AdminInterface(MYSQL* conn) {
    system("cls");
	int choice;
	cout << "Welcome, Admin " <<endl<<endl;
	cout << "1.Add Books\n2.Update Books\n3.Delete Books\n4.View Customer Information\n5.View Sales Report" << endl<<endl;
	cout << "What would you like to do? ";

    while (true) {
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else if (choice < 1 || choice > 5) {
            cout << "Invalid input. Please enter a number between 1 and 5: ";
        }
        else {
            break;
        }
    }

    switch (choice) {
    case 1:
        AddBooks(conn);
        break;
    case 2:
        UpdateBooks(conn);
        break;
    case 3:
        DeleteBooks(conn);
        break;
    case 4:
        ViewCustomer(conn);
        break;
    case 5:
        SalesReport(conn);
        break;
    deafult:
        break;
    }
}

void Admin::AddBooks(MYSQL* conn) {
    string isbn, title, genre, author, publisher, publishedYear,price,stock;

    do {
        system("cls");

        while (true) {
            cout << "ISBN: ";
            cin >> isbn;

            if (isNumeric(isbn)) {
                break;
            }
            cout << "Invalid input. Please enter only numbers for ISBN." << endl;
        }

        cout << "Title of the book: ";
        cin.ignore();
        getline(cin, title);

        while (true) {
            cout << "Price of the book: RM ";
            cin >> price;

            if (isNumeric(price)) {
                break;
            }
            cout << "Invalid input. Please enter the correct price." << endl;
        }

        while (true) {
            cout << "Stocks:  ";
            cin >> stock;

            if (isNumeric(stock)) {
                break;
            }
            cout << "Invalid input. Please enter the correct price." << endl;
        }

        cout << "Genre: ";
        cin >> genre;

        cout << "Author of the book: ";
        cin.ignore();
        getline(cin, author);

        cout << "Publisher of the book: ";
        cin.ignore();
        getline(cin, publisher);

        while (true) {
            cout << "Published Year: ";
            cin >> publishedYear;

            if (isNumeric(publishedYear)) {
                break;
            }
            cout << "Please enter the correct published year." << endl;
        }


        string insert_query = "INSERT INTO BOOK(ISBN, Title, Price,Stock, Genre, Author, Publisher, PublishedYear) "
                              "VALUES('" + isbn + "','" + title + "'," + price +","+stock+ ",'" + genre + "','" + author + "','" + publisher + "','" + publishedYear + "')";

        const char* q = insert_query.c_str();
        int qstate = mysql_query(conn, q); 

        if (!qstate) {
            cout << endl << "Book has been successfully added to the database." << endl;
        }
        else {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        }

        char choice;
        cout << "Do you want to input another book? (y/n): ";
        cin >> choice;

        if (tolower(choice) == 'n') {
            break;
        }

    } while (true);
    AdminInterface(conn);
}

void Admin::UpdateBooks(MYSQL* conn) {
    cout << "soon";
    _getch();
}

void Admin::DeleteBooks(MYSQL* conn) {
    int bookNumber;

    while (true) {
        system("cls");
        DisplayBooks(conn);
        cout << "Enter the number of the book you want to delete (0 to quit): ";
        cin >> bookNumber;

        // Exit the loop if the user inputs 0
        if (bookNumber == 0) {
            AdminInterface(conn);
        }

        // Query to get the title of the selected book by book number
        string query = "SELECT TITLE FROM BOOK LIMIT 1 OFFSET " + to_string(bookNumber - 1);
        const char* q = query.c_str();

        dbConn.res = mysql_store_result(conn);
        if (dbConn.res == NULL) {
            cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
            return;
        }


        dbConn.row = mysql_fetch_row(dbConn.res);
        if (dbConn.row == NULL) {
            cout << "Invalid book number." << endl;
            mysql_free_result(dbConn.res);
            continue; //loop again ,ignore others
        }

        string Title = dbConn.row[0];
        mysql_free_result(dbConn.res);

        string delete_query = "DELETE FROM BOOK WHERE TITLE = '" + Title + "'";
        const char* deleteQ = delete_query.c_str();

        if (!mysql_query(conn, deleteQ)) {
            cout << "Book with Title " << Title << " has been successfully deleted from the database." << endl;
            _getch();
        }
        else {
            cout << "Error deleting book. Error Code: " << mysql_errno(conn) << endl;
        }
    }
}


void Admin::SalesReport(MYSQL* conn) {
    cout << "soon";
    _getch();
}

void Admin::ViewCustomer(MYSQL* conn) {
    system("cls");
    cout << "\n\t\t\t\t--- List of All Customers ---\n" << endl;

    int qstate = mysql_query(conn, "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER where Role='customer'");

    if (!qstate) {
        cout << "+-----------------------------------------------------------------------------------------------------------------------+" << endl;
        cout << "| UserID | Name                 | IC No           | Phone No        | Address                         | Username        |" << endl;
        cout << "+-----------------------------------------------------------------------------------------------------------------------+" << endl;

  
        dbConn.res = mysql_store_result(conn);
        while (dbConn.row = mysql_fetch_row(dbConn.res)) {
            cout << "| " << setw(6) << left << dbConn.row[0] 
                << " | " << setw(20) << left << dbConn.row[1] 
                << " | " << setw(15) << left << dbConn.row[2] 
                << " | " << setw(15) << left << dbConn.row[3] 
                << " | " << setw(25) << left << dbConn.row[4] 
                << " | " << setw(15) << left << dbConn.row[5] 
                << " |" << endl;
        }

 
        cout << "+-----------------------------------------------------------------------------------------------------------------------+" << endl;
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }

    cout << "\nPress Enter To Go Back...";
    _getch();
}


void Admin::DisplayBooks(MYSQL* conn) {
    string query = "SELECT Title FROM BOOK"; 
    const char* q = query.c_str();

    if (mysql_query(conn, q)) {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        return;
    }

    dbConn.res = mysql_store_result(conn);
    if (dbConn.res == NULL) {
        cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
        return;
    }

    dbConn.row;
    int count = 1;
    cout << "Books in the database:" << endl;
    while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
        cout << count << ".Title: " << dbConn.row[0] << endl;
        count++;
    }
}