#pragma once
#include <vector>
#include <string>

using namespace std;

bool isPrime(int p);
int gcd(int a, int b);
int aXmodP (int a, int x, int p);
int MulMod(int a, int b, int n);
vector<int> prime_factors(int n);
bool is_primitive_root(int g, int p);
tuple<int, int, int, int> KeysGenerator();
void ElGamalCrypt(int& p, int& g, int& x, int& y, const string& plaintext, string& ciphertext, ofstream& out, bool isPrinting);
void ElGamalDecrypt(int p, int x, const string& ciphertext, string& decryptedText, ofstream& out, bool isPrinting);
void ElGamal(string fileName, bool isPrinting);