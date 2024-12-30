#include "Admin.h"

void Admin::AdminInterface(MYSQL* conn) {
    string bookid;
    int choice;

    do {
        system("cls");
        cout << "Welcome, ADMIN " << endl << endl;
        cout << "1. Add Books\n2. Update Books\n3. Delete Books\n4. View Customer Information\n5. Search Books\n6. Sales Report\n0. Exit" << endl << endl;
        cout << "What would you like to do? ";

        // Input validation for menu choice
        while (true) {
            if (!(cin >> choice) || choice < 0 || choice > 6) {
                cout << "Invalid input. Please enter a number between 0 and 6: ";
                cin.clear();  // Clear the error flag
                cin.ignore(10000, '\n');  // Ignore any invalid input
            }
            else {
                break;  // Valid input, break the loop
            }
        }

        switch (choice) {
        case 0:
            return;  // Exit the program
        case 1:
            AddBooks(conn);  // Call AddBooks function
            break;
        case 2:
            // Option to select a book and update
            cout << "To update a book, you need to select one." << endl;

            // Display all books
            if (!DisplayBooks(conn)) {
                cout << "No books available to update." << endl;
                break; // Exit if there are no books in the database
            }

            while (true) {
                cout << "Enter the BookID of the book you want to update (or type '0' to cancel): ";
                cin >> bookid;

                if (bookid == "0") { // Allow user to cancel
                    cout << "Update operation canceled." << endl;
                    break;
                }

                // Check if the BookID exists
                string checkQuery = "SELECT COUNT(*) FROM book WHERE BookID = '" + bookid + "'";
                if (mysql_query(conn, checkQuery.c_str())) {
                    cerr << "Error checking BookID: " << mysql_error(conn) << endl;
                    break;
                }

                MYSQL_RES* res = mysql_store_result(conn);
                if (res == nullptr) {
                    cerr << "Error fetching result: " << mysql_error(conn) << endl;
                    break;
                }

                MYSQL_ROW row = mysql_fetch_row(res);
                int bookCount = row ? atoi(row[0]) : 0;
                mysql_free_result(res);

                if (bookCount > 0) {
                    system("cls");
                    UpdateBooks(conn, bookid);
                    break; 
                }
                else {
                    cout << "Error: No book found with the BookID '" << bookid << "'. Please try again." << endl;
                    cout << "------------------------------------------------------------------------------------" << endl;
                }
            }
            break;
        case 3:
            // Option to select a book and delete
            cout << "To delete a book, you need to select one." << endl;
            // Display all books
            if (!DisplayBooks(conn)) {
                cout << "No books available to delete." << endl;
                break; // Exit if there are no books in the database
            }

            while (true) {
                cout << "Enter the BookID of the book you want to delete (or type '0' to cancel): ";
                cin >> bookid;

                if (bookid == "0") { // Allow user to cancel
                    cout << "Delete operation canceled." << endl;
                    break;
                }

                // Check if the BookID exists
                string checkQuery = "SELECT COUNT(*) FROM book WHERE BookID = '" + bookid + "'";
                if (mysql_query(conn, checkQuery.c_str())) {
                    cerr << "Error checking BookID: " << mysql_error(conn) << endl;
                    break;
                }

                MYSQL_RES* res = mysql_store_result(conn);
                if (res == nullptr) {
                    cerr << "Error fetching result: " << mysql_error(conn) << endl;
                    break;
                }

                MYSQL_ROW row = mysql_fetch_row(res);
                int bookCount = row ? atoi(row[0]) : 0;
                mysql_free_result(res);

                if (bookCount > 0) {
                    system("cls");
                    DeleteBooks(conn, bookid);
                    break; // Exit the loop after a successful delete
                }
                else {
                    cout << "Error: No book found with the BookID '" << bookid << "'. Please try again." << endl;
                    cout << "------------------------------------------------------------------------------------" << endl;
                }
            }
            break;
        case 4:
            ViewCustomer(conn);
            break;
        case 5:
            SearchBooks(conn);
            break;
        case 6:
            SalesReport(conn);
            break;
        default:
            break;
        }
    } while (choice != 0);
}


void Admin::AddBooks(MYSQL* conn) {
    string isbn, title, author, publisher, publishedYear;
    double price;
    int stock;

    do {
        system("cls");

        while (true) {
            cout << "Enter ISBN :";
            cin >> isbn;

            // Check if input is numeric and exactly 13 digits long
            if (isNumeric(isbn) && isbn.length() == 13) {
                break; 
            }
            cout << "Invalid input. ISBN must be exactly 13 digits long." << endl;
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
        while (true) {
            cout << "Do you want to input another book? (y/n): ";
            cin >> choice;

            choice = tolower(choice);
            if (choice == 'y' || choice == 'n') {
                break; // Exit loop if valid input
            }

            cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << endl;
        }

        if (tolower(choice) == 'n') {
            break;
        }

    } while (true);

    AdminInterface(conn);
}

void Admin::UpdateBooks(MYSQL* conn, const string& bookid) {
    system("cls");
    string newValue;
    int fieldChoice;
    string choice;

    // Display the book details
    string query = "SELECT * FROM book WHERE BookID = '" + bookid + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "Error executing query: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr) {
        cerr << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_num_rows(res) == 0) {
        cout << "No book found with BookID: " << bookid << endl;
        mysql_free_result(res);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    cout << "+---------------------------------------------------------------------------------+" << endl;
    cout << "|                                                                                 |" << endl;
    cout << "| BookID        : " << row[0] << setw(65 - strlen(row[0])) << "   |" << endl;
    cout << "| ISBN          : " << row[1] << setw(65 - strlen(row[1])) << "   |" << endl;
    cout << "| Title         : " << row[2] << setw(65 - strlen(row[2])) << "   |" << endl;
    cout << "| Price         : RM" << row[3] << setw(65 - strlen(row[3]) - 2) << "   |" << endl;
    cout << "| Author        : " << row[4] << setw(65 - strlen(row[4])) << "   |" << endl;
    cout << "| Publisher     : " << row[5] << setw(65 - strlen(row[5])) << "   |" << endl;
    cout << "| Published Year: " << row[6] << setw(65 - strlen(row[6])) << "   |" << endl;
    cout << "|                                                                                 |" << endl;
    cout << "+---------------------------------------------------------------------------------+" << endl;
    mysql_free_result(res);

    do {
        while (true) {
            cout << "\nSelect the field you want to update or enter 0 to quit:" << endl;
            cout << "1. Title" << endl;
            cout << "2. Price" << endl;
            cout << "3. Stock" << endl;
            cout << "4. Author" << endl;
            cout << "5. Publisher" << endl;
            cout << "6. Published Year" << endl;
            cout << "Enter your choice (1-6, or 0 to quit): ";
            cin >> fieldChoice;

            if (cin.fail() || fieldChoice < 0 || fieldChoice > 6) {
                cin.clear();  // Clear input error flags
                cin.ignore(1000, '\n');  // Discard invalid input
                cerr << "Invalid input. Please enter a number between 0 and 6." << endl<<endl;
            }
            else {
                break;  // Valid input
            }
        }

        if (fieldChoice == 0) {
            cout << "Exiting update process." << endl;
            return;
        }

        string field;
        switch (fieldChoice) {
        case 1: field = "Title"; break;
        case 2: field = "Price"; break;
        case 3: field = "Stock"; break;
        case 4: field = "Author"; break;
        case 5: field = "Publisher"; break;
        case 6: field = "PublishedYear"; break;
        }

        // Handle input for the selected field
        while (true) {
            cout << "Enter the new value for " << field << " (or press Enter to cancel): ";
            cin.ignore();
            getline(cin, newValue);

            if (newValue.empty()) {
                cout << "Cancelled update for this field." << endl;
                break;
            }

            // Validate Price
            if (field == "Price") {
                try {
                    double price = stod(newValue); // Convert string into double
                    stringstream stream;
                    stream << fixed << setprecision(2) << price;
                    if (stream.str() != newValue) {
                        cerr << "Error: Price must be a numeric value with up to 2 decimal places. Please enter again." << endl;
                        continue; // Ask for price again
                    }
                }
                catch (const exception&) {
                    cerr << "Error: Price must be a numeric value with up to 2 decimal places. Please enter again." << endl;
                    continue; // Ask for price again
                }
            }
            // Validate Stock and Published Year
            else if (field == "Stock" || field == "PublishedYear") {
                try {
                    int number = stoi(newValue); // Convert to int
                    if (to_string(number) != newValue) {
                        cerr << "Error: This field must be a whole number. Please enter again." << endl;
                        continue; // Ask for stock again
                    }
                }
                catch (const std::exception&) {
                    cerr << "Error: This field must be a whole number. Please enter again." << endl;
                    continue; // Ask for stock again
                }
            }

            // Confirmation step
            cout << "You entered: " << field << " = " << newValue << ". Do you want to confirm this update? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                string updateQuery = "UPDATE book SET " + field + " = '" + newValue + "' WHERE BookID = '" + bookid + "'";

                if (mysql_query(conn, updateQuery.c_str()) == 0) {
                    cout << field << " updated successfully!" << endl;
                }
                else {
                    cerr << "Error updating book: " << mysql_error(conn) << endl;
                }
                break; // Exit the input loop after successful update
            }
            else {
                cout << "Update for " << field << " has been cancelled." << endl;
                continue; // Ask for the field again
            }
        }

        // Ask to update another field
        while (true) {
            cout << "Do you want to update another attribute in the SAME book? (y/n): ";
            cin >> choice;

            if (choice == "y" || choice == "n") {
                break;
            }

            cerr << "Invalid input. Please enter 'y' for yes or 'n' for no." << endl;
        }

        if (choice == "n") {
            cout << "Exiting update process." << endl;
            return;
        }
        cout << "---------------------------------------------------------------------------";

    } while (true);
}


void Admin::DeleteBooks(MYSQL* conn,const string& bookid) {
    // Check if the book exists
    string checkQuery = "SELECT BookID FROM book WHERE BookID = '" + bookid + "'";
    const char* checkQ = checkQuery.c_str();

    if (mysql_query(conn, checkQ)) {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr) {
        cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return;
    }

    if (mysql_num_rows(res) == 0) {
        cout << "No book found with the given ID: " << bookid << endl;
        mysql_free_result(res);
        return;
    }

    // Free the result set as the book does exist
    mysql_free_result(res);

    char confirmation;
    cout << "Are you sure you want to delete the book with ID " << bookid << "? (y/n): "<<endl;
    cin >> confirmation;

    if (tolower(confirmation) != 'y') {
        cout << "Deletion canceled. Press any key to continue...\n";
        _getch();
        return;
    }

    string deleteQuery = "DELETE FROM book WHERE BookID = '" + bookid + "'";
    const char* deleteQ = deleteQuery.c_str();

    if (!mysql_query(conn, deleteQ)) {
        if (mysql_affected_rows(conn) > 0) {
            cout << "Book with BookID " << bookid << " has been successfully deleted from the database." << endl;
        }
        else {
            cout << "No changes made. Book might have already been deleted." << endl;
        }
    }
    else {
        cout << "Error deleting book. Error Code: " << mysql_errno(conn) << endl;
        _getch();
    }
}


void Admin::SalesReport(MYSQL* conn) {
    cout << "soon";
    _getch();
}

void Admin::ViewCustomer(MYSQL* conn) {
    system("cls");
    cout << "\n\t\t\t\t--- List of All Customers ---\n" << endl;
    
    //mysql_query's return type is int
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

    cout << "\nPress Any Key To Go Back...";
    _getch();
}

bool Admin::DisplayBooks(MYSQL* conn) {
    system("cls");
    string query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book";
    const char* q = query.c_str();

    if (mysql_query(conn, q)) {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return false;
    }

    dbConn.res = mysql_store_result(conn);

    if (dbConn.res == nullptr) {
        cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return false;
    }

    // Check if there are any rows in the result set
    if (mysql_num_rows(dbConn.res) == 0) {
        cout << "There are no books in the database." << endl;
        mysql_free_result(dbConn.res);
        return false;
    }

    cout << "===================================================================================" << endl;
    cout << "                              Books in the Database                                " << endl;
    cout << "===================================================================================" << endl << endl;

    int count = 1; // Counter for numbering books
    while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
        cout << "-----------------------------------------------------------------------------------" << endl;
        setConsoleTextColor(14);
        cout << " BOOK " << count++ << endl;
        setConsoleTextColor(7);
        cout << " BookID        : " << dbConn.row[0] << endl;
        cout << " ISBN          : " << dbConn.row[1] << endl;
        cout << " Title         : " << dbConn.row[2] << endl;
        cout << " Price         : RM" << dbConn.row[3] << endl;
        cout << " Author        : " << dbConn.row[4] << endl;
        cout << " Publisher     : " << dbConn.row[5] << endl;
        cout << " Published Year: " << dbConn.row[6] << endl;
        cout << "-----------------------------------------------------------------------------------" << endl;
        cout << "\n\n";
    }

    mysql_free_result(dbConn.res);
    return true; // Books were found
}

void Admin::SearchBooks(MYSQL* conn) {
    system("cls");
    string searchValue;
    string query;
    char retryChoice;

    while (true) {
        system("cls");
        cout << "+---------------------------------------------+" << endl;
        cout << "|                Search Books                 |" << endl;
        cout << "+---------------------------------------------+" << endl;
        cout << "Enter search value (ID, ISBN, or Title) or '0' to quit: ";
        cin.ignore();
        getline(cin, searchValue);

        if (searchValue == "0") {
            return;
        }

        if (searchValue.empty()) {
            cout << "Search value cannot be empty." << endl;
            _getch();
            continue;
        }

        if (isdigit(searchValue[0])) {
            if (searchValue.length() == 13) {
                query = "SELECT * FROM book WHERE ISBN = '" + searchValue + "'";
            }
            else {
                query = "SELECT * FROM book WHERE BookID = '" + searchValue + "'";
            }
        }
        else {
            query = "SELECT * FROM book WHERE Title LIKE '%" + searchValue + "%'";
        }

        const char* q = query.c_str();
        if (mysql_query(conn, q)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        if (mysql_num_rows(res) == 0) {
            cout << "No books found matching your criteria." << endl;

            while (true) {
                cout << "Would you like to try again? (y/n): ";
                cin >> retryChoice;

                if (retryChoice == 'y' || retryChoice == 'Y' || retryChoice == 'n' || retryChoice == 'N') {
                    break;
                }
                cout << "Invalid input! Please enter 'y' for yes or 'n' for no." << endl;
            }

            if (retryChoice == 'n' || retryChoice == 'N') {
                mysql_free_result(res);
                cout << "Exiting search..." << endl;
                _getch();
                return;
            }

            mysql_free_result(res);
            continue;
        }

        cout << "\n\n";
        cout << "+---------------------------------------------------------------------------------+" << endl;
        cout << "|                                 Search Results                                  |" << endl;
        cout << "+---------------------------------------------------------------------------------+" << endl;

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr) {
            cout << "+---------------------------------------------------------------------------------+" << endl;
            cout << "| BookID        : " << row[0] << setw(65 - strlen(row[0])) << "   |" << endl;
            cout << "| ISBN          : " << row[1] << setw(65 - strlen(row[1])) << "   |" << endl;
            cout << "| Title         : " << row[2] << setw(65 - strlen(row[2])) << "   |" << endl;
            cout << "| Price         : RM" << row[3] << setw(65 - strlen(row[3]) - 2) << "   |" << endl;
            cout << "| Author        : " << row[4] << setw(65 - strlen(row[4])) << "   |" << endl;
            cout << "| Publisher     : " << row[5] << setw(65 - strlen(row[5])) << "   |" << endl;
            cout << "| Published Year: " << row[6] << setw(65 - strlen(row[6])) << "   |" << endl;
            cout << "+---------------------------------------------------------------------------------+" << endl;
        }

        mysql_free_result(res);

        string bookId;
        cout << "\nEnter the BookID of the book you want to manage (or '0' to quit): ";
        cin >> bookId;

        if (bookId == "0") {
            cout << "Exiting search..." << endl;
            return;
        }

        query = "SELECT * FROM book WHERE BookID = '" + bookId + "'";
        if (mysql_query(conn, query.c_str())) {
            cout << "Error fetching book details. Error Code: " << mysql_errno(conn) << endl;
            continue;
        }

        res = mysql_store_result(conn);
        if (res == nullptr || mysql_num_rows(res) == 0) {
            cout << "Invalid BookID. Please try again." << endl;
            mysql_free_result(res);
            continue;
        }

        MYSQL_ROW selectedRow = mysql_fetch_row(res);
        cout << "You selected Book: " << selectedRow[2] << "\n"; // Display title

        cout << "What would you like to do with this book?" << endl;
        cout << "1. Update Book" << endl;
        cout << "2. Delete Book" << endl;
        cout << "Enter your choice: ";

        int action;
        cin >> action;

        switch (action) {
        case 1:
            UpdateBooks(conn, selectedRow[0]);
            break;
        case 2:
            DeleteBooks(conn, selectedRow[0]);
            break;
        default:
            cout << "Invalid action!" << endl;
            break;
        }

        mysql_free_result(res);

        while (true) {
            cout << "Would you like to search again? (y/n): ";
            cin >> retryChoice;

            if (retryChoice == 'y' || retryChoice == 'Y' || retryChoice == 'n' || retryChoice == 'N') {
                break;
            }
            cout << "Invalid input! Please enter 'y' for yes or 'n' for no." << endl;
        }

        if (retryChoice == 'n' || retryChoice == 'N') {
            return;
        }
    }
}







