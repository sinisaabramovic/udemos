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
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

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
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

using byte = unsigned char;

bool generateKeyIVFromPrivateKeyPEM(const std::string& privateKeyPEM, byte* key, size_t keyLength, byte* iv, size_t ivLength) {
    std::string base64KeyData = privateKeyPEM;
    size_t pos = base64KeyData.find("-----BEGIN PRIVATE KEY-----");
    if (pos != std::string::npos) {
        base64KeyData.erase(pos, strlen("-----BEGIN PRIVATE KEY-----"));
    }
    pos = base64KeyData.find("-----END PRIVATE KEY-----");
    if (pos != std::string::npos) {
        base64KeyData.erase(pos, strlen("-----END PRIVATE KEY-----"));
    }
    base64KeyData.erase(std::remove(base64KeyData.begin(), base64KeyData.end(), ' '), base64KeyData.end());
    base64KeyData.erase(std::remove(base64KeyData.begin(), base64KeyData.end(), '\n'), base64KeyData.end());
    base64KeyData.erase(std::remove(base64KeyData.begin(), base64KeyData.end(), '\r'), base64KeyData.end());
    
    BIO* bio_mem = BIO_new_mem_buf(base64KeyData.data(), base64KeyData.size());
    BIO* bio64 = BIO_new(BIO_f_base64());
    BIO_set_flags(bio64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(bio64, bio_mem);
    
    char buffer[1024] = {0};
    std::string output;
    int bytesRead;
    while ((bytesRead = BIO_read(bio64, buffer, sizeof(buffer))) > 0) {
        output.append(buffer, bytesRead);
    }
    
    BIO_free_all(bio64);
    
    byte digest[SHA512_DIGEST_LENGTH];
    SHA512(reinterpret_cast<const unsigned char*>(output.data()), output.size(), digest);
    
    byte* source = digest;
    size_t sourceLength = sizeof(digest);
    
    if (keyLength > sourceLength || ivLength > sourceLength) {
        std::cerr << "Key or IV length is too large" << std::endl;
        return false;
    }
    
    memcpy(key, source, keyLength);
    memcpy(iv, source + keyLength, ivLength);
    
    return true;
}

#endif /* AESUtil_h */
