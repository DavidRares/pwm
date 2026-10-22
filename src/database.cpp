#include "database.h"

using namespace std;

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

UsersDatabase::UsersDatabase(DatabaseService& database): database(database)  {
    sqlite3* db = this->database.get_db();

    if (!db) 
        throw runtime_error("Database connection is null.");

    const char* query = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            salt BLOB NOT NULL
        );
    )";

    char* err_msg = nullptr;

    const int ret = sqlite3_exec(db, query, nullptr, nullptr, &err_msg);

    if (ret != SQLITE_OK) {
        string error = err_msg ? err_msg : sqlite3_errmsg(db);
        sqlite3_free(err_msg);
        runtime_error("Failed to create users table: " + error);
    }
}

optional<User> UsersDatabase::get(const string username) {
    sqlite3* db = database.get_db();

    if (!db) {
        throw runtime_error("Database connection is null.");
    }

    Statement statement(
        db,
        "SELECT id, username, password_hash, salt "
        "FROM users WHERE username = ?;"
    );

    sqlite3_stmt* stmt = statement.get();

    if (sqlite3_bind_text(
            stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        throw runtime_error(sqlite3_errmsg(db));
    }

    int result = sqlite3_step(stmt);

    if (result == SQLITE_DONE) {
        return nullopt;
    }

    if (result != SQLITE_ROW) {
        throw runtime_error(sqlite3_errmsg(db));
    }

    int id = sqlite3_column_int(stmt, 0);

    const auto* username_text = sqlite3_column_text(stmt, 1);
    const auto* hash_text = sqlite3_column_text(stmt, 2);

    string db_username = username_text
        ? reinterpret_cast<const char*>(username_text)
        : "";

    string password_hash = hash_text
        ? reinterpret_cast<const char*>(hash_text)
        : "";

    const void* salt_blob = sqlite3_column_blob(stmt, 3);
    int salt_size = sqlite3_column_bytes(stmt, 3);

    vector<unsigned char> salt;

    if (salt_blob && salt_size > 0) {
        const auto* bytes =
            static_cast<const unsigned char*>(salt_blob);

        salt.assign(bytes, bytes + salt_size);
    }

    return User(db_username, password_hash, salt, id);
}

void UsersDatabase::add(User& new_user) {
    sqlite3* db = database.get_db();

    if (!db) {
        throw runtime_error("Database connection is null.");
    }

    Statement statement(
        db,
        "INSERT INTO users (username, password_hash, salt) "
        "VALUES (?, ?, ?);"
    );

    sqlite3_stmt* stmt = statement.get();

    const string username = new_user.get_username();
    const string hash = new_user.get_password_hash();
    const vector<unsigned char> salt = new_user.get_salt();

    if (sqlite3_bind_text(
            stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(
            stmt, 2, hash.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_blob(
            stmt, 3,
            salt.empty() ? nullptr : salt.data(),
            static_cast<int>(salt.size()),
            SQLITE_TRANSIENT) != SQLITE_OK) {
        throw runtime_error(sqlite3_errmsg(db));
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw runtime_error(sqlite3_errmsg(db));
    }

    new_user.set_id(
        static_cast<int>(sqlite3_last_insert_rowid(db))
    );
}