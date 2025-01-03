#include "Customer.h"
using namespace std;

void Customer::CustomerRegistration(MYSQL* conn) {
    system("cls");
    cout << "Customer Sign In" << endl;

    cin.ignore();
    cout << "Name: ";
    getline(cin, name);

    while (true) {
        cout << "IC no (12 digits): ";
        getline(cin, ic_no);
        if (ic_no.length() == 12 && isNumeric(ic_no)) {
            break;
        }
        cout << "Invalid input. Please enter exactly 12 digits for IC no." << endl;
    }

    while (true) {
        cout << "Phone no(Ex.:0123456789): ";
        getline(cin, phone_no);
        if (isNumeric(phone_no)) {
            break;
        }
        cout << "Invalid input. Please enter only numbers for Phone no." << endl;
    }

    cout << "Address: ";
    getline(cin, address);

    while (true) {
        cout << "Username: ";
        getline(cin, cus_username);

        if (cus_username.length() < 5) {
            cout << "Invalid input. Username must be at least 5 letters long." << endl;
            continue;
        }

        // Check if username already exists in the database
        string query = "SELECT COUNT(*) FROM user WHERE username = '" + cus_username + "'";
        const char* q = query.c_str();

        if (mysql_query(conn, q)) {
            cerr << "Error checking username. Error Code: " << mysql_errno(conn) << endl;
            continue; // Retry input
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "Error fetching result. Error Code: " << mysql_errno(conn) << endl;
            continue; // Retry input
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        int count = atoi(row[0]);
        mysql_free_result(res);

        if (count > 0) {
            cout << "This username is already taken. Please try a different one." << endl;
        }
        else {
            // Username is unique
            break;
        }
    }

    cout << "Password: ";
    getline(cin, cus_password);//check 2 times,adjust later

    string insert_query ="INSERT INTO USER (Name, IC_no, Phone_no, Address, username, password, Role) "
                         "VALUES ('" + name + "', '" + ic_no + "', '" + phone_no + "', '" + address +"', '" + cus_username + "', '" + cus_password + "', 'customer')";

    const char* q = insert_query.c_str();

    int qstate = mysql_query(conn, q);

    if (!qstate) {
        cout << endl << "Customer has been successfully added to the database." << endl;
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }
    _getch();
}

void Customer::CustomerInteface(MYSQL* conn,int userID) {
    this->userID = userID;//do it so we don't need to pass userID all the time

    int choice;

    do {
        system("cls");
        cout << "Welcome, CUSTOMER " << endl << endl;
        cout << "1. View Books\n2. View Order Cart\n0. Exit" << endl << endl;
        cout << "What would you like to do? ";

        // Input validation for menu choice
        while (true) {
            if (!(cin >> choice) || choice < 0 || choice > 3) {
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
            ViewBooks(conn);
            break;
        case 2:
            OrderCart(conn);
            break;
            break;
        default:
            break;
        }
    } while (choice != 0);
}

void Customer::ViewBooks(MYSQL* conn) {
    while (true) {
        system("cls");
        string query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book";
        const char* q = query.c_str();

        if (mysql_query(conn, q)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
        }

        dbConn.res = mysql_store_result(conn);

        if (dbConn.res == nullptr) {
            cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
            _getch();
        }

        // Check if there are any rows in the result set
        if (mysql_num_rows(dbConn.res) == 0) {
            cout << "There are no books in the database." << endl;
            mysql_free_result(dbConn.res);
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

        cout << "\n\n1. Choose books to order \n2. Search books \n3. Sort books \n4. Exit" << endl;
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            chooseBooksToOrder(conn);
            break;
        case 2:
            searchBooks(conn);
            break;
        case 3:
            sortBooks(conn);
            break;
        case 4:
            return; // Exit the loop
        default:
            cout << "Invalid choice. Please try again." << endl;
            _getch();
            break;
        }
    }
}


void Customer::chooseBooksToOrder(MYSQL* conn) {
    int bookID, quantity;
    double price;
    int stock;
    double totalAmount = 0.0;
    int orderID;
    bool orderCreated = false;

    // Check for an existing pending order
    string checkOrderQuery = "SELECT orderID, totalAmount FROM `order` WHERE UserID = " + to_string(userID) + " AND orderStatus = 'pending'";
    const char* checkOrderQ = checkOrderQuery.c_str();

    if (mysql_query(conn, checkOrderQ)) {
        cout << "Failed to check existing orders. Error Code: " << mysql_errno(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);

    if (res && mysql_num_rows(res) > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        orderID = atoi(row[0]);               // Get the existing order ID
        totalAmount = atof(row[1]);           // Get the current total amount
        orderCreated = true;                  // Indicate that an order exists
        mysql_free_result(res);
    }
    else {
        mysql_free_result(res);
    }

    while (true) {
        cout << "Enter the BookID of the book you want to order (or 'q' to finish): ";
        string input;
        cin >> input;

        if (input == "q" || input == "Q") {
            break; // Exit the loop
        }

        try {
            bookID = stoi(input);
        }
        catch (...) {
            cout << "Invalid input. Please enter a valid BookID or 'q' to quit." << endl;
            continue;
        }

        // Check if the book exists and fetch its price and stock
        string query = "SELECT Price, Stock FROM book WHERE BookID = " + to_string(bookID);
        const char* q = query.c_str();

        if (mysql_query(conn, q)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            continue;
        }

        res = mysql_store_result(conn);

        if (res == nullptr || mysql_num_rows(res) == 0) {
            cout << "Book not found with the provided BookID." << endl;
            mysql_free_result(res);
            continue;
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        price = atof(row[0]); // Get the price from the result
        stock = atoi(row[1]); // Get the stock from the result
        mysql_free_result(res);

        cout << "Enter the quantity: ";
        cin >> quantity;

        if (quantity > stock) {
            cout << "Not enough stock available. Only " << stock << " items left." << endl;
            continue;
        }

        // Create a new order if no pending order exists
        if (!orderCreated) {
            string orderQuery = "INSERT INTO `order` (orderDate, orderStatus, totalAmount, UserID) VALUES (CURDATE(), 'pending', 0, " + to_string(userID) + ")";
            const char* orderQ = orderQuery.c_str();

            if (mysql_query(conn, orderQ)) {
                cout << "Failed to create order. Error Code: " << mysql_errno(conn) << endl;
                return;
            }

            // Get the last inserted order ID
            orderID = mysql_insert_id(conn);
            orderCreated = true;
        }

        // Insert into the `book_order` table
        string bookOrderQuery = "INSERT INTO book_order (orderID, BookID, quantity, price) VALUES (" + to_string(orderID) + ", " + to_string(bookID) + ", " + to_string(quantity) + ", " + to_string(price * quantity) + ")";
        const char* bookOrderQ = bookOrderQuery.c_str();

        if (mysql_query(conn, bookOrderQ)) {
            cout << "Failed to add book to order. Error Code: " << mysql_errno(conn) << endl;
            continue;
        }

        // Update the stock in the `book` table
        string updateStockQuery = "UPDATE book SET Stock = Stock - " + to_string(quantity) + " WHERE BookID = " + to_string(bookID);
        const char* updateStockQ = updateStockQuery.c_str();

        if (mysql_query(conn, updateStockQ)) {
            cout << "Failed to update stock. Error Code: " << mysql_errno(conn) << endl;
            continue;
        }

        totalAmount += price * quantity;
        cout << "Book with ID " << bookID << " has been added to your order." << endl;
    }

    // Update total amount in the `order` table
    if (orderCreated) {
        string updateOrderQuery = "UPDATE `order` SET totalAmount = " + to_string(totalAmount) + " WHERE orderID = " + to_string(orderID);
        const char* updateOrderQ = updateOrderQuery.c_str();

        if (mysql_query(conn, updateOrderQ)) {
            cout << "Failed to update total amount for the order. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Order completed successfully. Total Amount: RM" << totalAmount << endl;
        }
    }
}

void Customer::searchBooks(MYSQL* conn) {
    while (true) {
        system("cls");
        cout << "===================================================================================" << endl;
        cout << "                              Search Books                                         " << endl;
        cout << "===================================================================================" << endl;

        cout << "1. Search by Title\n2. Search by Author\n3. Search by ISBN\n4. Back to Main Menu" << endl;
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Ignore trailing newline

        string keyword;
        string query;
        MYSQL_RES* res;

        switch (choice) {
        case 1:
            cout << "Enter the Title to search: ";
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book WHERE Title LIKE '%" + keyword + "%'";
            break;
        case 2:
            cout << "Enter the Author to search: ";
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book WHERE Author LIKE '%" + keyword + "%'";
            break;
        case 3:
            cout << "Enter the ISBN to search: ";
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book WHERE ISBN LIKE '%" + keyword + "%'";
            break;
        case 4:
            return; // Exit the function
        default:
            cout << "Invalid choice. Please try again." << endl;
            _getch();
            continue;
        }

        const char* q = query.c_str();
        if (mysql_query(conn, q)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            continue;
        }

        res = mysql_store_result(conn);
        if (res == nullptr || mysql_num_rows(res) == 0) {
            cout << "No books found matching your criteria." << endl;
            mysql_free_result(res);
            _getch();
            continue;
        }

        cout << "\nBooks Found:\n";
        int count = 1;
        MYSQL_ROW rows[100]; // To store up to 100 rows for validation
        int rowIndex = 0;

        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            cout << "\nBook " << count++ << ":\n";
            cout << "BookID: " << row[0] << "\nISBN: " << row[1] << "\nTitle: " << row[2] << "\nPrice: RM" << row[3] << "\nAuthor: " << row[4] << "\nPublisher: " << row[5] << "\nPublished Year: " << row[6] << "\n";
            rows[rowIndex++] = row;
        }

        mysql_free_result(res);

        while (true) {
            cout << "\nEnter the BookID of the book you want to buy (or '0' to search again): ";
            string selectedBookID;
            getline(cin, selectedBookID);

            if (selectedBookID == "0") {
                break; // Exit to search again
            }

            bool validID = false;
            for (int i = 0; i < rowIndex; ++i) {
                if (rows[i] && selectedBookID == rows[i][0]) {
                    validID = true;
                    break;
                }
            }

            if (validID) {
                cout << "You have selected BookID: " << selectedBookID << ".\n";

                // Process the selected book
                query = "SELECT Price, Stock FROM book WHERE BookID = " + selectedBookID;
                if (mysql_query(conn, query.c_str())) {
                    cout << "Error fetching book details! Error Code: " << mysql_errno(conn) << endl;
                    continue;
                }

                res = mysql_store_result(conn);
                if (res && mysql_num_rows(res) > 0) {
                    MYSQL_ROW row = mysql_fetch_row(res);
                    double price = atof(row[0]);
                    int stock = atoi(row[1]);
                    mysql_free_result(res);

                    if (stock <= 0) {
                        cout << "Sorry, this book is out of stock.\n";
                        continue;
                    }

                    cout << "Enter quantity to buy: ";
                    int quantity;
                    cin >> quantity;

                    if (quantity > stock) {
                        cout << "Not enough stock available. Only " << stock << " left.\n";
                        continue;
                    }

                    // Check or create order
                    query = "SELECT orderID FROM `order` WHERE UserID = " + to_string(userID) + " AND orderStatus = 'pending'";
                    if (mysql_query(conn, query.c_str())) {
                        cout << "Error checking pending order! Error Code: " << mysql_errno(conn) << endl;
                        continue;
                    }

                    res = mysql_store_result(conn);
                    int orderID = 0;
                    if (res && mysql_num_rows(res) > 0) {
                        MYSQL_ROW orderRow = mysql_fetch_row(res);
                        orderID = atoi(orderRow[0]);
                        mysql_free_result(res);
                    }
                    else {
                        mysql_free_result(res);
                        // Create new order
                        query = "INSERT INTO `order` (UserID, orderStatus) VALUES (" + to_string(userID) + ", 'pending')";
                        if (mysql_query(conn, query.c_str())) {
                            cout << "Error creating new order! Error Code: " << mysql_errno(conn) << endl;
                            continue;
                        }
                        orderID = mysql_insert_id(conn);
                    }

                    // Add book to order
                    query = "INSERT INTO book_order (orderID, BookID, quantity, price) VALUES (" +
                        to_string(orderID) + ", " + selectedBookID + ", " + to_string(quantity) + ", " +
                        to_string(price * quantity) + ")";
                    if (mysql_query(conn, query.c_str())) {
                        cout << "Error adding book to order! Error Code: " << mysql_errno(conn) << endl;
                        continue;
                    }

                    string newTotalAmount = "UPDATE `order` SET totalAmount = totalAmount + " + to_string(price * quantity) + " WHERE orderID = " + to_string(orderID);
                    const char* totalAmount = newTotalAmount.c_str();
                    if (mysql_query(conn, totalAmount)) {
                        cout << "Failed to update total amount. Error Code: " << mysql_errno(conn) << endl;
                        continue;
                    }

                    // Update stock
                    query = "UPDATE book SET Stock = Stock - " + to_string(quantity) + " WHERE BookID = " + selectedBookID;
                    if (mysql_query(conn, query.c_str())) {
                        cout << "Error updating stock! Error Code: " << mysql_errno(conn) << endl;
                        continue;
                    }

                    cout << "Book added to order successfully.\n";
                }
                else {
                    mysql_free_result(res);
                    cout << "BookID " << selectedBookID << " not found.\n";
                }
            }
        }
    }
}


void Customer::sortBooks(MYSQL* conn) {
    while (true) {
        system("cls");
        cout << "===================================================================================" << endl;
        cout << "                              Sort Books                                          " << endl;
        cout << "===================================================================================" << endl;

        cout << "1. Sort by Price\n2. Sort by Title\n3. Sort by Published Year\n4. Sort by Price Range\n5. Back to Main Menu" << endl;
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 5) {
            return; // Exit the function
        }

        string query;
        if (choice == 4) {
            double minPrice, maxPrice;
            cout << "Enter minimum price: ";
            cin >> minPrice;
            cout << "Enter maximum price: ";
            cin >> maxPrice;

            query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book WHERE Price BETWEEN " + to_string(minPrice) + " AND " + to_string(maxPrice) + " ORDER BY Price ASC";
        }
        else {
            cout << "Choose sorting order:\n1. Ascending\n2. Descending" << endl;
            int orderChoice;
            cout << "Enter your choice: ";
            cin >> orderChoice;

            string order = (orderChoice == 2) ? "DESC" : "ASC";

            if (choice == 1) {
                query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book ORDER BY Price " + order;
            }
            else if (choice == 2) {
                query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book ORDER BY Title " + order;
            }
            else if (choice == 3) {
                query = "SELECT BookID, ISBN, Title, Price, Author, Publisher, PublishedYear FROM book ORDER BY PublishedYear " + order;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                _getch();
                continue;
            }
        }

        const char* q = query.c_str();
        if (mysql_query(conn, q)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            continue;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr || mysql_num_rows(res) == 0) {
            cout << "No books found." << endl;
            mysql_free_result(res);
            _getch();
            continue;
        }

        cout << "\nSorted Books:\n";
        int bookIDs[100];
        int bookCount = 0;
        int count = 1;

        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            cout << "\nBook " << count++ << ":\n";
            cout << "BookID: " << row[0] << "\nISBN: " << row[1] << "\nTitle: " << row[2] << "\nPrice: RM" << row[3] << "\nAuthor: " << row[4] << "\nPublisher: " << row[5] << "\nPublished Year: " << row[6] << "\n";

            if (bookCount < 100) {
                bookIDs[bookCount++] = atoi(row[0]);
            }
        }

        mysql_free_result(res);

        while (true) {
            cout << "\nEnter BookID to add to your cart (0 to return to Sort Menus): ";
            int selectedBookID;
            cin >> selectedBookID;

            if (selectedBookID == 0) break;

            bool validBookID = false;
            for (int i = 0; i < bookCount; i++) {
                if (bookIDs[i] == selectedBookID) {
                    validBookID = true;
                    break;
                }
            }

            if (validBookID) {
                int quantity;
                cout << "Enter quantity: ";
                cin >> quantity;

                query = "SELECT Price, Stock FROM book WHERE BookID = " + to_string(selectedBookID);
                const char* priceQuery = query.c_str();
                if (mysql_query(conn, priceQuery)) {
                    cout << "Failed to fetch book details. Error Code: " << mysql_errno(conn) << endl;
                    continue;
                }

                res = mysql_store_result(conn);
                if (res == nullptr || mysql_num_rows(res) == 0) {
                    cout << "Book not found or unavailable." << endl;
                    mysql_free_result(res);
                    continue;
                }

                MYSQL_ROW row = mysql_fetch_row(res);
                double price = atof(row[0]);
                int stock = atoi(row[1]);
                mysql_free_result(res);

                if (quantity > stock) {
                    cout << "Not enough stock available. Only " << stock << " left." << endl;
                    continue;
                }

                query = "SELECT orderID FROM `order` WHERE UserID = " + to_string(userID) + " AND orderStatus = 'pending'";
                const char* checkOrderQuery = query.c_str();
                if (mysql_query(conn, checkOrderQuery)) {
                    cout << "Failed to check order. Error Code: " << mysql_errno(conn) << endl;
                    continue;
                }

                res = mysql_store_result(conn);
                int orderID = 0;
                if (mysql_num_rows(res) > 0) {
                    MYSQL_ROW orderRow = mysql_fetch_row(res);
                    orderID = atoi(orderRow[0]);
                }
                else {
                    mysql_free_result(res);
                    query = "INSERT INTO `order` (UserID, orderStatus) VALUES (" + to_string(userID) + ", 'pending')";
                    const char* createOrderQuery = query.c_str();
                    if (mysql_query(conn, createOrderQuery)) {
                        cout << "Failed to create order. Error Code: " << mysql_errno(conn) << endl;
                        continue;
                    }
                    orderID = mysql_insert_id(conn);
                }
                mysql_free_result(res);

                string orderQuery = "INSERT INTO book_order (orderID, BookID, quantity, price) VALUES (" + to_string(orderID) + ", " + to_string(selectedBookID) + ", " + to_string(quantity) + ", " + to_string(price * quantity) + ")";
                const char* orderQ = orderQuery.c_str();
                if (mysql_query(conn, orderQ)) {
                    cout << "Failed to add book to order. Error Code: " << mysql_errno(conn) << endl;
                    continue;
                }

                string updateStockQuery = "UPDATE book SET Stock = Stock - " + to_string(quantity) + " WHERE BookID = " + to_string(selectedBookID);
                const char* updateStockQ = updateStockQuery.c_str();
                if (mysql_query(conn, updateStockQ)) {
                    cout << "Failed to update stock. Error Code: " << mysql_errno(conn) << endl;
                    continue;
                }
                string newTotalAmount = "UPDATE `order` SET totalAmount = totalAmount + " + to_string(price * quantity) + " WHERE orderID = " + to_string(orderID);
                const char* totalAmount = newTotalAmount.c_str();
                if (mysql_query(conn, totalAmount)) {
                    cout << "Failed to update total amount. Error Code: " << mysql_errno(conn) << endl;
                    continue;
                }

                cout << "Book added to cart successfully!" << endl;
            }
            else {
                cout << "Invalid BookID selected." << endl;
            }
        }

        _getch();
    }
}

void Customer::OrderCart(MYSQL* conn) {
    system("cls");
    cout << "===================================================================================\n";
    cout << "                              Order Cart                                           \n";
    cout << "===================================================================================\n";

    string query = "SELECT o.orderID, bo.BookID, b.Title, bo.quantity, bo.price, b.Price AS UnitPrice FROM `order` o "
        "JOIN book_order bo ON o.orderID = bo.orderID "
        "JOIN book b ON bo.BookID = b.BookID "
        "WHERE o.UserID = " + to_string(userID) + " AND o.orderStatus = 'pending'";
    const char* q = query.c_str();

    if (mysql_query(conn, q)) {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return;
    }

    dbConn.res = mysql_store_result(conn);

    if (dbConn.res == nullptr || mysql_num_rows(dbConn.res) == 0) {
        cout << "Your order cart is empty." << endl;
        mysql_free_result(dbConn.res);
        _getch();
        return;
    }

    double totalPrice = 0;
    cout << "Your Order Cart:\n";
    cout << "-----------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "BookID" << setw(30) << "Title" << setw(10) << "Quantity" << setw(10) << "Unit Price" << setw(10) << "Total" << "\n";
    cout << "-----------------------------------------------------------------------------------\n";

    while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
        int bookID = atoi(dbConn.row[1]);
        string title = dbConn.row[2];
        int quantity = atoi(dbConn.row[3]);
        double unitPrice = atof(dbConn.row[5]);
        double total = quantity * unitPrice;

        cout << left << setw(10) << bookID
            << setw(30) << title
            << setw(10) << quantity
            << setw(10) << "RM" << fixed << setprecision(2) << unitPrice
            << setw(10) << "RM" << fixed << setprecision(2) << total << "\n";

        totalPrice += total;
    }

    mysql_free_result(dbConn.res);

    cout << "-----------------------------------------------------------------------------------\n";
    cout << "Total Price: RM" << totalPrice << "\n";
    cout << "-----------------------------------------------------------------------------------\n";

    while (true) {
        cout << "\nOptions:\n1. Confirm Order\n2. Remove Item\n3. Back to Main Menu\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            confirmOrder(conn);
            return;
        case 2:
            removeItemFromCart(conn);
            return;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}



void Customer::confirmOrder(MYSQL* conn) {
    system("cls");
    cout << "===================================================================================\n";
    cout << "                              Confirm Order                                        \n";
    cout << "===================================================================================\n";

    string query = "SELECT orderID FROM `order` WHERE UserID = " + to_string(userID) + " AND orderStatus = 'pending'";
    const char* q = query.c_str();

    if (mysql_query(conn, q)) {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return;
    }

    dbConn.res = mysql_store_result(conn);

    if (dbConn.res == nullptr || mysql_num_rows(dbConn.res) == 0) {
        cout << "You have no pending orders to confirm." << endl;
        mysql_free_result(dbConn.res);
        _getch();
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(dbConn.res);
    int orderID = atoi(row[0]);
    mysql_free_result(dbConn.res);

    cout << "Are you sure you want to confirm your order? (1 for Yes, 0 for No): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        string updateQuery = "UPDATE `order` SET orderStatus = 'confirmed' WHERE orderID = " + to_string(orderID);
        const char* updateQ = updateQuery.c_str();

        if (mysql_query(conn, updateQ)) {
            cout << "Failed to confirm order. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Order confirmed successfully!" << endl;
        }
    }
    else {
        cout << "Order confirmation canceled." << endl;
    }

    _getch();
}


void Customer::removeItemFromCart(MYSQL* conn) {
    system("cls");
    cout << "===================================================================================\n";
    cout << "                              Remove Item From Cart                                \n";
    cout << "===================================================================================\n";

    string query = "SELECT bo.BookID, b.Title, bo.quantity FROM `order` o JOIN book_order bo ON o.orderID = bo.orderID JOIN book b ON bo.BookID = b.BookID WHERE o.UserID = " + to_string(userID) + " AND o.orderStatus = 'pending'";
    const char* q = query.c_str();

    if (mysql_query(conn, q)) {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return;
    }

    dbConn.res = mysql_store_result(conn);

    if (dbConn.res == nullptr || mysql_num_rows(dbConn.res) == 0) {
        cout << "Your order cart is empty." << endl;
        mysql_free_result(dbConn.res);
        _getch();
        return;
    }

    cout << "Your Order Cart:\n";

    bool validBookID = false;
    while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
        cout << "BookID: " << dbConn.row[0] << "\n";
        cout << "Title: " << dbConn.row[1] << "\n";
        cout << "Quantity: " << dbConn.row[2] << "\n\n";
    }

    mysql_data_seek(dbConn.res, 0); // Reset result set cursor

    cout << "Enter the BookID of the item to remove (or 0 to cancel): ";
    int selectedBookID;
    cin >> selectedBookID;

    if (selectedBookID == 0) {
        cout << "Item removal canceled." << endl;
        mysql_free_result(dbConn.res);
        _getch();
        return;
    }

    while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
        if (atoi(dbConn.row[0]) == selectedBookID) {
            validBookID = true;
            break;
        }
    }

    if (!validBookID) {
        cout << "Invalid BookID." << endl;
        mysql_free_result(dbConn.res);
        _getch();
        return;
    }

    mysql_free_result(dbConn.res);



    string priceQuery = "SELECT b.price FROM book b WHERE b.BookID = " + to_string(selectedBookID);
    if (mysql_query(conn, priceQuery.c_str())) {
        cout << "Failed to fetch book price. Error Code: " << mysql_errno(conn) << endl;
    }
    else {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            MYSQL_ROW row = mysql_fetch_row(res);
            double bookPrice = atof(row[0]);
            mysql_free_result(res);

            // Query to delete the book from the cart
            string deleteQuery = "DELETE bo FROM book_order bo JOIN `order` o ON bo.orderID = o.orderID WHERE bo.BookID = " +
                to_string(selectedBookID) + " AND o.UserID = " + to_string(userID) + " AND o.orderStatus = 'pending'";
            if (mysql_query(conn, deleteQuery.c_str())) {
                cout << "Failed to remove item. Error Code: " << mysql_errno(conn) << endl;
            }
            else {
                cout << "Item removed successfully from cart." << endl;

                // Query to update the totalAmount in the `order` table
                string updateTotalQuery = "UPDATE `order` SET totalAmount = totalAmount - " + to_string(bookPrice) +
                    " WHERE UserID = " + to_string(userID) + " AND orderStatus = 'pending'";
                if (mysql_query(conn, updateTotalQuery.c_str())) {
                    cout << "Failed to update total amount. Error Code: " << mysql_errno(conn) << endl;
                }
                else {
                    cout << "Total amount updated successfully." << endl;
                }
            }
        }
        else {
            cout << "Book price not found." << endl;
        }
    }
}








