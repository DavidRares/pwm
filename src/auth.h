#ifndef AUTH_H
#define AUTH_H

#include <optional>
#include "crypto.h"
#include "database.h"

#include <iostream>

#define UNALLOWED_CHARS "%%\\<>`´*"

using namespace std;

class UsersDatabase; 

class User{
    public:
        User(string username, string password);
        User(string username, string password_hash, vector<unsigned char> salt, int id);
        void set_id(int id);
        int get_id();
        const string get_username();
        const string get_password_hash();
        const vector<unsigned char> get_salt();
        const string get_password();
    private:
        int id;
        string username;
        string password_hash;
        string password;
        vector<unsigned char> salt;
};

class AuthService{
    public:
        AuthService(UsersDatabase& user_table): user_table(user_table){};
        void register_user(User& user);
        void login_user(User& user);
    private:
        UsersDatabase& user_table;
}extern auth_service;

#endif