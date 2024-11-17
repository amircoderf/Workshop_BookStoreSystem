#include "DBConnection.h"

void db_connection::ConnectionFunction() {
    conn = mysql_init(0);
    if (conn) 
        cout << "Database Initialized" << endl;
    else
        cout << "Database Initialization Failed" << endl;

    conn = mysql_real_connect(conn, "localhost", "root", "", "bookstore", 3306, NULL, 0);
    if (conn) {
        cout << "Database Connected To MySQL" << endl;
    }
    else
        cout << "Database Connection Failed "  << endl;

    this_thread::sleep_for(seconds(1));
}

//close the connection
void db_connection::CloseConnection() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
    }
}
