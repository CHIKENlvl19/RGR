#pragma once

#include <iostream>
#include <fstream>

using namespace std;

namespace Colors {
    const string ERROR = "\033[1;31m";
    const string WARNING = "\033[1;33m";
    const string SUCCESS = "\033[1;32m";
    const string IMPORTANT = "\033[1;37m";
    const string MESSAGE = "\033[42m";

    const string RESET = "\033[0m";
}

string randomKeyGenerator(size_t length);
string vernamEncrypt (const string& plaintext, const string& key, ofstream& out, bool isShowingKeys);
void vernamDecrypt (const string& ciphertext, const string& key, ofstream& out);
extern "C" void Vernam_run(const char* fileName, int isShowingKeys);