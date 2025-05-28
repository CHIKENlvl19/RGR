#include <iostream>
#include <string>
#include "header.h"

using namespace std;

int main() {
    PasswordCheck();
    Greeting();

    string fileName;

    cout << "\nИспользовать готовый файл? (да/нет)\nПри выборе ответа \"нет\" будет создан новый файл.\n";
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

    return 0;
}