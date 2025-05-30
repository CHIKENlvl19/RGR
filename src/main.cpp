#include <iostream>
#include <string>
#include "header.h"

using namespace std;

int main() {
    PasswordCheck();
    Greeting();

    string fileName;

    filesQuestion();
    if(FileMethods() == "да")
    {
        fileName = PreparedFiles();
    }
    else
    {
        fileName = createUserFiles();
    }

    bool isShowingKeys = isPrintingKeys();
    crypt(cipherChoice(), fileName, isShowingKeys);

    resultsOutput();

    return 0;
}