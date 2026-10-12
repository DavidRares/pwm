#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>
#include <sodium.h>

#define HASH_LENGTH 32
#define HASH_BYTES ((HASH_LENGTH) * 2 + 1)

using namespace std;

class PasswordHasher{
    public:
        PasswordHasher();
        const string hash_password(const string& plaintext, vector<unsigned char> salt);
        vector<unsigned char> generate_salt(const size_t size);
}extern hasher;
#endif