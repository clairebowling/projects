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
    string encrypted;
    string decrypt;
    string perfectKey = " ";
    
    // make key the same size as plaintext
    for (int i = 0, j = 0; i < plainText.length(); ++i, ++j){
        if (j == key.length())
            j = 0;
        eChar = key[j];
        perfectKey.push_back(eChar);
    }
    
    // Encrypt
    char newChar = ' ';
    for (int i = 0, j = 0; i < plainText.length(); ++i)
              {
                  char eChar = plainText[i];
   
                  if (eChar >= 'a' && eChar <= 'z')
                      eChar += 'A' - 'a';
                  else if (eChar < 'A' || eChar > 'Z')
                      continue;
   
                  newChar = (eChar + key[j]) % 26 + 'A';
                  j = (j + 1) % key.length();
                  cout << newChar;
              }
    
    cout <<  endl;
    // Decrypt

    
    return 0;
}
