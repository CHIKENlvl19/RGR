#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <fstream>
#include <tuple>
#include <filesystem>
#include "../include/ElGamal/ElGamal_functions.h"

using namespace std;
using namespace Colors;

bool isPrime(int p) {
    if (p % 2 == 0 || p % 3 == 0 || p <= 1) 
    {
        return false;
    }

    int squareRootN = static_cast<int>(sqrt(p)) + 1;
    
    for (int i = 5; i * i <= squareRootN; i += 6) 
    {
        if (p % i == 0 || p % (i + 2) == 0)
        {
            return false;
        } 
    }

    return true;
}

int gcd(int a, int b) {

    while (b != 0) 
    {
        int r = b;
        b = a % b;
        a = r;
    }

    return a;
}

int aXmodP(int a, int x, int p) {
    int result = 1;
    a = a % p; // убедимся, что a меньше p

    while (x > 0) 
    {
        if (x % 2 == 1) 
        {
            result = (result * a) % p;
        }

        a = (a * a) % p;
        x /= 2;
    }

    return result;
}

int MulMod(int a, int b, int n) {
    return (a * b) % n;
}

vector<int> prime_factors(int n) {
    vector<int> factors;

    if (n % 2 == 0) 
    {
        factors.push_back(2);

        while (n % 2 == 0) 
        {
            n /= 2;
        }
    }
    for (int i = 3; i * i <= n; i += 2) 
    {
        if (n % i == 0) 
        {
            factors.push_back(i);

            while (n % i == 0) 
            {
                n /= i;
            }
        }
    }
    if (n > 2) 
    {
        factors.push_back(n);
    }

    return factors;
}

bool is_primitive_root(int g, int p) {
    if (g == 1 || gcd(g, p) != 1) 
    {
        return false;
    }
    int m = p - 1;

    vector<int> factors = prime_factors(m);
    for (int q : factors) 
    {
        if (aXmodP(g, m / q, p) == 1) 
        {
            return false;
        }
    }

    return true;
}

tuple<int, int, int, int> KeysGenerator(bool isShowingKeys) {
    int p, g, x, y;
    
    do {
        p = rand() % 9000 + 1000;
    } while (!isPrime(p));
    do {
        g = rand() % (p - 1) + 1;
    } while (!is_primitive_root(g, p));
    
    x = rand() % (p - 1) + 1;
    y = aXmodP(g, x, p);

    if (isShowingKeys)
    {
        cout << "\nОткрытый ключ (p, g, y) = (" << p << ", " << g << ", " << y << ")" << endl;
        cout << "Закрытый ключ x = " << x << endl;
    }   

    return make_tuple(p, g, x, y);
}

void ElGamalCrypt(int& p, int& g, int& y, const string& plaintext, ofstream& out) {
    vector<int> kValues(plaintext.size());

    for (size_t i = 0; i < plaintext.size(); ++i) 
    {
        kValues[i] = rand() % (p - 2) + 1;
    }

    for (size_t i = 0; i < plaintext.size(); ++i) 
    {
        int m = static_cast<int>(plaintext[i]);
        int k = kValues[i];

        int a = aXmodP(g, k, p);
        int b = MulMod(aXmodP(y, k, p), m, p);

        out.write(reinterpret_cast<char*>(&a), sizeof(a));
        out.write(reinterpret_cast<char*>(&b), sizeof(b));
    }
}

void ElGamalDecrypt(int p, int x, ifstream& in, string& decryptedText) {
    int a, b;

    while (in.read(reinterpret_cast<char*>(&a), sizeof(a)) && 
           in.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        int deM = MulMod(b, aXmodP(a, p - 1 - x, p), p);
        decryptedText += static_cast<char>(deM);
    }
}


string ElGamalPasswordDecrypt(int p, int x, const string& ciphertext) {
    
    stringstream ss(ciphertext);
    string decryptedText;
    int a, b;
    while (ss >> a >> b) 
    {
        int deM = MulMod(b, aXmodP(a, p - 1 - x, p), p);
        decryptedText += char(deM);
    }

    return decryptedText;
}

extern "C" char* ElGamal_PasswordDecrypt(int p, int x, const char* ciphertext) {
    
    string decrypted = ElGamalPasswordDecrypt(p, x, ciphertext);
    char* result = new char[decrypted.size() + 1];

    strcpy(result, decrypted.c_str());
    
    return result;
}

extern "C" void ElGamal_run(const char* fileName, int isShowingKeys) {

    try {
        srand(time(NULL));

        auto generated_keys = KeysGenerator(isShowingKeys);
        int p = get<0>(generated_keys);
        int g = get<1>(generated_keys);
        int x = get<2>(generated_keys);
        int y = get<3>(generated_keys);

        ifstream inFile(fileName, ios::binary);
        if (!inFile) 
        {
            throw runtime_error ("Ошибка открытия входного файла: ");
        }


        string plaintext;
        inFile.seekg(0, ios::end);
        size_t size = inFile.tellg();
        inFile.seekg(0, ios::beg);

        if (size > 0) 
        {
            plaintext.assign(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
        } else {
            cerr << "Входной файл пуст: " << fileName << endl;
        }
        inFile.close();

        filesystem::path inputPath(fileName);
        string baseName = inputPath.filename().string();

        ofstream encryptedOutFile("encrypted_" + baseName, ios::binary);
        if (!encryptedOutFile) 
        {
            throw runtime_error ("Ошибка открытия выходного файла для шифрования.");

        }
        cout << WARNING << "\nФайл шируется, подождите..." << RESET << endl;
        ElGamalCrypt(p, g, y, plaintext, encryptedOutFile);
        encryptedOutFile.close();
        cout << SUCCESS << "Файл зашифрован!" << RESET << endl;

        ifstream encryptedInFile("encrypted_" + baseName, ios::binary);
        if (!encryptedInFile) 
        {
            throw runtime_error ("Ошибка открытия зашифрованного файла.");
        }

        ofstream decryptedOutFile("decrypted_" + baseName, ios::binary);
        if (!decryptedOutFile) 
        {
            throw runtime_error ("Ошибка открытия выходного файла для расшифрования.");
        }


        cout << WARNING << "\nФайл дешируется, подождите..." << RESET << endl;
        string decryptedText;
        ElGamalDecrypt(p, x, encryptedInFile, decryptedText);
        encryptedInFile.close();

        decryptedOutFile << decryptedText;
        decryptedOutFile.close();
        cout << SUCCESS << "Файл расшифрован!" << RESET << endl;

    } catch (const exception& e) {
        cerr << ERROR << "Ошибка: " << RESET << e.what() << endl;
    }    
}
