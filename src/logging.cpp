#include "logging.hpp"
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>

Log::Log(){
    this->log_time = time(nullptr);
    this->content = "";
}

Log::Log(time_t log_time, std::string content): content(content), log_time(log_time){  
}

void Log::setContent(std::string content){
    this->content = content;
}

std::string Log::getContent(){
    return content;
}

std::time_t Log::getLogTime(){
    return log_time;
} 

std::string Log::asJson(){
    std::stringstream sstr;
    sstr << "{ \"log_time\":" << std::ctime(&this->log_time) << ",\"content\":" << this->content << " }";
    return sstr.str();
}

std::string Log::asString(){
    return asJson(); 
}

ExpenseLog::ExpenseLog(time_t log_time, std::string content, unsigned int category, unsigned int price, unsigned int emotion, std::string subcategory): 
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

ExpenseLog::ExpenseLog(time_t log_time, std::string content, PurchaseCategory category, unsigned int price, Satisfaction emotion, std::string subcategory):
    Log(log_time, content), category(category), price(price), emotion(emotion), subcategory(subcategory) 
{

}

void ExpenseLog::setCategory(unsigned int category){
    if(category > 2){
        throw std::invalid_argument("argument category must be unsigned integer between 0 and 2");
    }
    this->category = (PurchaseCategory)category;
}

void ExpenseLog::setEmotion(unsigned int emotion){
    if(emotion > 4){
        throw std::invalid_argument("argument emotion must be unsigned integer between 0 and 4");
    }
    this->emotion = (Satisfaction)emotion;
}

std::string ExpenseLog::asJson(){
    std::stringstream ss;
    ss << this->Log::asJson();
    ss.seekp(-1, ss.cur);
    ss << ",\"category\":" << getCategory() << ",\"subcategory\":" << getSubcategory() << ",\"price\":" << getPrice()
        << ",\"emotion\":" << getEmotion() << "}";
    return ss.str();
}

std::string ExpenseLog::asString(){
    return asJson();
}

std::string ExpenseLog::asSQLQuery(){
    /* output a string containing the value of an sql query with the format datetime, 
    price, category, subcategory, satisfaction, details*/
    std::stringstream ss;
    time_t datet = getLogTime();
    auto ptm = std::localtime(&datet);
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);

    ss << "VALUES ('" << buffer << "', " << getPrice() << ", " << getCategory() << ", '" + getSubcategory() + "', " 
        << getEmotion() << ", '" + getContent() + "')" ;

    return ss.str();
}