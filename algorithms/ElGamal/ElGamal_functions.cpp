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
#include <random>
#include <gmpxx.h>
#include <chrono>
#include "../include/ElGamal/ElGamal_functions.h"

using namespace std;
using namespace Colors;
using namespace std::chrono;
namespace fs = filesystem;

bool isPrime(const mpz_class& p) {
    return mpz_probab_prime_p(p.get_mpz_t(), 25) > 0;
}

mpz_class gcd(const mpz_class& a, const mpz_class& b) {
    mpz_class result;
    mpz_gcd(result.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());

    return result;
}

mpz_class aXmodP(const mpz_class& a, const mpz_class& x, const mpz_class& p) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), a.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());
    return result;
}

mpz_class MulMod(const mpz_class& a, const mpz_class& b, const mpz_class& n) {
    mpz_class result;
    mpz_mul(result.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    mpz_mod(result.get_mpz_t(), result.get_mpz_t(), n.get_mpz_t());
    return result;
}

vector<mpz_class> prime_factors(const mpz_class& n) {
    vector<mpz_class> factors;
    
    if (n <= 1) {
        return factors;
    }

    mpz_class num = n; //с копией

    if (num % 2 == 0) {
        factors.push_back(2);
        while (num % 2 == 0) {
            num /= 2;
        }
    }


    mpz_class i = 3;
    mpz_class max_factor;
    mpz_sqrt(max_factor.get_mpz_t(), num.get_mpz_t()); // max_factor = floor(sqrt(num))

    while (i <= max_factor && num > 1) {
        if (num % i == 0) {
            factors.push_back(i);
            while (num % i == 0) {
                num /= i;
            }
            // max_factor после деления
            mpz_sqrt(max_factor.get_mpz_t(), num.get_mpz_t());
        }
        i += 2; // только нечётные числа
    }

    // если остался множитель > 1 (простое число)
    if (num > 1) {
        factors.push_back(num);
    }

    return factors;
}

mpz_class find_primitive_root(const mpz_class& p, const mpz_class& pm1, const vector<mpz_class>& factors) {
    for (mpz_class g = 2; g < p; g++) {
        bool is_primitive = true;
        for (const auto& q : factors) {
            if (aXmodP(g, pm1 / q, p) == 1) {
                is_primitive = false;
                break;
            }
        }
        if (is_primitive) {
            return g;
        }
    }
    throw runtime_error("Не найден примитивный корень");
}

mpz_class generate_safe_prime(gmp_randstate_t state, unsigned long bits) {
    mpz_class q, p;
    do {
        mpz_urandomb(q.get_mpz_t(), state, bits - 1);
        mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());
        p = 2 * q + 1;
        if (mpz_probab_prime_p(p.get_mpz_t(), 25) > 0) {
            return p;
        }
    } while (true);
}

tuple<mpz_class, mpz_class, mpz_class, mpz_class> KeysGenerator(bool isShowingKeys, gmp_randstate_t state) {
    mpz_class x, y;
    
    mpz_class p = generate_safe_prime(state, 128); // 128-битное безопасное простое
    mpz_class pm1 = p - 1;
    vector<mpz_class> factors = {mpz_class(2), pm1 / 2}; // p-1 = 2*q
    mpz_class g = find_primitive_root(p, pm1, factors); 

    // секретный ключ
    mpz_urandomm(x.get_mpz_t(), state, pm1.get_mpz_t());
    y = aXmodP(g, x, p);
    
    if (isShowingKeys) {
        gmp_printf("\nОткрытый ключ (p, g, y) = (%Zd, %Zd, %Zd)\n", p.get_mpz_t(), g.get_mpz_t(), y.get_mpz_t());
        gmp_printf("Закрытый ключ x = %Zd\n", x.get_mpz_t());
    }
    
    return make_tuple(p, g, x, y);
}

void ElGamalCrypt(mpz_class& p, mpz_class& g, mpz_class& y, const string& plaintext, ofstream& out, gmp_randstate_t state) {
    for (size_t i = 0; i < plaintext.size(); ++i) {
        mpz_class m = static_cast<unsigned char>(plaintext[i]);
        mpz_class k;
        
        do {
            mpz_urandomm(k.get_mpz_t(), state, p.get_mpz_t());
        } while (k <= 1 || mpz_cmp(k.get_mpz_t(), p.get_mpz_t()) >= 0);
        
        mpz_class a = aXmodP(g, k, p);
        mpz_class b = MulMod(aXmodP(y, k, p), m, p);
        
        string a_str = a.get_str();
        string b_str = b.get_str();
        
        out << a_str << " " << b_str << " ";
    }
}

void ElGamalDecrypt(const mpz_class& p, const mpz_class& x, ifstream& in, string& decryptedText) {
    string a_str, b_str;
    while (in >> a_str >> b_str) {
        mpz_class a, b;
        a = a_str;
        b = b_str;

        mpz_class s = aXmodP(a, p - 1 - x, p); // s = a^(p-1-x) mod p
        mpz_class m = MulMod(b, s, p); // m = b * s mod p

        decryptedText += static_cast<char>(m.get_ui()); // восстановление байта
    }
}

int simpleAxModP(int a, int x, int p) {
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

int simpleMulMod(int a, int b, int n) {
    return (a * b) % n;
}

string ElGamalPasswordDecrypt(int p, int x, const string& ciphertext) {
    
    stringstream ss(ciphertext);
    string decryptedText;
    int a, b;
    while (ss >> a >> b) 
    {
        int deM = simpleMulMod(b, simpleAxModP(a, p - 1 - x, p), p);
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

extern "C" void ElGamal_run(const char* fileName, int isShowingKeys) {

    try {
        gmp_randstate_t state;
        gmp_randinit_default(state);

        random_device rd;
        unsigned long seed = rd();
        mpz_class seed_mpz = seed;
        gmp_randseed(state, seed_mpz.get_mpz_t());

        auto generated_keys = KeysGenerator(isShowingKeys, state);
        mpz_class p = get<0>(generated_keys);
        mpz_class g = get<1>(generated_keys);
        mpz_class x = get<2>(generated_keys);
        mpz_class y = get<3>(generated_keys);

        ifstream inFile(fileName, ios::binary);
        if (!inFile) 
        {
            throw runtime_error ("Ошибка открытия входного файла");
        }

        inFile.seekg(0, ios::end);
        size_t size = inFile.tellg();
        inFile.seekg(0, ios::beg);

        vector<char> buffer(size);
        if (size > 0) {
            inFile.read(buffer.data(), size);
        } else {
            cerr << "Входной файл пуст: " << fileName << endl;
        }
        inFile.close();
        string plaintext(buffer.data(), size);

        filesystem::path inputPath(fileName);
        string baseName = inputPath.filename().string();

        ofstream encryptedOutFile("encrypted_" + baseName, ios::binary);
        if (!encryptedOutFile) 
        {
            throw runtime_error ("Ошибка открытия выходного файла для шифрования.");

        }
        cout << WARNING << "\nФайл шифруется, подождите..." << RESET << endl;
        auto startEncrypt = high_resolution_clock::now();
        ElGamalCrypt(p, g, y, plaintext, encryptedOutFile, state);
        auto endEncrypt = high_resolution_clock::now();
        encryptedOutFile.close();
        cout << SUCCESS << "Файл успешно зашифрован!" << RESET << endl;

        auto encryptDuration = duration_cast<milliseconds>(endEncrypt - startEncrypt);
        auto encryptMinutes = duration_cast<minutes>(encryptDuration);
        auto encryptSeconds = duration_cast<seconds>(encryptDuration - encryptMinutes);
        auto encryptMilliseconds = duration_cast<milliseconds>(encryptDuration - encryptMinutes - encryptSeconds);

        cout << "Время шифрования: " << encryptMinutes.count() << " мин " << encryptSeconds.count() << " сек " << encryptMilliseconds.count() << " мс" << endl;

        ifstream encryptedInFile("encrypted_" + baseName, ios::binary);
        if (!encryptedInFile) 
        {
            throw runtime_error ("Ошибка открытия зашифрованного файла.");
        }

        ofstream decryptedOutFile(baseName, ios::binary);
        if (!decryptedOutFile) 
        {
            throw runtime_error ("Ошибка открытия выходного файла для расшифрования.");
        }


        cout << WARNING << "\nФайл дефшируется, подождите..." << RESET << endl;
        auto startDecrypt = high_resolution_clock::now();
        string decryptedText;
        ElGamalDecrypt(p, x, encryptedInFile, decryptedText);
        auto endDecrypt = high_resolution_clock::now();
        encryptedInFile.close();

        decryptedOutFile.write(decryptedText.data(), decryptedText.size());
        decryptedOutFile.close();
        cout << SUCCESS << "Файл успешно расшифрован!" << RESET << endl;

        auto decryptDuration = duration_cast<milliseconds>(endDecrypt - startDecrypt);
        auto decryptMinutes = duration_cast<minutes>(decryptDuration);
        auto decryptSeconds = duration_cast<seconds>(decryptDuration - decryptMinutes);
        auto decryptMilliseconds = duration_cast<milliseconds>(decryptDuration - decryptMinutes - decryptSeconds);

        cout << "Время дешифрования: " << decryptMinutes.count() << " мин " << decryptSeconds.count() << " сек " << decryptMilliseconds.count() << " мс" << endl;

        uintmax_t originalSize = fs::file_size(inputPath);
        uintmax_t encryptedSize = fs::file_size("encrypted_" + baseName);

        cout << IMPORTANT << "\nРазмер файла до шифрования: " << humanReadableSize(originalSize) << RESET << endl;
        cout << IMPORTANT << "Размер зашифрованного файла: " << humanReadableSize(encryptedSize) << RESET << endl;

        gmp_randclear(state); // очистка состояния генератора
    } catch (const exception& e) {
        cerr << ERROR << "Ошибка: " << RESET << e.what() << endl;
    }    
}
