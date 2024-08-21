#pragma once
#ifndef LOGGING_HPP
#define LOGGING_HPP
#endif

#include <ctime>
#include <string>

class Log {
    private:
        std::time_t log_time;
        std::string content;

    public:
        Log();
        Log(std::time_t, std::string);
        void setContent(std::string);
        std::string getContent();
        std::time_t getLogTime();
        virtual std::string asString();
        virtual std::string asJson();
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
        unsigned int price;
        PurchaseCategory category;
        Satisfaction emotion;
        std::string subcategory;

    public:
        ExpenseLog(std::time_t, std::string, unsigned int, unsigned int, unsigned int, std::string);
        ExpenseLog(std::time_t, std::string, PurchaseCategory, unsigned int, Satisfaction, std::string);
        void setCategory(unsigned int);
        void setPrice(unsigned int price) { this->price = price; };
        void setEmotion(unsigned int);
        void setSubcategory(std::string subcategory) { this->subcategory = subcategory; };
        unsigned int getPrice() { return this->price; };
        std::string getSubcategory() { return this->subcategory; };
        PurchaseCategory getCategory() { return this->category; };
        Satisfaction getEmotion() { return this->emotion; };
        std::string asJson();
        std::string asString();
        std::string asSQLQuery();
};