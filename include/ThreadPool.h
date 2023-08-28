/******************************************************************************
 * File: ThreadPool.h
 * Description: A basic thread pool implementation.
 * Author: Robert Tetreault
 ******************************************************************************/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
    public:
        ThreadPool();
        ThreadPool(size_t numThreads);
        ~ThreadPool();

        void enqueue(std::function<void()> task);
        void waitForCompletion();

        std::atomic<int> activeJobs;                // The number of tasks that are currently being processed

    private:
        static void workerThread(ThreadPool *pool);     

        std::vector<std::thread> workers;           // A list of threads in the pool
        std::queue<std::function<void()>> tasks;    // A queue of tasks to be executed
        
        std::mutex queueMutex;                      // A mutex to lock the queue
        std::condition_variable condition;          // A condition variable to notify threads when a task is available
        std::condition_variable everythingDone;     // A condition variable to notify the main thread when all tasks are complete

        size_t numThreads;                          // The number of threads in the pool
        bool stop = false;                          // A flag to stop the threads
};

#endif
