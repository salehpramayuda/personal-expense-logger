cmake_minimum_required(VERSION 3.0.0)
project(personal_expense_logger VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

add_executable(personal_expense_logger main.cpp)

set_property(TARGET personal_expense_logger PROPERTY CXX_STANDARD 17)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

find_package(SQLite3)
if (SQLITE3_FOUND)
    include_directories(${SQLite3_INCLUDE_DIRS})
    target_link_libraries(personal_expense_logger ${SQLite3_LIBRARIES})
endif (SQLITE3_FOUND)