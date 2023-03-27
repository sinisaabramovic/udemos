//
//  AESCipher.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 20.03.2023..
//

#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <openssl/sha.h>

#include "AESCipher.hpp"
#include "AESUtil.h"

AESCipher::AESCipher(const std::string &privateKeyPath) : AESCipher(privateKeyPath, EVP_aes_256_cbc()) {}
AESCipher::~AESCipher() {}

AESCipher::AESCipher(const std::string &privateKeyPath, const EVP_CIPHER *cipher)
    : ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free),
      key(keyLength),
      iv(ivLength)
{
    if (!ctx)
    {
        throw std::runtime_error("Failed to create cipher context.");
    }

    std::string privateKeyPem = this->readPrivateKey(privateKeyPath);
    if (!generateKeyIVFromPrivateKeyPEM(privateKeyPem, key.data(), key.size(), iv.data(), iv.size()))
    {
        throw std::runtime_error("Failed to generate AES key and IV");
    }

    if (EVP_EncryptInit_ex(ctx.get(), cipher, nullptr, key.data(), iv.data()) != 1)
    {
        throw std::runtime_error("Failed to initialize encryption operation.");
    }

    EVP_CIPHER_CTX_set_padding(ctx.get(), 1);
}

std::string AESCipher::readPrivateKey(const std::string &keypath)
{
    std::ifstream privateKeyFile(keypath);
    std::stringstream privateKeyBuffer;
    privateKeyBuffer << privateKeyFile.rdbuf();
    std::string privateKeyPem = privateKeyBuffer.str();
    return privateKeyPem;
}

std::vector<byte> AESCipher::encrypt(const byte *plaintext, size_t plaintextLength)
{
    int cipherTextLength = static_cast<int>(plaintextLength) + EVP_CIPHER_CTX_block_size(ctx.get());
    std::vector<byte> ciphertext(cipherTextLength);

    int outLength1;
    if (!EVP_EncryptInit_ex(ctx.get(), nullptr, nullptr, key.data(), iv.data()) ||
        !EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &outLength1, plaintext, plaintextLength))
    {
        throw std::runtime_error("Encryption failed");
    }

    int outLength2;
    if (!EVP_EncryptFinal_ex(ctx.get(), ciphertext.data() + outLength1, &outLength2))
    {
        throw std::runtime_error("Encryption failed");
    }

    ciphertext.resize(outLength1 + outLength2);
    return ciphertext;
}

std::vector<byte> AESCipher::decrypt(const byte *ciphertext, size_t ciphertextLength)
{
    std::vector<byte> plaintext(ciphertextLength);

    unsigned char outLength1 = 0; 
    int outLength2 = 0;
    if (!EVP_DecryptInit_ex(ctx.get(), nullptr, nullptr, key.data(), iv.data()))
    {
        throw std::runtime_error("Decryption failed on init");
    }

    if (!EVP_DecryptUpdate(ctx.get(), plaintext.data(), (int *)&outLength1, ciphertext, (int)ciphertextLength))
    {
        throw std::runtime_error("Decryption failed on update");
    }

    if (!EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + outLength1, &outLength2))
    {
        throw std::runtime_error("Decryption failed on final");
    }

    EVP_CIPHER_CTX_cleanup(ctx.get());

    plaintext.resize(outLength1 + outLength2);
    return plaintext;
}

std::string AESCipher::base64Encode(const std::vector<byte> &data)
{
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::stringstream os;

    typedef boost::archive::iterators::base64_from_binary<
        boost::archive::iterators::transform_width<
            std::vector<byte>::const_iterator, 6, 8>>
        base64_enc;

    std::copy(
        base64_enc(data.begin()), base64_enc(data.end()),
        std::ostream_iterator<char>(os));

    size_t padding = (3 - data.size() % 3) % 3;

    for (size_t i = 0; i < padding; ++i)
    {
        os.put('=');
    }

    return os.str();
}

std::vector<byte> AESCipher::base64Decode(const std::string &input)
{
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    auto is_base64 = [](unsigned char c) -> bool
    {
        return (std::isalnum(c) || (c == '+') || (c == '/'));
    };

    int in_len = input.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    byte char_array_4[4], char_array_3[3];
    std::vector<byte> ret;

    while (in_len-- && (input[in_] != '=') && is_base64(input[in_]))
    {
        char_array_4[i++] = input[in_++];
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
            {
                ret.push_back(char_array_3[i]);
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
        {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++)
        {
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
        {
            ret.push_back(char_array_3[j]);
        }
    }

    return ret;
}