#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <chrono>
#include "../include/Viginer/Viginer_functions.h"

using namespace std;
using namespace Colors;
namespace fs = std::filesystem;
using namespace std::chrono;

string randomKeyGenerator(const size_t& keyLength, bool isShowingKeys) {
    string key;
    mt19937 mt(random_device{}());
    uniform_int_distribution<int> dist(0, 255);

    for (size_t i = 0; i < keyLength; i++) 
    {
        key += static_cast<char>(dist(mt));
    }

    if (isShowingKeys)
    {
        cout << "Автоматически сгенерированный ключ: ";
        for (char c : key) 
        {
            cout << hex << (int)(unsigned char)c;
        }
        cout << endl;
    }

    return key;
}

string ViginerEncrypt(const string& plainText, const string& key) {
    string cipherText;
    size_t keyLength = key.length();

    for (size_t i = 0; i < plainText.length(); i++) 
    {
        char textChar = plainText[i];
        char keyChar = key[i % keyLength];

        char cipherChar = static_cast<char>((static_cast<unsigned char>(textChar) + static_cast<unsigned char>(keyChar)) % 256);
        cipherText += cipherChar;
    }

    return cipherText;
}

string ViginerDecrypt(const string& cipherText, const string& key) {
    string plainText;
    size_t keyLength = key.length();

    for (size_t i = 0; i < cipherText.length(); i++) 
    {
        char cipherChar = cipherText[i];
        char keyChar = key[i % keyLength];

        char decryptedChar = static_cast<char>((static_cast<unsigned char>(cipherChar) - static_cast<unsigned char>(keyChar) + 256) % 256);
        plainText += decryptedChar;
    }

    return plainText;
}

extern "C" void Viginer_run (const char* fileName, int isShowingKeys) {
    try {

        ifstream inFile(fileName, ios::binary);
        if (!inFile) 
        {
            throw runtime_error("Ошибка открытия входного файла");
        }

        string plainText((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        string key;
        cout << "Желаете, чтобы ключ был сгенерирован автоматически? (да/нет): ";
        string choice;
        cin >> choice;

        if (choice == "да" || choice == "Да" || choice == "ДА") 
        {
            size_t keyLength;
            cout << "Какой длины желаете ключ? Введите количество желаемых символов ключа: ";
            cin >> keyLength;
            key = randomKeyGenerator(keyLength, isShowingKeys);
        } else {
            cout << "Введите ключ: ";
            cin >> key;
        }

        fs::path inputPath(fileName);
        string baseName = inputPath.filename().string();

        cout << WARNING << "\nФайл шифруется, подождите..." << RESET << endl;
        auto startEncrypt = high_resolution_clock::now();
        string cipherText = ViginerEncrypt(plainText, key);
        auto endEncrypt = high_resolution_clock::now();

        ofstream encryptedOutFile("encrypted_" + baseName, ios::binary);
        if (!encryptedOutFile) 
        {
            throw runtime_error ("Ошибка открытия выходного файла для шифрования.");
        }
        encryptedOutFile.write(cipherText.data(), cipherText.size());
        encryptedOutFile.close();
        cout << SUCCESS << "Файл успешно зашифрован!" << RESET << endl;

        auto encryptDuration = duration_cast<milliseconds>(endEncrypt - startEncrypt);
        auto encryptMinutes = duration_cast<minutes>(encryptDuration);
        auto encryptSeconds = duration_cast<seconds>(encryptDuration - encryptMinutes);
        auto encryptMilliseconds = duration_cast<milliseconds>(encryptDuration - encryptMinutes - encryptSeconds);

        cout << "Время шифрования: " << encryptMinutes.count() << " мин " << encryptSeconds.count() << " сек " << encryptMilliseconds.count() << " мс" << endl;

        cout << WARNING << "\nФайл дешифруется, подождите..." << RESET << endl;
        auto startDecrypt = high_resolution_clock::now();
        string decryptedText = ViginerDecrypt(cipherText, key);
        auto endDecrypt = high_resolution_clock::now();

        ofstream decryptedOutFile(baseName, ios::binary);
        if (!decryptedOutFile) 
        {
            throw runtime_error ("Ошибка открытия выходного файла для расшифрования.");
        }
        decryptedOutFile.write(decryptedText.data(), decryptedText.size());
        decryptedOutFile.close();
        cout << SUCCESS << "Файл успешно дешифрован!" << RESET << endl;

        auto decryptDuration = duration_cast<milliseconds>(endDecrypt - startDecrypt);
        auto decryptMinutes = duration_cast<minutes>(decryptDuration);
        auto decryptSeconds = duration_cast<seconds>(decryptDuration - decryptMinutes);
        auto decryptMilliseconds = duration_cast<milliseconds>(decryptDuration - decryptMinutes - decryptSeconds);

        cout << "Время дешифрования: " << decryptMinutes.count() << " мин " << decryptSeconds.count() << " сек " << decryptMilliseconds.count() << " мс" << endl;

    } catch (const exception& e) {
        cerr << ERROR<< "Ошибка: " << RESET << e.what() << endl;
    }
}
