#pragma once
// ====== System headers ======
#include <iostream>    // for std::cout, std::cerr
#include <memory>      // for std::unique_ptr (RAII)
#include <vector>      // for std::vector container
#include <string>      // for std::string
#include <iomanip>     // for std::setw, formatting output

// ====== MySQL Connector headers ======
// These come from the "include" directory of MySQL Connector/C++
#include <mysql_driver.h>                // for sql::mysql::get_mysql_driver_instance()
#include <cppconn/driver.h>              // defines sql::Driver class
#include <cppconn/exception.h>           // defines sql::SQLException for catching DB errors
#include <cppconn/connection.h>          // defines sql::Connection (represents a DB connection)
#include <cppconn/statement.h>           // defines sql::Statement (for SQL commands)
#include <cppconn/prepared_statement.h>  // defines sql::PreparedStatement (parameterized SQL)
#include <cppconn/resultset.h>           // defines sql::ResultSet (returned query results)

struct DbConfig {
    std::string host = "tcp://127.0.0.1:3306";
    std::string user = "user";
    std::string pass = "password";
    std::string schema = "next_watch";
};

struct User {
    int id;
    std::string username;
    std::string password;
};

struct Film {
    std::string title;
    std::string form;
    std::string director;
    int release_year;
    std::string rating;
    std::string genres;
    std::string description;
    std::string cast;
    std::string country;
    std::string duration;
    std::string service;
};

struct Review {
    int score;
    std::string text;
};

int tryLogin(sql::Connection *con, std::string username, std::string password);

int tryCreateAccount(sql::Connection *con, std::string username, std::string password);

std::vector<std::string> QueryAttribute(sql::Connection *con, std::string attribute, std::string field);

int reviewUpload(sql::Connection *con, std::string title, std::string text, int score, int UID);

std::vector<std::string> reviewSearch(sql::Connection *con, std::string title);

int getRealId(sql::Connection *con, int show_id, std::string service);

void sendReview(sql::Connection *con, std::string title, std::string text, int score, int UID, std::string service);
