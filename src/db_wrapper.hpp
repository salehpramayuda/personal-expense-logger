#ifndef DB_WRAPPER_HPP
#define DB_WRAPPER_HPP
#endif

#include "logging.hpp"

#include <sqlite3.h>
#include <string>

class IDBWrapper{
    public:
        virtual void open();
        virtual void createTable();
        virtual void close();

};

class SQLiteWrapper : public IDBWrapper{
    private:
        std::string db_location;
        sqlite3* connection;
        bool connected;

    public:
        SQLiteWrapper();
        SQLiteWrapper(std::string);
        ~SQLiteWrapper();
        void open();
        void open(std::string);
        void createTable();
        bool checkConnection();
        void addObject(ExpenseLog);
        void close();
};