cmake_minimum_required(VERSION 3.0.0)
project(personal_expense_logger VERSION 0.1.0 LANGUAGES CXX)


add_executable(personal_expense_logger main.cpp db_wrapper.cpp logging.cpp)
set_property(TARGET personal_expense_logger PROPERTY CXX_STANDARD 20)


#include(CTest)
#enable_testing()

if(NOT TARGET spdlog)
    # Stand-alone build
    #file(COPY "/usr/include/spdlog" DESTINATION "src/include/")
    find_package(spdlog REQUIRED)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(SQLite3)
if (SQLITE3_FOUND)
    #include_directories(${SQLite3_INCLUDE_DIRS})
    target_link_libraries(personal_expense_logger PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> ${SQLite3_LIBRARY} ) 
    #target_link_libraries(personal_expense_logger PRIVATE ${SQLite3_LIBRARIES})
endif (SQLITE3_FOUND)

