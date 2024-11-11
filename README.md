# Linear and Parallel Quicksort

## Table of contents
* [Mock Data Script](/docs/mockData.md)
* [Partitioning the Vector](/docs/partition.md)
* [Thread Pool](/docs/threadPool.md)
* [Linear Quicksort](#linear-quicksort)
* [Parallel Quicksort](#parallel-quicksort)

This code tests the performance of a linear quicksort against its parallel conterpart to see which can compete the task of sorting a vector of integers faster.

## How to Run The Program

1. The program runs best in Microsoft Visual Studio. To open the project, click on `\30051129_CS3S666\30051129_CS3S666.vcxproj`
2. Once the progam is open in Visual Studio, click the green play button or go to the `Debug` dropdown menu and select `run without debugging`.

<p align="center">
<img src="/docs/assets/startProgam.jpg" alt="how to press play" >
</p>

## Output

The progam outputs to the console and displays the size of the vector and the amount of time (millisecond) it took for the quicksort to complete the task.

<p align="center">
<img src="/docs/assets/output.jpg" alt="how to press play" >
</p>

The vector size can be manipulated on `line 43` of `30051129_CS3S666.cpp` by changing the value of `int amount`. This is also how results were determined for the graph located in the report. Each value on the graphs x-axis was manually inserted and exected to record the results.

Two vector are created using the `mockData` function. find out more a bout [mock data here](/docs/mockData.md). 
```cpp
    vector<int> linear = mockData(amount);
    vector<int> parallel = mockData(amount);
```
Next a `ThreadPool` object is made with the desired amount of thread to be pooled. See more about [ThreadPool](/docs/threadPool.md) implementation. In this case, the hardware capability is requested with `thread::hardware_concurrency().` 

Then, both quicksorts are executed and measured by a start and finish timestamp 
```cpp
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

```
(`chrono::high_resolution_clock::now()`), which is subtracted to find the duration of execution.
This is printed to the console. The time is multiplied to display milliseconds.

Below is the implementation of `main()`.

```cpp
int main() {

    int amount = 100000;

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
```

# Linear Quicksort

A linear or sequential quicksort sorts a data structure by dviding it into two partitions by selecting a `pivot` element and then placing all values less than itself on the `left` and values greater on the `right`. It does this recursively and usually sorts the left side, followed by the right. The process continues until the data structure is fully sorted. 

```cpp
void quicksortLinear(vector<int>& mockData, int low, int high) {

    if (low < high) {
        // Partition the array and get the pivot index
        int pi = partition(mockData, low, high);

        // Recursively apply quicksort to the left and right of the pivot
        quicksortLinear(mockData, low, pi - 1);
        quicksortLinear(mockData, pi + 1, high);
    }
}
```
`quicksortLinear` uses the `partition` function to find its pivot and then sorts the 'left` side recursively, followed by the right. To click throught to see the [partition function](/docs/partition.md).

# Parallel Quicksort

Parallel quicksort is an adaptation of quicksort that performs sorting tasks simultaneously across multiple threads or processors. Like quicksort, it selects a `pivot`, partitions elements into subarrays, and sorts recursively. In the parallel approach, each partition can be handled as a separate task, allowing `left` and `right` subarrays to be sorted concurrently. This parallelism accelerates sorting by dividing the workload and distributing it across available resources. A `Thread pool` is used to manage thread allocation. 

```cpp
void quicksortParallel(vector<int>& mockData, int low, int high, ThreadPool& pool) {

    if (low < high) {
        int pi = partition(mockData, low, high);

        // Enqueue parallel tasks for each partition.
        // Runs the function using a lambda expression with args in [].
        // Uses a Thread Pool to manage thread depth.
        pool.enqueue([&mockData, low, pi, &pool] { quicksortParallel(mockData, low, pi - 1, pool); });
        pool.enqueue([&mockData, pi, high, &pool] { quicksortParallel(mockData, pi + 1, high, pool); });
    }
}
```

`quicksortParallel` also uses the `partition` function to find its `pivot`. click through to the [partition function.](/docs/partition.md)

# Implementation

```cpp
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

    int amount = 100000;

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

```


[return to top.](#linear-and-parallel-quicksort)