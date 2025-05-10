#include <iostream>
#include <sstream>
#include <cctype>
#include <string>
#include <locale>
#include <numeric>
#include "header.h"

using namespace std;

// обрез пробелов в начале и конце строки 
string trim (const string& input) {
    size_t first = input.find_first_not_of(' ');
    if (first == string::npos)
    {
        return "";
    }
    size_t last = input.find_first_not_of(' ');

    return input.substr(first, (last - first + 1));
}

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
}

string ChoiceInput() {
    string input;

    do {
        cout << "Доступные методы ввода: \n";
        cout << "• С клавиатуры\n";
        cout << "• Из файла\n";

        if (!getline(cin, input))
        {
            cin.clear();
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nНекорректный ввод. Пожалуйста, повторите попытку.\n";
            continue;
        }

        input = trim(input);

        if (input.empty())
        {
            cout << "Пустой ввод не допускатеся. Пожалуйста, выберите метод ввода.\n";
            continue;
        }

        locale loc("ru_RU.UTF-8");
        string lower_case_input;

        for (char c : input)
        {
            lower_case_input += tolower(c, loc);
        }

        if (lower_case_input == "с клавиатуры"  || lower_case_input == "клавиатура")
        {
            return "keyboard";
        } else if (lower_case_input == "из файла" || lower_case_input == "файл")
        {
            return "file";
        } else
        {
            cout << "\nНекорректный ввод. Пожалуйста, выберите метод ввода из предложенных.\n";
        }
    } while (true);
}

