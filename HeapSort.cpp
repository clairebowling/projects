#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

int randInt(){
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<double> uD(0, 10);
    int num = uD(gen);
    return num;
}

template<typename Iterator>
void heapsort(Iterator first, Iterator last){
  std::make_heap(first, last);
  while (last != first)
    {
        std::pop_heap(first, last-1);
     --last;
    }
}

int main(int argc, const char * argv[]) {
    int numInts = 0;
    std::cout << "Enter number of integers in vector." << std::endl;
    std::cin >> numInts;
    
    std::vector<int> vect;
    for (int i = 0; i < numInts; i++){
        vect.push_back(randInt());
    }
    
    // heapsort
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    start_time = std::chrono::steady_clock::now();
    heapsort(vect.begin(), vect.end());
    std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = stop_time - start_time;
    std::cout << std::endl << "Heapsort for " << numInts << " integers: " << elapsed_time.count() << " milliseconds" << std::endl;
    
    // stl sort()
    start_time = std::chrono::steady_clock::now();
    std::sort(vect.begin(), vect.end());
    stop_time = std::chrono::steady_clock::now();
    elapsed_time = stop_time - start_time;
    std::cout << std::endl << "STL sort for " << numInts << " integers: " << elapsed_time.count() << " milliseconds" << std::endl;
    
    return 0;
}
