#include "threadPool.h"

using namespace std;

// Constructor: starts the specified number of worker threads
ThreadPool::ThreadPool(int numThreads) {
    for (int i = 0; i < numThreads; ++i) {
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


