#include <iostream>
#include <string>
#include "header.h"

using namespace std;

int main() {
    PasswordCheck();
    Greeting();

    string input_method = ChoiceInput();
    if (input_method == "keyboard")
    {
        cout << "Вы выбрали ввод с клавиатуры.\n";
    }
    else if (input_method == "file")
    {
        cout << "Вы выбрали ввод из файла.\n";
    }

    return 0;
}