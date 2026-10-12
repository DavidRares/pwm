#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <exception>
#include <optional>

#include "auth.h"

using namespace std;

class DatabaseService{
    public:
        DatabaseService();
        ~DatabaseService();
        sqlite3* get_db();
    private:
        static sqlite3* db;
}extern database;

class UsersDatabase : public DatabaseService{ 
    public:
        UsersDatabase();
        bool create_user(const User& new_user); //TODO
        bool delete_user(const string username); //TODO
        User* getUser(const string username); //TODO
};
#endif