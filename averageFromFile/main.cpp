#include <iostream>
#include <fstream>
#include <random>

using namespace std;

 int randInt(){
     std::random_device rd{};
     std::mt19937 gen{rd()};
     std::uniform_int_distribution<int> uD(0, 1000);
     int num = uD(gen);
     return num;
 }

int main(int argc, const char * argv[]) {
    double sum = 0;
    int totalNums = 0;
    double numData = 0;

// GENERATE A RANDOM NUMBER OF INPUT NUMBERS IN INPUT FILE
     int num_of_nums = randInt();
     ofstream randInputs;
     randInputs.open("test.txt");

     if (randInputs.is_open()){
         for (int i = 0; i < 15; i++){
             int rand_int_val = randInt();
             randInputs << rand_int_val << endl;
         }
         randInputs.close();
     }
     else{
         cout << "Error generating random inputs" << endl;
     }


// READING INPUTS FROM FILE
    ifstream randNums;
    randNums.open("test.txt");

    if (randNums.is_open()){
        while (randNums >> numData){
            cout << numData;
            sum += numData;
            cout << endl;
        }
        randNums.close();
    }
    else{
        cout << "Error opening input file" << endl;
    }

// COMPUTE AVERAGE
    cout << "SUM: " << sum << endl;
    double average = sum / 25;
    cout << average << endl;    // test: Average accurate? print to terminal

// WRITE AVERAGE TO OUTPUT FILE
    ofstream outFile;
    outFile.open("average.txt");

    if(outFile.is_open()){
        outFile << "The average of the numbers is: " << average;
        outFile.close();
    }
    else{
        cout << "Error opening output file" << endl;
    }
    return 0;
}
