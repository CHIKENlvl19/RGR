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
#include "ElGamal/ElGamal_functions.h"

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
    cout << "Добро пожаловать в программу шифрования и дешифровки текста Cryptographer!\n\n";
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

string PreparedFiles() {
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
            return "1k_symbols.txt";
        }
        else if (choice == 2)
        {
            return "10k_symbols.txt";
        }
        else if (choice == 3)
        {
            return "100k_symbols.txt";
        }
        else
        {
            cout << "Неверный формат ввода. Введите номер заготовленного файла.\n";
        }
    }
}

string createUserFiles() {
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
        
        return filename;
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

int cipherChoice() {
    cout << "\nКакой метод шифрования вы хотите использовать?\n";
    cout << "1. Шифр Вижинера.\n";
    cout << "2. Шифр Вернама.\n";
    cout << "3. Шифр Эль-Гамаля.\n";
    
    int choice;
    while (true)
    {
        cin >> choice;
        if (choice == 1)
        {
            return 1;
        }
        else if (choice == 2)
        {
            return 2;
        }
        else if (choice == 3)
        {
            return 3;
        }
        else
        {
            cout << "Неверный формат ввода. Введите номер шифра.\n";
        }
    }
}

void crypt(int cipherChoice, string fileName) {
    if (cipherChoice == 1)
    {
        
    }
    else if (cipherChoice == 2)
    {

    }
    else if (cipherChoice == 3)
    {
        ElGamal(fileName);
    }

}