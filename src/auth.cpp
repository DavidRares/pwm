#include "auth.h"

using namespace std;

User::User(string username, string password){
            this->username = username;
            this->password_hash = password; //TODO implement PasswordHasher class and call function to hash the password + add salt 
            this->id = 0;
}