#ifndef ud_encrypt_aes_hpp
#define ud_encrypt_aes_hpp

#include <iostream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/opensslconf.h>

using namespace std;

// class encrypt_aes
// {
// private:
//     /* data */
// public:
//     encrypt_aes(/* args */);
//     ~encrypt_aes();
//     string encryptAES(const string &message, const unsigned char *key);
//     string decryptAES(const string &ciphertext, const unsigned char *key);
// };

// encrypt_aes::encrypt_aes(/* args */)
// {
// }

// encrypt_aes::~encrypt_aes()
// {
// }

// string encrypt_aes::encryptAES(const string &message, const unsigned char *key)
// {
//     const int AES_BLOCK_SIZE_BYTES = AES_BLOCK_SIZE;
//     const int messageLen = message.length();
//     unsigned char iv[AES_BLOCK_SIZE_BYTES];
//     unsigned char ciphertext[messageLen + AES_BLOCK_SIZE_BYTES];
//     RAND_bytes(iv, AES_BLOCK_SIZE_BYTES); // Generate random IV
//     AES_KEY aesKey;
//     if (AES_set_encrypt_key(key, 256, &aesKey) != 0)
//     { // Set AES key
//         cout << "Error setting AES key" << endl;
//         return "";
//     }
//     AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(message.c_str()), ciphertext, messageLen, &aesKey, iv, AES_ENCRYPT); // Encrypt message using AES-256-CBC
//     memcpy(ciphertext + messageLen, iv, AES_BLOCK_SIZE_BYTES);                                                                   // Append IV to ciphertext
//     return string(reinterpret_cast<const char *>(ciphertext), AES_BLOCK_SIZE_BYTES + messageLen);
// }

// // Function to decrypt message using AES-256-CBC
// string encrypt_aes::decryptAES(const string &ciphertext, const unsigned char *key)
// {
//     const int AES_BLOCK_SIZE_BYTES = AES_BLOCK_SIZE;
//     const int ciphertextLen = ciphertext.length();
//     const unsigned char *iv = reinterpret_cast<const unsigned char *>(ciphertext.c_str()) + ciphertextLen - AES_BLOCK_SIZE_BYTES;
//     unsigned char decryptedtext[ciphertextLen - AES_BLOCK_SIZE_BYTES];
//     AES_KEY aesKey;
//     if (AES_set_decrypt_key(key, 256, &aesKey) != 0)
//     { // Set AES key
//         cout << "Error setting AES key" << endl;
//         return "";
//     }
//     AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(ciphertext.c_str()), decryptedtext, ciphertextLen - AES_BLOCK_SIZE_BYTES, &aesKey, iv, AES_DECRYPT); // Decrypt message using AES-256-CBC
//     return string(reinterpret_cast<const char *>(decryptedtext), ciphertextLen - AES_BLOCK_SIZE_BYTES);
// }

#endif