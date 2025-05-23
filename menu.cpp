#include <iostream>
#include <sstream>
#include <cctype>
#include <string>
#include <locale>
#include <limits>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include "header.h"

using namespace std;

void PasswordCheck() {
    cout << "Для начала работы с приложением введите пароль.\n";
    int password = 2303;
    int passwordInput;

    int attempts = 0;
    while (attempts < 10)
    {
        cout << "Пароль: ";
        cin >> passwordInput;
        if (password == passwordInput)
        {
            cout << "Пароль верный. Доступ разрешен.\n\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка буфера
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
    cout << "Добро пожаловать в программу шифрования и дешифровки текста!\nВведите, пожалуйста, текстом, метод ввода: \n\n";
}

string ChoiceInput() {
    string input;
    locale::global(locale("ru_RU.UTF-8"));

    cout << "Доступные методы ввода: \n";
    cout << "• С клавиатуры\n";
    cout << "• Из файла\n";

    while (true) {

        if (!getline(cin, input))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nНекорректный ввод. Пожалуйста, повторите попытку.\n";
            continue;
        }

        if (input.empty())
        {
            cout << "Пустой ввод не допускатеся. Пожалуйста, выберите метод ввода.\n";
            continue;
        }

        icu::UnicodeString unicodeStr = icu::UnicodeString::fromUTF8(input);
        unicodeStr.toLower(icu::Locale("ru_RU")); // Явное указание русской локали
        
        string lowerInput;
        unicodeStr.toUTF8String(lowerInput);

        if (lowerInput == "с клавиатуры" || lowerInput == "клавиатура") {
            return "keyboard";
        } else if (lowerInput == "из файла" || lowerInput == "файл") {
            return "file";
        } else {
            cout << "\nНекорректный ввод. Пожалуйста, выберите метод ввода из предложенных.\n";
        }
    }
}
