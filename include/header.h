#pragma once

using namespace std;

enum class cipherAlgorithm {
    Viginer = 1,
    Vernam,
    ElGamal
};

namespace Colors {
    const string ERROR = "\033[1;31m";
    const string WARNING = "\033[1;33m";
    const string SUCCESS = "\033[1;32m";
    const string IMPORTANT = "\033[1;37m";
    
    const string RESET = "\033[0m";
}

void showHelp();
void showVersion();
void startUninstall();
void Greeting();
void filesQuestion();
void PasswordCheck();
void oneMoreFile();
string FileMethods();
string PreparedFiles();
string createUserFiles();
cipherAlgorithm cipherChoice();
void crypt(cipherAlgorithm cipherChoice, const string& fileName, bool showKeys);
bool isPrintingKeys();
string userFiles();