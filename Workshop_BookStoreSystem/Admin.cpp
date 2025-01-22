#include "Admin.h"

string getMonthName(const string& month);
void displayTable(const vector<string>& salesMonths, const vector<double>& totalSales, const vector<string>& percentageChanges, int year);
void displayBarChart(const vector<string>& salesMonths, const vector<double>& totalSales);

void Admin::AdminInterface(MYSQL* conn) {
    int choice;
    char confirmLogout;

    do {
        system("cls");
        setConsoleTextColor(14);
        cout << "+=============================================+" << endl;
        cout << "|              WELCOME, ADMIN                 |" << endl;
        cout << "+=============================================+" << endl;
        setConsoleTextColor(7);
        cout << endl;

        setConsoleTextColor(11);
        cout << "1. Book Management\n2. View Customer Information\n3. Sales Report\n0. Exit" << endl << endl;
        setConsoleTextColor(7);

        cout << "What would you like to do? ";

        while (true) {
            if (!(cin >> choice) || choice < 0 || choice > 3) {
                setConsoleTextColor(12);
                cout << "Invalid input. Please enter a number between 0 and 3: ";
                setConsoleTextColor(7);
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else {
                break;
            }
        }

        switch (choice) {
        case 0:
            system("cls");
            while (true) {
                setConsoleTextColor(14);
                cout << "Are you sure you want to log out? (Y for yes / N for no ): ";
                setConsoleTextColor(7);
                cin >> confirmLogout;
                cin.ignore(10000, '\n');

                confirmLogout = toupper(confirmLogout);

                if (confirmLogout == 'Y' || confirmLogout == 'N') {
                    break;
                }
                else {
                    setConsoleTextColor(12);
                    cout << "Invalid input. Please enter 'Y' or 'N'." << endl;
                    setConsoleTextColor(7);
                }
            }

            if (confirmLogout == 'Y') {
                setConsoleTextColor(10);
                cout << "Logging out... Goodbye!" << endl;
                this_thread::sleep_for(seconds(1));
                setConsoleTextColor(7);
                return;
            }
            else {
                setConsoleTextColor(11);
                cout << "Logout canceled. Returning to the main menu..." << endl;
                this_thread::sleep_for(seconds(1));
                setConsoleTextColor(7);
            }
            break;
        case 1:
            BookManagementMenu(conn);
            break;
        case 2:
            CustomerManagementMenu(conn);
            break;
        case 3:
            SalesReport(conn);
            break;
        default:
            break;
        }
    } while (true);
}

void Admin::BookManagementMenu(MYSQL* conn) {
    string bookid;
    int choice;
    char continueChoice;

    do {
        system("cls");
        setConsoleTextColor(14);
        cout << "=============================================" << endl;
        cout << "           BOOK MANAGEMENT MENU              " << endl;
        cout << "=============================================" << endl;
        setConsoleTextColor(7);

        setConsoleTextColor(11);
        cout << "1. View Books\n2. Add Books\n3. Update Books\n4. Delete Books\n5. Search Books\n0. Return to Main Menu" << endl << endl;
        setConsoleTextColor(7);

        cout << "What would you like to do? ";

        while (true) {
            if (!(cin >> choice) || choice < 0 || choice > 6) {
                setConsoleTextColor(12);
                cout << "Invalid input. Please enter a number between 0 and 6: ";
                setConsoleTextColor(7);
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else {
                break;
            }
        }

        switch (choice) {
        case 0:
            return;  
        case 1:
            // View all books
            if (!DisplayBooks(conn)) {
                setConsoleTextColor(12);
                cout << "No books available to display." << endl;
                setConsoleTextColor(7);
            }
            setConsoleTextColor(11);
            cout << "\n\nPress any key to return to Book Management Menu...";
            setConsoleTextColor(7);
            _getch();
            break;
        case 2:
            AddBooks(conn);  
            break;
        case 3:
            while (true) {
                // Display all books
                if (!DisplayBooks(conn)) {
                    setConsoleTextColor(12);
                    cout << "No books available to update." << endl;
                    setConsoleTextColor(7);
                    break;
                }

                setConsoleTextColor(11);
                cout << "\n\nEnter the BookID of the book you want to update (or type '0' to return to Book Management Menu): ";
                setConsoleTextColor(7);
                cin >> bookid;

                if (bookid == "0") {
                    setConsoleTextColor(12);
                    cout << "Returning to Book Management Menu..." << endl;
                    setConsoleTextColor(7);
                    break; 
                }

                // Check if the BookID exists
                string checkQuery = "SELECT COUNT(*) FROM book WHERE BookID = '" + bookid + "'";
                if (mysql_query(conn, checkQuery.c_str())) {
                    setConsoleTextColor(12);
                    cerr << "Error checking BookID: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7);
                    this_thread::sleep_for(seconds(1));
                    continue; 
                }

                MYSQL_RES* res = mysql_store_result(conn);
                if (res == nullptr) {
                    setConsoleTextColor(12);
                    cerr << "Error fetching result: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7);
                    this_thread::sleep_for(seconds(1));
                    continue; 
                }

                MYSQL_ROW row = mysql_fetch_row(res);
                int bookCount = row ? atoi(row[0]) : 0;
                mysql_free_result(res);

                if (bookCount > 0) {
                    system("cls");
                    UpdateBooks(conn, bookid); 

                    // Ask if the user wants to update another book
                    while (true) {
                        setConsoleTextColor(11);
                        cout << "\nDo you want to update another book? (y/n): ";
                        setConsoleTextColor(7);
                        cin >> continueChoice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

                        if (continueChoice == 'y' || continueChoice == 'Y') {
                            break; 
                        }
                        else if (continueChoice == 'n' || continueChoice == 'N') {
                            setConsoleTextColor(12);
                            cout << "Returning to Book Management Menu..." << endl;
                            setConsoleTextColor(7);
                            break;
                        }
                        else {
                            setConsoleTextColor(12);
                            cout << "Invalid input. Please enter 'y' or 'n'." << endl;
                            setConsoleTextColor(7);
                        }
                    }

                    if (continueChoice == 'n' || continueChoice == 'N') {
                        break;
                    }
                }
                else {
                    setConsoleTextColor(12);
                    cout << "Error: No book found with the BookID '" << bookid << "'. Please try again." << endl;
                    setConsoleTextColor(7);
                    cout << "------------------------------------------------------------------------------------" << endl;
                    this_thread::sleep_for(seconds(1));
                }
            }
            break;
        case 4:
            char deleteAnother;
            do {
                // Display all books
                if (!DisplayBooks(conn)) {
                    setConsoleTextColor(12);
                    cout << "No books available to delete." << endl;
                    setConsoleTextColor(7);
                    break;
                }

                setConsoleTextColor(11);
                cout << "\n\nEnter the BookID of the book you want to delete (or type '0' to cancel): ";
                setConsoleTextColor(7);
                cin >> bookid;

                if (bookid == "0") {
                    setConsoleTextColor(12);
                    cout << "Delete operation canceled." << endl;
                    setConsoleTextColor(7);
                    break;
                }

                // Check if the BookID exists
                string checkQuery = "SELECT COUNT(*) FROM book WHERE BookID = '" + bookid + "'";
                if (mysql_query(conn, checkQuery.c_str())) {
                    setConsoleTextColor(12);
                    cerr << "Error checking BookID: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7);
                    continue; 
                }

                MYSQL_RES* res = mysql_store_result(conn);
                if (res == nullptr) {
                    setConsoleTextColor(12);
                    cerr << "Error fetching result: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7);
                    continue; 
                }

                MYSQL_ROW row = mysql_fetch_row(res);
                int bookCount = row ? atoi(row[0]) : 0;
                mysql_free_result(res);

                if (bookCount > 0) {
                    system("cls");
                    DeleteBooks(conn, bookid); 
                }
                else {
                    setConsoleTextColor(12);
                    cout << "Error: No book found with the BookID '" << bookid << "'. Please try again." << endl;
                    setConsoleTextColor(7);
                    cout << "------------------------------------------------------------------------------------" << endl;
                }

                // Ask if the user wants to delete another book
                setConsoleTextColor(11);
                cout << "\nDo you want to delete another book? (y/n): ";
                setConsoleTextColor(7);
                cin >> deleteAnother;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            } while (deleteAnother == 'y' || deleteAnother == 'Y');

            break;
        case 5:
            cin.ignore();
            SearchBooks(conn); 
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

    auto confirmCancel = []() -> bool {
        while (true) {
            cout << "\n=============================================================================\n";
            cout << "Are you sure you want to return to the menu? (Y for yes / N for no): ";
            string confirm;
            getline(cin, confirm);
            cout << "==============================================================================\n";

            confirm.erase(0, confirm.find_first_not_of(' '));
            confirm.erase(confirm.find_last_not_of(' ') + 1);

            if (confirm == "Y" || confirm == "y") {
                return true;  
            }
            else if (confirm == "N" || confirm == "n") {
                return false; 
            }
            else {
                cout << "Invalid input. Please enter 'Y' for yes or 'N' for no." << endl;
                cout << "Press any key to try again...";
                cin.get();
            }
        }
        };

    do {
        system("cls");
        setConsoleTextColor(11); 
        cout << "=============================================\n";
        cout << "|          BOOK MANAGEMENT SYSTEM           |\n";
        cout << "=============================================\n";
        setConsoleTextColor(7);


        setConsoleTextColor(12);
        cout << "+--------------------------------------------+\n";
        cout << "| You can type '/cancel' at any time to exit.|\n";
        cout << "+--------------------------------------------+\n";
        setConsoleTextColor(7);

        // ISBN Input
        while (true) {
            setConsoleTextColor(14); 
            cout << "Enter ISBN (13 digits) : ";
            setConsoleTextColor(7); 
            cin >> isbn;

            if (isbn == "/cancel") {
                cin.ignore();
                if (confirmCancel()) {
                    return;
                }
                else {
                    continue; 
                }
            }

            // Validate ISBN (must be 13 digits and numeric)
            if (isNumeric(isbn) && isbn.length() == 13) {
                // Check if ISBN already exists in the database
                string check_query = "SELECT ISBN FROM BOOK WHERE ISBN = '" + isbn + "'";
                if (mysql_query(conn, check_query.c_str()) == 0) {
                    MYSQL_RES* res = mysql_store_result(conn);
                    if (res != nullptr) {
                        if (mysql_num_rows(res) > 0) {
                            mysql_free_result(res);
                            setConsoleTextColor(12); 
                            cout << "ISBN already exists in the database. Please enter a different ISBN.\n";
                            setConsoleTextColor(7); 
                            continue; 
                        }
                        else {
                            mysql_free_result(res);
                            break; 
                        }
                    }
                }
                else {
                    setConsoleTextColor(12); 
                    cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
                    setConsoleTextColor(7); 
                    return;
                }
            }
            else {
                setConsoleTextColor(12); 
                cout << "Invalid input. ISBN must be exactly 13 digits long.\n";
                setConsoleTextColor(7); 
            }
        }

        // Title Input
        setConsoleTextColor(14); 
        cout << "Title of the book : ";
        setConsoleTextColor(7); 
        cin.ignore();
        getline(cin, title);

        if (title == "/cancel") {
            cin.ignore();
            if (confirmCancel()) {
                return;
            }
            else {
                continue; // Go back to the input prompt
            }
        }

        // Price Input
        while (true) {
            setConsoleTextColor(14); 
            cout << "Price of the book (RM) : ";
            setConsoleTextColor(7);
            string priceInput;
            cin >> priceInput;

            if (priceInput == "/cancel") {
                cin.ignore();
                if (confirmCancel()) {
                    setConsoleTextColor(12); 
                    cout << "Operation cancelled by user.\n";
                    setConsoleTextColor(7); 
                    return;
                }
                else {
                    continue;
                }
            }

            // Check if the input has more than two decimal places
            size_t decimalPos = priceInput.find('.');
            if (decimalPos != string::npos && priceInput.length() - decimalPos - 1 > 2) {
                setConsoleTextColor(12); 
                cout << "Invalid input. Price can have at most two decimal places.\n";
                setConsoleTextColor(7);
                continue;
            }

            try {
                price = stod(priceInput);
                if (price >= 0) {
                    break;
                }
                else {
                    setConsoleTextColor(12);
                    cout << "Invalid input. Price cannot be negative.\n";
                    setConsoleTextColor(7); 
                }
            }
            catch (invalid_argument&) {
                setConsoleTextColor(12); 
                cout << "Invalid input. Please enter a valid price.\n";
                setConsoleTextColor(7); 
            }
        }

        // Stock Input
        while (true) {
            setConsoleTextColor(14);
            cout << "Stock : ";
            setConsoleTextColor(7);
            string stockInput;
            cin >> stockInput;

            if (stockInput == "/cancel") {
                cin.ignore();
                if (confirmCancel()) {
                    return;
                }
                else {
                    continue; 
                }
            }

            try {
                stock = stoi(stockInput);
                if (stock >= 0) {
                    break;
                }
                else {
                    setConsoleTextColor(12); 
                    cout << "Invalid input. Stock cannot be negative.\n";
                    setConsoleTextColor(7);
                }
            }
            catch (invalid_argument&) {
                setConsoleTextColor(12); 
                cout << "Invalid input. Please enter a valid stock value.\n";
                setConsoleTextColor(7); 
            }
        }

        // Author Input
        setConsoleTextColor(14); 
        cout << "Author of the book : ";
        setConsoleTextColor(7); 
        cin.ignore();
        getline(cin, author);

        if (author == "/cancel") {
            cin.ignore();
            if (confirmCancel()) {
                setConsoleTextColor(12); 
                cout << "Operation cancelled by user.\n";
                setConsoleTextColor(7);
                return;
            }
            else {
                continue; 
            }
        }

        // Publisher Input
        setConsoleTextColor(14); 
        cout << "Publisher of the book : ";
        setConsoleTextColor(7); 
        getline(cin, publisher);

        if (publisher == "/cancel") {
            cin.ignore();
            if (confirmCancel()) {
                setConsoleTextColor(12);
                cout << "Operation cancelled by user.\n";
                setConsoleTextColor(7); 
                return;
            }
            else {
                continue; 
            }
        }

        // Published Year Input
        while (true) {
            setConsoleTextColor(14); 
            cout << "Published Year (4 digits) : ";
            setConsoleTextColor(7); 
            cin >> publishedYear;

            if (publishedYear == "/cancel") {
                if (confirmCancel()) {
                    setConsoleTextColor(12); 
                    cout << "Operation cancelled by user.\n";
                    setConsoleTextColor(7);
                    return;
                }
                else {
                    continue;
                }
            }

            if (isNumeric(publishedYear) && publishedYear.length() == 4) {
                break;
            }
            setConsoleTextColor(12); 
            cout << "Invalid input. Please enter a valid 4-digit year.\n";
            setConsoleTextColor(7); 
        }

        // Insert into Database
        string insert_query = "INSERT INTO BOOK (ISBN, Title, Price, Stock, Author, Publisher, PublishedYear) "
            "VALUES ('" + isbn + "', '" + title + "', " + to_string(price) + ", " + to_string(stock) + ", '" + author + "', '" + publisher + "', '" + publishedYear + "')";

        if (mysql_query(conn, insert_query.c_str()) == 0) {
            setConsoleTextColor(10); // Green color for success message
            cout << "\nBook has been successfully added to the database.\n";
            setConsoleTextColor(7); 
        }
        else {
            setConsoleTextColor(12); 
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            setConsoleTextColor(7); 
        }

        // Ask if the user wants to add another book
        char choice;
        while (true) {
            setConsoleTextColor(14);
            cout << "Do you want to input another book? (y/n): ";
            setConsoleTextColor(7); 
            cin >> choice;

            choice = tolower(choice);
            if (choice == 'y' || choice == 'n') {
                break; // Exit loop if valid input
            }

            setConsoleTextColor(12); 
            cout << "Invalid input. Please enter 'y' for yes or 'n' for no.\n";
            setConsoleTextColor(7); 
        }

        if (tolower(choice) == 'n') {
            break;
        }

    } while (true);

    return;
}

void Admin::UpdateBooks(MYSQL* conn, const string& bookid) {
    string newValue;
    int fieldChoice;
    char updateAnother;

    do {
        system("cls");
        setConsoleTextColor(14);
        cout << "=============================================" << endl;
        cout << "           UPDATE BOOK DETAILS               " << endl;
        cout << "=============================================" << endl;
        setConsoleTextColor(7);

        // Fetch and display book details
        string query = "SELECT * FROM book WHERE BookID = '" + bookid + "'";
        if (mysql_query(conn, query.c_str())) {
            setConsoleTextColor(12);
            cerr << "Error executing query: " << mysql_error(conn) << endl;
            setConsoleTextColor(7);
            _getch();
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            setConsoleTextColor(12);
            cerr << "Error storing result: " << mysql_error(conn) << endl;
            setConsoleTextColor(7);
            _getch();
            return;
        }

        if (mysql_num_rows(res) == 0) {
            setConsoleTextColor(12);
            cout << "No book found with BookID: " << bookid << endl;
            setConsoleTextColor(7);
            mysql_free_result(res);
            _getch();
            return;
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        setConsoleTextColor(11);
        cout << "+---------------------------------------------------------------------------------+" << endl;
        cout << "|                                                                                 |" << endl;
        cout << "| BookID        : " << row[0] << setw(65 - strlen(row[0])) << "   |" << endl;
        cout << "| ISBN          : " << row[1] << setw(65 - strlen(row[1])) << "   |" << endl;
        cout << "| Title         : " << row[2] << setw(65 - strlen(row[2])) << "   |" << endl;
        cout << "| Price         : RM" << row[3] << setw(65 - strlen(row[3]) - 2) << "   |" << endl;
        cout << "| Stock         : " << row[4] << setw(65 - strlen(row[4])) << "   |" << endl;
        cout << "| Author        : " << row[5] << setw(65 - strlen(row[5])) << "   |" << endl;
        cout << "| Publisher     : " << row[6] << setw(65 - strlen(row[6])) << "   |" << endl;
        cout << "| Published Year: " << row[7] << setw(65 - strlen(row[7])) << "   |" << endl;
        cout << "|                                                                                 |" << endl;
        cout << "+---------------------------------------------------------------------------------+" << endl;
        setConsoleTextColor(7);
        mysql_free_result(res);

        // Select the field to update
        while (true) {
            setConsoleTextColor(14);
            cout << "\nSelect the field you want to update or enter 0 to quit:" << endl;
            setConsoleTextColor(11);
            cout << "1. Title" << endl;
            cout << "2. Price" << endl;
            cout << "3. Stock" << endl;
            cout << "4. Author" << endl;
            cout << "5. Publisher" << endl;
            cout << "6. Published Year" << endl;
            setConsoleTextColor(7);
            cout << "Enter your choice (1-6, or 0 to quit): ";
            cin >> fieldChoice;

            if (cin.fail() || fieldChoice < 0 || fieldChoice > 6) {
                cin.clear();
                cin.ignore(1000, '\n');
                setConsoleTextColor(12);
                cerr << "Invalid input. Please enter a number between 0 and 6." << endl << endl;
                setConsoleTextColor(7);
            }
            else {
                break;
            }
        }

        if (fieldChoice == 0) {
            setConsoleTextColor(12);
            cout << "Exiting update process. Returning to the Book Management Menu..." << endl;
            setConsoleTextColor(7);
            this_thread::sleep_for(chrono::seconds(1)); 
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
            cin.ignore();
            setConsoleTextColor(11);
            cout << "Enter the new value for " << field << " (or press Enter to cancel): ";
            setConsoleTextColor(7);
            getline(cin, newValue);

            if (newValue.empty()) {
                setConsoleTextColor(12);
                cout << "Cancelled update for this field." << endl;
                setConsoleTextColor(7);
                break;
            }

            // Validate Price
            if (field == "Price") {
                try {
                    double price = stod(newValue);
                    if (price < 0) {
                        setConsoleTextColor(12);
                        cerr << "Error: Price must be a non-negative numeric value with up to 2 decimal places. Please enter again." << endl;
                        setConsoleTextColor(7);
                        newValue.clear();
                        continue;
                    }
                    stringstream stream;
                    stream << fixed << setprecision(2) << price;
                    if (stream.str() != newValue) {
                        setConsoleTextColor(12);
                        cerr << "Error: Price must be a numeric value with up to 2 decimal places. Please enter again." << endl;
                        setConsoleTextColor(7);
                        newValue.clear();
                        continue;
                    }
                }
                catch (const exception&) {
                    setConsoleTextColor(12);
                    cerr << "Error: Price must be a numeric value with up to 2 decimal places. Please enter again." << endl;
                    setConsoleTextColor(7);
                    newValue.clear(); 
                    continue;
                }
            }
            // Validate Stock and Published Year
            else if (field == "Stock" || field == "PublishedYear") {
                try {
                    int number = stoi(newValue);
                    if (number < 0) {
                        setConsoleTextColor(12);
                        cerr << "Error: " << field << " must be a non-negative whole number. Please enter again." << endl;
                        setConsoleTextColor(7);
                        newValue.clear(); // Clear newValue for re-prompt
                        continue;
                    }
                    if (to_string(number) != newValue) {
                        setConsoleTextColor(12);
                        cerr << "Error: " << field << " must be a whole number. Please enter again." << endl;
                        setConsoleTextColor(7);
                        newValue.clear(); // Clear newValue for re-prompt
                        continue;
                    }
                }
                catch (const std::exception&) {
                    setConsoleTextColor(12);
                    cerr << "Error: " << field << " must be a whole number. Please enter again." << endl;
                    setConsoleTextColor(7);
                    newValue.clear(); // Clear newValue for re-prompt
                    continue;
                }
            }

            // Confirmation step
            setConsoleTextColor(11);
            cout << "You entered: " << field << " = " << newValue << ". Do you want to confirm this update? (y/n): ";
            setConsoleTextColor(7);
            char confirm;
            cin >> confirm;

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if (confirm == 'y' || confirm == 'Y') {
                string updateQuery = "UPDATE book SET " + field + " = '" + newValue + "' WHERE BookID = '" + bookid + "'";

                if (mysql_query(conn, updateQuery.c_str()) == 0) {
                    // Check if any rows were affected
                    if (mysql_affected_rows(conn) > 0) {
                        setConsoleTextColor(10); 
                        cout << field << " updated successfully!" << endl;
                        setConsoleTextColor(7);
                    }
                    else {
                        setConsoleTextColor(12); 
                        cout << "Warning: No rows were updated. The book with BookID " << bookid << " may not exist or the data is unchanged." << endl;
                        setConsoleTextColor(7);
                    }
                }
                else {
                    setConsoleTextColor(12);
                    cerr << "Error updating book: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7);
                    cerr << "Failed query: " << updateQuery << endl;
                    (void)_getch();
                }
                break;
            }
            else if (confirm == 'n' || confirm == 'N') {
                setConsoleTextColor(12); 
                cout << "Update for " << field << " has been cancelled." << endl;
                setConsoleTextColor(7);
                continue;
            }
            else {
                setConsoleTextColor(12); 
                cerr << "Invalid input. Please enter 'y' to confirm or 'n' to cancel." << endl;
                setConsoleTextColor(7);
                continue;
            }
        }
        // Ask to update another attribute
        while (true) {
            setConsoleTextColor(11);
            cout << "Do you want to update another attribute in the SAME book? (y/n): ";
            setConsoleTextColor(7);
            cin >> updateAnother;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if (updateAnother == 'y' || updateAnother == 'Y') {
                break; 
            }
            else if (updateAnother == 'n' || updateAnother == 'N') {
                return; 
            }
            else {
                setConsoleTextColor(12);
                cerr << "Invalid input. Please enter 'y' or 'n'." << endl;
                setConsoleTextColor(7);
            }
        }
    } while (true);
}

void Admin::DeleteBooks(MYSQL* conn, const string& bookid) {
    // Check if the book exists and fetch its details
    string checkQuery = "SELECT * FROM book WHERE BookID = '" + bookid + "'";
    const char* checkQ = checkQuery.c_str();

    if (mysql_query(conn, checkQ)) {
        setConsoleTextColor(12); 
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        setConsoleTextColor(7); 
        _getch();
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr) {
        setConsoleTextColor(12);
        cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
        setConsoleTextColor(7); 
        _getch();
        return;
    }

    if (mysql_num_rows(res) == 0) {
        setConsoleTextColor(12);
        cout << "No book found with the given ID: " << bookid << endl;
        setConsoleTextColor(7); 
        mysql_free_result(res);
        _getch();
        return;
    }

    // Fetch and display book details
    MYSQL_ROW row = mysql_fetch_row(res);
    setConsoleTextColor(11); 
    cout << "\n==============================================================" << endl;
    cout << "                      BOOK DETAILS                           " << endl;
    cout << "==============================================================" << endl;
    cout << " BookID        : " << row[0] << endl;
    cout << " ISBN          : " << row[1] << endl;
    cout << " Title         : " << row[2] << endl;
    cout << " Price         : RM" << row[3] << endl;
    cout << " Stock         : " << row[4] << endl;
    cout << " Author        : " << row[5] << endl;
    cout << " Publisher     : " << row[6] << endl;
    cout << " Published Year: " << row[7] << endl;
    cout << "==============================================================\n" << endl;

    // Free the result set
    mysql_free_result(res);

    setConsoleTextColor(11); 
    cout << "Are you sure you want to delete this book? (y/n): ";
    setConsoleTextColor(7); 
    char confirmation;
    cin >> confirmation;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (tolower(confirmation) != 'y') {
        setConsoleTextColor(12); 
        cout << "Deletion canceled. Press any key to continue...\n";
        setConsoleTextColor(7); 
        _getch();
        return;
    }

    // Delete the book from the database
    string deleteQuery = "DELETE FROM book WHERE BookID = '" + bookid + "'";
    const char* deleteQ = deleteQuery.c_str();

    if (!mysql_query(conn, deleteQ)) {
        if (mysql_affected_rows(conn) > 0) {
            setConsoleTextColor(10); 
            cout << "\n==============================================================" << endl;
            cout << "  Book with BookID " << bookid << " has been successfully deleted.  " << endl;
            cout << "==============================================================\n" << endl;
            setConsoleTextColor(7); 
        }
        else {
            setConsoleTextColor(12); 
            cout << "\n==============================================================" << endl;
            cout << "  No changes made. Book might have already been deleted.        " << endl;
            cout << "==============================================================\n" << endl;
            setConsoleTextColor(7);
        }
    }
    else {
        setConsoleTextColor(12); 
        cout << "\n==============================================================" << endl;
        cout << "  Error deleting book. Error Code: " << mysql_errno(conn) << "         " << endl;
        cout << "==============================================================\n" << endl;
        setConsoleTextColor(7); 
    }
}

void Admin::SalesReport(MYSQL* conn) {
    int choice;

    if (conn == nullptr) {
        setConsoleTextColor(12); 
        cout << "Error: Database connection is invalid.\n";
        setConsoleTextColor(7); 
        _getch();
        return;
    }

    do {
        system("cls");
        setConsoleTextColor(14); 
        cout << "========================================\n";
        cout << "          SALES REPORT MENU\n";
        cout << "========================================\n";
        setConsoleTextColor(7);

        setConsoleTextColor(11); 
        cout << "\nChoose Report Type:\n";
        cout << "1. Monthly Sales Report\n";
        cout << "2. Total Book Sales Report\n";
        cout << "3. Book Sales in a Month\n";
        cout << "0. Exit\n";
        setConsoleTextColor(7);

        setConsoleTextColor(10); 
        cout << "\nEnter your choice: ";
        setConsoleTextColor(7);

        // Input validation
        if (!(cin >> choice)) {
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            setConsoleTextColor(12); 
            cout << "Invalid input. Please enter a valid number.\n";
            setConsoleTextColor(7); 
            _getch(); 
            continue; 
        }

        // Handle user choice
        try {
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
                return;
            default:
                setConsoleTextColor(12); 
                cout << "\nInvalid choice. Please select a valid option.\n";
                setConsoleTextColor(7); 
                _getch(); 
                break;
            }
        }
        catch (const exception& e) {
            setConsoleTextColor(12); 
            cout << "\nAn error occurred: " << e.what() << "\n";
            setConsoleTextColor(7); 
            _getch(); 
        }
    } while (true); 
}

// Function to display the table
void displayTable(const vector<string>& salesMonths, const vector<double>& totalSales, const vector<string>& percentageChanges, int year) {
    // Create a table object
    Table salesTable;

    salesTable.add_row({ "Sales Month", "Total Sales (RM)", "Sales Change (%)" });

    // Format headers with color
    salesTable[0].format()
        .font_color(Color::yellow) // Yellow text for headers
        .font_style({ FontStyle::bold }) // Bold 
        .font_align(FontAlign::center); 

    // Add table rows with color
    for (size_t i = 0; i < salesMonths.size(); ++i) {
        salesTable.add_row({
            salesMonths[i],
            to_string(totalSales[i]).substr(0, to_string(totalSales[i]).find(".") + 3), // Format to 2 decimal places
            percentageChanges[i]
            });

        if (i % 2 == 0) {
            salesTable[i + 1].format()
                .font_color(Color::green) 
                .font_align(FontAlign::center); 
        }
        else {
            salesTable[i + 1].format()
                .font_color(Color::cyan) 
                .font_align(FontAlign::center); 
        }
    }

    // Format the table borders with color
    salesTable.format()
        .border_top("=").border_bottom("=").border_left("|").border_right("|").corner("+")
        .border_color(Color::magenta) 
        .column_separator("|")
        .font_align(FontAlign::center); 

    // Print the table
    setConsoleTextColor(14); // Yellow color for the title
    cout << "\t    MONTHLY SALES REPORT FOR " << year << endl;
    setConsoleTextColor(7); 
    cout << salesTable << endl;
}

// Function to display the bar chart
void displayBarChart(const vector<string>& salesMonths, const vector<double>& totalSales) {
    // Determine the maximum sales value to scale the chart
    double maxSales = 0;
    for (double sales : totalSales) {
        if (sales > maxSales) {
            maxSales = sales;
        }
    }

    cout << "\n\t\t\t--- Monthly Sales Report ---\n";
    cout << "\t\tSales Chart (Total Sales represented by colored bars)\n\n";

    for (size_t i = 0; i < salesMonths.size(); ++i) {
        // Calculate the number of blocks to represent sales
        int numBlocks = static_cast<int>((totalSales[i] / maxSales) * 50); // Max width of 50 chars

        // Print the month
        setConsoleTextColor(14); 
        cout << setw(15) << salesMonths[i] << " | ";
        setConsoleTextColor(7); 


        setConsoleTextColor(176); 
        for (int j = 0; j < numBlocks; ++j) {
            cout << " "; 
        }
        setConsoleTextColor(7); 

        // Print the sales value
        cout << " RM" << fixed << setprecision(2) << totalSales[i] << endl;

        // Add space between bars
        cout << "                | ";
        cout << endl; 
    }
}

void Admin::MonthlySalesReport(MYSQL* conn) {
    while (true) {
        system("cls");
        string input;
        int year;

        // Prompt for the year
        while (true) {
            cout << "Enter the year for the sales report (e.g., 2025) or 'B' to return to the report menu: ";
            cin >> input;

            // Check if the user wants to go back
            if (tolower(input[0]) == 'b') {
                return; // Exit the function to go back to the report menu
            }

            // Validate year input
            try {
                year = stoi(input);
                if (year >= 2000 && year <= 2100) {
                    break; 
                }
                else {
                    setConsoleTextColor(12);
                    cout << "Invalid year. Please enter a year between 2000 and 2100." << endl;
                    setConsoleTextColor(7);
                }
            }
            catch (...) {
                setConsoleTextColor(12);
                cout << "Invalid input. Please enter a valid year (e.g., 2025) or 'B' to go back." << endl;
                setConsoleTextColor(7);
            }
        }

        // Query to fetch sales data
        string query = "SELECT DATE_FORMAT(o.orderDate, '%Y-%m') AS Sales_Month, "
            "SUM(bo.price) AS Total_Sales "
            "FROM book_order bo "
            "JOIN `order` o ON bo.orderID = o.orderID "
            "WHERE o.orderStatus = 'completed' AND "
            "(YEAR(o.orderDate) = " + to_string(year) + " OR "
            "(YEAR(o.orderDate) = " + to_string(year - 1) + " AND MONTH(o.orderDate) = 12)) "
            "GROUP BY Sales_Month "
            "ORDER BY Sales_Month ASC;";

        if (mysql_query(conn, query.c_str())) {
            cerr << "Query execution failed: " << mysql_error(conn) << endl;
            _getch();
            continue;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (!result) {
            cerr << "Failed to store result: " << mysql_error(conn) << endl;
            _getch();
            continue;
        }

        // Vectors to hold months, sales, and percentage changes,pdsosa
        vector<string> salesMonths;
        vector<double> totalSales;
        vector<string> percentageChanges;

        MYSQL_ROW row;
        double decemberPreviousYearSales = 0.0;
        bool isFirstMonth = true;

        while ((row = mysql_fetch_row(result))) {
            string salesMonth = row[0];
            double totalSalesAmount = atof(row[1]);

            // Check if this is December of the previous year
            bool isDecemberPreviousYear = (salesMonth.substr(0, 4) == to_string(year - 1) && salesMonth.substr(5, 2) == "12");

            if (isDecemberPreviousYear) {
                decemberPreviousYearSales = totalSalesAmount;
                continue;
            }

            string monthName = getMonthName(salesMonth.substr(5, 2));
            salesMonths.push_back(monthName);
            totalSales.push_back(totalSalesAmount);

            string percentageChange = "N/A";
            if (isFirstMonth) {
                if (decemberPreviousYearSales > 0) {
                    double change = ((totalSalesAmount - decemberPreviousYearSales) / decemberPreviousYearSales) * 100;
                    percentageChange = change >= 0 ? "Increases by " + to_string(change).substr(0, to_string(change).find(".") + 3) + "%"
                        : "Decreases by " + to_string(abs(change)).substr(0, to_string(abs(change)).find(".") + 3) + "%";
                }
            }
            else {
                double change = ((totalSalesAmount - totalSales[totalSales.size() - 2]) / totalSales[totalSales.size() - 2]) * 100;
                percentageChange = change >= 0 ? "Increases by " + to_string(change).substr(0, to_string(change).find(".") + 3) + "%"
                    : "Decreases by " + to_string(abs(change)).substr(0, to_string(abs(change)).find(".") + 3) + "%";
            }
            percentageChanges.push_back(percentageChange);
            isFirstMonth = false;
        }

        mysql_free_result(result);

        // Toggle between table and bar chart
        bool showTable = true;
        while (true) {
            system("cls");

            if (showTable) {
                displayTable(salesMonths, totalSales, percentageChanges, year);
            }
            else {
                displayBarChart(salesMonths, totalSales);
            }

            setConsoleTextColor(14);
            cout << "\nPress 'T' to view the Table, 'C' to view the Bar Graph, 'B' to input another year, or 'Q' to quit...\n";
            setConsoleTextColor(7);

            char choice = _getch();
            choice = tolower(choice);

            if (choice == 't') {
                showTable = true;
            }
            else if (choice == 'c') {
                showTable = false;
            }
            else if (choice == 'b') {
                break; 
            }
            else if (choice == 'q') {
                return;
            }
        }
    }
}

void Admin::BookSalesReport(MYSQL* conn) {
    system("cls");

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

    // Variables to hold total quantity sold and the table for book sales report
    int totalQuantitySoldForAllBooks = 0;
    Table bookSalesTable;
    bookSalesTable.add_row({ "Book Title", "Total Quantity Sold", "Total Sales", "Popularity Percentage(%)" });

    //Calculate total quantity sold for all books
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        int quantitySold = atoi(row[1]); // Total quantity sold for the book
        totalQuantitySoldForAllBooks += quantitySold; // Sum up the total quantity sold
    }

    // Move the result pointer back to the beginning 
    mysql_data_seek(result, 0);

    // Second pass: Populate the table and calculate the popularity percentage based on quantity sold
    while ((row = mysql_fetch_row(result))) {
        string bookTitle = row[0];
        int quantitySold = atoi(row[1]);
        double totalSales = atof(row[2]);

        // Calculate the popularity percentage based on quantity sold
        double popularityPercentage = (double(quantitySold) / totalQuantitySoldForAllBooks) * 100;

        // Add the row to the table
        std::ostringstream salesStream;
        salesStream << std::fixed << std::setprecision(2) << totalSales;

        std::ostringstream percentageStream;
        percentageStream << std::fixed << std::setprecision(2) << popularityPercentage;

        bookSalesTable.add_row({ bookTitle, std::to_string(quantitySold), salesStream.str(), percentageStream.str() });
    }


    setConsoleTextColor(14); // Yellow color 
    cout << "======================================================================\n";
    cout << "                     BOOK SALES PERFORMANCE REPORT         \n";
    cout << "======================================================================\n";
    setConsoleTextColor(7); 

    setConsoleTextColor(11);
    cout << "This report provides insights into the sales performance of books.\n";
    cout << "It includes the total quantity sold, total sales revenue,\n";
    cout << "and the popularity percentage of each book based on the total quantity sold.\n";
    cout << "======================================================================\n";
    setConsoleTextColor(7); 

    // Format and display the report table
    reportTableFormat(bookSalesTable);
    cout << bookSalesTable << endl;

    mysql_free_result(result);
    cout << "\nPress any key to return to the menu...";
    _getch();
}

void Admin::BookSalesInMonth(MYSQL* conn) {
    while (true) {
        system("cls");

        setConsoleTextColor(14); 
        cout << "===================================================================\n";
        cout << "              MONTHLY BOOK SALES REPORT             \n";
        cout << "===================================================================\n";
        setConsoleTextColor(7); 

        setConsoleTextColor(11); 
        cout << "This report shows the total quantity sold and total sales revenue\n";
        cout << "for each book in the selected month.\n";
        cout << "===================================================================\n\n";
        setConsoleTextColor(7); 

        string month;

        setConsoleTextColor(14);
        cout << "Enter the month for the report (format: YYYY-MM) or 'B' to return to the report menu: ";
        setConsoleTextColor(7); 
        cin >> month;

        if (tolower(month[0]) == 'b') {
            setConsoleTextColor(12); 
            cout << "\nAre you sure you want to return to the report menu? (Y/N): ";
            setConsoleTextColor(7); 
            char confirm;
            cin >> confirm;
            if (tolower(confirm) == 'y') {
                return; 
            }
            else {
                continue;
            }
        }

        // Validate month format,see workshop 1 sequel
        regex month_format("^\\d{4}-(0[1-9]|1[0-2])$");
        if (!regex_match(month, month_format)) {
            setConsoleTextColor(12); 
            cerr << "Invalid month format. Please use YYYY-MM." << endl;
            setConsoleTextColor(7);
            cout << "\nPress any key to try again...";
            _getch();
            continue; 
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
            setConsoleTextColor(12); 
            cerr << "Query execution failed: " << mysql_error(conn) << endl;
            setConsoleTextColor(7); 
            cout << "\nPress any key to try again...";
            _getch();
            continue; 
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (!result) {
            setConsoleTextColor(12); 
            cerr << "Failed to store result: " << mysql_error(conn) << endl;
            setConsoleTextColor(7); 
            cout << "\nPress any key to try again...";
            _getch();
            continue; 
        }

        // Display the selected month
        setConsoleTextColor(14); 
        cout << "\n===================================================================\n";
        cout << "              Book Sales Report for " << month << "\n";
        cout << "=====================================================================\n";
        setConsoleTextColor(7); 

        Table monthlyBookSalesTable;
        monthlyBookSalesTable.add_row({ "Book Title", "Total Quantity Sold", "Total Sales" });

        MYSQL_ROW row;
        bool hasData = false;
        while ((row = mysql_fetch_row(result))) {
            hasData = true;
            monthlyBookSalesTable.add_row({ row[0], row[1], row[2] });
        }

        if (!hasData) {
            setConsoleTextColor(12); 
            cout << "No sales data found for the month " << month << ".\n";
            setConsoleTextColor(7); 
        }
        else {
            reportTableFormat(monthlyBookSalesTable);
            cout << monthlyBookSalesTable << endl;
        }

        mysql_free_result(result);

        // Provide an option to go back to the month input
        setConsoleTextColor(14); 
        cout << "\nPress 'B' to input another month or any other key to return to the report menu...";
        setConsoleTextColor(7); 
        char choice = _getch();
        if (tolower(choice) == 'b') {
            continue; 
        }
        else {
            break; // Exit the function 
        }
    }
}

void Admin::ViewCustomerInfo(MYSQL* conn) {
    // Check if the connection to the database is successful
    if (conn == nullptr) {
        cout << "\033[1;31mError: Database connection failed!\033[0m" << endl;
        (void)_getch();
        return; // Exit the function if connection failed
    }

    int choice = 0;
    bool continueSearching = true;

    while (continueSearching) {
        system("cls");
        setConsoleTextColor(14); // Yellow for the title
        cout << "===================================================\n";
        cout << "              VIEW CUSTOMER INFORMATION            \n";
        cout << "===================================================\n";
        setConsoleTextColor(7); 

        setConsoleTextColor(11); 
        cout << "\n1. View all customers\n";
        cout << "2. Search by Name\n";
        cout << "3. Search by Username\n";
        cout << "4. Search by Phone No\n";
        cout << "5. Search by Address\n";
        cout << "0. Go back\n";
        setConsoleTextColor(7); 

        setConsoleTextColor(10); 
        cout << "\nEnter your choice: ";
        setConsoleTextColor(7); 


        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            setConsoleTextColor(12); 
            cout << "\nInvalid input! Please enter a number between 0 and 5.\n";
            setConsoleTextColor(7); 
            this_thread::sleep_for(chrono::seconds(1));
            continue; 
        }

        // If user chooses 0, go back
        if (choice == 0) {
            setConsoleTextColor(14);
            cout << "\nReturning to the previous menu...\n";
            setConsoleTextColor(7);
            this_thread::sleep_for(chrono::seconds(1));
            return; 
        }

        if (choice < 1 || choice > 5) {
            setConsoleTextColor(12); 
            cout << "\nInvalid choice! Please enter a number between 0 and 5.\n";
            setConsoleTextColor(7);
            this_thread::sleep_for(chrono::seconds(1));
            continue; 
        }

        cin.ignore();

        string searchQuery;
        string keyword;

        bool validSearch = false;
        while (!validSearch) {
            system("cls");
            switch (choice) {
            case 1:
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer'";
                validSearch = true;
                break;
            case 2:
                setConsoleTextColor(11); 
                cout << "Enter Name to search: ";
                setConsoleTextColor(7); 
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND Name LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            case 3:
                setConsoleTextColor(11); 
                cout << "Enter Username to search: ";
                setConsoleTextColor(7); 
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND username LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            case 4:
                setConsoleTextColor(11);
                cout << "Enter Phone No to search: ";
                setConsoleTextColor(7); 
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND Phone_no LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            case 5:
                setConsoleTextColor(11); 
                cout << "Enter Address to search: ";
                setConsoleTextColor(7); 
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND Address LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            }


            int qstate = mysql_query(conn, searchQuery.c_str());

            if (qstate) {
                setConsoleTextColor(12); 
                cout << "\nQuery Execution Problem! Error Code: " << mysql_errno(conn) << " - " << mysql_error(conn) << "\n";
                setConsoleTextColor(7); 
                _getch();
                return; // Exit the function 
            }

            MYSQL_RES* res = mysql_store_result(conn);
            if (res == nullptr) {
                setConsoleTextColor(12); 
                cout << "\nError fetching result. Error Code: " << mysql_errno(conn) << " - " << mysql_error(conn) << "\n";
                setConsoleTextColor(7); 
                _getch();
                return;
            }

            if (mysql_num_rows(res) == 0) {
                setConsoleTextColor(12); 
                cout << "\nNo customers found matching the criteria. Please try again.\n";
                setConsoleTextColor(7); 
                mysql_free_result(res);
                this_thread::sleep_for(chrono::seconds(1));
                continue; 
            }

            // Prepare to display the results in a table format
            Table customerInformation;
            customerInformation.add_row({ "USER ID", "NAME", "IC NO", "PHONE NO", "ADDRESS", "USERNAME" });

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                customerInformation.add_row({ row[0], row[1], row[2], row[3], row[4], row[5] });
            }

            
            customerTableFormat(customerInformation);
            cout << customerInformation << endl;
            mysql_free_result(res);
            validSearch = true;
        }

        // Prompt to continue or stop
        setConsoleTextColor(11); 
        cout << "\nWould you like to search again? (Y/N): ";
        setConsoleTextColor(7); 
        char contChoice;
        cin >> contChoice;
        cin.ignore(); 

        if (contChoice == 'Y' || contChoice == 'y') {
            continueSearching = true;
        }
        else if (contChoice == 'N' || contChoice == 'n') {
            continueSearching = false;
        }
        else {
            setConsoleTextColor(12); 
            cout << "\nInvalid input! Please enter Y or N.\n";
            setConsoleTextColor(7); 
        }
    }

    setConsoleTextColor(14); 
    cout << "\nPress Any Key To Go Back...";
    setConsoleTextColor(7); 
    _getch();
}

void Admin::ViewCustomerOrders(MYSQL* conn) {
    int customerID = 0;
    bool continueSearching = true;

    while (continueSearching) {
        system("cls");
        // Query to fetch customer details and the number of completed orders they have made
        string query = "SELECT u.UserID, u.Phone_no, u.Name, u.username, "
            "COUNT(CASE WHEN o.orderStatus = 'completed' THEN o.orderID END) AS total_completed_orders "
            "FROM USER u "
            "LEFT JOIN `order` o ON u.UserID = o.UserID "
            "WHERE u.Role = 'customer' "
            "GROUP BY u.UserID "
            "ORDER BY u.UserID";

        int qstate = mysql_query(conn, query.c_str());

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;

            Table customerOrdersTable;
            customerOrdersTable.add_row({ "User ID", "Phone No", "Name", "Username", "Total Completed Orders" });
            customerOrdersTable[0]
                .format()
                .font_style({ FontStyle::bold })
                .font_color(Color::green);

            // Fetch and display the results
            while ((row = mysql_fetch_row(res))) {
                customerOrdersTable.add_row({
                    row[0],  // User ID
                    row[1],  // Phone No
                    row[2],  // Name
                    row[3],  // Username
                    row[4]   // Completed Orders
                    });
            }

            // Print the table
            cout << "\n\t\t\t--- Customer Orders Summary ---\n" << endl;
            cout << customerOrdersTable << "\n";

            mysql_free_result(res);
        }
        else {
            cout << "\033[1;31mQuery Execution Problem! Error Code: " << mysql_errno(conn) << "\033[0m" << endl;
        }


        cout << "\nEnter Customer ID to view completed orders (or enter 0 to go back): ";
        cin >> customerID;

        if (customerID == 0) {
            continueSearching = false;  
            break;
        }

        // Check if the Customer ID exists in the database
        string checkQuery = "SELECT UserID FROM USER WHERE UserID = " + to_string(customerID) + " AND Role = 'customer'";
        qstate = mysql_query(conn, checkQuery.c_str());

        if (!qstate) {
            MYSQL_RES* checkRes = mysql_store_result(conn);
            if (mysql_num_rows(checkRes) == 0) {
                cout << "\033[1;31mError: Customer ID " << customerID << " does not exist or is not a customer.\033[0m" << endl;
                mysql_free_result(checkRes);
                this_thread::sleep_for(chrono::seconds(2)); // Wait for 2 seconds 
                continue; 
            }
            mysql_free_result(checkRes);
        }
        else {
            cout << "\033[1;31mQuery Execution Problem! Error Code: " << mysql_errno(conn) << "\033[0m" << endl;
            this_thread::sleep_for(chrono::seconds(2)); 
            continue; 
        }

        // Query to fetch order details for the given customer
        query = "SELECT o.orderID, o.orderDate, o.totalAmount, b.Title, bo.quantity, bo.price "
            "FROM `order` o "
            "JOIN `book_order` bo ON o.orderID = bo.orderID "
            "JOIN `book` b ON bo.BookID = b.BookID "
            "WHERE o.UserID = " + to_string(customerID) + " AND o.orderStatus = 'completed' "
            "ORDER BY o.orderID DESC";

        qstate = mysql_query(conn, query.c_str());

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;

            int currentOrderID = -1;
            Table orderTable;
            int totalCompletedOrders = 0; 

            while ((row = mysql_fetch_row(res))) {
                int orderID = stoi(row[0]);

                // When a new order starts
                if (orderID != currentOrderID) {
                    // Print the table for the previous order if it exists
                    if (currentOrderID != -1) {
                        cout << orderTable << "\n\n";
                    }

                    cout << "+-----------------------------------------------------------------------------+\n";

                    // Reset the table for the next order
                    orderTable = Table();
                    orderTable.add_row({ "Title", "Quantity", "Price (RM)" });
                    orderTable[0]
                        .format()
                        .font_style({ FontStyle::bold })
                        .font_color(Color::green);

                    currentOrderID = orderID;
                    totalCompletedOrders++; // Increment the total completed orders count

                    // Print order-level information
                    cout << "Order ID: " << row[0]
                        << "\nOrder Date: " << row[1]
                        << "\nTotal Amount: RM " << row[2] << "\n";
                    cout << "Books in this order:\n";
                }

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

            // Display the total number of completed orders
            cout << "\n\033[1;32mTotal Completed Orders: " << totalCompletedOrders << "\033[0m\n";

            mysql_free_result(res);
        }
        else {
            cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        }

        // Ask if the user wants to continue searching
        char contChoice;
        cout << "\nWould you like to search for another customer? (Y/N): ";
        cin >> contChoice;

        if (contChoice == 'Y' || contChoice == 'y') {
            continueSearching = true;  // Continue searching for another customer
        }
        else if (contChoice == 'N' || contChoice == 'n') {
            continueSearching = false; // Exit the loop
        }
        else {
            cout << "\033[1;31mInvalid input! Please enter Y or N.\033[0m" << endl;
        }
    }
}

bool Admin::DisplayBooks(MYSQL* conn) {
    system("cls");
    string query = "SELECT ISBN,BookID, Title, Price, Stock, Author, Publisher, PublishedYear FROM book";
    const char* q = query.c_str();

    if (mysql_query(conn, q)) {
        cout << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr) {
        cout << "Error fetching result set. Error Code: " << mysql_errno(conn) << endl;
        _getch();
        return false;
    }

    if (mysql_num_rows(res) == 0) {
        cout << "There are no books in the database." << endl;
        mysql_free_result(res);
        return false;
    }

    // Create a Table object and add headers
    Table viewBook;
    viewBook.add_row({"ISBN", "BookID", "Title", "Price (RM)", "Available Stock", "Author", "Publisher", "Published Year" });

    // Loop through the result set and add rows
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL) {
        viewBook.add_row({
            row[0] ? row[0] : "NULL",  
            row[1] ? row[1] : "NULL",  
            row[2] ? row[2] : "NULL",  
            row[3] ? row[3] : "NULL",  
            row[4] ? row[4] : "NULL", 
            row[5] ? row[5] : "NULL", 
            row[6] ? row[6] : "NULL",  
            row[7] ? row[7] : "NULL"  
            });
    }
    cartTableFormat(viewBook);
    cout << viewBook;

    mysql_free_result(res);
    return true;
}

void Admin::SearchBooks(MYSQL* conn) {
    while (true) {
        system("cls");
        setConsoleTextColor(14); 
        cout << "===================================================\n";
        cout << "                    SEARCH BOOKS             \n";
        cout << "===================================================\n";
        setConsoleTextColor(7); 

        // Display menu options with colors
        setConsoleTextColor(11); 
        cout << "\n1. Search by ISBN\n";
        cout << "2. Search by Book ID\n";
        cout << "3. Search by Title\n";
        cout << "4. Search by Author\n";
        cout << "5. Search by Publisher\n";
        cout << "6. Back to Admin Menu\n";
        setConsoleTextColor(7); 

        // Prompt for user input
        setConsoleTextColor(10); 
        cout << "\nEnter your choice: ";
        setConsoleTextColor(7); 

        int choice;
        if (!(cin >> choice) || choice < 1 || choice > 6) {
            setConsoleTextColor(12);
            cout << "\nInvalid input. Please enter a number between 1 and 6.\n";
            setConsoleTextColor(7); 
            cin.clear();
            cin.ignore(1000, '\n');
            _getch();
            continue;
        }

        cin.ignore(1000, '\n'); 

        if (choice == 6) {
            setConsoleTextColor(14); 
            cout << "\nReturning to Admin Menu...\n";
            setConsoleTextColor(7); 
            _getch();
            return;
        }

        string keyword;
        string query;
        switch (choice) {
        case 1: // Search by ISBN
            setConsoleTextColor(11); 
            cout << "\nEnter the ISBN to search: ";
            setConsoleTextColor(7); 
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE ISBN LIKE '%" + keyword + "%'";
            break;
        case 2: // Search by Book ID
            setConsoleTextColor(11); 
            cout << "\nEnter the Book ID to search: ";
            setConsoleTextColor(7); 
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE BookID LIKE '%" + keyword + "%'";
            break;
        case 3: // Search by Title
            setConsoleTextColor(11);
            cout << "\nEnter the Title to search: ";
            setConsoleTextColor(7); 
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE Title LIKE '%" + keyword + "%'";
            break;
        case 4: // Search by Author
            setConsoleTextColor(11); 
            cout << "\nEnter the Author to search: ";
            setConsoleTextColor(7); 
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE Author LIKE '%" + keyword + "%'";
            break;
        case 5:
            setConsoleTextColor(11); 
            cout << "\nEnter the Publisher to search: ";
            setConsoleTextColor(7); 
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE Publisher LIKE '%" + keyword + "%'";
            break;
        }

        const char* q = query.c_str();
        if (mysql_query(conn, q)) {
            setConsoleTextColor(12); 
            cout << "\nQuery Execution Problem! Error Code: " << mysql_errno(conn) << "\n";
            setConsoleTextColor(7); 
            _getch();
            continue;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr || mysql_num_rows(res) == 0) {
            setConsoleTextColor(12); 
            cout << "\nNo books found matching your criteria.\n";
            setConsoleTextColor(7); 
            mysql_free_result(res);
            _getch();
            continue;
        }

        vector<MYSQL_ROW> rows;
        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            rows.push_back(row);
        }

        while (true) {
            system("cls");
            cout << "\nBooks Found:\n";
            Table table;
            table.add_row({ "BookID", "ISBN", "Title", "Price", "Available Stock", "Author", "Publisher", "Published Year" });

            for (const auto& row : rows) {
                table.add_row({ row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7] });
            }

            cartTableFormat(table);
            cout << table << endl;

            setConsoleTextColor(11); 
            cout << "\nEnter the BookID of the book you want to manage (Press '0' to skip): ";
            setConsoleTextColor(7); 
            string selectedBookID;
            getline(cin, selectedBookID);

            if (selectedBookID == "0") {
                break; // Exit to search again
            }

            bool validID = false;
            MYSQL_ROW selectedRow = nullptr;

            for (const auto& r : rows) {
                if (r && selectedBookID == r[0]) {
                    validID = true;
                    selectedRow = r;
                    break;
                }
            }

            if (!validID) {
                setConsoleTextColor(12); 
                cout << "\nInvalid BookID. Please try again.\n";
                setConsoleTextColor(7); 
                _getch();
                continue;
            }

            setConsoleTextColor(14); 
            cout << "\nYou selected Book: " << selectedRow[2] << "\n";
            setConsoleTextColor(7); 

            // Ask the user what they want to do with the selected book
            while (true) {
                setConsoleTextColor(11);
                cout << "What would you like to do with this book?\n";
                cout << "1. Update Book\n";
                cout << "2. Delete Book\n";
                cout << "0. Back to Book Selection\n"; 
                setConsoleTextColor(7); 

                setConsoleTextColor(10); 
                cout << "Enter your choice: ";
                setConsoleTextColor(7); 
                int action;
                cin >> action;
                cin.ignore(); 

                if (action == 0) {
                    break; 
                }

                switch (action) {
                case 1:
                    UpdateBooks(conn, selectedRow[0]); 
                    break;
                case 2:
                    system("cls");
                    DeleteBooks(conn, selectedRow[0]); 
                    break;
                default:
                    setConsoleTextColor(12); 
                    cout << "\nInvalid action!\n";
                    setConsoleTextColor(7); 
                    break;
                }
                break;
            }
        }

        mysql_free_result(res);

        setConsoleTextColor(11); 
        cout << "\nWould you like to search again? (y/n): ";
        setConsoleTextColor(7); 
        char retryChoice;
        cin >> retryChoice;
        cin.ignore();

        if (retryChoice == 'n' || retryChoice == 'N') {
            setConsoleTextColor(14); 
            cout << "\nReturning to Book Management Menu...\n";
            setConsoleTextColor(7); 
            this_thread::sleep_for(seconds(1));
            return; 
        }
    }
}

void Admin::CustomerManagementMenu(MYSQL* conn) {
    while (true) {
        int choice;
        system("cls");

        setConsoleTextColor(14); 
        cout << "========================================\n";
        cout << "       CUSTOMER MANAGEMENT MENU\n";
        cout << "========================================\n";
        setConsoleTextColor(7); 

 
        setConsoleTextColor(11); 
        cout << "\n1. Search for Customers\n";
        cout << "2. View Completed Orders of a Customer\n";
        cout << "0. Back to Admin Menu\n";
        setConsoleTextColor(7); 

        // Prompt for user input
        setConsoleTextColor(10); 
        cout << "\nEnter your choice: ";
        setConsoleTextColor(7); 

        if (!(cin >> choice) || choice < 0 || choice > 2) {
            setConsoleTextColor(12); 
            cout << "\n\033[1;31mInvalid input! Please enter a valid number between 0 and 2.\033[0m\n";
            setConsoleTextColor(7); 
            cin.clear(); 
            cin.ignore(10000, '\n');  
            this_thread::sleep_for(chrono::seconds(1));  
            continue; 
        }

        switch (choice) {
        case 1:
            ViewCustomerInfo(conn);  // Function to search for customers
            break;
        case 2:
            ViewCustomerOrders(conn);  // Function to view completed orders
            break;
        case 0:
            setConsoleTextColor(14);
            cout << "\nReturning to Admin Menu...\n";
            setConsoleTextColor(7); 
            this_thread::sleep_for(chrono::seconds(1));  
            return;
        default:
            break;
        }
    }
}

string getMonthName(const string& month) {
    const string months[] = {"January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December" };

    int monthIndex = stoi(month) - 1; // Convert string to int and adjust to 0-based index
    if (monthIndex >= 0 && monthIndex < 12) {
        return months[monthIndex];
    }
    return "Invalid Month";
}






