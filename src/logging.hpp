#pragma once
#ifndef LOGGING_HPP
#define LOGGING_HPP
#endif

#include <sqlite3.h>
#include <ctime>
#include <string>

class Log {
    private:
        int id;
        std::time_t log_time;
        std::string content;

    public:
        Log();
        Log(std::time_t, std::string);
        void setContent(std::string);
        void setLogTime(std::time_t);
        void setId(int id);
        std::string getContent();
        std::time_t getLogTime();
        int getId();
};

enum PurchaseCategory { 
    MANDATORY = 0, 
    OPTIONAL = 1, 
    FUTURE  = 2
};

enum Satisfaction { 
    SAD = 0, 
    DISSATISFIED = 1, 
    NEUTRAL = 2, 
    SATISFIED = 3, 
    HAPPY = 4 
};

class ExpenseLog : public Log {
    private:
        float price;
        PurchaseCategory category;
        Satisfaction emotion;
        std::string subcategory;

    public:
        ExpenseLog(std::time_t, std::string, unsigned int, float, unsigned int, std::string);
        ExpenseLog(std::time_t, std::string, PurchaseCategory, float, Satisfaction, std::string);
        ExpenseLog(sqlite3_stmt*);
        void setCategory(unsigned int);
        void setPrice(float price) { this->price = price; };
        void setEmotion(unsigned int);
        void setSubcategory(std::string subcategory) { this->subcategory = subcategory; };
        float getPrice() { return this->price; };
        std::string getSubcategory() { return this->subcategory; };
        PurchaseCategory getCategory() { return this->category; };
        Satisfaction getEmotion() { return this->emotion; };
        std::string asSQLQuery();
};
