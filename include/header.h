#pragma once

using namespace std;

void showHelp();
void showVersion();
void startUninstall();
void Greeting();
void filesQuestion();
void PasswordCheck();
string FileMethods();
string PreparedFiles();
string createUserFiles();
int cipherChoice();
void crypt(int cipherChoice, string fileName, bool showKeys);
bool isPrintingKeys();
void resultsOutput();