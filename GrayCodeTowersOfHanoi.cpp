#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// FUNCTION PURPOSE: GENERATE GRAY CODE VALUES
std::vector<std::string> generateGray(int n) {
    // Base case
    if (n <= 0) {
        return {"0"};
    }
    if (n == 1) {
      return {"0", "1"};
    }
    //Recursive case
    std::vector<std::string> recAns= generateGray(n-1);
    std::vector<std::string> mainAns;
    // Append 0 to the first half
    for(int i=0;i<recAns.size();i++) {
        std::string s=recAns[i];
      mainAns.push_back("0"+s);
    }
     // Append 1 to the second half
    for(int i=recAns.size()-1;i>=0;i--) {
        std::string s=recAns[i];
       mainAns.push_back("1"+s);
    }
    return mainAns;
}

// FUNCTION PURPOSE: RETURN SMALLEST PEG MOVE
char smallestMove (int numDisks, char smallMove){
    char newMove = ' ';

    // ODD CASE
    if (numDisks % 2 == 1){
        switch(smallMove){
            case 'F':
                std::cout << "Smallest disk moved to Final peg" << std::endl;
                newMove = 'T';
                break;
            case 'T':
                std::cout << "Smallest disk moved to Remaining peg" << std::endl;
                newMove = 'R';
                break;
            case 'R':
                std::cout << "Smallest disk moved to First peg" << std::endl;
                newMove = 'F';
                break;
            default:
                break;
        }
    }
    
    // EVEN CASE
    if (numDisks % 2 == 0){
        switch(smallMove){
            case 'F':
                std::cout << "Smallest disk moved to Remaining peg" << std::endl;
                newMove = 'R';
                break;
            case 'R':
                std::cout << "Smallest disk moved to Final peg" << std::endl;
                newMove = 'T';
                break;
            case 'T':
                std::cout << "Smallest disk moved to First peg" << std::endl;
                newMove = 'F';
                break;
            default:
                break;
        }
    }
        return newMove;
}
        
        
// FUNCTION PURPOSE: GRAY CODE SOLUTION
void solution(std::vector<std::string> grayCode, int numDisks){
    char move = 'F';
    char smallDiskTemp = ' ';
    
    for (int i = 0; i < grayCode.size() + 1;  i++){
        for (int j = 0; j < numDisks;  j++){
            int tempi = i + 1;
            if (grayCode[tempi][j] == grayCode[i][j]){
                if (j == 0){
                    smallDiskTemp = move;
                    move = smallestMove(numDisks, smallDiskTemp);
                }
                else{
                    std::cout << j+1 << " disk moved" << std::endl;
                }
            }
        }
     }
}

int main(int argc, const char * argv[]) {

    int numDisks = 0;

    std::cout << "Enter the number of disks: ";
    std::cin >> numDisks;

    std::vector<std::string> gray;
    gray = generateGray(numDisks);
    
    
    solution(gray, numDisks);
    
    return 0;
}
