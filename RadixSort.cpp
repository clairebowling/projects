#include <iostream>
#include <vector>

int findMax(std::vector<int>);
void radixSort(std::vector<int>, int);

const int numBuckets = 10;

int main(int argc, const char * argv[]) {
    // Initialize a vector with random integers
    std::vector<int> initialList;
    int size = rand() % 1 + 10;
    for (int i = 0; i < size; i++){
        int randFill = rand() % 1000000 + 9000000;
        initialList.push_back(randFill);
    }
    
    std::cout << "Initial Unsorted List: " << std::endl;
    for(int i = 0; i < initialList.size(); i++){
        std::cout << initialList[i] << std::endl;
    }
    
    // Find the maximum number of digits. This will be for loop count
    int digits = findMax(initialList);
    
    radixSort(initialList, digits);
    
    return 0;
}

// Function to find the number of maximum digits
int findMax(std::vector<int> myList){
    int temp = 0;
    int count = 0;
    for (int i = 0; i < myList.size(); i++){
        count = 0;
        while (myList[i]  > 0) {
            myList[i] = myList[i]  / 10;
            count++;
        }
        
        // Update max digits if count is greater than currentmax digits
        if (count > temp)
            temp = count;
    }
    std::cout << "Maximum number of digits: " << temp << std::endl;
    return temp;
}


// Function to perform Radix Sort
void radixSort(std::vector<int> list, int maxDigit){
    int digit = maxDigit;
    std::vector<int> bucket[numBuckets];
    
    for (int q = 0; q < maxDigit; q++){
        digit--;
        for(int i = 0; i < list.size(); i++){
            int comparison = list[i];
            
            // Convert int to string to access placeholder then convert char to int
            std::string str = std::to_string(comparison);
            int temp = int(str[digit])-48;
            
            // place integer into array of corresponding placeholder
            bucket[temp].push_back(comparison);
            
        }
        std::cout << std::endl;
        list.clear();
        // print contents of each bucket
        for(int y = 0; y < numBuckets; y++){
        std::cout << "In bucket " << y << " pass " << q+1 << std::endl;
            for (int z = 0 ; z < bucket[y].size(); z++){
                // merge buckets into original list
                list.push_back(bucket[y][z]);
                std::cout << bucket[y][z] << std::endl;
            }
            bucket[y].clear();
        }
    }
    
    // print the final sorted list
    std::cout << std::endl << "Final Sorted List: " << std::endl;
    for(int i = 0; i < list.size(); i++)
        std::cout << list[i] << std::endl;
}
 
