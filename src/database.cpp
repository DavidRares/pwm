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
            auth_salt BLOB NOT NULL,
            key_nonce BLOB NOT NULL
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

    if (!db) 
        throw runtime_error("Database connection is null.");

    Statement statement( db,
        "SELECT id, username, password_hash, auth_salt, key_nonce "
        "FROM users WHERE username = ?;"
    );

    sqlite3_stmt* stmt = statement.get();

    if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) 
        throw runtime_error(sqlite3_errmsg(db));

    int result = sqlite3_step(stmt);

    if (result == SQLITE_DONE) 
        return nullopt;

    if (result != SQLITE_ROW) 
        throw runtime_error(sqlite3_errmsg(db));

    int id = sqlite3_column_int(stmt, 0);

    const auto* username_text = sqlite3_column_text(stmt, 1);
    const auto* hash_text = sqlite3_column_text(stmt, 2);

    string db_username = username_text ? reinterpret_cast<const char*>(username_text): "";
    string password_hash = hash_text ? reinterpret_cast<const char*>(hash_text) : "";

    const void* auth_salt_blob = sqlite3_column_blob(stmt, 3);
    int auth_salt_size = sqlite3_column_bytes(stmt, 3);

    const void* key_nonce_blob = sqlite3_column_blob(stmt, 4);
    int key_nonce_size = sqlite3_column_bytes(stmt, 4);

    vector<unsigned char> auth_salt;
    vector<unsigned char> key_nonce;

    if (auth_salt_blob && auth_salt_size > 0) {
        const auto* bytes = static_cast<const unsigned char*>(auth_salt_blob);
        auth_salt.assign(bytes, bytes + auth_salt_size);
    }

    if (key_nonce_blob && key_nonce_size > 0) {
        const auto* bytes = static_cast<const unsigned char*>(key_nonce_blob);
        key_nonce.assign(bytes, bytes + key_nonce_size);
    }

    return User(db_username, password_hash, auth_salt, key_nonce, id);
}

void UsersDatabase::add(User& new_user) {
    sqlite3* db = database.get_db();

    if (!db) {
        throw runtime_error("Database connection is null.");
    }

    Statement statement(db,
        "INSERT INTO users (username, password_hash, auth_salt, key_nonce) "
        "VALUES (?, ?, ?, ?);"
    );

    sqlite3_stmt* stmt = statement.get();

    const string username = new_user.get_username();
    const string hash = new_user.get_password_hash();
    const vector<unsigned char> auth_salt = new_user.get_auth_salt();
    const vector<unsigned char> key_nonce = new_user.get_key_nonce();

    if (sqlite3_bind_text( stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_blob( stmt, 3, auth_salt.empty() ? nullptr : auth_salt.data(), static_cast<int>(auth_salt.size()), SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_blob( stmt, 4, key_nonce.empty() ? nullptr : key_nonce.data(), static_cast<int>(key_nonce.size()), SQLITE_TRANSIENT) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
    }

    if (sqlite3_step(stmt) != SQLITE_DONE)
        throw runtime_error(sqlite3_errmsg(db));

    new_user.set_id(static_cast<int>(sqlite3_last_insert_rowid(db)));
}

}