//
//  AESUtil.h
//  udemos
//
//  Created by Sinisa Abramovic on 21.03.2023..
//

#ifndef AESUtil_h
#define AESUtil_h

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
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <string>
#include <vector>
#include <sstream>

using byte = unsigned char;

bool generate_key_iv_from_password(const std::string& password, byte* key, size_t keyLength, byte* iv, size_t ivLength) {
    byte salt[8];
    
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        std::cerr << "Failed to generate random salt" << std::endl;
        return false;
    }
    
    // Derive key and IV from password and salt using PKCS5_PBKDF2_HMAC
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt, sizeof(salt), 10000,
                          EVP_sha256(), keyLength, key) != 1) {
        std::cerr << "Failed to derive key" << std::endl;
        return false;
    }
    
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt, sizeof(salt), 10000,
                          EVP_sha256(), ivLength, iv) != 1) {
        std::cerr << "Failed to derive IV" << std::endl;
        return false;
    }
    
    return true;
}

bool generate_testable_key_iv(const std::string& password, byte* key, size_t keyLength, byte* iv, size_t ivLength) {
    byte salt[8] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF};
    
    // Derive key and IV from password and salt using PKCS5_PBKDF2_HMAC
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt, sizeof(salt), 10000,
                          EVP_sha256(), keyLength, key) != 1) {
        std::cerr << "Failed to derive key" << std::endl;
        return false;
    }
    
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt, sizeof(salt), 10000,
                          EVP_sha256(), ivLength, iv) != 1) {
        std::cerr << "Failed to derive IV" << std::endl;
        return false;
    }
    
    return true;
}

#endif /* AESUtil_h */
