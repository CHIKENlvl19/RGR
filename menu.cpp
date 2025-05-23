#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <string>
#include <locale>
#include <limits>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <stdexcept>
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
            cin.ignore(1000, '\n');
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
            cin.ignore(1000, '\n');
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

        if (lowerInput == "с клавиатуры" || lowerInput == "клавиатура") 
        {
            return "keyboard";
        } 
        else if (lowerInput == "из файла" || lowerInput == "файл") 
        {
            return "file";
        } 
        else 
        {
            cout << "\nНекорректный ввод. Пожалуйста, выберите метод ввода из предложенных.\n";
        }
    }
}

string FileMethods() {
    string answer;

    while (true)
    {
        if(!getline(cin, answer))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nНекорректный ввод. Пожалуйста, повторите попытку.\n";
            continue;
        }


        if (answer.empty())
        {
            cout << "Ввод не должен быть пустым. Пожалуйста, повторите попытку.\n";
            continue;
        }

        return answer;
    }
}

void PreparedFiles() {
    cout << "\nФайл какого размера Вы бы хотели использовать?\n";
    cout << "1. 1.000 символов - отрывок из романа в стихах \"Евгений Онегин\", Александр Сергеевич Пушкин\n";
    cout << "2. 10.000 символов - отрывок из книги \"ХАКИНГ Искусство эксплойта Второе издание\", Джон Эриксон\n";
    cout << "3. 100.000 символов - отрывок из романа-эпопеи \"Война и мир\", Лев Николаевич Толстой\n";

    int choice;
    while (true)
    {
        cin >> choice;
        if (choice == 1)
        {

        }
        else if (choice == 2)
        {

        }
        else if (choice == 3)
        {

        }
        else
        {
            cout << "Неверный формат ввода. Введите номер заготовленного файла.\n";
        }
    }
}

void createUserFiles() {
    try {
        string filename;
        //string text;

        cout << "\nВведите имя файла (например, myfile.txt): ";
        getline(cin, filename);

        // cоздание и открытие файла для записи
        ofstream outFile(filename);

        cout << "\nВведите текст для записи (для завершения введите пустую строку):\n";

        while (true)
        {
            string line;
            getline(cin, line);

            if (line.empty())
            {
                break;
            }

            outFile << line << endl;
        }

        outFile.close();

        cout << "\nФайл успешно создан, данные записаны!\n";
    }
    catch (const ios_base::failure& e)
    {
        cerr << "\nОшибка ввода\\вывода: " << e.what()
             << "Код ошибки: " << e.code() << endl;
        throw;
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << endl;
        throw;
    }
    
}