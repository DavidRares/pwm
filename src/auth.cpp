#include "auth.h"

User::User(string username, string password){
            this->username = username;
            this->salt = hasher.generate_salt(crypto_pwhash_SALTBYTES);
            this->password_hash = hasher.hash_password(password, this->salt);
            this->password = password;
            this->id = 0;
}

User::User(string username, string password_hash, vector<unsigned char> salt, int id){
    this->username = username;
    this->password_hash = password_hash;
    this -> id = id;
    this->salt = salt;
}

void User::set_id(int id){
    this->id = id;
}

int User::get_id(){
    return this->id;
}

const string User::get_username(){
    return this->username;
}

const string User::get_password_hash(){
    return this->password_hash;
}

const string User::get_password(){
    return this->password;
}

const vector<unsigned char> User::get_salt(){
    return this->salt;
}

void AuthService::register_user(User& user){
    optional<User> existing_user = user_table.get(user.get_username());
    if (existing_user)
        throw runtime_error("user already exists.");
    
    user_table.add(user);
}

void AuthService::login_user(User& user){
    optional<User> existing_user = user_table.get(user.get_username());
    if(!existing_user)
        throw runtime_error("Invalid credentials.");

    const string user_password_hash = hasher.hash_password(user.get_password(), existing_user->get_salt());

    if(existing_user->get_password_hash() != user_password_hash)
        throw runtime_error("Invalid credentials.");
    else
        user = *existing_user;
}
