#ifndef AUTH_H
#define AUTH_H

#include <string>
#include "crypto.h"

#define UNALLOWED_CHARS "%%\\<>`´*"

using namespace std;

class User{
    public:
        User(string username, string password);
        void set_id(int id);
    private:
        int id;
        string username;
        string password_hash;
        vector<unsigned char> salt;
};

class Entry{
    public:
        Entry(string title, string password);

    private:
        string domain;
        string title_hash;
        string password_hash;

};

class AuthService{
    public:
        //User register_user(const string& username, const string& password);
        //User login_user(const string& username, const string& password);
        //void change_password(const string& new_password);

}extern auth_service;

#endif