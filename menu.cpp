#include <iostream>
#include <sstream>
#include "header.h"

using namespace std;

void PasswordCheck() {
    cout << "Для начала работы с приложением введите пароль.\n";
    int password = 2303;
    int password_input;

    int attempts = 0;
    while (attempts < 10)
    {
        cout << "Пароль: ";
        cin >> password_input;
        if (password == password_input)
        {
            cout << "Пароль верный. Доступ разрешен.\n\n";
            break;
        }
        else
        {
            cout << "Пароль неверный, осталось попыток: " << 10 - attempts - 1 << endl;
            attempts++;
        }

        if (attempts == 10)
        {
            cout << "Превышен лимит попыток. Доступ запрещен.\n";
            exit(0);
        }
    }

}

void Greeting() {
    cout << "Добро пожаловать в программу шифрования и дешифровки текста! Введите, пожалуйста, текстом, метод ввода: \n\n";
    cout << "Доступные методы ввода: \n";
    cout << "• С клавиатуры\n";
    cout << "• Из файла\n";


}

