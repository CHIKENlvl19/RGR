#pragma once

#include <string>

using namespace std;

string randomKeyGenerator(const size_t& keyLength, bool isShowingKeys);
string ViginerEncrypt(const string& plaintext, const string& key, ofstream& out);
string ViginerDecrypt(const string& cipherText, const string& key);
extern "C" void Viginer_run (const char* fileName, int isShowingKeys);