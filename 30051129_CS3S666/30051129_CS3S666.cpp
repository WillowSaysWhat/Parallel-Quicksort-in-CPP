#include <iostream>
#include <vector>
#include "partition.h"
#include "mockData.h"
#include "threadPool.h"
#include <functional>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;


void quicksortLinear(vector<int>& mockData, int low, int high) {

    if (low < high) {
        // Partition the array and get the pivot index
        int pi = partition(mockData, low, high);

        // Recursively apply quicksort to the left and right of the pivot
        quicksortLinear(mockData, low, pi - 1);
        quicksortLinear(mockData, pi + 1, high);
    }
}


// Parallel quicksort function
void quicksortParallel(vector<int>& mockData, int low, int high, ThreadPool& pool) {

    if (low < high) {
        int pi = partition(mockData, low, high);

        // Enqueue parallel tasks for each partition.
        // runs the function using a lambda expression with args in [].
        // // Uses a Thread Pool to manage thread depth/ 
        pool.enqueue([&mockData, low, pi, &pool] { quicksortParallel(mockData, low, pi - 1, pool); });
        pool.enqueue([&mockData, pi, high, &pool] { quicksortParallel(mockData, pi + 1, high, pool); });
    }
}


int main() {

    int amount = 1000000;

    vector<int> linear = mockData(amount);
    vector<int> parallel = mockData(amount);

    
    // Create thread pool with number of available threads. This can be lowered if necessary.
    ThreadPool pool(thread::hardware_concurrency());

    // time start
    auto pStart = chrono::high_resolution_clock::now();
    // Sort using parallel quicksort, static cast to prevent size_t to int data wrap.
    quicksortParallel(parallel, 0, static_cast<int>(parallel.size() - 1), pool);
    //time end
    auto pFinish =  chrono::high_resolution_clock::now();


    auto lStart = chrono::high_resolution_clock::now();
    quicksortLinear(linear, 0, static_cast<int>(linear.size() - 1));
    auto lFinish = chrono::high_resolution_clock::now();

    // find the duration
    chrono::duration<double> parallelTime = pFinish - pStart;
    chrono::duration<double> linearTime = lFinish - lStart;

    // display.
    cout << "Parallel at " << amount << " results: " << fixed << std::setprecision(5)<< parallelTime.count() * 1000 << " milliseconds" << endl;
    cout << "Linear at " << amount << " results: " << linearTime.count() * 1000 << " Milliseconds" << endl;
      
}
