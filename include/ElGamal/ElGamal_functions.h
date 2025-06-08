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

namespace Colors {
    const string ERROR = "\033[1;31m";
    const string WARNING = "\033[1;33m";
    const string SUCCESS = "\033[1;32m";
    const string IMPORTANT = "\033[1;37m";
    const string MESSAGE = "\033[42m";

    const string RESET = "\033[0m";
}

bool isPrime(int p);
int gcd(int a, int b);
int aXmodP (int a, int x, int p);
int MulMod(int a, int b, int n);
vector<int> prime_factors(int n);
bool is_primitive_root(int g, int p);
tuple<int, int, int, int> KeysGenerator();
void ElGamalCrypt(int& p, int& g, int& y, const string& plaintext, ofstream& out);
void ElGamalDecrypt(int p, int x, ifstream& in, string& decryptedText);
string ElGamalPasswordDecrypt(int p, int x, const string& ciphertext);