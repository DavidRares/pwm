#include "crypto.h"

PasswordHasher::PasswordHasher(){
      if (sodium_init() < 0)
        throw std::runtime_error("Failed to initialize libsodium");

}

vector<unsigned char> PasswordHasher::generate_salt(const size_t size){
    std::vector<unsigned char> salt(size);
    randombytes_buf(salt.data(), salt.size());
    return salt;
}

const string PasswordHasher::hash_password(const string& plaintext, vector<unsigned char> salt){
    unsigned char hash[HASH_LENGTH];

    if (crypto_pwhash(hash, sizeof(hash), plaintext.data(), static_cast<unsigned long long>(plaintext.size()), salt.data(), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_ARGON2ID13) != 0) {
        sodium_memzero(hash, sizeof(hash));
        throw std::runtime_error("Password hashing failed");
    }

    char hex_hash[HASH_BYTES];

    sodium_bin2hex(hex_hash, sizeof(hex_hash), hash, sizeof(hash));

    sodium_memzero(hash, sizeof(hash));
    return std::string(hex_hash);
}