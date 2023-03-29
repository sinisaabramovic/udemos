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
#include <openssl/err.h>

#include "AESCipher.hpp"
#include "AESUtil.h"

AESCipher::AESCipher(const std::string &privateKeyPath) : AESCipher(privateKeyPath, EVP_aes_256_cbc()) {}
AESCipher::~AESCipher() {}

AESCipher::AESCipher(const std::string &privateKeyPath, const EVP_CIPHER *cipher)
    : encryptCtx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free),
      decryptCtx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free),
      key(keyLength),
      iv(ivLength)
{
    if (!decryptCtx || !encryptCtx)
    {
        throw std::runtime_error("Failed to create cipher context.");
    }

    std::string privateKeyPem = this->readPrivateKey(privateKeyPath);
    if (!generateKeyIVFromPrivateKeyPEM(privateKeyPem, key.data(), key.size(), iv.data(), iv.size()))
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Failed to generate AES key and IV: ") + err_buf);
    }

    std::cout << "Key: ";
    for (const auto &byte : key)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout << std::endl;

    std::cout << "IV: ";
    for (const auto &byte : iv)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout << std::endl;

    if (EVP_EncryptInit_ex(encryptCtx.get(), cipher, nullptr, key.data(), iv.data()) != 1)
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Failed to initialize encryption operation: ") + err_buf);
    }

    if (EVP_EncryptInit_ex(decryptCtx.get(), cipher, nullptr, key.data(), iv.data()) != 1)
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Failed to initialize decryption operation.: ") + err_buf);
    }

    EVP_CIPHER_CTX_set_padding(encryptCtx.get(), 1);
    EVP_CIPHER_CTX_set_padding(decryptCtx.get(), 1);
}

std::string AESCipher::readPrivateKey(const std::string &keypath)
{
    std::string privateKeyPem = R"(-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCp+ueNSiZTrSMa
Ixmzx4iGnrJApl5MkkAIwBYgVUJxa8+rCOePEfzdlhvl11lFngDB5rM3ANIp7k4w
D+D8FFb1y0dZIeGJjWttWk0aXa1s2YBAwL3nwFzJS6exj+w2Zjm/OYdndnssoa1J
UcOB8Pb8egLHQG+Lh7/7a/e2woGkenr5tFBRmdYfzDQCeHZ/PqNF09Wsh6y3wevY
kWuo27lB43yaIiwkKTnSWTNna+GcsJQvphtSTVFgNMs11g7ulT74Z3Jbzx76HyMi
VFWna7RzdzqjrkyI4S3AQllkrA3yHRPiZdXKAebPuP1JKSUbAc66OobSWSKeMrry
20uf4IB1AgMBAAECggEAH0jcoTaKg1AUa9UV/+6zhANlT1zPckxgmLux1tWaWwG6
wQHsjHqFDct46v6NobCI9yccyPzPsYP4xAPIJt0ffDGjuhXwSr7KjN/X3QhsyOHn
JwPSb/sJmMrh3F5e3UxHSJiwASzfUsGXAS2ZTCsraiCSrlV/sjCT9xRSDUMTVVAg
y4ebGaNhHWnOc1fdEblOj+d6L2EAENQcTu7eG+BX8bmFBxyVRelUlRGBD3kxk9eP
0ui7KCZldJsENP8SYrCifWTsnn8HWPTlBzxodcpjwJZUhX+GTck7cxtPF+6Nb2oT
jCdHosNiTlG25tXo3OzG49vTa/vhLi7Dx9z2M6eoKQKBgQDXC2klOOuwe2MVq7kv
NjqeXqtIuVqaeBTiFjCu6GE3l1q42wA9bRxNA/vU36a3e2PCxHaoCCZSqvvpM1AU
NeSTae3K6jCJBPzeNJIn3HBvb5gR6Clg8IyXVafPLC3ESYq7YFwC5Ykpx3UDF35h
OsMK7P0j66BXEpbhRB6mhCw2bwKBgQDKWlsoWnanAUQQmp0kXBvpM5rxvoQkgTX9
2hS8A7Se6TKqWYWXOHWss7+W9u81JPgD68n/8QW5f1izfL6eBX+IlE2NDcpv7M8d
fgyNl6UGuV/2mxalcbaSgQrPFh5T45V3Z55aAGFeorskwc0Rqgwr6V4ATNKUELmQ
cpK/f1TJWwKBgQDQNNcX1/WyyLRVvg8tW+cDgQt7/SJxHsSxZeS65HmEFoIbKscf
si3Nj4bbQ7OqQMiFQizUYhegwLXowurMtqVZ6Rz38ykM8QxXpDTCbVdbPYPSCqNN
nXmx6JfyKYciXkX5R8GueEkGb3ymgq+1Oeyv/mbVyYRT6ca10nuOo7ge9wKBgD6+
T5+ulTXpdZJ0bp9UlC72Gmg0+8JZ8kTZNy6pM3tGsbgSdBLUpeA8B/8maDuhT+nf
Vc4vjOfcrcNRvcU/zTCrU3SWPxv/YKUzbW8GA1yXZEMSCa6ZGNRKgPB/Wj4GxlS0
42vizE5MxRx7nzuH76g5/zS2nWDd4rwAWrJYKBEbAoGAek0V+3F6QNuUO7pYv5hG
bIhfXoM/lm+cHZgVU/sDbkZ/4Bbyt0X6dBy+/3oHRB68e7EzBt3CpZpbJJVTLtxg
KkN+Vn5JgzpQnn75nu+fn3aYQ/o+00ZnWAuNpUCQQP7BGoB67r5/oSB2f9y1un4t
yviDg42q+AMOQU3QFXfn7E4=
-----END PRIVATE KEY-----
)";
    return privateKeyPem;
}

std::vector<byte> AESCipher::encrypt(const byte *plaintext, size_t plaintextLength)
{
    int cipherTextLength = static_cast<int>(plaintextLength) + EVP_CIPHER_CTX_block_size(encryptCtx.get());
    std::vector<byte> ciphertext(cipherTextLength);

    int outLength1;
    if (!EVP_EncryptInit_ex(encryptCtx.get(), nullptr, nullptr, key.data(), iv.data()) ||
        !EVP_EncryptUpdate(encryptCtx.get(), ciphertext.data(), &outLength1, plaintext, plaintextLength))
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Encryption failed :") + err_buf);
    }

    int outLength2;
    if (!EVP_EncryptFinal_ex(encryptCtx.get(), ciphertext.data() + outLength1, &outLength2))
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Encryption failed: ") + err_buf);
    }

    ciphertext.resize(outLength1 + outLength2);
    return ciphertext;
}

void AESCipher::print_ciphertext(const byte *ciphertext, size_t length)
{
    std::cout << "Ciphertext: ";
    for (size_t i = 0; i < length; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ciphertext[i]);
    }
    std::cout << std::dec << std::endl;
}

std::vector<byte> AESCipher::decrypt(const byte *ciphertext, size_t ciphertextLength)
{
    std::vector<byte> plaintext(ciphertextLength);

    print_ciphertext(ciphertext, ciphertextLength);

    unsigned char outLength1 = 0;
    int outLength2 = 0;
    if (!EVP_DecryptInit_ex(decryptCtx.get(), nullptr, nullptr, key.data(), iv.data()))
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Decryption failed on init: ") + err_buf);
    }

    if (!EVP_DecryptUpdate(decryptCtx.get(), plaintext.data(), (int *)&outLength1, ciphertext, (int)ciphertextLength))
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Decryption failed on update : ") + err_buf);
    }

    if (!EVP_DecryptFinal_ex(decryptCtx.get(), plaintext.data() + outLength1, &outLength2))
    {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Decryption failed on final : ") + err_buf);
    }

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

std::vector<std::uint8_t> AESCipher::base64Decode(const std::string &input)
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
