#include "Admin.h"

void Admin::AdminInterface(MYSQL* conn) {
    string bookid;
	int choice;

    do {
        system("cls");
        cout << "Welcome, ADMIN " << endl << endl;
        cout << "1.Add Books\n2.Update Books\n3.Delete Books\n4.View Customer Information\n5.Search Books\n6.Sales Report\n0.Exit" << endl << endl;
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
            DisplayBooks(conn);
            cout << "Enter the BookID of the book you want to update: ";
            cin >> bookid;
            // Call UpdateBooks with the selected BookID
            UpdateBooks(conn, bookid);
            break;
        case 3:
            DeleteBooks(conn);  
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
    string newValue;
    int fieldChoice;
    char choice;

    // Display the book details
    string query = "SELECT * FROM book WHERE BookID = '" + bookid + "'";
    if (mysql_query(conn, query.c_str())) {
        cout << "Error executing query: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr || mysql_num_rows(res) == 0) {
        cout << "No book found with BookID: " << bookid << endl;
        mysql_free_result(res);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    cout << "+-----------------------------------------------------------------------------------+" << endl;
    cout << "| BookID        : " << row[0] << endl;
    cout << "| ISBN          : " << row[1] << endl;
    cout << "| Title         : " << row[2] << endl;
    cout << "| Price         : RM" << row[3] << endl;
    cout << "| Author        : " << row[4] << endl;
    cout << "| Publisher     : " << row[5] << endl;
    cout << "| Published Year: " << row[6] << endl;
    cout << "+-----------------------------------------------------------------------------------+" << endl;

    mysql_free_result(res);

    // Now allow the admin to update fields
    do {
        do {
            cout << "\nSelect the field you want to update:" << endl;
            cout << "1. Title" << endl;
            cout << "2. Price" << endl;
            cout << "3. Stock" << endl;
            cout << "4. Author" << endl;
            cout << "5. Publisher" << endl;
            cout << "6. Published Year" << endl;
            cout << "Enter your choice (1-6): ";
            cin >> fieldChoice;

            if (fieldChoice < 1 || fieldChoice > 6) {
                cout << "Invalid choice. Please select a number between 1 and 6." << endl;
            }
        } while (fieldChoice < 1 || fieldChoice > 6);

        string field;
        switch (fieldChoice) {
        case 1: field = "Title"; break;
        case 2: field = "Price"; break;
        case 3: field = "Stock"; break;
        case 4: field = "Author"; break;
        case 5: field = "Publisher"; break;
        case 6: field = "PublishedYear"; break;
        }

        cout << "Enter the new value for " << field << ": ";
        cin.ignore();
        getline(cin, newValue);

        string updateQuery = "UPDATE book SET " + field + " = '" + newValue + "' WHERE BookID = '" + bookid + "'";

        if (mysql_query(conn, updateQuery.c_str()) == 0) {
            cout << field << " updated successfully!" << endl;
        }
        else {
            cout << "Error updating book: " << mysql_error(conn) << endl;
        }

        while (true) {
            cout << "Do you want to update another attribute in the SAME book? (y/n): ";
            cin >> choice;

            choice = tolower(choice);
            if (choice == 'y' || choice == 'n') {
                break; // Exit loop 
            }

            cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << endl;
        }

    } while (tolower(choice) == 'y');
}



void Admin::DeleteBooks(MYSQL* conn) {
    int bookid;

    while (true) {
        system("cls");

        bool hasBooks = DisplayBooks(conn);  // Check if there are any books
        if (!hasBooks) {
            cout << "\nPress Any Key to Go Back......";
            _getch(); 
            return;  
        }
        cout << "\nEnter the book ID of the book you want to delete (0 to return to Admin Menu): ";
        cin >> bookid;

        //if user enters 0,return
        if (bookid == 0) {
            return;
        }

        // Check if the book exists
        string checkQuery = "SELECT BookID FROM BOOK WHERE BookID = " + to_string(bookid);
        const char* checkQ = checkQuery.c_str();

        if (mysql_query(conn, checkQ)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        dbConn.res = mysql_store_result(conn);
        if (dbConn.res == nullptr) {
            cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        if (mysql_num_rows(dbConn.res) == 0) {
            cout << "No book found with the given ID. Please try again.\n";
            mysql_free_result(dbConn.res);
            continue; // loop again
        }

        // Free the result set as the book does exist
        mysql_free_result(dbConn.res);

        char confirmation;
        cout << "Are you sure you want to delete the book with ID " << bookid << "? (y/n): ";
        cin >> confirmation;

        if (tolower(confirmation) != 'y') {
            cout << "Deletion canceled. Press any key to continue...\n";
            _getch();
            continue; // loop again
        }

        string deleteQuery = "DELETE FROM BOOK WHERE BookID = " + to_string(bookid);
        const char* deleteQ = deleteQuery.c_str();

        if (!mysql_query(conn, deleteQ)) {

            if (mysql_affected_rows(conn) > 0) {
                cout << "Book with book ID " << bookid << " has been successfully deleted from the database." << endl;
            }
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
    string query = "SELECT ISBN, Title, Price, Author, Publisher, PublishedYear FROM book";
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
    cout << "===================================================================================" << endl<<endl;

    int count = 1; // Counter for numbering books
    while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
        cout << "-----------------------------------------------------------------------------------" << endl;
        setConsoleTextColor(14);
        cout << " BOOK " << count++ << endl;
        setConsoleTextColor(7);
        cout << " ISBN          : " << dbConn.row[0] << endl;
        cout << " Title         : " << dbConn.row[1] << endl;
        cout << " Price         : RM" << dbConn.row[2] << endl;
        cout << " Author        : " << dbConn.row[3] << endl;
        cout << " Publisher     : " << dbConn.row[4] << endl;
        cout << " Published Year: " << dbConn.row[5] << endl;
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
    int choice;

    while (true) {
        system("cls");
        cout << "+---------------------------------------------+" << endl;
        cout << "|                Search Books                 |" << endl;
        cout << "+---------------------------------------------+" << endl;
        cout << "Enter search value (ID, ISBN, or Title) or 'q' to quit: ";
        cin.ignore();
        getline(cin, searchValue);

        // Allow user to exit anytime by typing 'q'
        if (searchValue == "q" || searchValue == "Q") {
            cout << "Exiting search..." << endl;
            _getch();
            return;
        }

        if (searchValue.empty()) {
            cout << "Search value cannot be empty." << endl;
            _getch();
            continue;  // Restart the loop to let the user try again
        }

        // Determine if the input is numeric (ID or ISBN) or a string (Title)
        if (isdigit(searchValue[0])) {
            // Check if it's likely an ISBN or ID
            if (searchValue.length() == 13) { // Only 13-digit ISBN is allowed in the system
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

            // Ask if they want to retry or quit
            while (true) {
                cout << "Would you like to try again? (y/n) or press 'q' to quit: ";
                cin >> retryChoice;

                // Ensure retryChoice is either 'y', 'n' or 'q' for quit
                if (retryChoice == 'y' || retryChoice == 'Y' || retryChoice == 'n' || retryChoice == 'N' || retryChoice == 'q' || retryChoice == 'Q') {
                    break; 
                }
                else {
                    cout << "Invalid input! Please enter 'y' for yes, 'n' for no, or 'q' to quit." << endl;
                }
            }

            // If they want to quit, exit the loop
            if (retryChoice == 'n' || retryChoice == 'N' || retryChoice == 'q' || retryChoice == 'Q') {
                mysql_free_result(res);
                cout << "Exiting search..." << endl;
                _getch();
                return;
            }

            mysql_free_result(res);
            continue;
        }

        cout << "+-----------------------------------------------------------------------------------+" << endl;
        cout << "|                                 Search Results                                    |" << endl;
        cout << "+-----------------------------------------------------------------------------------+" << endl;
        int index = 1;
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr) {
            cout << "| " << index << endl;
            cout << "|    BookID        :"<<row[0]<<endl;
            cout << "|    ISBN          : " << row[1] << endl;
            cout << "|    Title         : " << row[2] << endl;
            cout << "|    Price         : RM" << row[3] << endl;
            cout << "|    Author        : " << row[4] << endl;
            cout << "|    Publisher     : " << row[5] << endl;
            cout << "|    Published Year: " << row[6] << endl;
            cout << "+-----------------------------------------------------------------------------------+" << endl;
            index++;
        }

        mysql_free_result(res);

        // Allow user to select a book by number or quit
        while (true) {
            cout << "\n\nEnter the number of the book you want to manage (1-" << index - 1 << "), or press 'q' to quit: "<<endl;
            string input;
            cin >> input;

            // If the user wants to quit
            if (input == "q" || input == "Q") {
                cout << "Exiting search..." << endl;
                _getch();
                return;
            }

            try {
                choice = stoi(input);
                if (choice >= 1 && choice < index) {
                    // Fetch the selected book's details using the index
                    MYSQL_ROW selectedRow = nullptr;
                    // Reset the result set cursor to the beginning
                    mysql_data_seek(res, choice - 1); // Move to the selected row (choice - 1 because index starts at 1)
                    selectedRow = mysql_fetch_row(res); // Fetch the selected row

                    if (selectedRow) {
                        cout << "You selected Book " << choice << ": " << selectedRow[2] << endl; // Title of the book

                        // Ask what action to perform
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
                            cout << "Delete logic for Book " << choice << endl;
                            // Call delete function here
                            break;
                        case 3:
                            cout << "Displaying more details for Book " << choice << endl;
                            // Display more details logic here
                            break;
                        default:
                            cout << "Invalid action!" << endl;
                            break;
                        }
                    }
                    else {
                        cout << "Error fetching the selected book details." << endl;
                    }
                    break;  // Exit the selection loop after processing the action
                }
                else {
                    cout << "Invalid number! Please enter a number between 1 and " << index - 1 << "." << endl;
                }
            }
            catch (invalid_argument& e) {
                cout << "Invalid input! Please enter a valid number or 'q' to quit." << endl;
            }
        }

        // Ask the user if they want to search again or quit
        while (true) {
            cout << "Would you like to search again? (y/n) or press 'q' to quit: ";
            cin >> retryChoice;

            if (retryChoice == 'y' || retryChoice == 'Y' || retryChoice == 'n' || retryChoice == 'N' || retryChoice == 'q' || retryChoice == 'Q') {
                break;
            }
            else {
                cout << "Invalid input! Please enter 'y' for yes, 'n' for no, or 'q' to quit." << endl;
            }
        }

        // If the user wants to quit, exit the loop
        if (retryChoice == 'n' || retryChoice == 'N' || retryChoice == 'q' || retryChoice == 'Q') {
            return;
        }
    }
}






