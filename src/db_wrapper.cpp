#include "db_wrapper.hpp"
#include "logging.hpp"

#include <ctime>
#include <spdlog/spdlog.h>
#include <stdexcept>

int callback(void*, int, char**, char**);

SQLiteWrapper::SQLiteWrapper(){
    db_location = "";
    last_primary_key = -1;
    connected = false;
}

SQLiteWrapper::SQLiteWrapper(std::string filename): db_location(filename) {
    connected = false;
    last_primary_key = -1;
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
        if (!checkConnection()) return;
        sql_query = "CREATE TABLE IF NOT EXISTS Expense ("
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "DATETIME TEXT NOT NULL DEFAULT (CURRENT_TIMESTAMP), "
                    "PRICE INTEGER NOT NULL, "
                    "CATEGORY INTEGER NOT NULL, "
                    "SUBCATEGORY TEXT, "
                    "SATISFACTION INTEGER NOT NULL, "
                    "DETAIL TEXT"
                    ");";
        char* errMsg;
        int exit = sqlite3_exec(db, sql_query.c_str(), NULL, NULL, &errMsg);
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
    if (!checkConnection()) return;
    if (Id > last_primary_key){
        spdlog::warn("Inputed ID does not exist in the database. Function call aborted.");
        return;
    }
    std::string id_str = std::to_string(Id);
    spdlog::debug("Retrieving log with ID " + id_str);
    sql_query = "SELECT * FROM Expense WHERE Expense.ID>" + id_str; 
    try{
        spdlog::debug("SQL statement to query data by ID is being prepared.");
        int rc = sqlite3_prepare_v2(db, sql_query.c_str(), -1,  &res, 0);
        if (rc != SQLITE_OK){
            spdlog::warn("Failed to prepare SQL statement.");
            spdlog::error("Error occured when preparing SQL statement.");
            throw std::exception();
        }

        spdlog::debug("Executing SQL statement.");
        while (rc != SQLITE_DONE){
            spdlog::debug("Fetching query");
            rc = sqlite3_step(res);
            try{
                if (rc == SQLITE_ROW){
                    logs.push_back(new ExpenseLog(res));
                }    
            }
            catch (std::exception) {
                spdlog::error("Error occured when deserializing retrievied query.");
            }
        }
        
        sqlite3_finalize(res);
        spdlog::debug("SQL statement successfully executed.");
        
    }
    catch (std::exception e){
        spdlog::error("Error when retreiving log from table.");
    }
    
}

void SQLiteWrapper::addObject(ExpenseLog* log){
    try{
        if (!checkConnection()) return;
        log->setId(++last_primary_key);
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
        spdlog::warn("No connection to database. Please call the open()-function after setting the right address.");
        spdlog::error("Database Wrapper is not connected to any database.");
        return false;
    }
    else{
        try{
            sql_query = "SELECT seq FROM sqlite_sequence WHERE name='Expense'";
            char *errMsg;
            int rc = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &res, 0);
            if (rc != SQLITE_OK){
                spdlog::warn("Failed to prepare sql statement.");
                throw std::exception();
            }
            rc = sqlite3_step(res);
            if(rc != SQLITE_ROW){
                if (rc != SQLITE_DONE){
                    spdlog::warn("Execution of SQL statement failed.");
                    throw std::exception();
                }
                spdlog::warn("Table of name 'Expense' has not ben created");
                last_primary_key = 0;
                sqlite3_finalize(res);
                return connected;
            }
            last_primary_key = sqlite3_column_int(res, 0);
            sqlite3_finalize(res);
            return connected;
        }
        catch(std::exception){
            spdlog::error("Error occured when checking connection to database. Try reopening the databaase connection.");
            connected = false;
        }
        return connected;
    }
}