#pragma once

#include <string>

using namespace std;

namespace Colors {
    const string ERROR = "\033[1;31m";
    const string WARNING = "\033[1;33m";
    const string SUCCESS = "\033[1;32m";
    const string IMPORTANT = "\033[1;37m";
    const string MESSAGE = "\033[42m";

    const string RESET = "\033[0m";
}

#ifdef __cplusplus
extern "C" {
#endif

extern "C" void Viginer_run (const char* fileName, int isShowingKeys);

#ifdef __cplusplus
}
#endif

/*string randomKeyGenerator(const size_t& keyLength, bool isShowingKeys);
string ViginerEncrypt(const string& plaintext, const string& key);
string ViginerDecrypt(const string& cipherText, const string& key);*/