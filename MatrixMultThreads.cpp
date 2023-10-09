
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <thread>
#include <pthread.h>

#define SIZE 100 // size of 2 square matrices


// FUNCTION: RANDOM FILL SQUARE DYNAMIC ARRAY
int** fill(int dimension){
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> uD(0,1000);

    // Allocate space for array
    int** arr = new int * [dimension];
    for(int i = 0; i < dimension; ++i)
          arr[i] = new int[dimension];
  
    // Initialize array with random ints
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++){
            int num = uD(gen);
            arr[i][j] = num;
        }
    }

    return arr;
}



// FUCNTION: MULTIPLY MATRICES
int** multMatrix(int dimension, int **a, int **b){
    
    // Allocate space for product array
    int** product = new int * [SIZE];
    for(int i = 0; i < SIZE; ++i)
          product[i] = new int[SIZE];
    
    for(int i = 0; i < SIZE; i++)
        {
            for(int j = 0; j < SIZE; j++)
            {
                product[i][j] = 0;
                for(int k=0; k < SIZE; k++)
                {
                    product[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    
    return product;
}



// PERFORM MATRIX MULTIPLICATION USING SLICE METHOD
void sliceMult(){
    
    
    
}

// FUNCTION: USE THREADS TO COMPUTE PRODUCT
int** pThread(int size, int **a, int **b){
    // Allocate space for product array
    int** product = new int * [SIZE + 1];
    for(int i = 0; i < SIZE; ++i)
          product[i] = new int[SIZE + 1];

    std::vector<std::thread> threads;
    
    for (int i = 0; i < size; i++){
        threads.push_back(std::thread { sliceMult } );
        
    }
    
    
    for (auto& t: threads)
        t.join();
    
    
    return product;
}



int main(int argc, const char * argv[]) {
    // Working with square matrices -> rows and columns == for both arrays
    
    pthread_t threads[SIZE * SIZE];
    
    
    // initialize square matrices using fill function
    int** arr_1 = new int * [SIZE];
    for(int i = 0; i < SIZE; ++i)
          arr_1[i] = new int[SIZE + 1];

    arr_1 = fill(SIZE);
    
    int** arr_2 = new int * [SIZE];
    for(int i = 0; i < SIZE; ++i)
          arr_2[i] = new int[SIZE + 1];

    arr_2 = fill(SIZE);
    
    // Multiply Matrices using first function
    int** prod_1 = new int * [SIZE];
    for(int i = 0; i < SIZE; ++i)
          prod_1[i] = new int[SIZE + 1];
    
    prod_1 = multMatrix(SIZE, arr_1, arr_2);
    
    std::cout << "Product Array using first multiply method:" << std::endl;
    for (int i = 0 ; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            std::cout << prod_1[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    
     // Multiply Matrices using threads
    int** thread_prod = new int * [SIZE];
    for(int i = 0; i < SIZE; ++i)
          thread_prod[i] = new int[SIZE + 1];
    
    thread_prod = pThread(SIZE, arr_1, arr_2);
    
    
    
    
    
    
    
    
    std::cout << "Product Array using thread multiply method:" << std::endl;
    for (int i = 0 ; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            std::cout << prod_1[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
