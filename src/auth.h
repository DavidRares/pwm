#ifndef AUTH_H
#define AUTH_H

#include <string>

#define UNALLOWED_CHARS "%%\\<>`´*"
using namespace std;

class User{
    public:
        User(string username, string password);
        ~User() = default;
    private:
        int id;
        string username;
        string password_hash;
};

class Entry{
    public:
        Entry(string title, string password);
        ~Entry() = default;

    private:

        string title;
        string password_hash;

};

class AuthService{
    public:
        AuthService() = default;
        ~AuthService() = default;
        //User register_user(const string& username, const string& password);
        //User login_user(const string& username, const string& password);
        //void change_password(const string& new_password);

}extern auth_service;

#endif