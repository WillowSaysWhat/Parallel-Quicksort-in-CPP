# Thread Pool Class

<p align="center">
<img src="/docs/assets/ThreadPoolGIF.gif" alt="partitioning" >
</p>

A thread Pool manages threads so to prevent thread max from occuring. This allows the sort to manage the number of threads and monitoring task by queueing them in a First In First Out data structure.

This was very difficult to build. At first I tried to create a very simple function, but it soon became apparent that a thread pool is a class. In the end, I gaffa-tapped a working thread pool thanks to this Stack Overflow search: 

https://stackoverflow.com/search?q=thread+pool+c%2B%2B&s=7b2b29a3-79c4-4ae8-b44d-5926fd838067 

Without these forum posts, the thread pool would likely have not been completed. 

## Implementation

The thread pool class accepts a number (`int`) that decides how many threads the pool should implement. This is accepted upon initialisation. The Constructor creates `numThreads` number of threads and places them in the pool using a lambda expression. 

I've just discovered Lambda in C++ please forgive me! 

```cpp

// threadPool.h
ThreadPool(int numThreads);

//threadPool.cpp
ThreadPool::ThreadPool(size_t numThreads) {
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this] { worker_thread(); });
    }
}

```

The ~destructor joins all the threads and stops the pool.
```cpp
std::unique_lock<std::mutex> lock(queueMutex);
stop = true;
```
unique_lock<mutex> lock(queueMutex);: Acquires a unique lock on queueMutex, which ensures exclusive access to shared resources (such as the task queue and stop flag) by this thread. This prevents other threads from modifying these resources while the shutdown process is underway.
stop = true;: Sets the stop flag to true, which is a signal to all worker threads that they should exit once they complete their current tasks. When stop is true, worker threads will not wait for or pick up any new tasks.


```cpp
condition.notify_one();
for (thread& worker : threads)
        worker.join();
```

`condition.notify_all();` Wakes up all worker threads that may be waiting for tasks. Since stop is now true, the waiting threads will break out of their waiting loops and exit instead of attempting to pick up new tasks. This loop iterates over each thread in the threads vector and calls join on each one.

`worker.join();` Waits for the thread worker to complete its execution. By joining all threads, we ensure that the destructor waits until each worker thread has fully exited before continuing.
This guarantees that all worker threads are fully stopped before the ThreadPool object is destroyed.

```cpp
// Enqueue a task for the thread pool
void ThreadPool::enqueue(function<void()> task) {
    {
        unique_lock<mutex> lock(queueMutex);
        tasks.push(task);
    }
    condition.notify_one();
}
```
This code defines the enqueue function for a ThreadPool class. The purpose of this function is to add a new task to the pool's task queue so that one of the worker threads can execute it. 

The enqueue function takes a single parameter, task, of type `std::function<void()>`.
This task is a function object that takes no arguments and returns nothing (void). It can represent any callable object, such as a lambda, function pointer, or std::bind expression, allowing flexible use of different types of tasks.

A `std::unique_lock` is created with queueMutex, a mutex used to protect access to the shared task queue (tasks).
By locking queueMutex, we ensure that only one thread can modify the tasks queue at a time, preventing data races if multiple threads try to enqueue tasks concurrently.

`tasks.push(task);:` Adds the task to the end of the tasks queue.
tasks is a queue that holds pending tasks for the worker threads to execute. By pushing the task into this queue, it becomes available for one of the worker threads to pick up.

When the block containing `unique_lock<mutex> lock(queueMutex);` ends, lock goes out of scope, and the mutex queueMutex is automatically unlocked.
This releases exclusive access to tasks, allowing other threads to access it if needed.

`condition.notify_one();:` Signals one of the worker threads that a new task has been added to the queue.
`notify_one()` wakes up a single worker thread that is currently waiting on the condition variable condition.
The worker thread that is awakened will acquire the lock on `queueMutex`, check the tasks queue, and begin executing the task that was just enqueued.

```cpp
void ThreadPool::worker_thread() {
    while (true) {
        function<void()> task;
        {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop&& tasks.empty())
                return;
            task = move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
```
This code defines the `worker_thread` function of the ThreadPool class, which is the main loop for each worker thread in the pool. The `worker_thread` function continuously waits for and executes tasks from the task queue. Let’s go through it step-by-step:

The function runs in an infinite loop, allowing the worker thread to continually check for and process tasks in the tasks queue.
The loop only exits when stop is true and the tasks queue is empty, ensuring that all tasks are completed before the thread exits.

A `std::unique_lock` is created with queueMutex, which ensures exclusive access to the shared task queue (tasks) and the stop flag.
This lock protects the critical section where the worker checks and manipulates the tasks queue.

The `condition.wait()` call makes the thread wait until either:
stop is true, indicating that the thread pool is being shut down.
The tasks queue is not empty, meaning there is a task ready to be processed.
The lambda `[this] { return stop || !tasks.empty(); }` is a predicate that determines when the thread should stop waiting. If stop is true or tasks is not empty, the thread wakes up.

After waking up, the thread checks if stop is true and tasks is empty.
If both conditions are met, it means:
No more tasks will be added (since stop is true).
There are no tasks left to execute (`tasks.empty()`).
Therefore, the thread can safely exit the worker_thread function by returning, ending its execution.

If there is a task in the queue, it is retrieved using `tasks.front()` and moved to task using `std::move.`
`tasks.pop()` removes the task from the queue after it has been assigned, making space for new tasks and ensuring each task is executed only once.

Once the `unique_lock` goes out of scope at the end of the inner block, queueMutex is unlocked.
This allows other threads to access the tasks queue or add new tasks.

The worker thread executes the retrieved task by calling `task()`.
Since task is a `std::function<void()>`, it can represent any callable object that takes no arguments and returns void, allowing flexibility in the type of tasks the thread pool can handle.



## threadPool.h
```cpp
// threadPool.h
#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
    ThreadPool(int numThreads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;

    void worker_thread();
};

```
## threadPool.cpp

```cpp

#include "threadPool.h"

using namespace std;

// Constructor: starts the specified number of worker threads
ThreadPool::ThreadPool(int numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back([this] { worker_thread(); });
    }
}

// Destructor: joins all threads and stops the pool
ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (thread& worker : threads)
        worker.join();
}

// Enqueue a task for the thread pool
void ThreadPool::enqueue(function<void()> task) {
    {
        unique_lock<mutex> lock(queueMutex);
        tasks.push(task);
    }
    condition.notify_one();
}

// Worker thread function
void ThreadPool::worker_thread() {
    while (true) {
        function<void()> task;
        {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop&& tasks.empty())
                return;
            task = move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
```


