#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <string>
#include <map>
#include <locale>
#include <limits>
#include <stdexcept>
#include <dlfcn.h>
#include "header.h"
#include "crypto_interface.h"

using namespace std;

void showHelp() {
    cout << "Использование: cryptographer [ОПЦИЯ]...\n\n"
         << "Опции:\n"
         << "  -h, --help\t\tПоказать эту справку\n"
         << "  -v, --version\t\tПоказать версию программы\n"
         << "  --uninstall\t\tУдалить программу\n\n"
         << "Для работы с программой запустите без аргументов.\n";
}

void showVersion() {
    cout << "Cryptographer v1.0\n"
         << "Система криптографических преобразований\n"
         << "© 2025 НГТУ НЭТИ\n";
}

void startUninstall() {
    cout << "Для удаления программы требуются права администратора.\n"
         << "Выполните следующую команду:\n\n"
         << "sudo /usr/local/share/cryptographer/scripts/uninstall.sh\n\n"
         << "Или запустите: sudo cryptographer-uninstall\n";
}

void PasswordCheck() {
    cout << "Для начала работы с приложением введите пароль.\n";
    
    ifstream passwordFile("/usr/local/share/cryptographer/prepared_files/password.txt");
    string password; 
    getline(passwordFile, password);

    void* elgamalLib = dlopen("libelgamal.so", RTLD_LAZY);
    if (!elgamalLib) {
        cerr << "Ошибка загрузки библиотеки ElGamal: " << dlerror() << endl;
        exit(1);
    }
    
    typedef char* (*PasswordDecryptFunc)(int p, int x, const char* ciphertext);
    PasswordDecryptFunc decryptFunc = (PasswordDecryptFunc)dlsym(elgamalLib, "ElGamal_PasswordDecrypt");
    
    if (!decryptFunc) {
        cerr << "Ошибка загрузки функции расшифровки: " << dlerror() << endl;
        dlclose(elgamalLib);
        exit(1);
    }
    
    int attempts = 0;
    while (attempts < 10)
    {
        cout << "Пароль: ";
        string passwordInput;
        getline(cin, passwordInput);
        
        char* decryptedPassword = decryptFunc(4273, 1404, password.c_str());
        bool passwordMatch = (passwordInput == decryptedPassword);
        
        delete[] decryptedPassword;
        
        if (passwordMatch)
        {
            cout << "Пароль верный. Доступ разрешен. Нажмите Enter для продолжения.\n\n";
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
            cout << "\nПревышен лимит попыток. Доступ запрещен.\n";
            exit(0);
        }
    }
    
    dlclose(elgamalLib);
}

void Greeting() {
    cout << "Добро пожаловать в программу шифрования и дешифровки текста Cryptographer!\n\n";
}

void filesQuestion() {
    cout << "\nИспользовать готовый файл? (да/нет)\nПри выборе ответа \"нет\" будет создан новый файл.\n";
}

void oneMoreFile() {
    cout << "\nЖелаете зашифровать еще один файл? (да/нет)\n";

    try {
        while (true)
        {
            string choice;
            cin >> choice;

            if (choice == "да" || choice == "Да" || choice == "ДА")
            {
                break;
            }
            else if (choice == "нет" || choice == "Нет" || choice == "НЕТ")
            {
                cout << "\nРезультат работы программы можете увидеть в файле output.txt.\n";
                exit(0);
            }
            else
            {
                cerr << "\nНекорректный ввод, попробуйте еще раз.\n";
            }
        }
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << endl;
        exit(1);
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

string PreparedFiles() {
    cout << "\nФайл какого размера Вы бы хотели использовать?\n";
    cout << "1. 1.000 символов - отрывок из романа в стихах \"Евгений Онегин\", Александр Сергеевич Пушкин\n";
    cout << "2. 10.000 символов - отрывок из книги \"ХАКИНГ Искусство эксплойта Второе издание\", Джон Эриксон\n";
    cout << "3. 100.000 символов - отрывок из романа-эпопеи \"Война и мир\", Лев Николаевич Толстой\n";
    cout << "4. 180 символов - набор специальных символов.\n";

    int choice;
    while (true)
    {
        cin >> choice;
        switch (choice)
        {
            case 1:
                return "/usr/local/share/cryptographer/prepared_files/1k_symbols.txt";
            case 2:
                return "/usr/local/share/cryptographer/prepared_files/10k_symbols.txt";
            case 3:
                return "/usr/local/share/cryptographer/prepared_files/100k_symbols.txt";
            case 4:
                return "/usr/local/share/cryptographer/prepared_files/special_symbols.txt";
            default:
                cout << "Неверный формат ввода. Введите номер заготовленного файла.\n";
        }
    }
}

string createUserFiles() {
    try {
        string filename;

        cout << "\nВведите имя файла (например, myfile.txt): ";
        getline(cin, filename);

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

cipherAlgorithm cipherChoice() {
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
            return cipherAlgorithm::Viginer;
        }
        else if (choice == 2)
        {
            return cipherAlgorithm::Vernam;
        }
        else if (choice == 3)
        {
            return cipherAlgorithm::ElGamal;
        }
        else
        {
            cout << "Неверный формат ввода. Введите номер шифра (1-3).\n";
        }
    }
}

bool isPrintingKeys() {
    cout << "\nЖелаете видеть ключи, используемые для шифрования / дешифровки?\n";
    try {
        while (true)
        {
            string choice;
            cin >> choice;

            if (choice == "да" || choice == "Да" || choice == "ДА")
            {
                return true;
            }
            else if (choice == "нет" || choice == "Нет" || choice == "НЕТ")
            {
                return false;
            }
            else
            {
                cerr << "Некорректный ввод, попробуйте еще раз.\n";
            }
        }
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << endl;
        return false;
    }
}

void crypt(cipherAlgorithm cipherChoice, const string& fileName, bool showKeys) {
    void* libraryHandle = nullptr;
    const char* libraryName = nullptr;
    const char* algorithmFuncName = nullptr;
    
    map<cipherAlgorithm, pair<const char*, const char*>> algorithmMap = {
        {cipherAlgorithm::Viginer, {"libviginer.so", "Viginer_run"}},
        {cipherAlgorithm::Vernam, {"libvernam.so", "Vernam_run"}},
        {cipherAlgorithm::ElGamal, {"libelgamal.so", "ElGamal_run"}}
    };

    auto it = algorithmMap.find(cipherChoice);
    if (it != algorithmMap.end())
    {
        libraryName = it->second.first;
        algorithmFuncName = it->second.second;
    } else
    {
        cerr << "Неверный выбор алгоритма. Попробуйте еще раз.";
    }
    
    libraryHandle = dlopen(libraryName, RTLD_LAZY);
    if (!libraryHandle) {
        cerr << "Ошибка загрузки библиотеки " << libraryName << ": " << dlerror() << endl;
        return;
    }
    
    typedef void (*AlgorithmFunc)(const char* fileName, int isShowingKeys);
    AlgorithmFunc algorithmFunc = (AlgorithmFunc)dlsym(libraryHandle, algorithmFuncName);
    
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Ошибка загрузки функции " << algorithmFuncName << ": " << dlsym_error << endl;
        dlclose(libraryHandle);
        return;
    }
    
    algorithmFunc(fileName.c_str(), showKeys ? 1 : 0);
    
    dlclose(libraryHandle);
}

void resultsOutput() {

    cout << "\nЖелаете вывести результат работы программы в консоль? (да/нет): ";

    while (true)
    {
        string choice;
        cin >> choice;

        if (choice == "да" || choice == "Да" || choice == "ДА")
        {
            ifstream outputFile("output.txt");

            if (!outputFile)
            {
                cerr << "\nОшибка! Не удалость октрыть файл." << endl;
            }

            cout << endl;
            string line;
            while (getline(outputFile, line))
            {
                cout << line << endl;
            }

            outputFile.close();\

            break;
        }
        else if (choice == "нет" || choice == "Нет" || choice == "НЕТ")
        {
            cout << "\nРезультат работы программы можете увидеть в файле output.txt.\n";
            break;
        }
        else
        {
            cerr << "Некорректный ввод, попробуйте еще раз.\n";
        }
    }
}