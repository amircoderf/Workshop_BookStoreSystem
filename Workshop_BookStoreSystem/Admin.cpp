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
            if (!(cin >> choice) || choice < 0 || choice > 5) {
                setConsoleTextColor(12); 
                cout << "Invalid input. Please enter a number between 0 and 5: ";
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
            return;  // Return to the main menu
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
            AddBooks(conn);  // Call AddBooks function
            break;
        case 3:
            // Display all books
            if (!DisplayBooks(conn)) {
                setConsoleTextColor(12); 
                cout << "No books available to update." << endl;
                setConsoleTextColor(7); 
                break;
            }

            while (true) {
                setConsoleTextColor(11); 
                cout << "\n\nEnter the BookID of the book you want to update (or type '0' to cancel): ";
                setConsoleTextColor(7); 
                cin >> bookid;

                if (bookid == "0") { 
                    setConsoleTextColor(12); 
                    cout << "Update operation canceled." << endl;
                    setConsoleTextColor(7); 
                    break;
                }

                // Check if the BookID exists
                string checkQuery = "SELECT COUNT(*) FROM book WHERE BookID = '" + bookid + "'";
                if (mysql_query(conn, checkQuery.c_str())) {
                    setConsoleTextColor(12);
                    cerr << "Error checking BookID: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7);
                    break;
                }

                MYSQL_RES* res = mysql_store_result(conn);
                if (res == nullptr) {
                    setConsoleTextColor(12); 
                    cerr << "Error fetching result: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7);
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
                    setConsoleTextColor(12); 
                    cout << "Error: No book found with the BookID '" << bookid << "'. Please try again." << endl;
                    setConsoleTextColor(7); 
                    cout << "------------------------------------------------------------------------------------" << endl;
                }
            }
            break;
        case 4:
            // Display all books
            if (!DisplayBooks(conn)) {
                setConsoleTextColor(12); 
                cout << "No books available to delete." << endl;
                setConsoleTextColor(7); 
                break;
            }

            while (true) {
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
                    break;
                }

                MYSQL_RES* res = mysql_store_result(conn);
                if (res == nullptr) {
                    setConsoleTextColor(12); 
                    cerr << "Error fetching result: " << mysql_error(conn) << endl;
                    setConsoleTextColor(7); 
                    break;
                }

                MYSQL_ROW row = mysql_fetch_row(res);
                int bookCount = row ? atoi(row[0]) : 0;
                mysql_free_result(res);

                if (bookCount > 0) {
                    system("cls");
                    DeleteBooks(conn, bookid);
                    break; 
                }
                else {
                    setConsoleTextColor(12); 
                    cout << "Error: No book found with the BookID '" << bookid << "'. Please try again." << endl;
                    setConsoleTextColor(7); 
                    cout << "------------------------------------------------------------------------------------" << endl;
                }
            }
            break;
        case 5:
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

    do {
        system("cls");
        setConsoleTextColor(11); // Light cyan color
        cout << "=============================================\n";
        cout << "|          BOOK MANAGEMENT SYSTEM           |\n";
        cout << "=============================================\n";
        setConsoleTextColor(7); // Reset to default color

        while (true) {
            setConsoleTextColor(14); // Yellow color for input prompts
            cout << "Enter ISBN (or type '/cancel' to exit): ";
            setConsoleTextColor(7); // Reset to default color
            cin >> isbn;

            if (isbn == "/cancel") {
                setConsoleTextColor(12); // Red color for cancellation message
                cout << "Operation cancelled by user.\n";
                setConsoleTextColor(7); // Reset to default color
                return;
            }

            // Check if input is numeric and exactly 13 digits long
            if (isNumeric(isbn) && isbn.length() == 13) {
                // Check if ISBN already exists in the database
                string check_query = "SELECT ISBN FROM BOOK WHERE ISBN = '" + isbn + "'";
                if (mysql_query(conn, check_query.c_str()) == 0) {
                    MYSQL_RES* res = mysql_store_result(conn);
                    if (res != nullptr) {
                        if (mysql_num_rows(res) > 0) {
                            mysql_free_result(res);
                            setConsoleTextColor(12); // Red color for error message
                            cout << "ISBN already exists in the database. Please enter a different ISBN.\n";
                            setConsoleTextColor(7); // Reset to default color
                            continue; // Prompt the user to enter another ISBN
                        }
                        else {
                            mysql_free_result(res);
                            break; // Exit the loop if ISBN is unique
                        }
                    }
                }
                else {
                    setConsoleTextColor(12); // Red color for error message
                    cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
                    setConsoleTextColor(7); // Reset to default color
                    return;
                }
            }
            else {
                setConsoleTextColor(12); // Red color for error message
                cout << "Invalid input. ISBN must be exactly 13 digits long.\n";
                setConsoleTextColor(7); // Reset to default color
            }
        }

        // Title Input
        setConsoleTextColor(14); // Yellow color for input prompts
        cout << "Title of the book (or type '/cancel' to exit): ";
        setConsoleTextColor(7); // Reset to default color
        cin.ignore();
        getline(cin, title);

        if (title == "/cancel") {
            setConsoleTextColor(12); // Red color for cancellation message
            cout << "Operation cancelled by user.\n";
            setConsoleTextColor(7); // Reset to default color
            return;
        }

        // Price Input
        while (true) {
            setConsoleTextColor(14); // Yellow color for input prompts
            cout << "Price of the book (RM) (or type '/cancel' to exit): ";
            setConsoleTextColor(7); // Reset to default color
            string priceInput;
            cin >> priceInput;

            if (priceInput == "/cancel") {
                setConsoleTextColor(12); // Red color for cancellation message
                cout << "Operation cancelled by user.\n";
                setConsoleTextColor(7); // Reset to default color
                return;
            }

            try {
                price = stod(priceInput);
                if (price >= 0) {
                    break;
                }
                else {
                    setConsoleTextColor(12); // Red color for error message
                    cout << "Invalid input. Price cannot be negative.\n";
                    setConsoleTextColor(7); // Reset to default color
                }
            }
            catch (invalid_argument&) {
                setConsoleTextColor(12); // Red color for error message
                cout << "Invalid input. Please enter a valid price.\n";
                setConsoleTextColor(7); // Reset to default color
            }
        }

        // Stock Input
        while (true) {
            setConsoleTextColor(14); // Yellow color for input prompts
            cout << "Stock (or type '/cancel' to exit): ";
            setConsoleTextColor(7); // Reset to default color
            string stockInput;
            cin >> stockInput;

            if (stockInput == "/cancel") {
                setConsoleTextColor(12); // Red color for cancellation message
                cout << "Operation cancelled by user.\n";
                setConsoleTextColor(7); // Reset to default color
                return;
            }

            try {
                stock = stoi(stockInput);
                if (stock >= 0) {
                    break;
                }
                else {
                    setConsoleTextColor(12); // Red color for error message
                    cout << "Invalid input. Stock cannot be negative.\n";
                    setConsoleTextColor(7); // Reset to default color
                }
            }
            catch (invalid_argument&) {
                setConsoleTextColor(12); // Red color for error message
                cout << "Invalid input. Please enter a valid stock value.\n";
                setConsoleTextColor(7); // Reset to default color
            }
        }

        // Author Input
        setConsoleTextColor(14); // Yellow color for input prompts
        cout << "Author of the book (or type '/cancel' to exit): ";
        setConsoleTextColor(7); // Reset to default color
        cin.ignore();
        getline(cin, author);

        if (author == "/cancel") {
            setConsoleTextColor(12); // Red color for cancellation message
            cout << "Operation cancelled by user.\n";
            setConsoleTextColor(7); // Reset to default color
            return;
        }

        // Publisher Input
        setConsoleTextColor(14); // Yellow color for input prompts
        cout << "Publisher of the book (or type '/cancel' to exit): ";
        setConsoleTextColor(7); // Reset to default color
        getline(cin, publisher);

        if (publisher == "/cancel") {
            setConsoleTextColor(12); // Red color for cancellation message
            cout << "Operation cancelled by user.\n";
            setConsoleTextColor(7); // Reset to default color
            return;
        }

        // Published Year Input
        while (true) {
            setConsoleTextColor(14); // Yellow color for input prompts
            cout << "Published Year (or type '/cancel' to exit): ";
            setConsoleTextColor(7); // Reset to default color
            cin >> publishedYear;

            if (publishedYear == "/cancel") {
                setConsoleTextColor(12); // Red color for cancellation message
                cout << "Operation cancelled by user.\n";
                setConsoleTextColor(7); // Reset to default color
                return;
            }

            if (isNumeric(publishedYear) && publishedYear.length() == 4) {
                break;
            }
            setConsoleTextColor(12); // Red color for error message
            cout << "Invalid input. Please enter a valid 4-digit year.\n";
            setConsoleTextColor(7); // Reset to default color
        }

        // Insert into Database
        string insert_query = "INSERT INTO BOOK (ISBN, Title, Price, Stock, Author, Publisher, PublishedYear) "
            "VALUES ('" + isbn + "', '" + title + "', " + to_string(price) + ", " + to_string(stock) + ", '" + author + "', '" + publisher + "', '" + publishedYear + "')";

        if (mysql_query(conn, insert_query.c_str()) == 0) {
            setConsoleTextColor(10); // Green color for success message
            cout << "\nBook has been successfully added to the database.\n";
            setConsoleTextColor(7); // Reset to default color
        }
        else {
            setConsoleTextColor(12); // Red color for error message
            cerr << "Query Execution Problem! Error Code: " << mysql_errno(conn) << endl;
            setConsoleTextColor(7); // Reset to default color
        }

        // Ask if the user wants to add another book
        char choice;
        while (true) {
            setConsoleTextColor(14); // Yellow color for input prompts
            cout << "Do you want to input another book? (y/n): ";
            setConsoleTextColor(7); // Reset to default color
            cin >> choice;

            choice = tolower(choice);
            if (choice == 'y' || choice == 'n') {
                break; // Exit loop if valid input
            }

            setConsoleTextColor(12); // Red color for error message
            cout << "Invalid input. Please enter 'y' for yes or 'n' for no.\n";
            setConsoleTextColor(7); // Reset to default color
        }

        if (tolower(choice) == 'n') {
            break;
        }

    } while (true);

    return;
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

    // Check if the database connection is valid
    if (conn == nullptr) {
        setConsoleTextColor(12); // Red for error message
        cout << "Error: Database connection is invalid.\n";
        setConsoleTextColor(7); // Reset to default color
        _getch();
        return;
    }

    do {
        system("cls");
        // Set console text color for the title
        setConsoleTextColor(14); // Yellow for the title
        cout << "========================================\n";
        cout << "          SALES REPORT MENU\n";
        cout << "========================================\n";
        setConsoleTextColor(7); // Reset to default color

        // Display menu options with colors
        setConsoleTextColor(11); // Cyan for menu options
        cout << "\nChoose Report Type:\n";
        cout << "1. Monthly Sales Report\n";
        cout << "2. Total Book Sales Report\n";
        cout << "3. Book Sales in a Month\n";
        cout << "0. Exit\n";
        setConsoleTextColor(7); // Reset to default color

        // Prompt for user input
        setConsoleTextColor(10); // Green for input prompt
        cout << "\nEnter your choice: ";
        setConsoleTextColor(7); // Reset to default color

        // Input validation
        if (!(cin >> choice)) {
            cin.clear(); // Clear the error flag
            cin.ignore(1000, '\n'); // Discard invalid input
            setConsoleTextColor(12); // Red for error message
            cout << "Invalid input. Please enter a valid number.\n";
            setConsoleTextColor(7); // Reset to default color
            _getch(); // Wait for user to press a key
            continue; // Restart the loop
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
                setConsoleTextColor(12); // Red for invalid choice
                cout << "\nInvalid choice. Please select a valid option.\n";
                setConsoleTextColor(7); // Reset to default color
                _getch(); // Wait for user to press a key
                break;
            }
        }
        catch (const exception& e) {
            // Handle unexpected exceptions
            setConsoleTextColor(12); // Red for error message
            cout << "\nAn error occurred: " << e.what() << "\n";
            setConsoleTextColor(7); // Reset to default color
            _getch(); // Wait for user to press a key
        }
    } while (true); // Loop until the user chooses to exit
}

// Function to display the table
void displayTable(const vector<string>& salesMonths, const vector<double>& totalSales, const vector<string>& percentageChanges, int year) {
    // Create a table object
    Table salesTable;

    // Add table headers with color
    salesTable.add_row({ "Sales Month", "Total Sales (RM)", "Sales Change (%)" });

    // Format headers with color
    salesTable[0].format()
        .font_color(Color::yellow) // Yellow text for headers
        .font_style({ FontStyle::bold }) // Bold headers
        .font_align(FontAlign::center); // Center-align headers

    // Add table rows with color
    for (size_t i = 0; i < salesMonths.size(); ++i) {
        salesTable.add_row({
            salesMonths[i],
            to_string(totalSales[i]).substr(0, to_string(totalSales[i]).find(".") + 3), // Format to 2 decimal places
            percentageChanges[i]
            });

        // Format rows with alternating colors
        if (i % 2 == 0) {
            salesTable[i + 1].format()
                .font_color(Color::green) // Green text for even rows
                .font_align(FontAlign::center); // Center-align text
        }
        else {
            salesTable[i + 1].format()
                .font_color(Color::cyan) // Cyan text for odd rows
                .font_align(FontAlign::center); // Center-align text
        }
    }

    // Format the table borders with color
    salesTable.format()
        .border_top("=").border_bottom("=").border_left("|").border_right("|").corner("+")
        .border_color(Color::magenta) // Magenta borders
        .column_separator("|")
        .font_align(FontAlign::center); // Center-align all text

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

    // Print the bar chart header
    cout << "\n\t\t\t--- Monthly Sales Report ---\n";
    cout << "\t\tSales Chart (Total Sales represented by colored bars)\n\n";

    // Print the bars
    for (size_t i = 0; i < salesMonths.size(); ++i) {
        // Calculate the number of blocks to represent sales
        int numBlocks = static_cast<int>((totalSales[i] / maxSales) * 50); // Max width of 50 chars

        // Print the month
        setConsoleTextColor(14); // Yellow for month labels
        cout << setw(15) << salesMonths[i] << " | ";
        setConsoleTextColor(7); // Reset to default color

        // Print the bar with background color
        setConsoleTextColor(176); // Green background, black text
        for (int j = 0; j < numBlocks; ++j) {
            cout << " "; // Use spaces to create a solid bar
        }
        setConsoleTextColor(7); // Reset to default color

        // Print the sales value
        cout << " RM" << fixed << setprecision(2) << totalSales[i] << endl;

        // Add space between bars
        cout << "                | ";
        cout << endl; // Add a blank line after each bar
    }

    // Print instructions
}

void Admin::MonthlySalesReport(MYSQL* conn) {
    system("cls");
    int year;
    while (true) {
        cout << "Enter the year for the sales report (e.g., 2025): ";
        if (cin >> year) {
            // Check if the year is within a reasonable range
            if (year >= 2000 && year <= 2100) {
                break;
            }
            else {
                setConsoleTextColor(12);
                cout << "Invalid year. Please enter a year between 2000 and 2100." << endl;
                setConsoleTextColor(7);
            }
        }
        else {
            setConsoleTextColor(12); // Red for error message
            cout << "Invalid input. Please enter a valid year (e.g., 2025)." << endl;
            setConsoleTextColor(7); // Reset to default color
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    // Query to fetch sales data for the selected year and December of the previous year
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
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cerr << "Failed to store result: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    // Vectors to hold months, sales, and percentage changes
    vector<string> salesMonths;
    vector<double> totalSales;
    vector<string> percentageChanges;

    MYSQL_ROW row;
    double decemberPreviousYearSales = 0.0; // Store sales for December of the previous year
    bool isFirstMonth = true;

    while ((row = mysql_fetch_row(result))) {
        string salesMonth = row[0];
        double totalSalesAmount = atof(row[1]); // Total sales value

        // Check if this is December of the previous year
        bool isDecemberPreviousYear = (salesMonth.substr(0, 4) == to_string(year - 1) && salesMonth.substr(5, 2) == "12");

        if (isDecemberPreviousYear) {
            // Store December's sales for calculating the percentage change for January
            decemberPreviousYearSales = totalSalesAmount;
            continue; // Skip adding December to the display vectors
        }

        // Convert numeric month to month name (e.g., "01" -> "January")
        string monthName = getMonthName(salesMonth.substr(5, 2));

        salesMonths.push_back(monthName);
        totalSales.push_back(totalSalesAmount);

        // Calculate percentage change
        string percentageChange = "N/A"; // Default for the first month
        if (isFirstMonth) {
            // For January, calculate percentage change relative to December of the previous year
            if (decemberPreviousYearSales > 0) { // Ensure December data is available
                double change = ((totalSalesAmount - decemberPreviousYearSales) / decemberPreviousYearSales) * 100;
                if (change >= 0) {
                    percentageChange = "Increases by " + to_string(change).substr(0, to_string(change).find(".") + 3) + "%";
                }
                else {
                    percentageChange = "Decreases by " + to_string(abs(change)).substr(0, to_string(abs(change)).find(".") + 3) + "%";
                }
            }
        }
        else {
            // For subsequent months, calculate percentage change relative to the previous month
            double change = ((totalSalesAmount - totalSales[totalSales.size() - 2]) / totalSales[totalSales.size() - 2]) * 100;
            if (change >= 0) {
                percentageChange = "Increases by " + to_string(change).substr(0, to_string(change).find(".") + 3) + "%";
            }
            else {
                percentageChange = "Decreases by " + to_string(abs(change)).substr(0, to_string(abs(change)).find(".") + 3) + "%";
            }
        }
        percentageChanges.push_back(percentageChange);

        isFirstMonth = false;
    }

    mysql_free_result(result);

    // Toggle between table and bar chart
    bool showTable = true; // Start by showing the table
    while (true) {
        system("cls");

        if (showTable) {
            displayTable(salesMonths, totalSales, percentageChanges, year);
        }
        else {
            displayBarChart(salesMonths, totalSales);
        }

        // Instructions for toggling
        setConsoleTextColor(14); // Yellow for instructions
        cout << "\nPress 'T' to view the Table, 'C' to view the Bar Graph, or 'Q' to quit...\n";
        setConsoleTextColor(7); // Reset to default color

        char choice = _getch();
        choice = tolower(choice);

        if (choice == 't') {
            showTable = true;
        }
        else if (choice == 'c') {
            showTable = false;
        }
        else if (choice == 'q') {
            break; // Exit the loop and return to the menu
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

    // First pass: Calculate total quantity sold for all books
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        int quantitySold = atoi(row[1]); // Total quantity sold for the book
        totalQuantitySoldForAllBooks += quantitySold; // Sum up the total quantity sold
    }

    // Move the result pointer back to the beginning for the second pass
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

    // Format and display the report table
    reportTableFormat(bookSalesTable);
    cout << bookSalesTable << endl;

    mysql_free_result(result);
    cout << "\nPress any key to return to the menu...";
    _getch();
}

void Admin::BookSalesInMonth(MYSQL* conn) {
    system("cls");
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

void Admin::ViewCustomerInfo(MYSQL* conn)
{
    // Check if the connection to the database is successful
    if (conn == nullptr) {
        cout << "\033[1;31mError: Database connection failed!\033[0m" << endl;
        return; // Exit the function if connection failed
    }

    int choice = 0;
    bool continueSearching = true;

    while (continueSearching) {
        system("cls");
        // Ask the user what to search by
        cout << "1. View all customers\n";
        cout << "2. Search by Name\n";
        cout << "3. Search by Username\n";
        cout << "4. Search by Phone No\n";
        cout << "5. Search by Address\n";
        cout << "0. Go back\n"; // Option to go back
        cout << "Enter your choice: ";

        // Validate the choice input
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\033[1;31mInvalid input! Please enter a number between 0 and 5.\033[0m" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            continue; // Ask the user for input again
        }

        // If user chooses 0, go back
        if (choice == 0) {
            return; // Exit the search menu and go back to the previous menu
        }

        // Check if the choice is within the valid range
        if (choice < 1 || choice > 5) {
            cout << "\033[1;31mInvalid choice! Please enter a number between 0 and 5.\033[0m" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            continue; // Ask the user to input search criteria again
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
                cout << "Enter Name to search: ";
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND Name LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            case 3:
                cout << "Enter Username to search: ";
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND username LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            case 4:
                cout << "Enter Phone No to search: ";
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND Phone_no LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            case 5:
                cout << "Enter Address to search: ";
                getline(cin, keyword);
                searchQuery = "SELECT UserID, Name, IC_no, Phone_no, Address, username FROM USER WHERE Role='customer' AND Address LIKE '%" + keyword + "%'";
                validSearch = true;
                break;
            }

            // Execute the query
            int qstate = mysql_query(conn, searchQuery.c_str());

            if (qstate) {
                // If query execution fails, print error code and description
                cout << "\033[1;31mQuery Execution Problem! Error Code: " << mysql_errno(conn) << " - " << mysql_error(conn) << "\033[0m" << endl;
                return; // Exit the function if the query fails
            }

            MYSQL_RES* res = mysql_store_result(conn);
            if (res == nullptr) {
                cout << "\033[1;31mError fetching result. Error Code: " << mysql_errno(conn) << " - " << mysql_error(conn) << "\033[0m" << endl;
                return;
            }

            // Check if there are any rows in the result set
            if (mysql_num_rows(res) == 0) {
                cout << "\033[1;33mNo customers found matching the criteria. Please try again.\033[0m" << endl;
                mysql_free_result(res);
                this_thread::sleep_for(chrono::seconds(1)); 
                continue; // Ask the user to input search criteria again
            }

            // Prepare to display the results in a table format
            Table customerInformation;
            customerInformation.add_row({ "USER ID", "NAME", "IC NO", "PHONE NO", "ADDRESS", "USERNAME" });

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                customerInformation.add_row({ row[0], row[1], row[2], row[3], row[4], row[5] });
            }

            // Format the customer information and print it
            customerTableFormat(customerInformation);
            cout << customerInformation << endl;

            // Free the result memory
            mysql_free_result(res);

            validSearch = true; // Exit the loop since the search was successful
        }

        // Prompt to continue or stop
        cout << "\nWould you like to search again? (Y/N): ";
        char contChoice;
        cin >> contChoice;
        cin.ignore(); // Clear the buffer after reading the char input

        if (contChoice == 'Y' || contChoice == 'y') {
            continueSearching = true;
        }
        else if (contChoice == 'N' || contChoice == 'n') {
            continueSearching = false;
        }
        else {
            cout << "\033[1;31mInvalid input! Please enter Y or N.\033[0m" << endl;
        }
    }

    cout << "\nPress Any Key To Go Back...";
    _getch();
}

void Admin::ViewCustomerOrders(MYSQL* conn) {
    system("cls");
    int customerID = 0;
    bool continueSearching = true;

    while (continueSearching) {
        system("cls");
        cout << "\n\t\t\t--- View Completed Orders ---\n" << endl;
        cout << "Enter Customer ID to view completed orders (or enter 0 to go back): ";
        cin >> customerID;

        // If the user enters 0, go back to the previous menu
        if (customerID == 0) {
            continueSearching = false;  // Exit the loop and return to the previous menu
            break;
        }

        // Query to fetch order details for the given customer
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
        // Set console text color for the title
        setConsoleTextColor(14); // Yellow for the title
        cout << "===================================================\n";
        cout << "                    SEARCH BOOKS             \n";
        cout << "===================================================\n";
        setConsoleTextColor(7); // Reset to default color

        // Display menu options with colors
        setConsoleTextColor(11); // Cyan for menu options
        cout << "\n1. Search by Title\n";
        cout << "2. Search by Author\n";
        cout << "3. Search by Book ID\n";
        cout << "4. Search by Publisher\n";
        cout << "5. Back to Admin Menu\n";
        setConsoleTextColor(7); // Reset to default color

        // Prompt for user input
        setConsoleTextColor(10); // Green for input prompt
        cout << "\nEnter your choice: ";
        setConsoleTextColor(7); // Reset to default color

        int choice;
        // Check if the user enters a valid integer
        if (!(cin >> choice)) {
            setConsoleTextColor(12); // Red for error message
            cout << "\nInvalid input. Please enter a number between 1 and 5.\n";
            setConsoleTextColor(7); // Reset to default color
            _getch();
            cin.clear(); // Clear the error flag
            cin.ignore(1000, '\n'); // Ignore the invalid input
            continue;
        }

        // Handle invalid choice outside the range
        if (choice < 1 || choice > 5) {
            setConsoleTextColor(12); // Red for error message
            cout << "\nInvalid choice. Please enter a number between 1 and 5.\n";
            setConsoleTextColor(7); // Reset to default color
            _getch();
            continue;
        }

        cin.ignore(1000, '\n'); // Clear the input buffer

        string keyword;
        string query;
        MYSQL_RES* res;

        switch (choice) {
        case 1:
            setConsoleTextColor(11); // Cyan for input prompt
            cout << "\nEnter the Title to search: ";
            setConsoleTextColor(7); // Reset to default color
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE Title LIKE '%" + keyword + "%'";
            break;
        case 2:
            setConsoleTextColor(11); // Cyan for input prompt
            cout << "\nEnter the Author to search: ";
            setConsoleTextColor(7); // Reset to default color
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE Author LIKE '%" + keyword + "%'";
            break;
        case 3:
            setConsoleTextColor(11); // Cyan for input prompt
            cout << "\nEnter the Book ID to search: ";
            setConsoleTextColor(7); // Reset to default color
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE BookID LIKE '%" + keyword + "%'";
            break;
        case 4:
            setConsoleTextColor(11); // Cyan for input prompt
            cout << "\nEnter the Publisher to search: ";
            setConsoleTextColor(7); // Reset to default color
            getline(cin, keyword);
            query = "SELECT BookID, ISBN, Title, Price, Stock, Author, Publisher, PublishedYear FROM book WHERE Publisher LIKE '%" + keyword + "%'";
            break;
        case 5:
            setConsoleTextColor(14); // Yellow for exit message
            cout << "\nReturning to Admin Menu...\n";
            setConsoleTextColor(7); // Reset to default color
            _getch();
            return; // Exit the function if the user chooses to go back
        }

        const char* q = query.c_str();
        if (mysql_query(conn, q)) {
            setConsoleTextColor(12); // Red for error message
            cout << "\nQuery Execution Problem! Error Code: " << mysql_errno(conn) << "\n";
            setConsoleTextColor(7); // Reset to default color
            _getch();
            continue;
        }

        res = mysql_store_result(conn);
        if (res == nullptr || mysql_num_rows(res) == 0) {
            setConsoleTextColor(12); // Red for error message
            cout << "\nNo books found matching your criteria.\n";
            setConsoleTextColor(7); // Reset to default color
            mysql_free_result(res);
            _getch();
            continue;
        }

        cout << "\nBooks Found:\n";
        Table table;
        table.add_row({ "BookID", "ISBN", "Title", "Price", "Available Stock", "Author", "Publisher", "Published Year" });

        vector<MYSQL_ROW> rows;

        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            table.add_row({ row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7] });
            rows.push_back(row); // Store rows dynamically
        }

        cartTableFormat(table);
        cout << table << endl;

        while (true) {
            setConsoleTextColor(11); // Cyan for input prompt
            cout << "\nEnter the BookID of the book you want to manage (Press 'Enter' if you don't want to manage): ";
            setConsoleTextColor(7); // Reset to default color
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
                setConsoleTextColor(12); // Red for error message
                cout << "\nInvalid BookID. Please try again.\n";
                setConsoleTextColor(7); // Reset to default color
                continue;
            }

            // Display selected book details
            setConsoleTextColor(14); // Yellow for selected book
            cout << "\nYou selected Book: " << selectedRow[2] << "\n"; // Display title
            setConsoleTextColor(7); // Reset to default color

            setConsoleTextColor(11); // Cyan for menu options
            cout << "What would you like to do with this book?\n";
            cout << "1. Update Book\n";
            cout << "2. Delete Book\n";
            setConsoleTextColor(7); // Reset to default color

            setConsoleTextColor(10); // Green for input prompt
            cout << "Enter your choice: ";
            setConsoleTextColor(7); // Reset to default color

            int action;
            cin >> action;
            cin.ignore(); // Clear the input buffer

            switch (action) {
            case 1:
                UpdateBooks(conn, selectedRow[0]); // Pass BookID to update function
                break;
            case 2:
                DeleteBooks(conn, selectedRow[0]); // Pass BookID to delete function
                break;
            default:
                setConsoleTextColor(12); // Red for error message
                cout << "\nInvalid action!\n";
                setConsoleTextColor(7); // Reset to default color
                break;
            }

            break; // Exit the inner loop after performing an action
        }

        mysql_free_result(res);

        setConsoleTextColor(11); // Cyan for input prompt
        cout << "\nWould you like to search again? (y/n): ";
        setConsoleTextColor(7); // Reset to default color
        char retryChoice;
        cin >> retryChoice;
        cin.ignore(); // Clear the input buffer

        if (retryChoice == 'n' || retryChoice == 'N') {
            setConsoleTextColor(14); // Yellow for exit message
            cout << "\nReturning to Admin Menu...\n";
            setConsoleTextColor(7); // Reset to default color
            _getch();
            return; // Exit the function
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

        // Input validation
        if (!(cin >> choice) || choice < 0 || choice > 2) {
            setConsoleTextColor(12); 
            cout << "\n\033[1;31mInvalid input! Please enter a valid number between 0 and 2.\033[0m\n";
            setConsoleTextColor(7); 
            cin.clear(); 
            cin.ignore(10000, '\n');  
            this_thread::sleep_for(chrono::seconds(1));  // Pause for 1 second
            continue; 
        }

        // Handle user choice
        switch (choice) {
        case 1:
            ViewCustomerInfo(conn);  // Function to search for customers
            break;
        case 2:
            ViewCustomerOrders(conn);  // Function to view completed orders
            break;
        case 0:
            setConsoleTextColor(14); // Yellow for exit message
            cout << "\nReturning to Admin Menu...\n";
            setConsoleTextColor(7); // Reset to default color
            this_thread::sleep_for(chrono::seconds(1));  // Pause for 1 second
            return;
        default:
            break;
        }
    }
}

string getMonthName(const string& month) {
    if (month == "01") return "January";
    if (month == "02") return "February";
    if (month == "03") return "March";
    if (month == "04") return "April";
    if (month == "05") return "May";
    if (month == "06") return "June";
    if (month == "07") return "July";
    if (month == "08") return "August";
    if (month == "09") return "September";
    if (month == "10") return "October";
    if (month == "11") return "November";
    if (month == "12") return "December";
    return "Invalid Month";
}






