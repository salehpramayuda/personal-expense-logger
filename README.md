# personal-expense-logger
A simple project to design an application for logging personal expenses which uses similar logging framework as explained in Tory Dunlap's Financial Feminist

## Project
The expense logger will be divided into 3 stages:
1. CLI-Application
2. GUI-Application using Qt
3. Web-Application using RestAPI

## Logger Component
The task of expense logging can be defined simply as creating a log containing necessary information about a purchase/expense, storing all log into a database and processing logs in a certain time frame for doing analytics regarding personal expenses. The logging system will contain the following component:
1. ExpenseLog-Object
    An ExpenseLog object provides information regarding the expense details such as time of purchase, amount, purchase category, detail and also emotion regarding the purchase. It will inherit the properties and methods of its parent class Log that is designed to be more general (time and detail).

    The Log-Class contains the properties:
    - private ```time_t``` object that records the logging time
    - private ```string``` object that contains the log content
    - public constructor which can provide a specific log time, log content or none (log time will be designed to be current logging time and content will be set to an empty string)
    - public getter and setter for and content
    - operator overload for accessing all information as string or other format (json)

    The ExpenseLog-Class will extend its properties to have:
    - private ```uint``` variable that specify the price/amount of purchase (in cent)
    - private ```uint``` variable that corresponds to an ```Enum``` for purchase category (Mandatory, Free-Purchase and Future Goal)
    - private ```string``` variable that specify the subcategory of the purchase
    - private ```uint``` variable that corresponds to an ```Enum``` for emotional rating (sad, dissatisfied, neutral, satisfied, happy)
    - public constructor
    - public getter and setter for all properties
    - operator overload for accessing all information as string or other format (json)

2. Logger-Object
    The Logger object will be responsible of accepting user input and constructing a Log object before sending it to the database.

## CLI-Application
The CLI-Application will entail a few functionalities such as:
- Adding a log
- Removing a log with a specific Log-ID
- Exporting expenses log between a certain time frame into readable format (csv)
    