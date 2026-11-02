#include "crypto.h"

PasswordHasher::PasswordHasher(){
    if (sodium_init() < 0)
        throw runtime_error("Failed to initialize libsodium");
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
        throw runtime_error("Password hashing failed");
    }

    char hex_hash[HASH_BYTES];

    sodium_bin2hex(hex_hash, sizeof(hex_hash), hash, sizeof(hash));

    sodium_memzero(hash, sizeof(hash));
    return string(hex_hash);
}

vector<unsigned char> PasswordHasher::generate_nonce(const size_t size) {
    if (size != crypto_aead_xchacha20poly1305_ietf_NPUBBYTES)
        throw invalid_argument("Invalid nonce size");

    vector<unsigned char> nonce(size);

    randombytes_buf(nonce.data(), nonce.size());

    return nonce;
}

vector<unsigned char> PasswordHasher::encrypt_password(const string& plaintext, const vector<unsigned char>& nonce, const vector<unsigned char>& key){
    if (nonce.size() != crypto_aead_xchacha20poly1305_ietf_NPUBBYTES)
        throw invalid_argument("Invalid nonce size");

    if (key.size() != crypto_aead_xchacha20poly1305_ietf_KEYBYTES)
        throw invalid_argument("Invalid encryption key size");

    vector<unsigned char> ciphertext(plaintext.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES);

    unsigned long long ciphertext_length = 0;

    if (crypto_aead_xchacha20poly1305_ietf_encrypt(ciphertext.data(), &ciphertext_length, reinterpret_cast<const unsigned char*>(plaintext.data()),
        static_cast<unsigned long long>(plaintext.size()), nullptr, 0, nullptr, nonce.data(), key.data()) != 0) {
            throw runtime_error("Password encryption failed");
    }

    ciphertext.resize(ciphertext_length);

    return ciphertext;
}

string PasswordHasher::decrypt_password(const vector<unsigned char>& ciphertext, const vector<unsigned char>& nonce, const vector<unsigned char>& key){
    if (nonce.size() != crypto_aead_xchacha20poly1305_ietf_NPUBBYTES)
        throw invalid_argument("Invalid nonce size");

    if (key.size() != crypto_aead_xchacha20poly1305_ietf_KEYBYTES)
        throw invalid_argument("Invalid encryption key size");

    if (ciphertext.size() < crypto_aead_xchacha20poly1305_ietf_ABYTES)
        throw invalid_argument("Invalid ciphertext size");

    vector<unsigned char> plaintext(ciphertext.size() - crypto_aead_xchacha20poly1305_ietf_ABYTES);
    unsigned long long plaintext_length = 0;

    if (crypto_aead_xchacha20poly1305_ietf_decrypt(plaintext.data(), &plaintext_length, nullptr, ciphertext.data(), 
        static_cast<unsigned long long>(ciphertext.size()), nullptr, 0, nonce.data(), key.data()) != 0) {
            sodium_memzero(plaintext.data(), plaintext.size());
            throw runtime_error("Password decryption failed: invalid key or corrupted data");
    }

    return string(reinterpret_cast<const char*>(plaintext.data()), plaintext_length);
}