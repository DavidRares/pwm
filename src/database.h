#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include "auth.h"
#include "statement.h"
#include "entry.h"

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

class VaultDatabase {
public:
    explicit VaultDatabase(DatabaseService& database);
    
private:
    DatabaseService& database;
};

#endif