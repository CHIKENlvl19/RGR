#pragma once

#include <string>
#include <fstream>
#include <random>
#include <gmpxx.h>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

char* ElGamal_PasswordDecrypt(int p, int x, const char* ciphertext);
void ElGamal_run(const char* fileName, int isShowingKeys);

#ifdef __cplusplus
}
#endif

tuple<mpz_class, mpz_class, mpz_class, mpz_class> KeysGenerator(bool isShowingKeys, gmp_randstate_t state);