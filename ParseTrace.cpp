//
//  parse.cpp
//  Parse
//
//  Created by Claire Bowling on 9/20/22.
//

#include <iostream>
#include <string>
#include <stack>
#include <vector>

int action ();

using namespace std;

void printInput (vector<char>);

int main(int argc, const char * argv[]) {
    vector <char> input = {  'P', '+', 'P', '*', 'P','$'};
    
    //   id    +    *    (    )     $
    string actionTable[12][6] =
    {  { "S5", " ", " ", "S4", " ", " "},
        {" ", "S6", " ", " ", " ", "ACCEPT"},
        {" ", "R2", "S7", " ", "R2", "R2"},
        {" ", "R4", "R4", " ", "R4", "R4"},
        {"S5", " ", " ", "S4", " ", " "},
        {" ", "R6", "R6", " ", "R6", "R6"},
        {"S5", " ", " ", "S4", " ", " "},
        {"S5", " ", " ", "S4", " ", " "},
        {" ", "S6", " ", " ", "S11", " "},
        {" ", "R1", "S7", " ", "R1", "R1"},
        {" ", "R3", "R3", " ", "R3", "R3"},
        {" ", "R5", "R5", " ", "R5", "R5"}
    };
    
    stack <int> stateStack;
    stateStack.push(0);
    
    printInput(input);
    
    string tableValue = " ";
    char inputChar = ' ';
    
    while (!input.empty()){
        inputChar = input.front();
        int tableRow = stateStack.top();
        
        switch (inputChar){
            case 'P':{
                tableValue = actionTable[tableRow][0];
                break;
            }
                
            case '+':{
                tableValue = actionTable[tableRow][1];
                break;
            }
                
            case '*':{
                tableValue = actionTable[tableRow][2];
                break;
            }
                
            case '(':{
                tableRow = stateStack.top();
                tableValue = actionTable[tableRow][3];
                break;
            }
                
            case ')':{
                tableValue = actionTable[tableRow][4];
                break;
            }
                
            case '$':{
                tableValue = actionTable[tableRow][5];
                break;
            }
        }
        
        int goToValue;
        char tempChar = tableValue[0];
        
        if (tempChar == 'S'){
            if (tableValue == "S4"){
                stateStack.push(4);
            }
            else if (tableValue == "S5"){
                stateStack.push(5);
            }
            else if (tableValue == "S6"){
                stateStack.push(6);
            }
            else if (tableValue == "S7"){
                stateStack.push(7);
            }
            else if (tableValue == "S11"){
                stateStack.push(11);
            }
            input.erase(input.begin());
        }
        else if (tempChar == 'R'){
            if (tableValue == "R1"){
                string RedRule1 = "E -> E + T";
                for (int i = 0; i < 3; i++){
                    stateStack.pop();
                }
                goToValue = stateStack.top();
                cout << "Reduction Rule 1: " << RedRule1 << endl;
                if (goToValue == 0){
                    stateStack.push(1);
                }
                else if (goToValue == 4){
                    stateStack.push(8);
                }
                printInput(input);
            }
            
            else if (tableValue == "R2"){
                string RedRule2 = "E -> T";
                stateStack.pop();
                goToValue = stateStack.top();
                cout << "Reduction Rule 2: " << RedRule2 << endl;
                if (goToValue == 0){
                    stateStack.push(1);
                }
                else if (goToValue == 4){
                    stateStack.push(8);
                }
                printInput(input);
            }
            
            else if (tableValue == "R3"){
                string RedRule3 = "T -> T * F";
                for (int i = 0; i < 3; i++){
                    stateStack.pop();
                }
                goToValue = stateStack.top();
                cout << "Reduction Rule 3: " << RedRule3 << endl;
                if (goToValue == 0 | goToValue ==4){
                    stateStack.push(2);
                }
                else if (goToValue == 6){
                    stateStack.push(9);
                }
                printInput(input);
            }
            
            else if (tableValue == "R4"){
                string RedRule4 = "T -> F";
                stateStack.pop();
                goToValue = stateStack.top();
                cout << "Reduction Rule 4: " << RedRule4 << endl;
                if (goToValue == 0 | goToValue ==4){
                    stateStack.push(2);
                }
                else if (goToValue == 6){
                    stateStack.push(9);
                }
                printInput(input);
            }
            
            else if (tableValue == "R5"){
                string RedRule5 = "F -> ( E )";
                for (int i = 0; i < 3; i++){
                    stateStack.pop();
                }
                goToValue = stateStack.top();
                cout << "Reduction Rule 5: " << RedRule5 << endl;
                if (goToValue == 0 | goToValue == 4 | goToValue == 6){
                    stateStack.push(3);
                }
                else if (goToValue == 7){
                    stateStack.push(10);
                }
                printInput(input);
            }
            
            else if (tableValue == "R6"){
                string RedRule6 =" F -> id";
                stateStack.pop();
                cout << "Reduction Rule 6: " << RedRule6 << endl;
                goToValue = stateStack.top();
                if (goToValue == 0 | goToValue == 4 | goToValue == 6){
                    stateStack.push(3);
                }
                else if (goToValue == 7){
                    stateStack.push(10);
                }
                printInput(input);
            }
        }
        
        else if (tempChar == 'A'){
            cout << "Valid statement! ACCEPTED" << endl;
            input.erase(input.begin());
        }
    }
    return 0;
}

void printInput(vector<char> myVector){
    cout << "Remaining Input: ";
    for (int i = 0; i < myVector.size(); i++){
        cout << myVector[i];
    }
    cout << endl << endl;
    return;
}
