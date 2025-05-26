#pragma once

#include <iostream>
#include <fstream>

using namespace std;

string randomKeyGenerator(size_t length);
string vernamEncrypt (const string& plaintext, const string& key, ofstream& out);
void vernamDecrypt (const string& ciphertext, const string& key, ofstream& out);
void Vernam(string fileName);