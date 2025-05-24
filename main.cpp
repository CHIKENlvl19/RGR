#include <iostream>
#include <string>
#include "header.h"
#include "ElGamal/ElGamal_functions.h"

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
    
    crypt(cipherChoice(), fileName);

    return 0;
}