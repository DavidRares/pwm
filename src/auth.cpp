#include "auth.h"

User::User(string username, string password) : username(username),password(password),id(0){
            this->auth_salt = hasher.generate_salt(crypto_pwhash_SALTBYTES);
            this->password_hash = hasher.hash_password(password, this->auth_salt);
            this->key_nonce = hasher.generate_nonce(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
}

User::User(string username, string password_hash, vector<unsigned char> auth_salt, vector<unsigned char> key_nonce, int id) : username(username), password_hash(password_hash), id(id), auth_salt(auth_salt), key_nonce(key_nonce){}

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

const vector<unsigned char> User::get_auth_salt(){
    return this->auth_salt;
}

const vector<unsigned char> User::get_key_nonce(){
    return this->key_nonce;
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

    const string user_password_hash = hasher.hash_password(user.get_password(), existing_user->get_auth_salt());

    if(existing_user->get_password_hash() != user_password_hash)
        throw runtime_error("Invalid credentials.");
    else
        user = *existing_user;
}
