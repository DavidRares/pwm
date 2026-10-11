#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <exception>
#include <iostream>

using namespace std;

class DatabaseService{
    public:
        DatabaseService();
        ~DatabaseService();

    private:
        static sqlite3* db;
};
#endif