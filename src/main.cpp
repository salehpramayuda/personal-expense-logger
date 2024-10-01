#include "logging.hpp"
#include "db_wrapper.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    /* initializing logger using spdlog */
    try{
        auto logger = spdlog::rotating_logger_mt("app_log", "logs/app.log", 1048576 * 5, 3);
        logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%H:%M:%S %e] [%^%L%$] [thread %t] %v");
        spdlog::set_default_logger(logger);
       
        logger->debug("logger setup finished.");
        spdlog::debug("using global logger.");

        logger->info("printing hello message");

        vector<string> msg {"Expense Logger CLI!"};

        for (const string& word : msg)
        {
            cout << word << " ";
        }
        cout << endl;

        auto db_wrapper = new SQLiteWrapper("expenses.db");
        for (int i = 0; i < 100; i++){
            db_wrapper->addObject(new ExpenseLog(time(nullptr), "DM Soja Schnetzel", PurchaseCategory::OPTIONAL, 1.75, Satisfaction::HAPPY, "Groceries"));
        }
        
        db_wrapper->getByID(0);
        
        delete db_wrapper;

    }
    catch(std::exception) {
        spdlog::error("Error thrown during application runtime.");
    }
    return 0;
}
