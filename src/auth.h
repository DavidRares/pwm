#ifndef AUTH_H
#define AUTH_H

#include <string>

#define UNALLOWED_CHARS "%%\\<>`´*"
using namespace std;


class AuthService{
    public:
        AuthService() = default;
        ~AuthService() = default;
};
extern AuthService auth_service;

class User{
    public:
        User(string username, string password);
        ~User() = default;

    private:
        int id;
        string username;
        string password_hash;
};

#endif