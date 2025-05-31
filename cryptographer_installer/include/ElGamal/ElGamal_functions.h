#pragma once
#include <vector>
#include <string>
#include <tuple>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

char* ElGamal_PasswordDecrypt(int p, int x, const char* ciphertext);
void ElGamal_run(const char* fileName, int isShowingKeys);

#ifdef __cplusplus
}
#endif

bool isPrime(int p);
int gcd(int a, int b);
int aXmodP (int a, int x, int p);
int MulMod(int a, int b, int n);
vector<int> prime_factors(int n);
bool is_primitive_root(int g, int p);
tuple<int, int, int, int> KeysGenerator();
void ElGamalCrypt(int& p, int& g, int& x, int& y, const string& plaintext, string& ciphertext, ofstream& out, bool isShowingKeys);
void ElGamalDecrypt(int p, int x, const string& ciphertext, string& decryptedText, ofstream& out);