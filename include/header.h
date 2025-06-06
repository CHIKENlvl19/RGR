#pragma once

using namespace std;

enum class cipherAlgorithm {
    Viginer = 1,
    Vernam,
    ElGamal
};

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