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
    std::ifstream privateKeyFile(keypath);
    std::stringstream privateKeyBuffer;
    privateKeyBuffer << privateKeyFile.rdbuf();
    std::string privateKeyPem = privateKeyBuffer.str();
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


void AESCipher::print_ciphertext(const byte* ciphertext, size_t length) {
  std::cout << "Ciphertext: ";
  for (size_t i = 0; i < length; i++) {
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
