#include "logging.hpp"
#include <sstream>
#include <stdexcept>

Log::Log(){
    this->log_time = time(nullptr);
    this->content = "";
}

Log::Log(time_t log_time, std::string content): content(content){
    if (log_time == NULL){
        this->log_time = time(nullptr);
    }
    else{
        this->log_time = log_time;
    }
}

std::string Log::asString(){
    std::stringstream sstr;
    sstr << "{ \"log_time\":" << std::ctime(&this->log_time) << ",\"content\":" << this->content << " }";
    return sstr.str();
}

ExpenseLog::ExpenseLog(time_t log_time, std::string content, uint category, uint price, uint emotion, std::string subcategory): 
    Log(log_time, content), price(price), subcategory(subcategory) 
{
    if(category > 2 && emotion > 4){
        this->ExpenseLog::~ExpenseLog();
        throw std::invalid_argument("arguments for category and emotion must be unsigned integers between 0 and 2 and between 0 and 4 respectively.");
    }
    else{
        this->category = (PurchaseCategory)category;
        this->emotion = (Satisfaction)emotion;
    }
}

ExpenseLog::ExpenseLog(time_t log_time, std::string content, PurchaseCategory category, uint price, Satisfaction emotion, std::string subcategory):
    Log(log_time, content), category(category), price(price), emotion(emotion), subcategory(subcategory) 
{

}

void ExpenseLog::setCategory(uint category){
    if(category > 2){
        throw std::invalid_argument("argument category must be unsigned integer between 0 and 2");
    }
    this->category = (PurchaseCategory)category;
}

void ExpenseLog::setEmotion(uint emotion){
    if(emotion > 4){
        throw std::invalid_argument("argument emotion must be unsigned integer between 0 and 4");
    }
    this->emotion = (Satisfaction)emotion;
}

std::string ExpenseLog::asString(){
    std::stringstream ss;
    ss << this->Log::asString();
    ss.seekp(-1, ss.cur);
    ss << ",\"category\":" << getCategory() << ",\"subcategory\":" << getSubcategory() << ",\"price\":" << getPrice()
        << ",\"emotion\":" << getEmotion() << "}";
    return ss.str();
}