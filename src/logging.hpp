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
        void setContent(std::string content) { this->content = content; };
        std::string getContent() { return this->content; };
        std::time_t getLogTime() { return this->log_time; };
        std::string asString();
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
        uint price;
        PurchaseCategory category;
        Satisfaction emotion;
        std::string subcategory;

    public:
        ExpenseLog(std::time_t, std::string, uint, uint, uint, std::string);
        ExpenseLog(std::time_t, std::string, PurchaseCategory, uint, Satisfaction, std::string);
        void setCategory(uint);
        void setPrice(uint price) { this->price = price; };
        void setEmotion(uint);
        void setSubcategory(std::string subcategory) { this->subcategory = subcategory; };
        uint getPrice() { return this->price; };
        std::string getSubcategory() {return this->subcategory; };
        PurchaseCategory getCategory() { return this->category; };
        Satisfaction getEmotion() { return this->emotion; };
        std::string asString();
};