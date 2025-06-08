#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <filesystem>
#include "../include/Vernam/Vernam_functions.h"

using namespace std;
using namespace Colors;

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
        string cipherText = vernamEncrypt(plaintext, key, encryptedOutFile, isShowingKeys);
        cout << "\nФайл успешно зашифрован!\n";
        
        ofstream decryptedOutFile("decrypted_" + baseName, ios::binary);
        if (!decryptedOutFile) {
            throw runtime_error("Ошибка открытия выходного файла для расшифрования.");
        }
        cout << "\nФайл дешифруется, подождите...\n";
        vernamDecrypt(cipherText, key, decryptedOutFile);
        cout << "\nФайл успешно дешифрован!\n";
        decryptedOutFile.close();

    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << endl;
    }
    
}
