/*
 Sleeping Barber Problem:
 1. one barber’s chair means that the barber finishes cutting a customer’s hair, that customer pays and then
the barber looks to see if there are customers waiting for a haircut.
2. If so, he puts the customer who first arrived into the barber’s chair and cuts their hair.
3. If there are none, the barber takes a nap in his barber chair.
4. When a customer arrives, they look to see what the barber is doing at that time.
5. If they are sleeping, they wake them up and the customer gets their hair cut.
6. If the barber is cutting hair, the customer checks to see if there is an available seat.
7. If there is a seat, they take the seat and wait their turn for a haircut.
8. If no seat is available, the customer leaves and goes down the street to Great Clips.
9. The problem here is that a number of actions take a random amount of time:
    - The barber requires a random amount of time to do a haircut
    - The customer takes a random amount of time to get to a waiting-room chair (customers do like to talk).

The room status forms a critical section:
the barber must acquire a room status mutex before checking for customers and release the mutex when
they begin to sleep or cut hair.
A customer must acquire the room status mutex when they enter the shop and release it once they sit in a
waiting room chair, or they sit in the barber chair, or if they leave if no seats are available.
 
 2 threads, 2 mutexes, 1 sempahore
 
 
*/

// References: https://github.com/KajolAch/shift-sleeping-barber-problem/blob/master/Solution.cpp
// https://github.com/Stolichnayer/sleeping_barber/blob/master/sleeping_barber.c

#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <semaphore.h>


// Mutexes
std::mutex barberReady;
std::mutex waitChairsCanBeAccessed;



// Semaphore
sem_t readyCustomers;

/* Global Variables to be updated by multiple functions */
std::condition_variable cv;
int chair_cnt;
int total_custs;
int available_seats;
int num_Cuts;
int no_served_custs = 0;
std::string data;
bool ready = false;
bool processed = false;
bool hc_done = true;

// FUNCTION: Generate a random integer between 100 and 1000 to be used as time
int randNumGen(int val_1, int val_2){
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> uD(100,1000);
    int num = uD(gen);
    return num;
}

// FUNCTION: Barber
void Barber() {
    // Wait until main() sends data
    if(readyCustomers == 0){
        std::unique_lock<std::mutex> lk(barberReady);
        std::cout << "Barber is sleeping" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(randNumGen(10, 100)));
        sem_wait(&readyCustomers);
        
        data += " after processing";
        
        // Send data back to main()
        processed = true;
        //std::cout << "Worker thread signals data processing completed\n";
        
        // Manual unlocking is done before notifying, to avoid waking up
        // the waiting thread only to block again (see notify_one for details)
        lk.unlock();
        sem_post(&readyCustomers);
    }
    else if (readyCustomers > 0){
        available_seats++;
        waitChairsCanBeAccessed.unlock();
        barberReady.unlock();
    }
}


// FUNCTION: Customer
void Customer()
{
    //std::unique_lock<std::mutex> lk(waitChairsCanBeAccessed);
    hc_done = false;
    // Customer is able to get hair cut
    if(available_seats > 0){
        waitChairsCanBeAccessed.lock();
        available_seats--;
        num_Cuts++;
        std::cout << "Customer " << num_Cuts << " is getting HairCut" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(randNumGen(10, 50)));
        sem_post(&readyCustomers);
        waitChairsCanBeAccessed.unlock();
    }
    // Not enough available seats and customer leaves disappointed
    else{
        std::cout << "Customer leaving disappointed" << std::endl;
        no_served_custs++;
        waitChairsCanBeAccessed.unlock();
    }
}

int main()
{
    std::cout << "Please enter seats in the waiting room: ";
    std::cin >> chair_cnt; //number of seats
    available_seats = chair_cnt;

    // Initialize Barber Mutex as ready
    barberReady.unlock();
    std::thread barber_Joe(Barber);

    // send data to the barber thread
    {
        std::lock_guard<std::mutex> lk(barberReady);
        ready = true;
    }
    barberReady.unlock();

    int num_threads = randNumGen(100, 1000);


    // Shop Opening!!!
    // Launch Customer threads
    std::vector<std::thread> appt_list;
    for (int i = 0; i < num_threads; ++i) {
        appt_list.push_back(std::thread { Customer } );
        std::this_thread::sleep_for(std::chrono::milliseconds(randNumGen(100, 200)));

    }

    barber_Joe.join();

    for (int i = 0; i < num_threads; ++i) {
        appt_list[i].join();
    }
    
    // End of Day report
    std::cout << std::endl << std::endl << "Daily Summary" << std::endl;
    // Print the number of customers serviced
    std::cout << num_Cuts << " customers had their hair cut" << std::endl;

    // Print the number of customers who left
    std::cout << no_served_custs << " customers left the shop" << std::endl;
    return 0;
}


//
//    for (auto &i : appt_list) {
//        i.join();
//    }
