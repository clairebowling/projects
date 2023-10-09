//
//  main.cpp
//  RSA
//
//  Created by Claire Bowling on 9/21/22.
//

#include <iostream>
#include <string>
#include <math.h>

using namespace std;

string encrypt(string, int);
string decrypt(string, int);

const int p = 907;
const int q = 773;
const int e = 23; // public key
const string myPlaintext = "attackatdawn";

int main(int argc, const char * argv[]) {
    int n = p * q;
    int totient = 0;
    totient = (p - 1) * (q - 1);
    
    // calculate private key
    // e and d are relatively prime and e * d mod r = 1;
    int d = 0;
    for(d = 0;d < totient; d++)
    {
        if((d * e) % totient == 1)
            break;
    }
    cout << "Public Key: " << e << endl;
    cout << "Private Key: " << d << endl;
    
    string myPlaintext = "attackatdawn";
    string myCiphertext = "";

    myCiphertext = encrypt(myPlaintext, n);
    string decrypted = decrypt(myCiphertext, d);

    cout << "Ciphertext: " << myCiphertext << endl;
    cout << "Originial Plaintext: " << decrypted << endl;
    
    return 0;
}


string encrypt(string text, int n)
{
    string temp = "";
    char c = ' ';
    long long int x;
    
    for (int i = 0; i < text.length(); i++){
        long int h = text[i];
        x = ((long long int)pow(h, e)) % n;
        c = x;
        temp += c;
        c = ' ';
    }

    return temp;
}

string decrypt(string text, int d)
{
   string temp = "";
   char *x = new char[text.length()];
   int n = p * q;
   char c = '\0';
   for (int i = 0; i < text[i]; i++)
   {
       int h = text[i];
       long long a = ((long long)pow(h, d));
       a = a % n;
       c = a;
       temp += c;
       x[i] = (char)a;
       c = '\0';
   }
   return temp;
}
