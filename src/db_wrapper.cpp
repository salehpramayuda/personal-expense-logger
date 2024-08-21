#include "db_wrapper.hpp"
#include "logging.hpp"

#include <ctime>
#include <spdlog/spdlog.h>
#include <stdexcept>

int callback(void*, int, char**, char**);

SQLiteWrapper::SQLiteWrapper(){
    db_location = "";
}

SQLiteWrapper::SQLiteWrapper(std::string filename): db_location(filename) {
    connected = false;
    open();
}

SQLiteWrapper::~SQLiteWrapper(){
    close();
}
void SQLiteWrapper::open(){
    try{
        spdlog::debug("Opening connection to database.");
        if (db_location.empty()){
            spdlog::warn("Database location has not been set.");
            spdlog::error("connection to database failed to open.");
            throw std::exception();
        }
        int exit = 0;
        exit = sqlite3_open(db_location.c_str(), &db);
        if (exit != SQLITE_OK){
            spdlog::warn("Error occured when opening database to " + db_location);
            spdlog::error("SQLite3 ERROR CODE: " + std::to_string(exit));
            throw std::exception();
        }
        connected = true;
        spdlog::debug("Successfully opened connection to database.");
        spdlog::info("Connected to SQLite Database in " + db_location);
        createTable();
    }
    catch (std::exception& ex){
        spdlog::error("Error occured during connection establishment.");
        connected = false;
    }
}
        
void SQLiteWrapper::open(std::string filename){
    db_location = filename;
    spdlog::debug("Setting database location to " + filename);
    this->open();
}

void SQLiteWrapper::createTable(){
    try{
        spdlog::debug("Creating table for expenses.");
        checkConnection();
        sql_query = "CREATE TABLE IF NOT EXISTS Expense ("
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "DATETIME TEXT NOT NULL DEFAULT (CURRENT_TIMESTAMP), "
                    "PRICE INTEGER NOT NULL, "
                    "CATEGORY INTEGER NOT NULL, "
                    "SUBCATEGORY TEXT, "
                    "SATISFACTION INTEGER NOT NULL, "
                    "DETAIL TEXT"
                    ");";
        int exit = 0;
        char* errMsg;
        exit = sqlite3_exec(db, sql_query.c_str(), NULL, NULL, &errMsg);
        if (exit != SQLITE_OK){
            spdlog::error("SQL Query execution resulted in an error.");
            spdlog::error(errMsg);
            sqlite3_free(errMsg);
            throw std::exception();
        }
       spdlog::info("Table Expense successfully created."); 
    }
    catch (std::exception& ex){
        spdlog::error("Error when creating Table.");
    }
}

void SQLiteWrapper::getByID(int Id){
    checkConnection();
    std::string id_str = std::to_string(Id);
    spdlog::debug("Retrieving log with ID " + id_str);
    try{
        sql_query = "SELECT * FROM Expense WHERE Expense.ID=" + id_str;
        int exit = 0;
        char* errMsg;
        exit = sqlite3_exec(db, sql_query.c_str(), callback, 0, &errMsg);
    }
    catch (std::exception e){
        spdlog::error("Error when retreiving log from table.");
    }

    
}

void SQLiteWrapper::addObject(ExpenseLog* log){
    try{
        checkConnection();
        auto datetime = log->getLogTime();
        std::string datet = std::ctime(&datetime);
        sql_query = "INSERT INTO Expense (DATETIME, PRICE, CATEGORY, SUBCATEGORY, SATISFACTION, DETAIL)" 
            "" + log->asSQLQuery();
        int exit = 0;
        char* errMsg;
        exit = sqlite3_exec(db, sql_query.c_str(), NULL, NULL, &errMsg);
        if (exit != SQLITE_OK){
            spdlog::error("SQL Query execution resulted in an error.");
            spdlog::error(errMsg);
            sqlite3_free(errMsg);
            throw std::exception();
        }
    }
    catch (std::exception){
        spdlog::error("Error when adding new object to table");
    }
}
        
void SQLiteWrapper::close(){
    if (connected){
        int exit = 0;
        exit = sqlite3_close(db);
        if (exit != SQLITE_OK)
            throw std::exception();
    }
}

bool SQLiteWrapper::checkConnection(){
    if (not connected){
        spdlog::error("Database Wrapper is not connected to any database.");
        throw std::exception();
    }
    return connected;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName){
    return 0;
}
