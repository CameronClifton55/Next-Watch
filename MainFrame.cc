#include "MainFrame.h"
#include <wx/wx.h>
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


MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title) {
    // Initialize Frame
    CreateControls();
    SetupSizers();
    BindEventHandlers();


    CreateStatusBar();
    // Initialize Database Connectrion using default config
    DbConfig cfg;

    // Get Driver Instance
    driver = sql::mysql::get_mysql_driver_instance();

    // Create Connection to MySQL Server
    // std::unique_ptr<sql::Connection> con(driver->connect(cfg.host, cfg.user, cfg.pass));
    con.reset(driver->connect(cfg.host, cfg.user, cfg.pass));
    
    // Select database for use
    con->setSchema(cfg.schema);

    UID = -1;

    wxLogStatus("THIS IS A TEST");
}

void MainFrame::CreateControls()
{
    printf("CreateControls starting\n");
    const int FIELD_WIDTH = 400;
    wxFont headlineFont(wxFontInfo(wxSize(0,36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 24)));

    panel = new wxPanel(this);
    panel->SetFont(mainFont);

    // LOG IN PAGE
    login_headline = new wxStaticText(panel, wxID_ANY, "User Login");
    login_headline->SetFont(headlineFont);

    login_user_label = new wxStaticText(panel, wxID_ANY, "Enter Username: ");
    login_user_field = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));

    login_pass_label = new wxStaticText(panel, wxID_ANY, "Enter Password: ");
    login_pass_field = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));

    login_sign_on_button = new wxButton(panel, wxID_ANY, "Sign In");
    login_create_account_button = new wxButton(panel, wxID_ANY, "Create New Account");

    printf("Log in controls completed\n");

    // APPLICATION PAGE
    app_headline = new wxStaticText(panel, wxID_ANY, "Next Watch");
    app_headline->SetFont(headlineFont);

    printf("headline completed");

    app_query_label = new wxStaticText(panel, wxID_ANY, "Query: ");
    app_query_field = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_query_button_title = new wxButton(panel, wxID_ANY, "Query Titles");
    app_query_button_genre = new wxButton(panel, wxID_ANY, "Query Genres");
    app_query_button_country = new wxButton(panel, wxID_ANY, "Query Country");
    app_query_button_director = new wxButton(panel, wxID_ANY, "Query Director");
    app_query_button_rating = new wxButton(panel, wxID_ANY, "Query Rating");

    printf("query completed\n");

    app_review_title_label = new wxStaticText(panel, wxID_ANY, "Title: ");
    app_review_title_field = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_review_score_label = new wxStaticText(panel, wxID_ANY, "Score: ");
    app_review_score_field = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_WRAP, 0, 10);
    app_review_text_label = new wxStaticText(panel, wxID_ANY, "Text: ");
    app_review_text_field = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_review_upload_button = new wxButton(panel, wxID_ANY, "Upload Review");
    app_review_search_button = new wxButton(panel, wxID_ANY, "Search Reviews");

    printf("review completed\n");

    app_upload_label_title = new wxStaticText(panel, wxID_ANY, "Title: ");
    app_upload_field_title = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_type = new wxStaticText(panel, wxID_ANY, "Type: ");
    app_upload_field_type = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_director = new wxStaticText(panel, wxID_ANY, "Director: ");
    app_upload_field_director = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_year = new wxStaticText(panel, wxID_ANY, "Year: ");
    app_upload_field_year = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_rating = new wxStaticText(panel, wxID_ANY, "Rating: ");
    app_upload_field_rating = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_genre = new wxStaticText(panel, wxID_ANY, "Genre: ");
    app_upload_field_genre = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_descrip = new wxStaticText(panel, wxID_ANY, "Description: ");
    app_upload_field_descrip = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_cast = new wxStaticText(panel, wxID_ANY, "Cast: ");
    app_upload_field_cast = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_country = new wxStaticText(panel, wxID_ANY, "Country: ");
    app_upload_field_country = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_added = new wxStaticText(panel, wxID_ANY, "Date Added: ");
    app_upload_field_added = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_duration = new wxStaticText(panel, wxID_ANY, "Duration: ");
    app_upload_field_duration = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_label_service = new wxStaticText(panel, wxID_ANY, "Service: ");
    app_upload_field_service = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FIELD_WIDTH, -1));
    app_upload_send_button = new wxButton(panel, wxID_ANY, "Submit Film to Database");

    app_results = new wxListBox(panel, wxID_ANY);

    printf("CreateControls Ending\n");
}

void MainFrame::SetupSizers()
{
    printf("Setup Sizers Starting\n");
    // LOG IN SIZERS
    wxBoxSizer *login_sizer_user = new wxBoxSizer(wxHORIZONTAL);
    login_sizer_user->Add(login_user_label);
    login_sizer_user->Add(login_user_field);

    wxBoxSizer *login_sizer_pass = new wxBoxSizer(wxHORIZONTAL);
    login_sizer_pass->Add(login_pass_label);
    login_sizer_pass->Add(login_pass_field);

    wxBoxSizer *login_sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    login_sizer_buttons->Add(login_sign_on_button);
    login_sizer_buttons->Add(login_create_account_button);

    wxBoxSizer *login_sizer_main = new wxBoxSizer(wxVERTICAL);
    login_sizer_main->Add(login_headline, wxSizerFlags().CenterHorizontal());
    login_sizer_main->AddSpacer(25);
    login_sizer_main->Add(login_sizer_user, wxSizerFlags().CenterHorizontal());
    login_sizer_main->AddSpacer(5);
    login_sizer_main->Add(login_sizer_pass, wxSizerFlags().CenterHorizontal());
    login_sizer_main->AddSpacer(5);
    login_sizer_main->Add(login_sizer_buttons, wxSizerFlags().CenterHorizontal());

    login_sizer_border = new wxGridSizer(1);
    login_sizer_border->Add(login_sizer_main, wxSizerFlags().Border(wxALL, 25).Expand());


    panel->SetSizer(login_sizer_border);
    login_sizer_border->SetSizeHints(this);


    // APPLICATION SIZERS
    wxBoxSizer *app_sizer_query_field = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_query_field->Add(app_query_label);
    app_sizer_query_field->Add(app_query_field);

    wxBoxSizer *app_sizer_query_buttons = new wxBoxSizer(wxVERTICAL);
    app_sizer_query_buttons->Add(app_query_button_title);
    app_sizer_query_buttons->Add(app_query_button_genre);
    app_sizer_query_buttons->Add(app_query_button_country);
    app_sizer_query_buttons->Add(app_query_button_director);
    app_sizer_query_buttons->Add(app_query_button_rating);

    wxBoxSizer *app_sizer_review_title = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_review_title->Add(app_review_title_label);
    app_sizer_review_title->Add(app_review_title_field);

    wxBoxSizer *app_sizer_review_score = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_review_score->Add(app_review_score_label);
    app_sizer_review_score->Add(app_review_score_field);

    wxBoxSizer *app_sizer_review_text = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_review_text->Add(app_review_text_label);
    app_sizer_review_text->Add(app_review_text_field);

    wxBoxSizer *app_sizer_review_buttons = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_review_buttons->Add(app_review_upload_button);
    app_sizer_review_buttons->Add(app_review_search_button);

    wxBoxSizer *app_sizer_upload_title = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_title->Add(app_upload_label_title);
    app_sizer_upload_title->Add(app_upload_field_title);

    wxBoxSizer *app_sizer_upload_type = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_type->Add(app_upload_label_type);
    app_sizer_upload_type->Add(app_upload_field_type);

    wxBoxSizer *app_sizer_upload_director = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_director->Add(app_upload_label_director);
    app_sizer_upload_director->Add(app_upload_field_director);

    wxBoxSizer *app_sizer_upload_year = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_year->Add(app_upload_label_year);
    app_sizer_upload_year->Add(app_upload_field_year);

    wxBoxSizer *app_sizer_upload_rating = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_rating->Add(app_upload_label_rating);
    app_sizer_upload_rating->Add(app_upload_field_rating);

    wxBoxSizer *app_sizer_upload_genre = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_genre->Add(app_upload_label_genre);
    app_sizer_upload_genre->Add(app_upload_field_genre);

    wxBoxSizer *app_sizer_upload_descrip = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_descrip->Add(app_upload_label_descrip);
    app_sizer_upload_descrip->Add(app_upload_field_descrip);

    wxBoxSizer *app_sizer_upload_cast = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_cast->Add(app_upload_label_cast);
    app_sizer_upload_cast->Add(app_upload_field_cast);

    wxBoxSizer *app_sizer_upload_country = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_country->Add(app_upload_label_country);
    app_sizer_upload_country->Add(app_upload_field_country);

    wxBoxSizer *app_sizer_upload_added = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_added->Add(app_upload_label_added);
    app_sizer_upload_added->Add(app_upload_field_added);

    wxBoxSizer *app_sizer_upload_duration = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_duration->Add(app_upload_label_duration);
    app_sizer_upload_duration->Add(app_upload_field_duration);

    wxBoxSizer *app_sizer_upload_service = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_upload_service->Add(app_upload_label_service);
    app_sizer_upload_service->Add(app_upload_field_service);

    printf("Completed low-level Box Sizers\n");

    wxBoxSizer *app_sizer_main = new wxBoxSizer(wxVERTICAL);
    app_sizer_main->Add(app_headline, wxSizerFlags().CenterHorizontal());

    app_sizer_main->AddSpacer(25);
    
    app_sizer_main->Add(app_sizer_query_field, wxSizerFlags().CenterHorizontal());
    app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_query_buttons, wxSizerFlags().CenterHorizontal());

    app_sizer_main->AddSpacer(10);

    app_sizer_main->Add(app_sizer_review_title, wxSizerFlags().CenterHorizontal());
    app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_review_score, wxSizerFlags().CenterHorizontal());
    app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_review_text, wxSizerFlags().CenterHorizontal());
    app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_review_buttons, wxSizerFlags().CenterHorizontal());

    app_sizer_main->AddSpacer(10);

    printf("Human Added Sizers Complete\n");

    app_sizer_main->Add(app_sizer_upload_title, wxSizerFlags().CenterHorizontal());
    printf("title valid\n");
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_type, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_director, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_year, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_rating, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_genre, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_descrip, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_cast, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_country, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_added, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_duration, wxSizerFlags().CenterHorizontal());
    // app_sizer_main->AddSpacer(5);
    app_sizer_main->Add(app_sizer_upload_service, wxSizerFlags().CenterHorizontal());
    app_sizer_main->Add(app_upload_send_button, wxSizerFlags().CenterHorizontal());
    

    printf("Completed Main Box Sizer\n");

    wxBoxSizer *app_sizer_main_results = new wxBoxSizer(wxHORIZONTAL);
    app_sizer_main_results->Add(app_sizer_main, wxSizerFlags().Proportion(1));
    app_sizer_main_results->AddSpacer(5);
    app_sizer_main_results->Add(app_results, wxSizerFlags().Expand().Proportion(1));

    app_sizer_border = new wxGridSizer(1);
    app_sizer_border->Add(app_sizer_main_results, wxSizerFlags().Border(wxALL, 25).Expand());

    app_sizer_border->Show(false);

    printf("SetupSizers Ending\n");
}

void MainFrame::BindEventHandlers()
{
    login_sign_on_button->Bind(wxEVT_BUTTON, &MainFrame::loginPressSignIn, this);
    login_create_account_button->Bind(wxEVT_BUTTON, &MainFrame::loginPressCreateAccount, this);

    app_query_button_title->Bind(wxEVT_BUTTON, &MainFrame::QueryTitles, this);
    app_query_button_genre->Bind(wxEVT_BUTTON, &MainFrame::QueryGenres, this);
    app_query_button_country->Bind(wxEVT_BUTTON, &MainFrame::QueryCountries, this);
    app_query_button_director->Bind(wxEVT_BUTTON, &MainFrame::QueryDirectors, this);
    app_query_button_rating->Bind(wxEVT_BUTTON, &MainFrame::QueryRatings, this);

    app_review_upload_button->Bind(wxEVT_BUTTON, &MainFrame::reviewAttemptUpload, this);
    app_review_search_button->Bind(wxEVT_BUTTON, &MainFrame::tryReviewSearch, this);

    app_upload_send_button->Bind(wxEVT_BUTTON, &MainFrame::uploadToDatabase, this);
}

void MainFrame::loginPressSignIn(wxCommandEvent &evt)
{
    std::string username = login_user_field->GetValue().ToStdString();
    std::string password = login_pass_field->GetValue().ToStdString();

    UID = tryLogin(con.get(), username, password);

    if (UID == -1) {
        wxLogStatus("Failed to Sign In");
    } else {
        wxLogStatus("Successfully Logged In");
        login_sizer_border->Show(false);
        panel->SetSizer(app_sizer_border, false);
        app_sizer_border->SetSizeHints(this);
        app_sizer_border->Show(true);
    }

    login_user_field->SetValue("");
    login_pass_field->SetValue("");
}

void MainFrame::loginPressCreateAccount(wxCommandEvent &evt)
{
    std::string username = login_user_field->GetValue().ToStdString();
    std::string password = login_pass_field->GetValue().ToStdString();

    int value = tryCreateAccount(con.get(), username, password);

    if (value != 1) {
        wxLogStatus("Failed to create account");
    } else {
        wxLogStatus("Account Succesfully Created!");
    }

    login_user_field->SetValue("");
    login_pass_field->SetValue("");


}

void MainFrame::QueryTitles(wxCommandEvent &evt)
{
    app_results->Clear();
    std::vector<std::string> results = QueryAttribute(con.get(), "main.title", app_query_field->GetValue().ToStdString());

    app_query_field->SetValue("");

    for (int i = 0; i < (int) results.size(); ++i) {
        app_results->Append(results.back());
        results.pop_back();
    }
}

void MainFrame::QueryGenres(wxCommandEvent &evt)
{
    app_results->Clear();
    std::vector<std::string> results = QueryAttribute(con.get(), "searchable.listed_in", app_query_field->GetValue().ToStdString());

    app_query_field->SetValue("");

    for (int i = 0; i < (int) results.size(); ++i) {
        app_results->Append(results.back());
        results.pop_back();
    }
}

void MainFrame::QueryCountries(wxCommandEvent &evt)
{
    app_results->Clear();
    std::vector<std::string> results = QueryAttribute(con.get(), "detail.country", app_query_field->GetValue().ToStdString());

    app_query_field->SetValue("");

    for (int i = 0; i < (int) results.size(); ++i) {
        app_results->Append(results.back());
        results.pop_back();
    }
}

void MainFrame::QueryDirectors(wxCommandEvent &evt)
{
    app_results->Clear();
    std::vector<std::string> results = QueryAttribute(con.get(), "main.director", app_query_field->GetValue().ToStdString());

    app_query_field->SetValue("");

    for (int i = 0; i < (int) results.size(); ++i) {
        app_results->Append(results.back());
        results.pop_back();
    }
}

void MainFrame::QueryRatings(wxCommandEvent &evt)
{
    app_results->Clear();
    std::vector<std::string> results = QueryAttribute(con.get(), "searchable.rating", app_query_field->GetValue().ToStdString());

    app_query_field->SetValue("");

    for (int i = 0; i < (int) results.size(); ++i) {
        app_results->Append(results.back());
        results.pop_back();
    }
}

void MainFrame::reviewAttemptUpload(wxCommandEvent &evt)
{
    std::string title = app_review_title_field->GetValue().ToStdString();
    std::string text = app_review_text_field->GetValue().ToStdString();
    int score = app_review_score_field->GetValue();

    app_review_text_field->SetValue("");
    app_review_title_field->SetValue("");

    int i = reviewUpload(con.get(), title, text, score, UID);

    wxLogStatus("%d", i);

    
}

void MainFrame::tryReviewSearch(wxCommandEvent &evt)
{
    app_results->Clear();
    std::string title = app_review_title_field->GetValue().ToStdString();

    std::vector<std::string> results = reviewSearch(con.get(), title);

    app_review_title_field->SetValue("");


    for (int i = 0; i < (int) results.size(); ++i) {
        app_results->Append(results.back());
        results.pop_back();
    }

}

void MainFrame::uploadToDatabase(wxCommandEvent &evt)
{
    try{
    sql::Connection *con2 = con.get();

    int year = atoi(app_upload_field_year->GetValue().ToStdString().c_str());

    std::unique_ptr<sql::PreparedStatement> insertMain(
        con2->prepareStatement(
            "INSERT INTO other_main (title, form, director, release_year)"
            "VALUES (?, ?, ?, ?)"
        )
    );
    insertMain->setString(1, app_upload_field_title->GetValue().ToStdString());
    insertMain->setString(2, app_upload_field_type->GetValue().ToStdString());
    insertMain->setString(3, app_upload_field_director->GetValue().ToStdString());
    insertMain->setInt(4, year);

    insertMain->executeUpdate();

    long long ID; 

    std::unique_ptr<sql::Statement> st(con2->createStatement());
    std::unique_ptr<sql::ResultSet> rs(st->executeQuery("SELECT LAST_INSERT_ID()"));
    rs->next();
    ID = rs->getInt64(1);

    std::unique_ptr<sql::PreparedStatement> insertSearchable(
        con2->prepareStatement(
            "INSERT INTO other_searchable (show_id, rating, listed_in, descrip, service)"
            "VALUES (?, ?, ?, ?, ?)"
        )
    );
    insertSearchable->setInt(1, ID);
    insertSearchable->setString(2, app_upload_field_rating->GetValue().ToStdString());
    insertSearchable->setString(3, app_upload_field_genre->GetValue().ToStdString());
    insertSearchable->setString(4, app_upload_field_descrip->GetValue().ToStdString());
    insertSearchable->setString(5, app_upload_field_service->GetValue().ToStdString());

    insertSearchable->executeUpdate();

    std::unique_ptr<sql::PreparedStatement> insertDetail(
        con2->prepareStatement(
            "INSERT INTO other_detail (show_id, actors, country, duration)"
            "VALUES (?, ?, ?, ?)"
        )
    );

    insertDetail->setInt(1, ID);
    insertDetail->setString(2, app_upload_field_cast->GetValue().ToStdString());
    insertDetail->setString(3, app_upload_field_country->GetValue().ToStdString());
    insertDetail->setString(4, app_upload_field_duration->GetValue().ToStdString());

    insertDetail->executeUpdate();

    app_upload_field_title->SetValue("");
    app_upload_field_type->SetValue("");
    app_upload_field_director->SetValue("");
    app_upload_field_year->SetValue("");
    app_upload_field_rating->SetValue("");
    app_upload_field_genre->SetValue("");
    app_upload_field_descrip->SetValue("");
    app_upload_field_service->SetValue("");
    app_upload_field_cast->SetValue("");
    app_upload_field_country->SetValue("");
    app_upload_field_duration->SetValue("");
} catch (sql::SQLException& e) {
    printf("%s\n", e.what());
}
}
