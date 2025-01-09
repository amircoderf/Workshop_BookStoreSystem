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
            CustomerManagementMenu(conn);
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
    int choice;

    do {
        system("cls");
        cout << "\nChoose Report Type:\n";
        cout << "1. Monthly Sales Report\n";
        cout << "2. Total Book Sales Report\n";
        cout << "3. Book Sales in a Month\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Validate user input
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a valid number.\n";
            _getch();
            continue;
        }

        switch (choice) {
        case 1:
            MonthlySalesReport(conn);
            break;
        case 2:
            BookSalesReport(conn);
            break;
        case 3:
            BookSalesInMonth(conn);
            break;
        case 0:
            cout << "Exiting Sales Report menu...\n";
            break;
        default:
            cout << "Invalid choice. Please select a valid option.\n";
            _getch();
            break;
        }
    } while (choice != 0);
}

void Admin::MonthlySalesReport(MYSQL* conn) {
    string query = "SELECT DATE_FORMAT(o.orderDate, '%Y-%m') AS Sales_Month, "
        "SUM(bo.quantity) AS Total_Quantity_Sold, "
        "SUM(bo.price) AS Total_Sales "
        "FROM book_order bo "
        "JOIN `order` o ON bo.orderID = o.orderID "
        "WHERE o.orderStatus = 'completed' "
        "GROUP BY Sales_Month "
        "ORDER BY Sales_Month ASC;";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Query execution failed: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cerr << "Failed to store result: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    Table salesReportTable;
    salesReportTable.add_row({ "Sales Month", "Total Quantity Sold", "Total Sales(RM)", "Change in Total Sales (%)" });

    MYSQL_ROW row;
    double previousSales = 0.0; // To store the previous month's total sales

    while ((row = mysql_fetch_row(result))) {
        string salesMonth = row[0];
        string totalQuantitySold = row[1];
        double totalSales = atof(row[2]);
        string changePercent = "N/A"; // Default value for the first month

        // Calculate percentage change if there is a previous month's data
        if (previousSales > 0.0) {
            double change = ((totalSales - previousSales) / previousSales) * 100.0;
            std::ostringstream changeStream;

            // Add "increases by" or "decreases by" without negative signs
            if (change > 0) {
                changeStream << "increases by " << std::fixed << std::setprecision(2) << change << "%";
            }
            else if (change < 0) {
                changeStream << "decreases by " << std::fixed << std::setprecision(2) << fabs(change) << "%";
            }

            changePercent = changeStream.str();
        }

        // Format total sales to 2 decimal places
        std::ostringstream salesStream;
        salesStream << std::fixed << std::setprecision(2) << totalSales;

        salesReportTable.add_row({ salesMonth, totalQuantitySold, salesStream.str(), changePercent });
        previousSales = totalSales; // Update the previousSales for the next iteration
    }

    reportTableFormat(salesReportTable);
    cout << salesReportTable << endl;

    mysql_free_result(result);
    cout << "\nPress any key to return to the menu...";
    _getch();
}

void Admin::BookSalesReport(MYSQL* conn) {
    string query = "SELECT b.Title AS Book_Title, "
        "SUM(bo.quantity) AS Total_Quantity_Sold, "
        "SUM(bo.price) AS Total_Sales "
        "FROM book_order bo "
        "JOIN book b ON bo.BookID = b.BookID "
        "GROUP BY b.BookID "
        "ORDER BY Total_Sales DESC;";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Query execution failed: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cerr << "Failed to store result: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    Table bookSalesTable;
    bookSalesTable.add_row({ "Book Title", "Total Quantity Sold", "Total Sales" });

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        bookSalesTable.add_row({ row[0], row[1], row[2] });
    }

    reportTableFormat(bookSalesTable);
    cout << bookSalesTable << endl;

    mysql_free_result(result);
    cout << "\nPress any key to return to the menu...";
    _getch();
}

void Admin::BookSalesInMonth(MYSQL* conn) {
    string month;
    cout << "Enter the month for the report (format: YYYY-MM): ";
    cin >> month;

    regex month_format("^\\d{4}-(0[1-9]|1[0-2])$");
    if (!regex_match(month, month_format)) {
        cerr << "Invalid month format. Please use YYYY-MM." << endl;
        cout << "\nPress any key to return to the menu...";
        _getch();
        return;
    }

    string query = "SELECT b.Title AS Book_Title, "
        "SUM(bo.quantity) AS Total_Quantity_Sold, "
        "SUM(bo.price) AS Total_Sales "
        "FROM book_order bo "
        "JOIN book b ON bo.BookID = b.BookID "
        "JOIN `order` o ON bo.orderID = o.orderID "
        "WHERE o.orderStatus = 'completed' "
        "AND DATE_FORMAT(o.orderDate, '%Y-%m') = '" + month + "' "
        "GROUP BY b.BookID "
        "ORDER BY Total_Sales DESC;";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Query execution failed: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cerr << "Failed to store result: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    Table monthlyBookSalesTable;
    monthlyBookSalesTable.add_row({ "Book Title", "Total Quantity Sold", "Total Sales" });

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        monthlyBookSalesTable.add_row({ row[0], row[1], row[2] });
    }

    reportTableFormat(monthlyBookSalesTable);
    cout << monthlyBookSalesTable << endl;

    mysql_free_result(result);
    cout << "\nPress any key to return to the menu...";
    _getch();
}

void Admin::ViewCustomerInfo(MYSQL* conn) {
    system("cls");
    cout << "\n\t\t\t\t--- List of All Customers ---\n" << endl;

    int qstate = mysql_query(conn, "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer'");

    if (!qstate) {
        Table customerInformation;
        customerInformation.add_row({ "USER ID", "NAME", "IC NO", "PHONE NO", "ADDRESS", "USERNAME" });

        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res))) {
            customerInformation.add_row({ row[0], row[1], row[2], row[3], row[4], row[5] });
        }

        customerTableFormat(customerInformation);

        cout << customerInformation << endl;
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }

    cout << "\nPress Any Key To Go Back...";
    _getch();
}

void Admin::ViewCustomerOrders(MYSQL* conn, int customerID) {
    using namespace tabulate;
    system("cls");
    cout << "\n\t\t\t--- Completed Orders for Customer ID: " << customerID << " ---\n" << endl;

    // Query to fetch order details along with associated books
    string query = "SELECT o.orderID, o.orderDate, o.totalAmount, b.Title, bo.quantity, bo.price "
        "FROM `order` o "
        "JOIN `book_order` bo ON o.orderID = bo.orderID "
        "JOIN `book` b ON bo.BookID = b.BookID "
        "WHERE o.UserID = " + to_string(customerID) + " AND o.orderStatus = 'completed' "
        "ORDER BY o.orderID";

    int qstate = mysql_query(conn, query.c_str());

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        int currentOrderID = -1;
        Table orderTable;

        while ((row = mysql_fetch_row(res))) {
            int orderID = stoi(row[0]);

            // When a new order starts
            if (orderID != currentOrderID) {
                // Print the table for the previous order if it exists
                if (currentOrderID != -1) {
                    cout << orderTable << "\n\n";
                }

                cout << "+-------------------------------------------------------+\n";

                // Reset the table for the next order
                orderTable = Table();
                orderTable.add_row({ "Title", "Quantity", "Price (RM)" });
                orderTable[0]
                    .format()
                    .font_style({ FontStyle::bold })
                    .font_color(Color::green);

                currentOrderID = orderID;

                // Print order-level information
                cout << "Order ID: " << row[0]
                    << "\nOrder Date: " << row[1]
                    << "\nTotal Amount: RM " << row[2] << "\n";
                cout << "Books in this order:\n";
            }

            // Add book information to the table
            orderTable.add_row({
                row[3],                 // Book Title
                row[4],                 // Quantity
                row[5]                  // Price (RM)
                });
        }

        // Print the last order's table if it exists
        if (currentOrderID != -1) {
            cout << orderTable << "\n";
        }
        else {
            cout << "\nNo completed orders found for this customer.\n";
        }
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

void Admin::CustomerManagementMenu(MYSQL* conn) {
    int choice;
    do {
        system("cls");
        cout << "\n\t\t\t--- Customer Management ---\n";
        cout << "1. View All Customers\n";
        cout << "2. View Completed Orders of a Customer\n";
        cout << "3. Back to Admin Menu\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ViewCustomerInfo(conn);
            break;
        case 2: {
            system("cls");
            int customerID;
            cout << "\nEnter Customer ID: ";
            cin >> customerID;
            ViewCustomerOrders(conn, customerID);
            break;
        }
        case 3:
            cout << "\nReturning to Admin Menu...\n";
            break;
        default:
            cout << "\nInvalid choice! Please try again.\n";
            _getch();
        }
    } while (choice != 3);
}






