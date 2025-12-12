#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Database.h"
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

class MainFrame : public wxFrame {
    public:
        MainFrame(const wxString& title);
    private:
    wxPanel *panel;

    void CreateControls();
    void SetupSizers();
    void BindEventHandlers();
    
    // SQL DATABASE CONFIG
    DbConfig cfg;

    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> con;
    int UID;


    // LOGIN SCREEN CONTROLS
    wxStaticText *login_headline;

    wxStaticText *login_user_label;
    wxTextCtrl *login_user_field;

    wxStaticText *login_pass_label;
    wxTextCtrl *login_pass_field;

    wxButton *login_sign_on_button;
    wxButton *login_create_account_button;

    void loginPressSignIn(wxCommandEvent& evt);
    void loginPressCreateAccount(wxCommandEvent& evt);

    // APPLCATION CONTROLS
    wxStaticText *app_headline;

    wxStaticText *app_query_label;
    wxTextCtrl *app_query_field;
    wxButton *app_query_button_title;
    wxButton *app_query_button_genre;
    wxButton *app_query_button_country;
    wxButton *app_query_button_director;
    wxButton *app_query_button_rating;

    void QueryTitles(wxCommandEvent& evt);
    void QueryGenres(wxCommandEvent& evt);
    void QueryCountries(wxCommandEvent& evt);
    void QueryDirectors(wxCommandEvent& evt);
    void QueryRatings(wxCommandEvent& evt);

    wxStaticText *app_review_title_label;
    wxTextCtrl *app_review_title_field;
    wxStaticText *app_review_score_label;
    wxSpinCtrl *app_review_score_field;
    wxStaticText *app_review_text_label;
    wxTextCtrl *app_review_text_field;
    wxButton *app_review_upload_button;
    wxButton *app_review_search_button;

    void reviewAttemptUpload(wxCommandEvent& evt);
    void tryReviewSearch(wxCommandEvent& evt);

    wxStaticText *app_upload_label_title;
    wxTextCtrl *app_upload_field_title;
    wxStaticText *app_upload_label_type;
    wxTextCtrl *app_upload_field_type;
    wxStaticText *app_upload_label_director;
    wxTextCtrl *app_upload_field_director;
    wxStaticText *app_upload_label_year;
    wxTextCtrl *app_upload_field_year;
    wxStaticText *app_upload_label_rating;
    wxTextCtrl *app_upload_field_rating;
    wxStaticText *app_upload_label_genre;
    wxTextCtrl *app_upload_field_genre;
    wxStaticText *app_upload_label_descrip;
    wxTextCtrl *app_upload_field_descrip;
    wxStaticText *app_upload_label_cast;
    wxTextCtrl *app_upload_field_cast;
    wxStaticText *app_upload_label_country;
    wxTextCtrl *app_upload_field_country;
    wxStaticText *app_upload_label_added;
    wxTextCtrl *app_upload_field_added;
    wxStaticText *app_upload_label_duration;
    wxTextCtrl *app_upload_field_duration;
    wxStaticText *app_upload_label_service;
    wxTextCtrl *app_upload_field_service;
    wxButton *app_upload_send_button;

    void uploadToDatabase(wxCommandEvent& evt);

    wxListBox *app_results;

    // SIZERS
    wxGridSizer *login_sizer_border;
    wxGridSizer *app_sizer_border;


};