//
//  AESUtil.h
//  udemos
//
//  Created by Sinisa Abramovic on 21.03.2023..
//

#ifndef AESUtil_h
#define AESUtil_h

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
#include <iostream>
#include <sodium.h>

using byte = unsigned char;

bool generateKeyIVFromPrivateKeyPEM(const std::string &privateKeyPEM, byte *key, size_t keyLength, byte *iv, size_t ivLength)
{
    if (sodium_init() < 0)
    {
        std::cerr << "Failed to initialize libsodium" << std::endl;
        return false;
    }

    std::string base64KeyData = privateKeyPEM;
    size_t pos = base64KeyData.find("-----BEGIN PRIVATE KEY-----");
    if (pos != std::string::npos)
    {
        base64KeyData.erase(pos, strlen("-----BEGIN PRIVATE KEY-----"));
    }
    pos = base64KeyData.find("-----END PRIVATE KEY-----");
    if (pos != std::string::npos)
    {
        base64KeyData.erase(pos, strlen("-----END PRIVATE KEY-----"));
    }
    base64KeyData.erase(std::remove(base64KeyData.begin(), base64KeyData.end(), ' '), base64KeyData.end());
    base64KeyData.erase(std::remove(base64KeyData.begin(), base64KeyData.end(), '\n'), base64KeyData.end());
    base64KeyData.erase(std::remove(base64KeyData.begin(), base64KeyData.end(), '\r'), base64KeyData.end());

    std::vector<byte> decodedKeyData(base64KeyData.size());
    size_t decodedKeyDataSize;
    if (sodium_base642bin(decodedKeyData.data(), decodedKeyData.size(), base64KeyData.data(), base64KeyData.size(),
                          nullptr, &decodedKeyDataSize, nullptr, sodium_base64_VARIANT_ORIGINAL) != 0)
    {
        std::cerr << "Failed to decode base64" << std::endl;
        return false;
    }    

    decodedKeyData.resize(decodedKeyDataSize);

    byte digest[crypto_hash_sha512_BYTES];
    crypto_hash_sha512(digest, decodedKeyData.data(), decodedKeyData.size());

    byte *source = digest;
    size_t sourceLength = sizeof(digest);

    if (keyLength > sourceLength || ivLength > sourceLength)
    {
        std::cerr << "Key or IV length is too large" << std::endl;
        return false;
    }

    memcpy(key, source, keyLength * sizeof(byte));
    memcpy(iv, source + keyLength, ivLength * sizeof(byte));

    return true;
}

#endif /* AESUtil_h */
