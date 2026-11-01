#ifndef STATEMENT_H
#define STATEMENT_H

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