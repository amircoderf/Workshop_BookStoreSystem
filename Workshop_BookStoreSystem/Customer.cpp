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

void Customer::CustomerInteface(MYSQL* conn, int userID) {
    this->userID = userID;  // Store userID in the class to avoid passing it around

    int choice;

    do {
        system("cls");
        cout << "===================================================================================" << endl;
        cout << "                                 Customer Menu                                     " << endl;
        cout << "===================================================================================" << endl;
        cout << "Welcome, CUSTOMER" << endl << endl;
        cout << "1. View Books" << endl;
        cout << "2. View Order Cart" << endl;
        cout << "3. View Past Orders" << endl; // Add menu for past orders
        cout << "0. Exit" << endl << endl;
        cout << "What would you like to do? ";

        // Input validation for menu choice
        while (true) {
            if (!(cin >> choice) || choice < 0 || choice > 3) {
                cout << "Invalid input. Please enter a number between 0 and 3: ";
                cin.clear();  // Clear the error flag
                cin.ignore(10000, '\n');  // Ignore any invalid input
            }
            else {
                break;  // Valid input, break the loop
            }
        }

        // Handle user choice
        switch (choice) {
        case 0:
            return;  // Exit the program
        case 1:
            ViewBooks(conn);  // View available books
            break;
        case 2:
            OrderCart(conn);  // View order cart
            break;
        case 3:
            viewPastOrder(conn);  // View past orders
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            _getch();
            break;
        }
    } while (choice != 0);
}

void Customer::ViewBooks(MYSQL* conn) {
    while (true) {
        system("cls");
        string query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book";
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

        Table viewBook;

        viewBook.add_row({ "BookID", "Title", "Price (RM)","Available Stock", "Author", "Publisher", "Published Year"});
        while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
            viewBook.add_row({
               dbConn.row[0], // BookID
               dbConn.row[1], //title
               dbConn.row[2], // price
               dbConn.row[3], // stock
               dbConn.row[4], // Author
               dbConn.row[5], // Publisher
               dbConn.row[6]  // Published Year
                });
        }

        cartTableFormat(viewBook);

        cout << viewBook << endl;

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

// Function to add or update a book in the order
void Customer::addOrUpdateBookInOrder(MYSQL* conn, int orderID, int bookID, int quantity, double price) {
    // Check if the book already exists in the order
    string checkBookOrderQuery = "SELECT quantity FROM book_order WHERE orderID = " + to_string(orderID) + " AND BookID = " + to_string(bookID);
    const char* checkBookOrderQ = checkBookOrderQuery.c_str();

    if (mysql_query(conn, checkBookOrderQ)) {
        cout << "Failed to check existing book in order. Error Code: " << mysql_errno(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);

    if (res && mysql_num_rows(res) > 0) {
        // Book already exists in the order, update the quantity and total price
        MYSQL_ROW row = mysql_fetch_row(res);
        int existingQuantity = atoi(row[0]);

        // Update the existing record in the `book_order` table
        string updateBookOrderQuery = "UPDATE book_order SET quantity = quantity + " + to_string(quantity) + ", price = price + " + to_string(price * quantity) +
            " WHERE orderID = " + to_string(orderID) + " AND BookID = " + to_string(bookID);
        const char* updateBookOrderQ = updateBookOrderQuery.c_str();

        if (mysql_query(conn, updateBookOrderQ)) {
            cout << "Failed to update existing book in order. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Book with ID " << bookID << " quantity updated in your order." << endl;
        }
    }
    else {
        // Book does not exist in the order, insert a new record
        string bookOrderQuery = "INSERT INTO book_order (orderID, BookID, quantity, price) VALUES (" + to_string(orderID) + ", " + to_string(bookID) + ", " + to_string(quantity) + ", " + to_string(price * quantity) + ")";
        const char* bookOrderQ = bookOrderQuery.c_str();

        if (mysql_query(conn, bookOrderQ)) {
            cout << "Failed to add book to order. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Book with ID " << bookID << " has been added to your order." << endl;
        }
    }

    mysql_free_result(res);
}

// Modify the `chooseBooksToOrder` function to use the `addOrUpdateBookInOrder` function
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

        // Call the function to add or update the book in the order
        addOrUpdateBookInOrder(conn, orderID, bookID, quantity, price);

        totalAmount += price * quantity;
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
            query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book WHERE Title LIKE '%" + keyword + "%'";
            break;
        case 2:
            cout << "Enter the Author to search: ";
            getline(cin, keyword);
            query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book WHERE Author LIKE '%" + keyword + "%'";
            break;
        case 3:
            cout << "Enter the ISBN to search: ";
            getline(cin, keyword);
            query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book WHERE ISBN LIKE '%" + keyword + "%'";
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
        Table table;
        table.add_row({ "BookID", "Title", "Price","Available Stock", "Author", "Publisher", "Published Year" });

        vector<MYSQL_ROW> rows;

        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            table.add_row({ row[0], row[1], row[2], row[3], row[4], row[5], row[6] });
            rows.push_back(row); // Store rows dynamically
        }

        cartTableFormat(table);
        cout << table << endl;

        while (true) {
            cout << "\nEnter the BookID of the book you want to buy (or '0' to search again): ";
            string selectedBookID;
            getline(cin, selectedBookID);

            if (selectedBookID == "0") {
                break; // Exit to search again
            }

            bool validID = false;
            MYSQL_ROW selectedRow = nullptr;

            for (const auto& r : rows) {
                if (r && selectedBookID == r[0]) { // Check BookID
                    validID = true;
                    selectedRow = r;
                    break;
                }
            }

            if (!validID) {
                cout << "Invalid BookID. Please try again." << endl;
                continue;
            }

            // Fetch additional book details
            int bookID = stoi(selectedRow[0]);
            double price = atof(selectedRow[3]);
            int stock = 0;

            query = "SELECT Stock FROM book WHERE BookID = " + to_string(bookID);
            if (mysql_query(conn, query.c_str())) {
                cout << "Error fetching stock information! Error Code: " << mysql_errno(conn) << endl;
                continue;
            }

            res = mysql_store_result(conn);
            if (res && mysql_num_rows(res) > 0) {
                MYSQL_ROW stockRow = mysql_fetch_row(res);
                stock = atoi(stockRow[0]);
                mysql_free_result(res);
            }
            else {
                mysql_free_result(res);
                cout << "Stock information not available.\n";
                continue;
            }

            if (stock <= 0) {
                cout << "Sorry, this book is out of stock.\n";
                continue;
            }

            cout << "Enter quantity to buy: ";
            int quantity;
            cin >> quantity;
            cin.ignore(); // Clear input buffer

            if (quantity > stock) {
                cout << "Not enough stock available. Only " << stock << " left.\n";
                continue;
            }

            // Get or create order
            int orderID = 0;
            query = "SELECT orderID FROM `order` WHERE UserID = " + to_string(userID) + " AND orderStatus = 'pending'";
            if (mysql_query(conn, query.c_str())) {
                cout << "Error checking pending order! Error Code: " << mysql_errno(conn) << endl;
                continue;
            }

            res = mysql_store_result(conn);
            if (res && mysql_num_rows(res) > 0) {
                MYSQL_ROW orderRow = mysql_fetch_row(res);
                orderID = atoi(orderRow[0]);
                mysql_free_result(res);
            }
            else {
                mysql_free_result(res);
                query = "INSERT INTO `order` (UserID, orderStatus) VALUES (" + to_string(userID) + ", 'pending')";
                if (mysql_query(conn, query.c_str())) {
                    cout << "Error creating new order! Error Code: " << mysql_errno(conn) << endl;
                    continue;
                }
                orderID = mysql_insert_id(conn);
            }

            // Add or update the book in the order
            addOrUpdateBookInOrder(conn, orderID, bookID, quantity, price);

            // Update the stock
            query = "UPDATE book SET Stock = Stock - " + to_string(quantity) + " WHERE BookID = " + to_string(bookID);
            if (mysql_query(conn, query.c_str())) {
                cout << "Error updating stock! Error Code: " << mysql_errno(conn) << endl;
                continue;
            }

            cout << "Book added to order successfully.\n";
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
        //price range query
        string query;
        if (choice == 4) {
            double minPrice, maxPrice;
            cout << "Enter minimum price range: ";
            while (!(cin >> minPrice) || minPrice < 0) {
                cout << "Please enter a valid positive number for minimum price: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }

            cout << "Enter maximum price range: ";
            while (!(cin >> maxPrice) || maxPrice < minPrice) {
                cout << "Please enter a valid positive number greater than minimum price for maximum price: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }

            query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book WHERE Price BETWEEN " + to_string(minPrice) + " AND " + to_string(maxPrice) + " ORDER BY Price ASC";
        }
        else {
            cout << "Choose sorting order:\n1. Ascending\n2. Descending" << endl;
            int orderChoice;
            cout << "Enter your choice: ";
            cin >> orderChoice;

            string order = (orderChoice == 2) ? "DESC" : "ASC";

            if (choice == 1) {
                query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book ORDER BY Price " + order;
            }
            else if (choice == 2) {
                query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book ORDER BY Title " + order;
            }
            else if (choice == 3) {
                query = "SELECT BookID, Title, Price,Stock, Author, Publisher, PublishedYear FROM book ORDER BY PublishedYear " + order;
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

        vector<MYSQL_ROW> rows;
        Table sortedBook;

        sortedBook.add_row({ "BookID", "Title", "Price","Available Stock", "Author", "Publisher", "Published Year" });
        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            sortedBook.add_row({ row[0], row[1], row[2], row[3], row[4], row[5], row[6] });
            rows.push_back(row); // Store rows dynamically           
        }

        cartTableFormat(sortedBook);
        cout << sortedBook << endl;

        while (true) {
            cout << "\nEnter the BookID of the book you want to buy (or '0' to search again): ";
            string selectedBookID;
            getline(cin, selectedBookID);

            if (selectedBookID == "0") {
                break; // Exit to search again
            }

            bool validID = false;
            MYSQL_ROW selectedRow = nullptr;

            for (const auto& r : rows) {
                if (r && selectedBookID == r[0]) { // Check BookID
                    validID = true;
                    selectedRow = r;
                    break;
                }
            }

            if (!validID) {
                cout << "Invalid BookID. Please try again." << endl;
                continue;
            }

            int quantity;
            cout << "Enter quantity: ";
            while (!(cin >> quantity) || quantity <= 0) {
                cout << "Please enter a valid positive number for quantity: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }

            query = "SELECT Price, Stock FROM book WHERE BookID = " + selectedBookID;
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

            // Use addOrUpdateBookInOrder instead of direct insert
            addOrUpdateBookInOrder(conn, orderID, stoi(selectedBookID), quantity, price);

            // Update the stock in the `book` table
            string updateStockQuery = "UPDATE book SET Stock = Stock - " + to_string(quantity) + " WHERE BookID = " + selectedBookID;
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
    }
    _getch();
}

void Customer::OrderCart(MYSQL* conn) {
    while (true) {
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
            if (dbConn.res) {
                mysql_free_result(dbConn.res);
            }
            _getch();
            return;
        }

        // Create a table using the formatting library
        Table orderCartTable;
        orderCartTable.add_row({ "BookID", "Title", "Quantity", "Unit Price (RM)", "Total (RM)" });

        double totalPrice = 0.0;

        // Populate table rows
        while ((dbConn.row = mysql_fetch_row(dbConn.res))) {
            int bookID = atoi(dbConn.row[1]);
            string title = dbConn.row[2];
            int quantity = atoi(dbConn.row[3]);
            double unitPrice = atof(dbConn.row[5]);
            double total = quantity * unitPrice;

            // Format prices inline with `ostringstream`
            ostringstream unitPriceStream, totalStream;
            unitPriceStream << fixed << setprecision(2) << unitPrice;
            totalStream << fixed << setprecision(2) << total;

            orderCartTable.add_row({
                to_string(bookID),
                title,
                to_string(quantity),
                unitPriceStream.str(),
                totalStream.str()
                });

            totalPrice += total;
        }

        mysql_free_result(dbConn.res);

        // Add total price row
        ostringstream totalPriceStream;
        totalPriceStream << fixed << setprecision(2) << totalPrice;
        orderCartTable.add_row({
            "", "", "", "Total Price:", totalPriceStream.str()
            });

        // Apply table formatting
        cartTableFormat(orderCartTable);

        // Display the table
        cout << orderCartTable << endl;

        cout << "\nOptions:\n1. Confirm Order\n2. Update/Remove Item\n3. Back to Main Menu\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            confirmOrder(conn);
            break;
        case 2:
            adjustItemQuantity(conn);
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
            _getch();
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
        string updateQuery = "UPDATE `order` SET orderStatus = 'completed' WHERE orderID = " + to_string(orderID);
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

void Customer::adjustItemQuantity(MYSQL* conn) {
    int bookID, newQuantity;

    cout << "\nEnter the BookID to adjust quantity: ";
    cin >> bookID;
    cout << "Enter the new quantity (0 to remove): ";
    cin >> newQuantity;

    if (newQuantity < 0) {
        cout << "Invalid quantity. Please enter 0 or a positive number." << endl;
        _getch();
        return;
    }

    // Query to check if the BookID exists in the current order cart,otherwise it might return error
    string checkQuery = "SELECT bo.quantity FROM book_order bo "
        "JOIN `order` o ON bo.orderID = o.orderID "
        "WHERE o.UserID = " + to_string(userID) +
        " AND o.orderStatus = 'pending' AND bo.BookID = " + to_string(bookID);
    const char* checkQueryCStr = checkQuery.c_str();

    if (mysql_query(conn, checkQueryCStr)) {
        cout << "Failed to fetch book details. Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr || mysql_num_rows(res) == 0) {
        cout << "BookID not found in the order cart." << endl;
        mysql_free_result(res);
        _getch();
        return;
    }

    mysql_free_result(res);

    if (newQuantity == 0) {
        // Remove the item from the cart
        string deleteQuery = "DELETE bo FROM book_order bo "
            "JOIN `order` o ON bo.orderID = o.orderID "
            "WHERE o.UserID = " + to_string(userID) +
            " AND o.orderStatus = 'pending' AND bo.BookID = " + to_string(bookID);
        const char* deleteQueryCStr = deleteQuery.c_str();

        if (mysql_query(conn, deleteQueryCStr)) {
            cout << "Failed to remove item. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Item removed from the cart." << endl;
        }
    }
    else {
        // Update the quantity of the item
        string updateQuery = "UPDATE book_order bo "
            "JOIN `order` o ON bo.orderID = o.orderID "
            "SET bo.quantity = " + to_string(newQuantity) +
            " WHERE o.UserID = " + to_string(userID) +
            " AND o.orderStatus = 'pending' AND bo.BookID = " + to_string(bookID);
        const char* updateQueryCStr = updateQuery.c_str();

        if (mysql_query(conn, updateQueryCStr)) {
            cout << "Failed to update item quantity. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Item quantity updated to " << newQuantity << "." << endl;
        }
    }

    // Update the total price of the order when the order already exists,see
    string updateTotalQuery = "UPDATE `order` o "
        "JOIN (SELECT bo.orderID, SUM(bo.quantity * bo.price) AS newTotal "
        "      FROM book_order bo "
        "      JOIN `order` o ON bo.orderID = o.orderID "
        "      WHERE o.UserID = " + to_string(userID) +
        "        AND o.orderStatus = 'pending' "
        "      GROUP BY bo.orderID) AS updatedTotal "
        "ON o.orderID = updatedTotal.orderID "
        "SET o.totalAmount = updatedTotal.newTotal "
        "WHERE o.UserID = " + to_string(userID) +
        " AND o.orderStatus = 'pending'";
    const char* updateTotalQueryCStr = updateTotalQuery.c_str();

    if (mysql_query(conn, updateTotalQueryCStr)) {
        cout << "Failed to update the total amount of the order. Error Code: " << mysql_errno(conn) << endl;
    }
    else {
        cout << "Order total amount updated successfully." << endl;
    }

    _getch();
}

void Customer::viewPastOrder(MYSQL* conn) {
    while (true) {
        system("cls"); // Clear the screen at the start of each loop
        cout << "===================================================================================" << endl;
        cout << "                                PAST ORDERS                                   " << endl;
        cout << "===================================================================================" << endl;

        // Query to fetch past orders for the logged-in user
        string query = "SELECT orderID, orderDate, totalAmount, orderStatus FROM `order` WHERE UserID = " + to_string(userID) + " AND orderStatus != 'pending'";
        const char* pastOrdersQuery = query.c_str();

        if (mysql_query(conn, pastOrdersQuery)) {
            cout << "Failed to fetch past orders. Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr || mysql_num_rows(res) == 0) {
            cout << "No past orders found." << endl;
            mysql_free_result(res);
            _getch();
            return;
        }

        // Display past orders in tabular format
        Table table;
        table.add_row({ "OrderID", "Order Date", "Total Amount (RM)", "Status" });

        vector<int> orderIDs; // Store OrderIDs for further details

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            table.add_row({ row[0], row[1], row[2], row[3] });
            orderIDs.push_back(atoi(row[0]));
        }

        cout << table << endl;
        mysql_free_result(res);

        // Allow the user to view details of a specific order
        cout << "\nEnter OrderID to view details (0 to return to main menu): ";
        int selectedOrderID;
        cin >> selectedOrderID;

        if (selectedOrderID == 0) break;

        if (find(orderIDs.begin(), orderIDs.end(), selectedOrderID) != orderIDs.end()) {
            // Fetch details of the selected order
            query = "SELECT b.Title, o.quantity, b.Price, (o.quantity * b.Price) AS Total "
                "FROM book_order o "
                "JOIN book b ON o.BookID = b.BookID "
                "WHERE o.orderID = " + to_string(selectedOrderID);
            const char* orderDetailsQuery = query.c_str();

            if (mysql_query(conn, orderDetailsQuery)) {
                cout << "Failed to fetch order details. Error Code: " << mysql_errno(conn) << endl;
                continue;
            }

            res = mysql_store_result(conn);
            if (res == nullptr || mysql_num_rows(res) == 0) {
                cout << "No details found for the selected order." << endl;
                mysql_free_result(res);
                continue;
            }

            // Display order details
            Table detailsTable;
            detailsTable.add_row({ "Title", "Quantity", "Price (RM)", "Total (RM)" });

            while ((row = mysql_fetch_row(res))) {
                detailsTable.add_row({ row[0], row[1], row[2], row[3] });
            }

            // Prominently display Order ID
            cout << "\n===================================================================================" << endl;
            cout << "                               ORDER DETAILS                                    " << endl;
            cout << "===================================================================================" << endl;
            cout << "                           *** ORDER ID: " << selectedOrderID << " ***                     " << endl;
            cout << "===================================================================================" << endl;

            cout << detailsTable << endl;
            mysql_free_result(res);
        }
        else {
            cout << "Invalid OrderID. Please try again." << endl;
        }

        cout << "\nPress any key to continue..." << endl;
        _getch(); // Wait for user input before refreshing
    }
}

void Customer::myProfile(MYSQL* conn) {

}













