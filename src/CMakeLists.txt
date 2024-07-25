cmake_minimum_required(VERSION 3.0.0)
project(personal_expense_logger VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

add_executable(personal_expense_logger main.cpp)

set_property(TARGET personal_expense_logger PROPERTY CXX_STANDARD 17)