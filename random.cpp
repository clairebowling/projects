
#include <iostream>
#include <random>
#include <vector>

int countRandCall = 0;


// FUNCTION: RANDOM NUMBER GENERATOR
int randNumGen(int a, int b){
    std::vector<int> randVect;
    std::random_device rd{};
    std::mt19937 gen{rd()};
    // uniform distribution produces random integer i uniformly on the closed interval [a,b]
    std::uniform_int_distribution<int> uD(a,b);
    
   int num = uD(gen);

    return num;
}


// FUNCTION: RANDOMIZE VECTOR IN PLACE
std::vector<int> permutation(std::vector<int> vect){
    // random fill vector
    for(int i = 0; i < 10; ++i){
        vect.push_back(randNumGen(1, 10));
    }
    
    for (auto i = 0; i < vect.size(); i++)
        std::cout << vect[i] << std::endl;
        
    auto n = vect.size();
    int temp;
    
    // swap vect[i] with vect[random(i, n)]
    for(auto i = 0; i < n; i++){
        int randIndex = vect[randNumGen(i, n)];
        temp = vect[i];
        vect[i] = vect[randIndex];
        vect[randIndex] = temp;
    }
    
    std::cout << std::endl << std::endl;
    for (auto i = 0; i < vect.size(); i++)
        std::cout << vect[i] << std::endl;
    
    std::cout << std::endl << std::endl;
    
    return vect;
}

// FUNCTION: RANDOM SAMPLE
std::vector<int> randSample(int n, int m){
    std::vector<int> sampleVect;
    // random fill vector
    for(int i = 0; i < 10; ++i){
        sampleVect.push_back(randNumGen(1, 1000));
    }
    
    if (m == 0){
        // return empty list
        return sampleVect;
    }
    else{
        sampleVect = randSample(m - 1, n - 1);
        int i = randNumGen(i, n);
        countRandCall++;
        
        
        if (std::find(sampleVect.begin(), sampleVect.end(), i) != sampleVect.end() ){
            sampleVect.push_back(i);
        }
        else{
            sampleVect.push_back(n);
        }
    }
    std::cout << "n: " << n << "   m: " << m << std::endl;
    std::cout << "Calls to random: " << countRandCall << std::endl;
    return sampleVect;
}


int main(int argc, const char * argv[]) {
    std::vector<int> testVect;
    std::cout << "Test Vector: " << std::endl;
    for (int i = 0; i < testVect.size(); i++)
        std::cout << testVect[i] << std::endl;
    
    std::cout << "Randomized Vector: " << std::endl;
    std::vector<int> randomizedVect = permutation(testVect);
    for (int i = 0; i < randomizedVect.size(); i++)
        std::cout << randomizedVect[i] << std::endl;
    
    
    std::vector<int> sampleVect = randSample(300,15);
    
        for (int n = 0; n < sampleVect.size(); n++)
            std::cout << sampleVect[n] << std::endl;;
    return 0;
}
