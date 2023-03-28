//
//  AESCipher.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 20.03.2023..
//

#ifndef AESCipher_hpp
#define AESCipher_hpp

#include <openssl/conf.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/kdf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include <iostream>
#include <memory>
#include <vector>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

#include <iostream>
#include <memory>
#include <vector>
#include <cstring>
#include <string>

using byte = unsigned char;

static constexpr size_t keyLength = 32;
static constexpr size_t ivLength = 16;

class AESCipher {
public:
    AESCipher(const std::string& privateKeyPath);
    ~AESCipher();
    std::vector<byte> encrypt(const byte* plaintext, size_t plaintextLength);
    std::vector<byte> decrypt(const byte* ciphertext, size_t ciphertextLength);
    std::string base64Encode(const std::vector<byte>& data);
    std::vector<byte> base64Decode(const std::string& data);
    
private:
    AESCipher(const std::string& password, const EVP_CIPHER* cipher);
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)> ctx;
    std::string readPrivateKey(const std::string& keypath);
    std::vector<byte> key;
    std::vector<byte> iv;
};

#endif /* AESCipher_hpp */
