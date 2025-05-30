#pragma once

#include <string>

using namespace std;

string randomKeyGenerator(string& key, const size_t& keyLength, bool isShowingKeys, ofstream& out);
string ViginerEncrypt(const string& plaintext, const string& key, ofstream& out);
extern "C" void Viginer_run (const char* fileName, int isShowingKeys);