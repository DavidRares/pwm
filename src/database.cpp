#include "database.h"

using namespace std;

sqlite3* DatabaseService::db = nullptr;

DatabaseService::DatabaseService(){
    sqlite3* db;

    int ret = sqlite3_open("pwm.db", &db);

    if (ret) {
        throw runtime_error("Unable to open the database.");
    }

    this->db = db;
}

DatabaseService::~DatabaseService(){
    sqlite3_close(this->db);
}

sqlite3* DatabaseService::get_db(){
    return db;
 }

UsersDatabase::UsersDatabase(){
    sqlite3* db = get_db();

    if (!db) {
        throw std::runtime_error("Database connection is null.");
    }

    const char* query = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL);)";

    char* err_msg = nullptr;

    const int ret = sqlite3_exec(db, query, nullptr, nullptr, &err_msg);

    if (ret != SQLITE_OK) {
        std::string error = err_msg ? err_msg : sqlite3_errmsg(db);

        sqlite3_free(err_msg);
        throw std::runtime_error("Failed to create users table: " + error);
    }
}