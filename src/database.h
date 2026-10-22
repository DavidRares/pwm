#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include "auth.h"

using namespace std;
class User;

class DatabaseService{
    public:
        DatabaseService();
        ~DatabaseService();
        sqlite3* get_db();
    private:
        sqlite3* db = nullptr;
}extern database;


class UsersDatabase {
    public:
        explicit UsersDatabase(DatabaseService& database);
        optional<User> get(const string username);
        void add(User& new_user);

private:
    DatabaseService& database;
};

class Statement {
    public:
        Statement(sqlite3* db, const char* query) {
            if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(sqlite3_errmsg(db));
            }
        }

        ~Statement() {
            if (stmt) {
                sqlite3_finalize(stmt);
            }
        }

        Statement(const Statement&) = delete;
        Statement& operator=(const Statement&) = delete;

        Statement(Statement&& other) noexcept : stmt(other.stmt) {
            other.stmt = nullptr;
        }

        Statement& operator=(Statement&& other) noexcept {
            if (this != &other) {
                if (stmt) {
                    sqlite3_finalize(stmt);
                }

                stmt = other.stmt;
                other.stmt = nullptr;
            }
            return *this;
        }

        sqlite3_stmt* get() const noexcept {
            return stmt;
        }

    private:
        sqlite3_stmt* stmt = nullptr;
};

#endif