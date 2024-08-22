#pragma once
#ifndef DB_WRAPPER_HPP
#define DB_WRAPPER_HPP
#endif

#include "logging.hpp"
#include <vector>
#include <sqlite3.h>
#include <string>

class IDBWrapper{
    public:
        virtual void open() {};
        virtual void createTable() {};
        virtual void getByID(int) {};
        virtual void close() {};
        virtual ~IDBWrapper() {};

};

class SQLiteWrapper : public virtual IDBWrapper{
    private:
        std::string db_location;
        sqlite3 *db;
        int last_primary_key;
        std::string sql_query;
        sqlite3_stmt *res;
        bool connected;
        std::vector<ExpenseLog*> logs;

    public:
        SQLiteWrapper();
        SQLiteWrapper(std::string);
        ~SQLiteWrapper();
        void open();
        void open(std::string);
        void createTable();
        void getByID(int);
        bool checkConnection();
        void addObject(ExpenseLog*);
        void close();
};