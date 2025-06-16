#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <filesystem>
#include <chrono>
#include "../include/Vernam/Vernam_functions.h"

using namespace std;
using namespace Colors;
using namespace std::chrono;
namespace fs = filesystem;

string randomKeyGenerator(size_t length) {
    string key;
    mt19937 mt(random_device{}());
    uniform_int_distribution<int> dist(0, 255);

    for (size_t i = 0; i < length; i++)
    {
        key += static_cast<char>(dist(mt));
    }

    return key;
}

string vernamEncrypt (const string& plaintext, const string& key, ofstream& out, bool isShowingKeys) {
    
    if (isShowingKeys)
    {
        cout << "Случайно сгенерированный ключ: ";
    
        for (char c : key)
        {
            cout << hex << (int)(unsigned char)c;
        }
        cout << endl;
    }

    string ciphertext;
    for (size_t i = 0; i < plaintext.length(); i++)
    {
        char encryptedChar = plaintext[i] ^ key[i];
        ciphertext += encryptedChar;
    }

    for (char c : ciphertext)
    {
        out << hex << (int)(unsigned char)c;
    }
    out << endl;

    return ciphertext;
}

void vernamDecrypt (const string& ciphertext, const string& key, ofstream& out) {
    string plaintext;

    for (size_t i = 0; i < ciphertext.length(); i++)
    {
        char decryptedChar = ciphertext[i] ^ key[i];
        plaintext += decryptedChar;
    }

    out << plaintext;
}

string humanReadableSize(uintmax_t bytes) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    int unit = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024 && unit < 4)
    {
        size /= 1024;
        ++unit;
    }

    string result;
    if (unit == 0 || size >= 100)
    {
        result = to_string(static_cast<int>(size));
    } else {
        ostringstream oss;
        oss << fixed << setprecision(1) << size;
        result = oss.str();
    }
    
    return result + " " + units[unit];
}

extern "C" void Vernam_run(const char* fileName, int isShowingKeys) {
    try {
        ifstream inFile(fileName, ios::binary);
        if(!inFile)
        {
            cerr << ERROR << "Ошибка открытия входного файла: " << RESET << fileName << endl;
        }

        string plaintext;
        inFile.seekg(0, ios::end);

        size_t size = inFile.tellg();
        inFile.seekg(0, ios::beg);

        if (size > 0)
        {
            plaintext.assign(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
        } else {
            throw runtime_error("Входной файл пуст.");
        }
        inFile.close();

        const string key = randomKeyGenerator(plaintext.length());

        filesystem::path inputPath(fileName);
        string baseName = inputPath.filename().string();

        ofstream encryptedOutFile("encrypted_" + baseName, ios::binary);
        if (!encryptedOutFile) {
            throw runtime_error("Ошибка открытия выходного файла для шифрования.");
        }
        cout << "\nФайл шифруется, подождите...\n";
        auto startEncrypt = high_resolution_clock::now();
        string cipherText = vernamEncrypt(plaintext, key, encryptedOutFile, isShowingKeys);
        auto endEncrypt = high_resolution_clock::now();
        cout << "\nФайл успешно зашифрован!\n";

        auto encryptDuration = duration_cast<milliseconds>(endEncrypt - startEncrypt);
        auto encryptMinutes = duration_cast<minutes>(encryptDuration);
        auto encryptSeconds = duration_cast<seconds>(encryptDuration - encryptMinutes);
        auto encryptMilliseconds = duration_cast<milliseconds>(encryptDuration - encryptMinutes - encryptSeconds);

        cout << "Время шифрования: " << encryptMinutes.count() << " мин " << encryptSeconds.count() << " сек " << encryptMilliseconds.count() << " мс" << endl;

        ofstream decryptedOutFile("decrypted_" + baseName, ios::binary);
        if (!decryptedOutFile) {
            throw runtime_error("Ошибка открытия выходного файла для расшифрования.");
        }
        cout << "\nФайл дешифруется, подождите...\n";
        auto startDecrypt = high_resolution_clock::now();
        vernamDecrypt(cipherText, key, decryptedOutFile);
        auto endDecrypt = high_resolution_clock::now();
        cout << "\nФайл успешно дешифрован!\n";
        decryptedOutFile.close();

        auto decryptDuration = duration_cast<milliseconds>(endDecrypt - startDecrypt);
        auto decryptMinutes = duration_cast<minutes>(decryptDuration);
        auto decryptSeconds = duration_cast<seconds>(decryptDuration - decryptMinutes);
        auto decryptMilliseconds = duration_cast<milliseconds>(decryptDuration - decryptMinutes - decryptSeconds);

        cout << "Время дешифрования: " << decryptMinutes.count() << " мин " << decryptSeconds.count() << " сек " << decryptMilliseconds.count() << " мс" << endl;

        uintmax_t originalSize = fs::file_size(inputPath);
        uintmax_t encryptedSize = fs::file_size("encrypted_" + baseName);

        cout << IMPORTANT << "\nРазмер файла до шифрования: " << humanReadableSize(originalSize) << RESET << endl;
        cout << IMPORTANT << "Размер зашифрованного файла: " << humanReadableSize(encryptedSize) << RESET << endl;

    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << endl;
    }
    
}
