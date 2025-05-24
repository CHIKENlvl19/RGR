#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <fstream>
#include "ElGamal_functions.h"

using namespace std;

bool isPrime(int p){
    if (p % 2 == 0 || p % 3 == 0 || p <= 1)
    {
        return false;
    }

    int squareRootN = static_cast<int>(sqrt(p)) + 1;
    int maxI = (squareRootN + 1) / 6;

    for(int i = 1; i < maxI; i++)
    {
        int dividerMinusOne = 6 * i - 1;
        int dividerPlusOne = 6 * i + 1;
            
        if (dividerMinusOne <= squareRootN) 
        {
            if (p % dividerMinusOne == 0) return false;
        }

        if (dividerPlusOne <= squareRootN) 
        {
            if (p % dividerPlusOne == 0) return false;
        }
    }

    return true;
}

int gcd(int a, int b){
    while( b!= 0)
    {
        int r = b;
        b = a % b;
        a = r;
    }

    return a;
}


int aXmodP (int a, int x, int p){ // быстрое возведение в степень по модулю
    int degree = x % (p - 1); // применение теоремы Ферма

    int result = 1;
    for(int i = 1; i <= degree; i++)
    {
        result = result * a;
        result %= p;
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
    if (g == 1) 
    {
        return false;
    }

    if (gcd(g, p) != 1)
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

tuple<int, int, int, int> KeysGenerator(){

    int p, g, x, y;
    
    do {
        p = rand() % 9000 + 1000;
    } while (!isPrime(p));

    do {
        g = rand() % (p - 1) + 1;
    } while (!is_primitive_root(g, p));

    x = rand() % (p - 1) + 1;

    y = aXmodP(g, x, p);

    return make_tuple(p, g, x, y);
}

void crypt(int& p, int& g, int& x, int& y, const string& plaintext, string& ciphertext, ofstream& out) {
    out << "Открытый ключ (p, g, y) = (" << p << ", " << g << ", " << y << ")" << endl;
    out << "Закрытый ключ x = " << x << endl;

    out << "\nШифруемый текст:\n" << plaintext << endl << endl;

    out << "Зашифрованный текст: ";

    for (char c : plaintext) 
    {
        int m = static_cast<int>(c);
        int k;
        do {
            k = rand() % (p - 2) + 1;
        } while (k <= 1 || k >= p - 1);

        int a = aXmodP(g, k, p);
        int b = MulMod(aXmodP(y, k, p), m, p);

        ciphertext += to_string(a) + " " + to_string(b) + " ";
        out << "(" << a << ", " << b << ") ";
    }
    out << endl;
}

void decrypt(int p, int x, const string& ciphertext, string& decryptedText, ofstream& out) {
    out << "\nДешифрованный текст:\n";

    stringstream ss(ciphertext);
    int a, b;
    while (ss >> a >> b) 
    {
        int deM = MulMod(b, aXmodP(a, p - 1 - x, p), p);
        decryptedText += char(deM);
    }

    out << decryptedText << endl;
}

void ElGamal(string filename) {
    srand(time(NULL));

    auto generated_keys = KeysGenerator();

    int p = get<0>(generated_keys);
    int g = get<1>(generated_keys);
    int x = get<2>(generated_keys);
    int y = get<3>(generated_keys);
 
    // чтение всего файла целиком (включая переводы строк)
    ifstream inFile(filename);

    ofstream outFile("output.txt");

    string plaintext;
    inFile.seekg(0, ios::end);

    size_t size = inFile.tellg();
    inFile.seekg(0, ios::beg);

    if (size > 0) {
        plaintext.assign(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
    } else {
        cerr << "Входной файл пуст: " << filename << endl;
    }
    
    inFile.close();

    string ciphertext;
    crypt(p, g, x, y, plaintext, ciphertext, outFile);

    string decryptedText;
    decrypt(p, x, ciphertext, decryptedText, outFile);

    outFile.close();

    //return 0;
}
