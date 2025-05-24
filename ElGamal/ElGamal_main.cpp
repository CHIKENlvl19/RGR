/*#include <iostream>
#include <fstream>
#include "ElGamal_functions.h"

using namespace std;

int ElGamal() {
    srand(time(NULL));

    auto generated_keys = KeysGenerator();

    int p = get<0>(generated_keys);
    int g = get<1>(generated_keys);
    int x = get<2>(generated_keys);
    int y = get<3>(generated_keys);
 
    // чтение всего файла целиком (включая переводы строк)
    ifstream inFile("input.txt");
    ofstream outFile("output.txt");

    string plaintext;
    inFile.seekg(0, ios::end);

    size_t size = inFile.tellg();
    inFile.seekg(0, ios::beg);

    if (size > 0) {
        plaintext.assign(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
    }
    
    inFile.close();

    string ciphertext;
    crypt(p, g, x, y, plaintext, ciphertext, outFile);

    string decryptedText;
    decrypt(p, x, ciphertext, decryptedText, outFile);

    outFile.close();

    return 0;
}*/