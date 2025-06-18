#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <vector>
#include "../include/Viginer/Viginer_functions.h"

using namespace std;
using namespace Colors;
namespace fs = filesystem;
using namespace chrono;

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
            cout << hex << setw(2) << setfill('0') << (int)(unsigned char)c;
        }
        cout << endl;
    }

    return key;
}

void ViginerEncrypt(istream& in, ostream& out, const string& key) {
    const size_t keyLength = key.length();
    size_t keyIndex = 0;
    const size_t bufferSize = 4096; // 4KB блок
    vector<char> buffer(bufferSize);
    
    while (in) {
        in.read(buffer.data(), bufferSize);
        const size_t bytesRead = in.gcount();
        
        for (size_t i = 0; i < bytesRead; ++i) {
            unsigned char plainChar = static_cast<unsigned char>(buffer[i]);
            unsigned char keyChar = static_cast<unsigned char>(key[keyIndex]);
            buffer[i] = static_cast<char>((plainChar + keyChar) % 256);
            keyIndex = (keyIndex + 1) % keyLength;
        }
        
        out.write(buffer.data(), bytesRead);
    }
}

void ViginerDecrypt(istream& in, ostream& out, const string& key) {
    const size_t keyLength = key.length();
    size_t keyIndex = 0;
    const size_t bufferSize = 4096; // 4KB блок
    vector<char> buffer(bufferSize);
    
    while (in) {
        in.read(buffer.data(), bufferSize);
        const size_t bytesRead = in.gcount();
        
        for (size_t i = 0; i < bytesRead; ++i) {
            unsigned char cipherChar = static_cast<unsigned char>(buffer[i]);
            unsigned char keyChar = static_cast<unsigned char>(key[keyIndex]);
            buffer[i] = static_cast<char>((cipherChar - keyChar + 256) % 256);
            keyIndex = (keyIndex + 1) % keyLength;
        }
        
        out.write(buffer.data(), bytesRead);
    }
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

extern "C" void Viginer_run (const char* fileName, int isShowingKeys) {
    try {
        fs::path inputPath(fileName);
        string baseName = inputPath.filename().string();

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

        cout << WARNING << "\nФайл шифруется, подождите..." << RESET << endl;
        
        ifstream inFile(fileName, ios::binary);
        if (!inFile) 
        {
            throw runtime_error("Ошибка открытия входного файла");
        }
        
        string encryptedFileName = "encrypted_" + baseName;
        ofstream encryptedOutFile(encryptedFileName, ios::binary);
        if (!encryptedOutFile) 
        {
            throw runtime_error("Ошибка открытия выходного файла для шифрования.");
        }
        
        auto startEncrypt = high_resolution_clock::now();
        ViginerEncrypt(inFile, encryptedOutFile, key);
        auto endEncrypt = high_resolution_clock::now();
        
        inFile.close();
        encryptedOutFile.close();

        cout << SUCCESS << "Файл успешно зашифрован!" << RESET << endl;

        cout << WARNING << "\nФайл дешифруется, подождите..." << RESET << endl;
        
        ifstream encryptedInFile(encryptedFileName, ios::binary);
        if (!encryptedInFile) 
        {
            throw runtime_error("Ошибка открытия зашифрованного файла");
        }
        
        ofstream decryptedOutFile(baseName, ios::binary);
        if (!decryptedOutFile) 
        {
            throw runtime_error("Ошибка открытия выходного файла для расшифрования.");
        }
        
        auto startDecrypt = high_resolution_clock::now();
        ViginerDecrypt(encryptedInFile, decryptedOutFile, key);
        auto endDecrypt = high_resolution_clock::now();
        
        encryptedInFile.close();
        decryptedOutFile.close();
        
        cout << SUCCESS << "Файл успешно дешифрован!" << RESET << endl;

        auto encryptDuration = duration_cast<milliseconds>(endEncrypt - startEncrypt);
        auto encryptMinutes = duration_cast<minutes>(encryptDuration);
        auto encryptSeconds = duration_cast<seconds>(encryptDuration - encryptMinutes);
        auto encryptMilliseconds = encryptDuration - encryptMinutes - encryptSeconds;

        auto decryptDuration = duration_cast<milliseconds>(endDecrypt - startDecrypt);
        auto decryptMinutes = duration_cast<minutes>(decryptDuration);
        auto decryptSeconds = duration_cast<seconds>(decryptDuration - decryptMinutes);
        auto decryptMilliseconds = decryptDuration - decryptMinutes - decryptSeconds;

        cout << dec << "Время шифрования: " << encryptMinutes.count() << " мин "
             << encryptSeconds.count() << " сек "
             << encryptMilliseconds.count() << " мс" << endl;

        cout << dec << "Время дешифрования: " << decryptMinutes.count() << " мин "
             << decryptSeconds.count() << " сек "
             << decryptMilliseconds.count() << " мс" << endl;

        uintmax_t originalSize = fs::file_size(fileName);
        uintmax_t encryptedSize = fs::file_size(encryptedFileName);

        cout << IMPORTANT << "\nРазмер файла до шифрования: " 
             << humanReadableSize(originalSize) << RESET << endl;
        cout << IMPORTANT << "Размер зашифрованного файла: " 
             << humanReadableSize(encryptedSize) << RESET << endl;

    } catch (const exception& e) {
        cerr << ERROR << "Ошибка: " << RESET << e.what() << endl;
    }
}