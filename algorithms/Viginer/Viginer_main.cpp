#include <iostream>
#include <fstream>
#include <random>
#include "../include/Viginer/Viginer_functions.h"

using namespace std;

string randomKeyGenerator(string& key, const size_t& keyLength, bool isShowingKeys, ofstream& out) {
    mt19937 mt(random_device{}());
    uniform_int_distribution<int> dist(0, 255);

    for (size_t i = 0; i < keyLength; i++)
    {
        key += static_cast<char>(dist(mt));
    }

    if (isShowingKeys)
    {
        out << "Автоматически сгенерированный ключ: ";
        for (char c : key)
        {
            out << hex << (int)(unsigned char)c;
        }
        out << endl;
    }

    return key;
}

string ViginerEncrypt(const string& plaintext, const string& key, ofstream& out) {

    out << "Шифруемый текст: " << plaintext << endl;

    string cipherText;
    size_t keyLength = key.length();

    for (size_t i = 0; i < plaintext.length(); i++)
    {
        char textChar = plaintext[i];
        char keyChar = key[i % keyLength];

        char cipherChar = static_cast<char>( static_cast<unsigned char>(textChar) + static_cast<unsigned char>(keyChar) % 256 );
        cipherText += cipherChar;
    }

    out << "Зашифрованный текст: " << endl;
    for (char c : cipherText)
    {
        out << hex << (int)(unsigned char)c;
    }
    out << endl;

    return cipherText;
}

void ViginerDecrypt(const string& cipherText, const string& key, ofstream& out) {
    string plainText;
    size_t keyLength = key.length();

    for (size_t i = 0; i < cipherText.length(); i++)
    {
        char cipherChar = cipherText[i];
        char keyChar = key[i % keyLength];

        char decryptedChar = static_cast<char>( static_cast<unsigned char>(cipherChar) - static_cast<unsigned char>(keyChar) % 256 );
        plainText += decryptedChar;
    }

    out << "Дешифрованный текст: \n";
    out << plainText;
}

extern "C" void Viginer_run (const char* fileName, int isShowingKeys) {

    try {
        ifstream inFile(fileName);
        ofstream outFile("output.txt");

        string plaintext;
        inFile.seekg(0, ios::end);

        size_t size = inFile.tellg();
        inFile.seekg(0, ios::beg);

        if (size > 0) {
            plaintext.assign(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
        } else {
            cerr << "Входной файл пуст: " << fileName << endl;
        }

        inFile.close();

        string key;
        cout << "Желаете, чтобы ключ был сгенерирован автомтически? (да/нет): ";
        while (true)
        {
            string choice;
            cin >> choice;

            if (choice == "да" || choice == "Да" || choice == "ДА")
            {
                size_t keyLength;
                cout << "Какой длины желаете ключ? Введите количество желаемых символов ключа: ";
                cin >> keyLength;
                randomKeyGenerator(key, keyLength, isShowingKeys, outFile);
                break;
            }
            else if (choice == "нет" || choice == "Нет" || choice == "НЕТ")
            {
                cout << "Введите ключ: ";
                cin >> key;
                break;
            }
            else
            {
                cerr << "Некорректный ввод, попробуйте еще раз.\n";
            }
        }

        string chiperText = ViginerEncrypt(plaintext, key, outFile);

        string decryptedText;
        ViginerDecrypt(chiperText, key, outFile);

        outFile.close();
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << endl;
    }
}