#include "auth.h"

User::User(string username, string password){
            this->username = username;
            this->salt = hasher.generate_salt(crypto_pwhash_SALTBYTES);
            this->password_hash = hasher.hash_password(password, this->salt);
            this->id = 0;
}

void User::set_id(int id){
    this->id = id;
}