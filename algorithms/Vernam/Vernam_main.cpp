#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <vector>
#include "../include/Vernam/Vernam_functions.h"

using namespace std;
using namespace Colors;
using namespace std::chrono;
namespace fs = filesystem;

string randomKeyGenerator(size_t length, bool isShowingKeys) {
    string key;
    mt19937 mt(random_device{}());
    uniform_int_distribution<int> dist(0, 255);

    for (size_t i = 0; i < length; i++) 
    {
        key += static_cast<char>(dist(mt));
    }

    if (isShowingKeys && length > 0) 
    {
        // Сохраняем оригинальные настройки потока
        ios_base::fmtflags original_flags = cout.flags();
        char original_fill = cout.fill();
        
        cout << "Первые 16 байт ключа: ";
        for (size_t i = 0; i < min(static_cast<size_t>(16), length); i++) 
        {
            cout << hex << setw(2) << setfill('0') << (int)(unsigned char)key[i];
        }
        cout << dec << endl; // Важно: сбрасываем в десятичный формат
        
        // Восстанавливаем настройки потока
        cout.flags(original_flags);
        cout.fill(original_fill);
    }

    return key;
}

void vernamEncryptStream(istream& in, ostream& out, const string& key) {
    const size_t keyLength = key.length();
    size_t keyIndex = 0;
    const size_t bufferSize = 4096; // 4KB блок
    vector<char> buffer(bufferSize);
    
    while (in) {
        in.read(buffer.data(), bufferSize);
        const size_t bytesRead = in.gcount();
        
        for (size_t i = 0; i < bytesRead; ++i) 
        {
            buffer[i] = buffer[i] ^ key[keyIndex];
            keyIndex = (keyIndex + 1) % keyLength;
        }
        
        out.write(buffer.data(), bytesRead);
    }
}

void vernamDecryptStream(istream& in, ostream& out, const string& key) {
    vernamEncryptStream(in, out, key);
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
        fs::path inputPath(fileName);
        string baseName = inputPath.filename().string();
        string encryptedFileName = "encrypted_" + baseName;
        string decryptedFileName = "decrypted_" + baseName;

        uintmax_t fileSize = fs::file_size(fileName);
        if (fileSize == 0) 
        {
            throw runtime_error("Входной файл пуст.");
        }

        const string key = randomKeyGenerator(fileSize, isShowingKeys);

        cout << WARNING << "\nФайл шифруется, подождите..." << RESET << endl;
        
        ifstream inFile(fileName, ios::binary);
        if (!inFile) 
        {
            throw runtime_error("Ошибка открытия входного файла");
        }
        
        ofstream encryptedOutFile(encryptedFileName, ios::binary);
        if (!encryptedOutFile) 
        {
            throw runtime_error("Ошибка открытия выходного файла для шифрования.");
        }
        
        auto startEncrypt = high_resolution_clock::now();
        vernamEncryptStream(inFile, encryptedOutFile, key);
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
        
        ofstream decryptedOutFile(decryptedFileName, ios::binary);
        if (!decryptedOutFile) 
        {
            throw runtime_error("Ошибка открытия выходного файла для расшифрования.");
        }
        
        auto startDecrypt = high_resolution_clock::now();
        vernamDecryptStream(encryptedInFile, decryptedOutFile, key);
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

        // Вывод информации (явно указываем десятичный формат)
        cout << dec << "Время шифрования: " << encryptMinutes.count() << " мин "
             << encryptSeconds.count() << " сек "
             << encryptMilliseconds.count() << " мс" << endl;

        cout << dec << "Время дешифрования: " << decryptMinutes.count() << " мин "
             << decryptSeconds.count() << " сек "
             << decryptMilliseconds.count() << " мс" << endl;

        uintmax_t originalSize = fileSize;
        uintmax_t encryptedSize = fs::file_size(encryptedFileName);

        cout << IMPORTANT << "\nРазмер файла до шифрования: " 
             << humanReadableSize(originalSize) << RESET << endl;
        cout << IMPORTANT << "Размер зашифрованного файла: " 
             << humanReadableSize(encryptedSize) << RESET << endl;

    } catch (const exception& e) {
        cerr << ERROR << "Ошибка: " << RESET << e.what() << endl;
    }
}