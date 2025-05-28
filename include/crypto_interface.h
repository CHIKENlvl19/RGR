#ifndef CRYPTO_INTERFACE_H
#define CRYPTO_INTERFACE_H

#ifdef __cplusplus

extern "C" {
#endif

typedef void (*AlgorithmFunc)(const char* fileName, int isShowingKeys);
typedef char* (*PasswordDecryptFunc)(int p, int x, const char* cipherText);

#ifdef __cplusplus
}
#endif

#endif