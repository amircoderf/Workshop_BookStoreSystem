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

void Customer::setDBConnection(MYSQL* conn) {
    this->conn = conn;
}

void Customer::setUserId(int userID) {
    this->userID = userID;  // Store userID in the class to avoid passing it around
}

void Customer::CustomerInteface() {
    int choice;

    do {
        system("cls");
        cout << "===================================================================================" << endl;
        cout << "                                 Customer Menu                                     " << endl;
        cout << "===================================================================================" << endl;
        cout << "Welcome, CUSTOMER" << endl << endl;
        cout << "1. My Profile" << endl;
        cout << "2. View Books" << endl;
        cout << "3. View Order Cart" << endl; 
        cout << "4. View Past Orders" << endl;
        cout << "0. Exit" << endl << endl;
        cout << "What would you like to do? ";

        // Input validation for menu choice
        while (true) {
            if (!(cin >> choice) || choice < 0 || choice > 4) {
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
            myProfile();
            break;
        case 2:
            ViewBooks();  // View available books
            break;
        case 3:
            OrderCart();
            break;
        case 4:
            viewPastOrder();
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            _getch();
            break;
        }
    } while (choice != 0);
}

void Customer::ViewBooks()
{
    while (true) {
        system("cls");
        string query = "SELECT BookID, Title, Price, Stock, Author, Publisher, PublishedYear FROM book";
        const char* q = query.c_str();

        if (mysql_query(conn, q)) {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;  // Exit the function in case of query failure
        }

        MYSQL_RES* res = mysql_store_result(conn);

        if (res == nullptr) {
            cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
            _getch();
            return;  // Exit the function in case of result fetch failure
        }

        // Check if there are any rows in the result set
        if (mysql_num_rows(res) == 0) {
            cout << "There is no book in the database." << endl;
            mysql_free_result(res);
            _getch();
            return; // Exit the function if no books are found
        }

        cout << "===================================================================================" << endl;
        cout << "                              Books in the Database                                " << endl;
        cout << "===================================================================================" << endl << endl;

        Table viewBook;
        viewBook.add_row({ "BookID", "Title", "Price (RM)", "Available Stock", "Author", "Publisher", "Published Year" });

        // Loop through the result set and add rows
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL) {
            viewBook.add_row({
                row[0] ? row[0] : "NULL",  // BookID
                row[1] ? row[1] : "NULL",  // Title
                row[2] ? row[2] : "NULL",  // Price
                row[3] ? row[3] : "NULL",  // Stock
                row[4] ? row[4] : "NULL",  // Author
                row[5] ? row[5] : "NULL",  // Publisher
                row[6] ? row[6] : "NULL"   // Published Year
                });
        }

        cartTableFormat(viewBook);

        // Display the table
        cout << viewBook << endl;

        // Free the result set
        mysql_free_result(res);

        // Menu options for the user
        cout << "\n\n1. Choose books to order \n2. Search books \n3. Sort books \n4. Exit" << endl;
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            chooseBooksToOrder();
            break;
        case 2:
            searchBooks();
            break;
        case 3:
            sortBooks();
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
void Customer::addOrUpdateBookInOrder(int orderID, int bookID, int quantity, double price) {
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

void Customer::chooseBooksToOrder() {
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
        cout << "Enter the BookID of the book you want to order (or press 'Enter' to finish): ";
        string input;
        cin.ignore();
        getline(cin, input); // Use getline to handle empty input

        if (input.empty()) {
            break; // Exit the loop
        }

        try {
            bookID = stoi(input);
        }
        catch (...) {
            cout << "Invalid input. Please enter a valid BookID or press 'Enter' to quit.\n" << endl;
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
            cout << "Book not found with the provided BookID\n." << endl;
            mysql_free_result(res);
            continue;
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        price = atof(row[0]); // Get the price from the result
        stock = atoi(row[1]); // Get the stock from the result
        mysql_free_result(res);

        cout << "Enter the quantity (or '0' to cancel purchase): ";
        cin >> quantity;

        if (quantity == 0) {
            cout << "Purchase canceled for this book.\n" << endl;
            continue; // Skip to the next iteration of the loop
        }

        if (quantity > stock) {
            cout << "Not enough stock available. Only " << stock << " items left.\n" << endl;
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
        addOrUpdateBookInOrder(orderID, bookID, quantity, price);

        // Call the updateBookStock function to decrease the stock
        updateBookStock(bookID, quantity);

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
            cout << "Order completed successfully. Total Amount: RM" << totalAmount << endl<<endl;
        }
    }
}

void Customer::searchBooks() {
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
            //try a better way next time
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
            cout << "\nEnter the BookID of the book you want to buy (Press 'Enter' to return to Search Menu): ";
            string selectedBookID;
            getline(cin, selectedBookID);

            if (selectedBookID.empty()) {
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
            double price = atof(selectedRow[2]); // Price should be at the correct index
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

            int quantity;
            cout << "Enter the quantity (or '0' to cancel purchase): ";
            cin >> quantity;
            cin.ignore(); // Clear the newline character

            if (quantity == 0) {
                cout << "Purchase canceled for this book.\n" << endl;
                continue; // Stay in the inner loop for BookID selection
            }

            if (quantity > stock) {
                cout << "Not enough stock available. Only " << stock << " left.\n";
                continue; // Stay in the inner loop
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
            updateBookStock(bookID, quantity);
            addOrUpdateBookInOrder(orderID, bookID, quantity, price);
            cout << "Book added to order successfully.\n";
        }
    }
}

void Customer::sortBooks() {
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
            cout << "\nEnter the BookID of the book you want to buy (Press 'Enter' to go back to Sorting Menu): ";
            string selectedBookID;
            cin.ignore(1000, '\n');
            getline(cin, selectedBookID);

            if (selectedBookID.empty()) {
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
            cout << "Enter the quantity (or '0' to cancel purchase): ";
            cin >> quantity;

            if (quantity == 0) {
                cout << "Purchase canceled for this book.\n" << endl;
                continue; // Skip to the next iteration of the loop
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

            updateBookStock(stoi(selectedBookID), quantity);
            // Use addOrUpdateBookInOrder instead of direct insert
            addOrUpdateBookInOrder(orderID, stoi(selectedBookID), quantity, price);

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

void Customer::OrderCart() {
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

        MYSQL_RES* res = mysql_store_result(conn);

        if (res == nullptr || mysql_num_rows(res) == 0) {
            cout << "Your order cart is empty." << endl;
            if (res) {
                mysql_free_result(res);
            }
            _getch();
            return;
        }

        // Create a table using the formatting library
        Table orderCartTable;
        orderCartTable.add_row({ "BookID", "Title", "Quantity", "Unit Price (RM)", "Total (RM)" });

        double totalPrice = 0.0;

        // Populate table rows
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            int bookID = atoi(row[1]);
            string title = row[2];
            int quantity = atoi(row[3]);
            double unitPrice = atof(row[5]);
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

        mysql_free_result(res);

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

        cout << "\nOptions:\n1. Confirm Order\n2. Update/Remove Item Quantity\n3. Back to Main Menu\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            confirmOrder();
            break;
        case 2:
            adjustItemQuantity();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
            _getch();
        }
    }
}

void Customer::confirmOrder() {
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

    MYSQL_RES* res = mysql_store_result(conn);

    if (res == nullptr || mysql_num_rows(res) == 0) {
        cout << "You have no pending orders to confirm." << endl;
        mysql_free_result(res);
        _getch();
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    int orderID = atoi(row[0]);
    mysql_free_result(res);

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

void Customer::adjustItemQuantity() {
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

    // Query to check if the BookID exists in the current order cart
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

    MYSQL_ROW row = mysql_fetch_row(res);
    int currentQuantityInCart = atoi(row[0]);  // Get the current quantity of the book in the cart
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

        // Update the stock of the book in the `book` table
        string updateStockQuery = "UPDATE book b "
            "SET b.Stock = b.Stock + " + to_string(currentQuantityInCart) +  // Increase stock by the quantity in the cart
            " WHERE b.BookID = " + to_string(bookID);
        const char* updateStockQueryCStr = updateStockQuery.c_str();

        if (mysql_query(conn, updateStockQueryCStr)) {
            cout << "Failed to update book stock. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Book stock updated successfully." << endl;
        }
    }
    else {
        // Update the price and quantity of the item in the cart
        string updateQuery = "UPDATE book_order bo "
            "JOIN `order` o ON bo.orderID = o.orderID "
            "JOIN book b ON bo.BookID = b.BookID "
            "SET bo.quantity = " + to_string(newQuantity) +
            ", bo.price = b.Price * " + to_string(newQuantity) +
            // Update the price based on quantity and book price
            " WHERE o.UserID = " + to_string(userID) +
            " AND o.orderStatus = 'pending' AND bo.BookID = " + to_string(bookID);
        const char* updateQueryCStr = updateQuery.c_str();

        if (mysql_query(conn, updateQueryCStr)) {
            cout << "Failed to update item quantity and price. Error Code: " << mysql_errno(conn) << endl;
        }
        else {
            cout << "Item quantity updated to " << newQuantity << " and price updated based on current book price." << endl;
        }

        // Update the stock of the book in the `book` table (increase or decrease based on the quantity change)
        int quantityDifference = newQuantity - currentQuantityInCart; // Calculate the difference in quantity
        string updateStockQuery = "UPDATE book b "
            "SET b.Stock = b.Stock - " + to_string(quantityDifference) +  // Adjust the stock based on the quantity change
            " WHERE b.BookID = " + to_string(bookID);
        const char* updateStockQueryCStr = updateStockQuery.c_str();

        if (mysql_query(conn, updateStockQueryCStr)) {
            cout << "Failed to update book stock. Error Code: " << mysql_errno(conn) << endl;
        }
    }

    // Update the total price of the order
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

void Customer::viewPastOrder() {
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
        table.add_row({ "Invoice Number", "Order Date", "Total Amount (RM)", "Status" });

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

void Customer::myProfile() {
    system("cls");
    cout << "\n\t\t\t\t--- View Profile ---\n" << endl;

    // Query to retrieve the customer's profile information
    string query = "SELECT Name, IC_no, Phone_no, Address, username FROM USER WHERE UserID = " + to_string(userID);
    int qstate = mysql_query(conn, query.c_str());

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row) {
            cout << "NAME: " << row[0] << endl;
            cout << "IC NO: " << row[1] << endl;
            cout << "PHONE NO: " << row[2] << endl;
            cout << "ADDRESS: " << row[3] << endl;
            cout << "USERNAME: " << row[4] << endl;
        }
        else {
            cout << "No customer found with UserID " << userID << endl;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }

    cout << "\nPress 1 to edit profile (0 to return to main menu): ";
    int choice;

    // Input validation for edit option
    while (true) {
        if (!(cin >> choice) || (choice != 0 && choice != 1)) {
            cout << "Invalid input. Please enter 1 to edit profile or 0 to return: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else {
            break;
        }
    }

    if (choice == 1) {
        EditProfile();  // Call the edit profile function
    }
    else if (choice == 0) {
        return;  // Return to the main menu
    }
}



void Customer::updateBookStock(int bookID, int quantity) {
    // Query to update the stock in the book table
    string updateStockQuery = "UPDATE book SET Stock = Stock - " + to_string(quantity) + " WHERE BookID = " + to_string(bookID);
    const char* updateStockQueryCStr = updateStockQuery.c_str();

    if (mysql_query(conn, updateStockQueryCStr)) {
        cout << "Failed to update stock for the book with BookID " << bookID << ". Error Code: " << mysql_errno(conn) << endl;
    }
}


void Customer::EditProfile() {
    system("cls");

    // Query to retrieve the customer's current profile
    string query = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE UserID = " + to_string(userID);
    int qstate = mysql_query(conn, query.c_str());

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row) {
            cout << "\nCurrent Profile Information:" << endl;
            cout << "USER ID: " << row[0] << endl;
            cout << "NAME: " << row[1] << endl;
            cout << "IC NO: " << row[2] << endl;
            cout << "PHONE NO: " << row[3] << endl;
            cout << "ADDRESS: " << row[4] << endl;
            cout << "USERNAME: " << row[5] << endl;

            // Prompt for new values
            string newName, newIC_no, newPhone_no, newAddress, newUsername;
            cout << "\nEnter new Name (leave empty to keep current): ";
            cin.ignore();  // To clear the input buffer
            getline(cin, newName);
            if (newName.empty()) newName = row[1];

            cout << "Enter new IC No (leave empty to keep current): ";
            getline(cin, newIC_no);
            if (newIC_no.empty()) newIC_no = row[2];

            cout << "Enter new Phone No (leave empty to keep current): ";
            getline(cin, newPhone_no);
            if (newPhone_no.empty()) newPhone_no = row[3];

            cout << "Enter new Address (leave empty to keep current): ";
            getline(cin, newAddress);
            if (newAddress.empty()) newAddress = row[4];

            cout << "Enter new Username (leave empty to keep current): ";
            getline(cin, newUsername);
            if (newUsername.empty()) newUsername = row[5];

            // Update the customer profile in the database
            string updateQuery = "UPDATE USER SET Name = '" + newName + "', IC_no = '" + newIC_no + "', Phone_no = '" + newPhone_no + "', Address = '" + newAddress + "', username = '" + newUsername + "' WHERE UserID = " + to_string(userID);

            qstate = mysql_query(conn, updateQuery.c_str());

            if (!qstate) {
                cout << "\nCustomer profile updated successfully!" << endl;
            }
            else {
                cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            }
        }
        else {
            cout << "No customer found with UserID " << userID << endl;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
    }

    cout << "\nPress Any Key To Go Back...";
    _getch();
}










