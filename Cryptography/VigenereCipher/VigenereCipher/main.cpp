//
//  main.cpp
//  VigenereCipher
//
//  Created by Claire Bowling on 9/6/22.
//

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
    string key = "wearethestudentsincsfoureightfour";
    string plainText = "informationwarfareisanyactiontodenyexploitcorruptordestroytheinformationoftheenemyanditsfunctions";
    char eChar = ' ';
    string encrypted = " ";
    string decrypted = " ";
    string perfectKey = " ";
    // make key the same size as plaintext
    for (int i = 0, j = 0; i < plainText.length(); i++, j++){
        if (j == key.length())
            j = 0;
        eChar = key[j];
        perfectKey.push_back(eChar);
    }
    // Encrypt
    for (int i = 0; i <= plainText.length(); ++i){
        eChar = (plainText[i] + perfectKey[i])  % 26;
        eChar += 'a';
        encrypted.push_back(eChar);
    }
    cout << "Ciphertext: " << encrypted << endl;
    // Decrypt
    for (int i = 0; i <= plainText.length(); ++i){
        eChar = (encrypted[i] - perfectKey[i] + 26) % 26;
        eChar += 'a';
        decrypted.push_back(eChar);
    }
    cout << "Plaintext: " << decrypted << endl;
    return 0;
}
