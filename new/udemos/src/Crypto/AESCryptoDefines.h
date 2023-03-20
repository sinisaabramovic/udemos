//
//  AESCryptoDefines.h
//  udemos
//
//  Created by Sinisa Abramovic on 20.03.2023..
//

#ifndef AESCryptoDefines_h
#define AESCryptoDefines_h

#include <unistd.h>

constexpr uint32_t KEY_LEN = 32U;
constexpr uint32_t IV_LEN = 32U;
constexpr uint32_t SALT_LEN = 16U;

// Scrypt settings that makes key derivation consume 1 GB of memory
// and takes about 3 seconds on mac M1 PRO.
// N=1048576, r=8, p=1 (RAM = 1 GB)
//

constexpr uint32_t SCRYPT_N = 1048576U;
constexpr uint32_t SCRYPT_r = 8U;
constexpr uint32_t SCRYPT_p = 1U;

#endif /* AESCryptoDefines_h */
