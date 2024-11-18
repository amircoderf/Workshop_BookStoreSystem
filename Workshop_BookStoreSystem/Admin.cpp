#include "Admin.h"

void Admin::AdminInterface(MYSQL* conn) {
    system("cls");
	int choice;
	cout << "Welcome, ADMIN " <<endl<<endl;
	cout << "1.Add Books\n2.Update Books\n3.Delete Books\n4.View Customer Information\n5.View Sales Report\n0.Exit" << endl<<endl;
	cout << "What would you like to do? ";

    while (true) {
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else if (choice < 0 || choice > 5) {
            cout << "Invalid input. Please enter a number between 1 and 5: ";
        }
        else {
            break;
        }
    }

    switch (choice) {
    case 0:
        return;
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
    default:
        break;
    }
}

void Admin::AddBooks(MYSQL* conn) {
    string isbn, title, author, publisher, publishedYear;
    double price;
    int stock;

    do {
        system("cls");

        // ISBN Input
        while (true) {
            cout << "ISBN: ";
            cin >> isbn;
            if (isNumeric(isbn)) {
                break;
            }
            cout << "Invalid input. Please enter only numbers for ISBN." << endl;
        }

        // Title Input
        cout << "Title of the book: ";
        cin.ignore();
        getline(cin, title);

        // Price Input
        while (true) {
            cout << "Price of the book (RM): ";
            cin >> price;

            if (!cin.fail() && price >= 0) {
                break;
            }
            cin.clear(); 
            cin.ignore(); 
            cout << "Invalid input. Please enter a valid price." << endl;
        }

        // Stock Input
        while (true) {
            cout << "Stock: ";
            cin >> stock;

            if (!cin.fail() && stock >= 0) {
                break;
            }
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Please enter a valid stock value." << endl;
        }

        cout << "Author of the book: ";
        cin.ignore();
        getline(cin, author);

        cout << "Publisher of the book: ";
        getline(cin, publisher);

        while (true) {
            cout << "Published Year: ";
            cin >> publishedYear;

            if (isNumeric(publishedYear) && publishedYear.length() == 4) {
                break;
            }
            cout << "Invalid input. Please enter a valid 4-digit year." << endl;
        }


        string insert_query ="INSERT INTO BOOK (ISBN, Title, Price, Stock, Author, Publisher, PublishedYear) "
                             "VALUES ('" + isbn + "', '" + title + "', " + to_string(price) + ", " + to_string(stock) + ", '" + author + "', '" + publisher + "', '" + publishedYear + "')";


        if (mysql_query(conn, insert_query.c_str()) == 0) {
            cout << "\nBook has been successfully added to the database." << endl;
        }
        else {
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
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
    DisplayBooks(conn);


}


void Admin::DeleteBooks(MYSQL* conn) {
    int bookid;

    while (true) {
        system("cls");
        DisplayBooks(conn);
        cout << "\nEnter the book ID of the book you want to delete (0 to quit): ";
        cin >> bookid;

        // Exit the loop if the user inputs 0
        if (bookid == 0) {
            AdminInterface(conn);
        }

        string delete_query = "DELETE FROM BOOK WHERE BookID = " + to_string(bookid);
        const char* deleteQ = delete_query.c_str();

        if (!mysql_query(conn, deleteQ)) {
            cout << "Book with book ID " << to_string(bookid) << " has been successfully deleted from the database." << endl;
            _getch();
        }
        else {
            cout << "Error deleting book. Error Code: " << mysql_errno(conn) << endl;
            _getch();
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
    AdminInterface(conn);
}


void Admin::DisplayBooks(MYSQL* conn) {
    system("cls");
    string query = "SELECT BookID, Title FROM BOOK";
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
    cout << "Books in the database:" << endl;

    while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
        cout << "Book ID: " << dbConn.row[0] << ", Title: " << dbConn.row[1] << endl;
    }
}
