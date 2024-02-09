#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
  
//CONSTANTS
const int Nb = 4;   // Num columns
const int Nk_128 = 4;   // 128 num words
const int Nr_128 = 10;  // 128 rounds
const int Nk_192 = 6;   // 192 num words
const int Nr_192 = 12;  // 192 rounds
const int Nk_256 = 8;   // 256 num words
const int Nr_256 = 14;  // 256 rounds

const uint16_t AES_POLY = 0x11b;

const uint8_t Rcon[] = { 0x00,
           0x01, 0x02, 0x04, 0x08,
           0x10, 0x20, 0x40, 0x80,
           0x1B, 0x36, 0x6C, 0xD8,
           0xAB, 0x4D, 0x9A, 0x2F,
           0x5E, 0xBC, 0x63, 0xC6,
           0x97, 0x35, 0x6A, 0xD4,
           0xB3, 0x7D, 0xFA, 0xEF,
           0xC5, 0x91, 0x39, 0x72,
           0xE4, 0xD3, 0xBD, 0x61,
           0xC2, 0x9F, 0x25, 0x4A,
           0x94, 0x33, 0x66, 0xCC,
           0x83, 0x1D, 0x3A, 0x74,
           0xE8, 0xCB, 0x8D};


const uint8_t s_box[16][16] = {
    { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
    { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
    { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
    { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
    { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
    { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
    { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
    { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
    { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
    { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
    { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
    { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
    { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
    { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
    { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
    { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
  };

const uint8_t inv_s_box[16][16] = {
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb } ,
    { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb } ,
    { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e } ,
    { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 } ,
    { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 } ,
    { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 } ,
    { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 } ,
    { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b } ,
    { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 } ,
    { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e } ,
    { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b } ,
    { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 } ,
    { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f } ,
    { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef } ,
    { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 } ,
    { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
};

using namespace std;

struct Matrix{
    uint8_t data[4][4];
};

struct Word{
    uint8_t data[4];
};

uint8_t ffAdd(uint8_t, uint8_t);
uint8_t xtime(uint8_t);
uint8_t ffMultiply(uint8_t, uint8_t);


// METHODS FOR ENCRYPTION
Matrix SubBytes(Matrix&);
Matrix ShiftRows(Matrix&);
Matrix MixColumns(Matrix&);
Matrix AddRoundKey(Matrix&, Word&, Word&, Word&, Word&);
Matrix Cipher(string, int, vector<Word>);

// METHODS FOR KEY EXPANSION
Word SubWord(Word&);
Word RotWord(Word&);
vector<Word> KeyExpansion(string, int, int);

// METHODS FOR INVERSE CIPHER
Matrix InvShiftRows(Matrix&);
Matrix InvSubBytes(Matrix&);
Matrix InvMixColumns(Matrix&);
Matrix InvCipher(Matrix&, int, vector<Word>);

// HELPER FUNCTIONS
void print(string , int, Matrix&);
Matrix MakeMatrix(string);
void printKey(string, int, Word, Word, Word, Word);
void printWord(string, Word);

int main(){
    string plaintext = ("00112233445566778899aabbccddeeff");

    string KEY_128 = ("000102030405060708090a0b0c0d0e0f");
    string KEY_192 = ("000102030405060708090a0b0c0d0e0f1011121314151617");
    string KEY_256 = ("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");


    //  128 AES
    cout << "PLAINTEXT:                 " << plaintext << endl;
    cout << "KEY:                       " << KEY_128 << endl;
    vector<Word> keySchedule_128 = KeyExpansion(KEY_128, Nr_128, Nk_128);
    Matrix encrypted = Cipher(plaintext, Nr_128, keySchedule_128);
    cout << endl << endl;
    // string keyTest = "2b7e151628aed2a6abf7158809cf4f3c";
    // vector<Word> key = KeyExpansion(keyTest, Nr_128, Nk_128);
    Matrix decrypted = InvCipher(encrypted, Nr_128, keySchedule_128);
    cout << endl << endl;


    //  192 AES
    cout << "PLAINTEXT:                 " << plaintext << endl;
    cout << "KEY:                       " << KEY_192 << endl;
    vector<Word> keySchedule_192 = KeyExpansion(KEY_192, Nr_192, Nk_192);
    Matrix encrypted_192 = Cipher(plaintext, Nr_192, keySchedule_192);
    cout << endl << endl;
    Matrix decrypted_192 = InvCipher(encrypted_192, Nr_192, keySchedule_192);
    cout << endl << endl;


    // // 256 AES
    cout << "PLAINTEXT:                 " << plaintext << endl;
    cout << "KEY:                       " << KEY_256 << endl;
    vector<Word> keySchedule_256 = KeyExpansion(KEY_256, Nr_256, Nk_256);
    Matrix encrypted_256 = Cipher(plaintext, Nr_256, keySchedule_256);
    cout << endl << endl;
    Matrix decrypted_256 = InvCipher(encrypted_256, Nr_256, keySchedule_256);
    return 0;
}


// **************************************************************
// ****************        FINITE FIELDS         **************** 
// **************************************************************
uint8_t ffAdd(uint8_t a, uint8_t b){
    return a ^ b;
}

// xtime ---> Multiplies a finite field by x
uint8_t xtime(uint8_t b){
    uint16_t result = b << 1;
    if (result >= 0x100){
        result ^= AES_POLY;
        return result;
    } 
    else{
        return result;
    }
}


uint8_t ffMultiply(uint8_t a, uint8_t b){
    uint8_t sum = 0x00;
    uint8_t temp = b;

    for (int i = 0; i < 8; i++){
        if (i > 0){
            temp = xtime(temp);
        }
        if (a & 0x01){
            sum ^= temp;
        }
        a >>= 1;
    }
    return sum;
}   


// **************************************************************
// ****************           CIPHER             **************** 
// **************************************************************fAD
Matrix SubBytes(Matrix& state){
    Matrix sub;
    for (int i = 0; i < 4 ; i++) {
      for (int j = 0; j < 4 ; j++) {
        uint8_t byte = state.data[i][j];
        uint8_t row = (byte >> 4);
        uint8_t col = byte & 0x0f;
        sub.data[i][j] = s_box[row][col];
      }
   }
    return sub;
}


// **************************************************************
Matrix ShiftRows(Matrix& state){
    Matrix shiftRows;

    for (int i = 0; i < Nb; i++){
        for (int j = 0; j < Nb; j++){
            int newCol = (i + j) % Nb; 
            shiftRows.data[i][j] = state.data[i][newCol];
        }
    }
    return shiftRows;
}


// **************************************************************
Matrix MixColumns(Matrix& state){
    Matrix mixCols;
    int colNum;
    for (int j = 0; j < Nb; j++){
            mixCols.data[0][j] = (ffMultiply(0x02, state.data[0][j]) ^ ffMultiply(0x03, state.data[1][j]) ^ state.data[2][j] ^ state.data[3][j]);           
            mixCols.data[1][j] = (state.data[0][j] ^ ffMultiply(0x02, state.data[1][j]) ^ ffMultiply(0x03, state.data[2][j]) ^ state.data[3][j]);
            mixCols.data[2][j] = (state.data[0][j] ^ state.data[1][j] ^ ffMultiply(0x02, state.data[2][j]) ^ ffMultiply(0x03, state.data[3][j]));
            mixCols.data[3][j] = (ffMultiply(0x03, state.data[0][j]) ^ state.data[1][j] ^ state.data[2][j] ^ ffMultiply(0x02, state.data[3][j]));
    }
    return mixCols;
}


// **************************************************************
Matrix AddRoundKey(Matrix& state, Word& w0, Word& w1, Word& w2, Word& w3){
    vector<Word> temp;
    temp.push_back(w0);
    temp.push_back(w1);
    temp.push_back(w2);
    temp.push_back(w3);
    for (int i = 0; i < Nb; i++){
        for(int j = 0; j < Nb; j++){
            state.data[j][i] ^= temp[i].data[j];
        }
    }
    return state;
}


// **************************************************************
// Steps for encryption
Matrix Cipher(string plaintext, int Nr, vector<Word> w){
    Matrix state = MakeMatrix(plaintext);
    cout << "CIPHER (ENCRYPT): " << endl;
    int roundcount = 0;
    print("input", roundcount, state);
    printKey("k_sch", roundcount, w[0], w[1], w[2], w[3]);
    state = AddRoundKey(state, w[0], w[1], w[2], w[3]);
    for (int round = 1; round < Nr; round++){
        roundcount++;
        print("start", roundcount, state);

        state = SubBytes(state);
        print("s_box", roundcount, state);

        state = ShiftRows(state);
        print("s_row", roundcount, state);

        state = MixColumns(state);
        print("m_col", roundcount, state);

        printKey("k_sch", roundcount,  w[4*round], w[4*round+1], w[4*round+2], w[4*round+3]);
        state = AddRoundKey(state, w[4*round], w[4*round+1], w[4*round+2], w[4*round+3]);
    }
    roundcount++;
    print("start", roundcount, state);

    state = SubBytes(state);
    print("s_box", roundcount, state);

    state = ShiftRows(state);
    print("s_row", roundcount, state);

    state = AddRoundKey(state, w[4*Nr], w[4*Nr+1], w[4*Nr+2], w[4*Nr+3]);

    print("output", roundcount, state);
    return state;
}


// **************************************************************
// ****************       KEY EXPANSION          **************** 
// **************************************************************
Word SubWord(Word& word){
    Word subWord;
    for (int i = 0; i < Nb; i++){
        uint8_t byte = word.data[i];
        uint8_t row = (byte >> 4);
        uint8_t col = byte & 0x0f;
        subWord.data[i] = s_box[row][col];
    }
    return subWord;
}


// **************************************************************
Word RotWord(Word& word){
    Word rotWord;
    rotWord.data[0] = word.data[1];
    rotWord.data[1] = word.data[2];
    rotWord.data[2] = word.data[3];
    rotWord.data[3] = word.data[0];
    return rotWord;
}


// **************************************************************
vector<Word> KeyExpansion(string key, int Nr, int Nk){
    vector<Word> w;
    Word temp;
    vector<uint8_t> keyVect;

        // Parse the string of hex characters into bytes
    for (int i = 0; i < key.length(); i += 2) {
        string tempStr = key.substr(i, 2);
        uint32_t ch;
        istringstream toByte(tempStr);
        toByte >> hex >> ch;
        keyVect.push_back(ch);
    }

    // Construct the initial key schedule
    for (int j = 0; j < Nk; j++) {
        Word word;
        for (int k = 0; k < 4; k++) {
            word.data[k] = keyVect[j * 4 + k];
        }
        w.push_back(word);
    }

    // Expand the key schedule
    int round = 1;
    for (int i = Nk; i < Nb * (Nr + 1); i++) {
        temp = w[i - 1];
        if (i % Nk == 0) {
            temp = RotWord(temp);
            //printWord("rot word", temp);
            temp = SubWord(temp);
            //printWord("sub word", temp);
            temp.data[0] ^= Rcon[i / Nk];
            //printWord("rcon", temp);
            round++;
        } else if (Nk > 6 && i % Nk == 4) {
            temp = SubWord(temp);
        }

        Word word;
        for (int j = 0; j < 4; j++) {
            word.data[j] = temp.data[j] ^ w[i - Nk].data[j];
        }
        w.push_back(word);
    }
    return w;
}


// **************************************************************
// ****************       INVERSE CIPHER          **************** 
// **************************************************************
Matrix InvShiftRows(Matrix& state){

    Matrix invShiftRows;

    for (int k = 0; k < Nb; k++){
        invShiftRows.data[0][k] = state.data[0][k];
    }

    for (int i = 1; i < Nb; i++){
        for (int j = 0; j < Nb; j++){
            int newCol = (j + (4 - i)) % Nb; 
            invShiftRows.data[i][j] = state.data[i][newCol];
        }
    }
    return invShiftRows;
}


// **************************************************************
// Steps for decryption
Matrix InvSubBytes(Matrix& state){
    Matrix invSubBytes;
    for (int i = 0; i < 4 ; i++) {
      for (int j = 0; j < 4 ; j++) {
        uint8_t byte = state.data[i][j];
        uint8_t row = (byte >> 4);
        uint8_t col = byte & 0x0f;
        invSubBytes.data[i][j] = inv_s_box[row][col];
      }
   }
    return invSubBytes;
}


// **************************************************************
Matrix InvMixColumns(Matrix& state){
    Matrix invMixCols;

    for (int j = 0; j < Nb; j++){
        invMixCols.data[0][j] = (ffMultiply(0x0e, state.data[0][j]) ^
                                ffMultiply(0x0b, state.data[1][j]) ^
                                ffMultiply(0x0d, state.data[2][j]) ^
                                ffMultiply(0x09, state.data[3][j]));

        invMixCols.data[1][j] = (ffMultiply(0x09, state.data[0][j]) ^
                                ffMultiply(0x0e, state.data[1][j]) ^
                                ffMultiply(0x0b, state.data[2][j]) ^
                                ffMultiply(0x0d, state.data[3][j]));

        invMixCols.data[2][j] = (ffMultiply(0x0d, state.data[0][j]) ^
                                ffMultiply(0x09, state.data[1][j]) ^
                                ffMultiply(0x0e, state.data[2][j]) ^
                                ffMultiply(0x0b, state.data[3][j]));

        invMixCols.data[3][j] = (ffMultiply(0x0b, state.data[0][j]) ^
                                ffMultiply(0x0d, state.data[1][j]) ^
                                ffMultiply(0x09, state.data[2][j]) ^
                                ffMultiply(0x0e, state.data[3][j]));
    }
    return invMixCols;
}


// **************************************************************
Matrix InvCipher(Matrix& state, int Nr, vector<Word> w){
    cout << "INVERSE CIPHER (DECRYPT): " << endl;
    int roundcount = 0;
    print("input", roundcount, state);
    printKey("ik_sch", roundcount, w[4*Nr], w[4*Nr+1], w[4*Nr+2], w[4*Nr+3]);
    state = AddRoundKey(state, w[4*Nr], w[4*Nr+1], w[4*Nr+2], w[4*Nr+3]);
    for (int round = Nr - 1; round >= 1; round--){
        roundcount++;
        print("start", roundcount, state);

        state = InvShiftRows(state);
        print("is_row", roundcount, state);

        state = InvSubBytes(state);
        print("is_box", roundcount, state);

        printKey("ik_sch", roundcount, w[4*round], w[4*round+1], w[4*round+2], w[4*round+3]);
        state = AddRoundKey(state, w[4*round], w[4*round+1], w[4*round+2], w[4*round+3]);

        state = InvMixColumns(state);
        print("im_col", roundcount, state);
    }
    roundcount++;
    print("start", roundcount, state);

    state = InvShiftRows(state);
    print("is_row", roundcount, state);

    state = InvSubBytes(state);
    print("is_box", roundcount, state);
    
    printKey("ik_sch", roundcount, w[0], w[1], w[2], w[3]);
    state = AddRoundKey(state, w[0], w[1], w[2], w[3]);

    print("output", roundcount, state);
    return state;
}



void print(string label, int round, Matrix& state){
    cout << "round[ " << dec << round << " ]." << label << "      ";
    for (int i = 0; i < Nb; i++){
        for (int j = 0; j < Nb; j++){
           cout << hex << setw(2) << setfill('0') << int(state.data[j][i]) << "  ";
        }
    }
    cout << endl;
}

Matrix MakeMatrix(string text){
    vector<uint8_t> tempBytes;
    Matrix state;
    uint32_t ch;
    for (int i = 0; i < text.length() - 1; i++){
        string temp;
        temp.push_back(text[i]);
        temp.push_back(text[i++]);
        std::istringstream toByte(temp);
        toByte >> hex >> ch; 
        tempBytes.push_back(ch);
        temp.clear();
    }

    // Push Bytes to state matrix
    int byteIndex = 0;
    for (int i = 0; i < Nb; i++){
        for (int j = 0; j < Nb; j++){
            state.data[j][i] = tempBytes[byteIndex];
            byteIndex++;
        }
    }
    return state;
}

void printKey(string label, int round, Word w0, Word w1, Word w2, Word w3){
    vector<Word> temp;
    temp.push_back(w0);
    temp.push_back(w1);
    temp.push_back(w2);
    temp.push_back(w3);
    cout << "round[ " << dec << round << " ]." << label << "      ";
    for (int i = 0; i < Nb; i++){
        for(int j = 0; j < Nb; j++)
            cout << hex << setw(2) << setfill('0') << int(temp[i].data[j]) << "  ";
    }
    cout << endl;
}

void printWord(string label, Word temp){
    cout << label << "      ";
    for (int i = 0; i < Nb; i++){
            cout << hex << setw(2) << setfill('0') << int(temp.data[i]) << "  ";
    }
    cout << endl;
    return;
}
