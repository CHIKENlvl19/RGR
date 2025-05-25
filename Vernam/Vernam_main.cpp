#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;

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

void vernamEncrypt (const string& plaintext, const string& key, ofstream& out) {
    out << "Случайно сгенерированный ключ: " << key << endl;
    out << "Шифруемый текст: " << plaintext << endl;

    string ciphertext;

    for (size_t i = 0; i < plaintext.length(); i++)
    {
        char encryptedChar = plaintext[i] ^ key[i];
        ciphertext += encryptedChar;
    }

    out << "Зашифрованный текст: " << endl;
    out << ciphertext;
}

void vernamDecrypt (const string& ciphertext, const string& key, ofstream& out) {
    string plaintext;

    for (size_t i = 0; i < ciphertext.length(); i++)
    {
        char decryptedChar = ciphertext[i] ^ key[i];
        plaintext += decryptedChar;
    }

    out << "Дешифрованный текст: " << endl;
    out << plaintext;
}

void Vernam(string fileName) {

    ifstream inFile(fileName);
    ofstream outFile("output.txt");

    string plaintext;
    inFile.seekg(0, ios::end);

    size_t size = inFile.tellg();
    inFile.seekg(0, ios::beg);

    if (size > 0)
    {
        plaintext.assign(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
    } else {
        cerr << "Входной файл пуст: " << fileName;
    }
    inFile.close();

    const string key = randomKeyGenerator(plaintext.length());
    string cipherText;
    vernamEncrypt(plaintext, key, outFile);

    string decryptedText;
    vernamDecrypt(cipherText, key, outFile);

    outFile.close();
}