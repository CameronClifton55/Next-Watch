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
#include "Database.h"

int tryLogin(sql::Connection *con, std::string username, std::string password)
{
    User result;
    
    // Prepare statement
    std::unique_ptr<sql::PreparedStatement> ps(con->prepareStatement("SELECT id, pass FROM users WHERE username = ?"));
    ps->setString(1, username);

    // Execute query
    std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());

    // Check for result
    if (!rs->next()) {
        return -1;
    }

    result.username = username;
    result.password = rs->getString("pass");
    result.id = rs->getInt("id");
    

    if(result.password != password) {
        return -1;
    }

    return result.id;
}

int tryCreateAccount(sql::Connection *con, std::string username, std::string password)
{
    User result;

    // Prepare Statement
    std::unique_ptr<sql::PreparedStatement> ps(con->prepareStatement("SELECT id FROM users WHERE username = ?"));
    ps->setString(1, username);

    // Execute Query
    std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());

    // Check for existing username
    if (rs->next()) {
        return -1;
    }

    // Create new account
    std::unique_ptr<sql::PreparedStatement> ps2(con->prepareStatement("INSERT INTO users (username, pass) VALUES (?, ?)"));
    ps2->setString(1, username);
    ps2->setString(2, password);

    ps2->executeQuery();


    return 1;
}

std::vector<std::string> QueryAttribute(sql::Connection *con, std::string attribute, std::string field)
{
    
    printf("Beginning Attribute Query\n");
    std::vector<Film> Results;
    std::string formattedField = "%" + field + "%";


    printf("Preparing Netflix Query\n");

    std::string query = "SELECT * FROM netflix_main INNER JOIN netflix_searchable ON netflix_main.id = netflix_searchable.show_id INNER JOIN netflix_detail ON netflix_main.id = netflix_detail.show_id WHERE netflix_" + attribute + " LIKE ?";

    std::unique_ptr<sql::PreparedStatement> psNetflix(
        con->prepareStatement(query)
    );
    psNetflix->setString(1, formattedField);

    printf("Querying Netflix...\n");

    std::unique_ptr<sql::ResultSet> rsNetflix(psNetflix->executeQuery());

    printf("Succesfully Queried Netflix\n");

    printf("Preparing Disney Query\n");

    query = "SELECT * FROM disney_main INNER JOIN disney_searchable ON disney_main.id = disney_searchable.show_id INNER JOIN disney_detail ON disney_main.id = disney_detail.show_id WHERE disney_" + attribute + " LIKE ?";

    std::unique_ptr<sql::PreparedStatement> psDisney(
        con->prepareStatement(query)
    );
    psDisney->setString(1, formattedField);

    printf("Querying Disney...\n");

    std::unique_ptr<sql::ResultSet> rsDisney(psDisney->executeQuery());

    printf("Successfully Queried Disney\n");

    printf("Preparing Amazon Query\n");

    query = "SELECT * FROM amazon_main INNER JOIN amazon_searchable ON amazon_main.id = amazon_searchable.show_id INNER JOIN amazon_detail ON amazon_main.id = amazon_detail.show_id WHERE amazon_" + attribute + " LIKE ?";

    std::unique_ptr<sql::PreparedStatement> psAmazon(
        con->prepareStatement(query)
    );
    psAmazon->setString(1, formattedField);

    printf("Querying Amazon...\n");

    std::unique_ptr<sql::ResultSet> rsAmazon(psAmazon->executeQuery());

    printf("Successfully Queried Amazon\n");

    printf("Preparing Other Query\n");

    query = "SELECT * FROM other_main INNER JOIN other_searchable ON other_main.id = other_searchable.show_id INNER JOIN other_detail ON other_main.id = other_detail.show_id WHERE other_" + attribute + " LIKE ?";

    std::unique_ptr<sql::PreparedStatement> psOther(
        con->prepareStatement(query)
    );
    psOther->setString(1, formattedField);

    printf("Querying Other...\n");

    std::unique_ptr<sql::ResultSet> rsOther(psOther->executeQuery());

    printf("Succesfully Queried Other\n");

    printf("Reading Netflix Query Data\n");
    while(rsNetflix->next()) {
        Film temp;
        temp.title = rsNetflix->getString("title");
        temp.form = rsNetflix->getString("form");
        temp.director = rsNetflix->getString("director");
        temp.release_year = rsNetflix->getInt("release_year");

        temp.rating = rsNetflix->getString("rating");
        temp.genres = rsNetflix->getString("listed_in");
        temp.description = rsNetflix->getString("descrip");

        temp.cast = rsNetflix->getString("actors");
        temp.country = rsNetflix->getString("country");
        temp.duration = rsNetflix->getString("duration");
        temp.service = "Netflix";

        Results.push_back(temp);
    }
    printf("Reading Disney Query Data\n");

    
    while(rsDisney->next()) {
        Film temp;
        temp.title = rsDisney->getString("title");
        temp.form = rsDisney->getString("form");
        temp.director = rsDisney->getString("director");
        temp.release_year = rsDisney->getInt("release_year");

        temp.rating = rsDisney->getString("rating");
        temp.genres = rsDisney->getString("listed_in");
        temp.description = rsDisney->getString("descrip");

        temp.cast = rsDisney->getString("actors");
        temp.country = rsDisney->getString("country");
        temp.duration = rsDisney->getString("duration");
        temp.service = "Disney";

        Results.push_back(temp);
    }
    

    printf("Reading Amazon Query Data\n");

    
    while(rsAmazon->next()) {
        Film temp;
        temp.title = rsAmazon->getString("title");
        temp.form = rsAmazon->getString("form");
        temp.director = rsAmazon->getString("director");
        temp.release_year = rsAmazon->getInt("release_year");

        temp.rating = rsAmazon->getString("rating");
        temp.genres = rsAmazon->getString("listed_in");
        temp.description = rsAmazon->getString("descrip");

        temp.cast = rsAmazon->getString("actors");
        temp.country = rsAmazon->getString("country");
        temp.duration = rsAmazon->getString("duration");
        temp.service = "Amazon";

        Results.push_back(temp);
    }
    

    printf("Reading Other Query Data\n");

    
    while(rsOther->next()) {
        printf("looping...\n");
        Film temp;
        temp.title = rsOther->getString("title");
        temp.form = rsOther->getString("form");
        temp.director = rsOther->getString("director");
        temp.release_year = rsOther->getInt("release_year");

        temp.rating = rsOther->getString("rating");
        temp.genres = rsOther->getString("listed_in");
        temp.description = rsOther->getString("descrip");

        temp.cast = rsOther->getString("actors");
        temp.country = rsOther->getString("country");
        temp.duration = rsOther->getString("duration");

        temp.service = rsOther->getString("service");

        Results.push_back(temp);
    }
    

    printf("Preparing Data for Output\n");

    std::vector<std::string> output;

    for (int i = 0; i < (int) Results.size(); ++i) {
        std::string temp;
        temp = Results[i].title;
        temp += "	";
        temp += Results[i].form;
        temp += "	";
        temp += Results[i].director;
        temp += "	";
        temp += std::to_string(Results[i].release_year);
        temp += "	";
        temp += Results[i].rating;
        temp += "	";
        temp += Results[i].genres;
        temp += "	";
        temp += Results[i].description;
        temp += "	";
        temp += Results[i].cast;
        temp += "	";
        temp += Results[i].country;
        temp += "	";
        temp += Results[i].duration;
        temp += "	";
        temp += Results[i].service;

        output.push_back(temp);
    }

    printf("Ending Attribute Query\n");
    return output;
}

int reviewUpload(sql::Connection *con, std::string title, std::string text, int score, int UID)
{
    try {
    std::unique_ptr<sql::PreparedStatement> checkNetflix(
        con->prepareStatement("SELECT * FROM netflix_main WHERE title = ?")
    );
    checkNetflix->setString(1, title);

    std::unique_ptr<sql::ResultSet> netflixResult(checkNetflix->executeQuery());

    std::unique_ptr<sql::PreparedStatement> checkDisney(
        con->prepareStatement("SELECT * FROM disney_main WHERE title = ?")
    );
    checkDisney->setString(1, title);

    std::unique_ptr<sql::ResultSet> disneyResult(checkDisney->executeQuery());

    std::unique_ptr<sql::PreparedStatement> checkAmazon(
        con->prepareStatement("SELECT * FROM amazon_main WHERE title = ?")
    );
    checkAmazon->setString(1, title);

    std::unique_ptr<sql::ResultSet> amazonResult(checkAmazon->executeQuery());

    std::unique_ptr<sql::PreparedStatement> checkOther(
        con->prepareStatement("SELECT * FROM other_main WHERE title = ?")
    );
    checkOther->setString(1, title);

    std::unique_ptr<sql::ResultSet> otherResult(checkOther->executeQuery());

    bool netflix = netflixResult->next();
    bool disney = disneyResult->next();
    bool amazon = amazonResult->next();
    bool other = otherResult->next();

    if (!(netflix || disney || amazon || other)) {
        return 0;
    }

    if (netflix) {
        sendReview(con, title, text, score, UID, "netflix");
    }

    if (disney) {
        sendReview(con, title, text, score, UID, "disney");
    }

    if (amazon) {
        sendReview(con, title, text, score, UID, "amazon");
    }

    if (other) {
        sendReview(con, title, text, score, UID, "other");
    }

    return 1;
    } catch (const sql::SQLException& e) {
        printf("ERR: %s\n", e.what());
        return -5;
    }
}

std::vector<std::string> reviewSearch(sql::Connection *con, std::string title)
{
    try {
    std::vector<std::string> output;
    std::unique_ptr<sql::PreparedStatement> checkNetflix(
        con->prepareStatement("SELECT * FROM netflix_main WHERE title = ?")
    );
    checkNetflix->setString(1, title);

    std::unique_ptr<sql::ResultSet> netflixResult(checkNetflix->executeQuery());

    std::unique_ptr<sql::PreparedStatement> checkDisney(
        con->prepareStatement("SELECT * FROM disney_main WHERE title = ?")
    );
    checkDisney->setString(1, title);

    std::unique_ptr<sql::ResultSet> disneyResult(checkDisney->executeQuery());

    std::unique_ptr<sql::PreparedStatement> checkAmazon(
        con->prepareStatement("SELECT * FROM amazon_main WHERE title = ?")
    );
    checkAmazon->setString(1, title);

    std::unique_ptr<sql::ResultSet> amazonResult(checkAmazon->executeQuery());

    std::unique_ptr<sql::PreparedStatement> checkOther(
        con->prepareStatement("SELECT * FROM other_main WHERE title = ?")
    );
    checkOther->setString(1, title);

    std::unique_ptr<sql::ResultSet> otherResult(checkOther->executeQuery());

    int id;
    int temp;
    std::vector<int> realIds;

    if (netflixResult->next()) {
        id = netflixResult->getInt("id");
        temp = getRealId(con, id, "netflix");
        if (temp != -1) realIds.push_back(temp);
    }

    if (disneyResult->next()) {
        id = disneyResult->getInt("id");
        temp = getRealId(con, id, "disney");
        if (temp != -1) realIds.push_back(temp);
    }

    if (amazonResult->next()) {
        id = amazonResult->getInt("id");
        temp = getRealId(con, id, "amazon");
        if (temp != -1) realIds.push_back(temp);
    }

    if (otherResult->next()) {
        id = otherResult->getInt("id");
        temp = getRealId(con, id, "other");
        if (temp != -1) realIds.push_back(temp);
    }

    std::vector<Review> reviews;

    for (int i = 0; i < (int) realIds.size(); ++i) {
        int temp = realIds[i];
        std::unique_ptr<sql::PreparedStatement> ps1(
            con->prepareStatement("SELECT review_text, review_score FROM reviews WHERE film_id = ?")
        );
        ps1->setInt(1, temp);

        std::unique_ptr<sql::ResultSet> rs1(ps1->executeQuery());

        while(rs1->next()) {
            Review buff;
            buff.score = rs1->getInt("review_score");
            buff.text = rs1->getString("review_text");

            reviews.push_back(buff);
        }
    }

    for (int i = 0; i < (int) reviews.size(); ++i) {
        Review tempReview = reviews[i];
        std::string tempString = tempReview.text;
        tempString += "\t";
        tempString += std::to_string(tempReview.score);
        tempString += "/10";

        output.push_back(tempString);
    }

    return output;
} catch (sql::SQLException& e) {
    printf("SQL ERROR %s\n", e.what());
    std::vector<std::string> out;
    return out;
}
}

int getRealId(sql::Connection *con, int show_id, std::string service)
{
    std::unique_ptr<sql::PreparedStatement> ps(
        con->prepareStatement("SELECT id FROM shows WHERE " + service + "_id = ?")
    );
    ps->setInt(1, show_id);

    std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());

    if (!rs->next()) {
        return -1;
    }

    int real = rs->getInt("id");

    return real;
}

void sendReview(sql::Connection *con, std::string title, std::string text, int score, int UID, std::string service)
{

    // Get show ID from service
    std::unique_ptr<sql::PreparedStatement> ps(
        con->prepareStatement("SELECT id FROM " + service + "_main WHERE title = ?")
    );
    ps->setString(1, title);

    std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());

    if (!rs->next()) {
        printf("ERR: No %s_main entry found for title %s\n", service.c_str(), title.c_str());
        return;
    }

    int show_id = rs->getInt("id");

    // Get overall show ID
    std::unique_ptr<sql::PreparedStatement> ps2(
        con->prepareStatement("SELECT id FROM shows WHERE " + service + "_id = ?")
    );
    ps2->setInt(1, show_id);

    std::unique_ptr<sql::ResultSet> rs2(ps2->executeQuery());

    if (!rs2->next()) {
        printf("ERR: No global shows entry for %s id %d\n", service.c_str(), show_id);
        return;
    }

    int real_id = rs2->getInt("id");

    // Insert Review
    std::unique_ptr<sql::PreparedStatement> send(
        con->prepareStatement("INSERT INTO reviews (user_id, film_id, review_score, review_text) VALUES (?, ?, ?, ?)")
    );
    send->setInt(1, UID);
    send->setInt(2, real_id);
    send->setInt(3, score);
    send->setString(4, text);

    send->executeQuery();
}
