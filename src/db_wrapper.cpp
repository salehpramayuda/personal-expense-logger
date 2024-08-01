#include "db_wrapper.hpp"

#include <ctime>
#include <spdlog/spdlog.h>
#include <stdexcept>

SQLiteWrapper::SQLiteWrapper(){
    db_location = "";
}

SQLiteWrapper::SQLiteWrapper(std::string filename): db_location(filename) {
    connected = false;
    this->open();
}

SQLiteWrapper::~SQLiteWrapper(){
    close();
}
void SQLiteWrapper::open(){
    try{
        if (db_location.empty())
            throw std::exception();
        int exit = 0;
        exit = sqlite3_open(db_location.c_str(), &connection);
        if (exit != SQLITE_OK) 
            throw std::exception();
        connected = true;
    }
    catch (std::exception){
        connected = false;
    }
}
        
void SQLiteWrapper::open(std::string filename){
    db_location = filename;
    this->open();
}

void SQLiteWrapper::createTable(){
    try{
        checkConnection();
        std::string sql = "CREATE TABLE EXPENSE("
                        "ID INT PRIMARY KEY     NOT NULL, "
                        "DATETIME       TEXT    NOT NULL, "
                        "PRICE          INT     NOT NULL, "
                        "CATEGORY       INT     NOT NULL, "
                        "SUBCATEGORY    TEXT,"
                        "SATISFACTION   INT     NOT NULL, "
                        "DETAIL         TEXT,"
                        ");";
        int exit = 0;
        char* errMsg;
        exit = sqlite3_exec(connection, sql.c_str(), NULL, NULL, &errMsg);
        if (exit != SQLITE_OK){
            sqlite3_free(errMsg);
            throw std::exception();
        }
        
    }
    catch (std::exception){
        spdlog::error("Error when creating Table.");
    }
}

void SQLiteWrapper::addObject(ExpenseLog log){
    try{
        checkConnection();
        auto datetime = log.getLogTime();
        std::string datet = std::ctime(&datetime);
        std::string sql = "INSERT INTO EXPENSE (DATETIME, PRICE, CATEGORY, SUBCATEGORY, SATISFACTION, DETAIL)"
                        "VALUES('" + datet + "', " + std::to_string(log.getPrice()) + "', " + std::to_string(log.getCategory()) + ", '"
                        + log.getSubcategory() + "', "
                        ");";
    }
    catch (std::exception){
        spdlog::error("Error when adding new object to table");
    }
}
        
void SQLiteWrapper::close(){
    if (connected){
        int exit = 0;
        exit = sqlite3_close(connection);
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

