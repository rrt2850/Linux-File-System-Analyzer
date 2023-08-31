/******************************************************************************
 * File: ThreadPool.cpp
 * Description: A basic thread pool implementation.
 * Author: Robert Tetreault
 * Note: This code is slightly borrowed from code I found on StackOverflow.
 ******************************************************************************/

#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

//
//  Constructors and Destructors
//

ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) { 
    // if no argument is passed, use the number of hardware threads
}

ThreadPool::ThreadPool(size_t numThreads) : activeJobs(0), numThreads(numThreads), stop(false)  {
    // Create the worker threads and add them to the list
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(workerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    // Lock the queue and set the stop flag to true
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    // Notify all threads that stop is true
    condition.notify_all();

    // Join all threads
    for (std::thread &worker : workers) {
        worker.join();
    }
}

//
//  Public Methods
//

/******************************************************************************
 * enqueue: Adds a task to the queue and notifies a waiting worker thread.
 * 
 * @param task: The task to be added to the queue
 ******************************************************************************/
void ThreadPool::enqueue(std::function<void()> task) {
    // Lock the queue and add the task
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        // Wait until there's an available thread
        tasks.push([&, task]() { 
            task();         // Execute the task
            if (--activeJobs == 0) {            // Decrement the number of active jobs and check if it's 0
                everythingDone.notify_all();    // Notify the main thread that all tasks are complete
            }
        });

        // Increment the number of active jobs while the task is being executed
        activeJobs++;
    }

    // Notify a waiting thread
    condition.notify_one();
}


/******************************************************************************
 * workerThread: The function that each worker thread runs. It waits until
 *               there's a task in the queue, then executes it.
 * 
 * @param pool: A pointer to the thread pool
 ******************************************************************************/
void ThreadPool::workerThread(ThreadPool *pool) {
    std::function<void()> task;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(pool->queueMutex);

            // Wait until there's a task in the queue or we are stopping the threadpool
            pool->condition.wait(lock, [pool] { return pool->stop || !pool->tasks.empty(); });

            // Check if we are stopping the threadpool and the queue is empty
            if (pool->stop && pool->tasks.empty()) {
                return;
            }

            // If we get here, there's a task in the queue
            task = std::move(pool->tasks.front());      // Get the task
            pool->tasks.pop();                          // Remove the task from the queue
        }
        task(); // Execute the task
    }
}

/******************************************************************************
 * waitForCompletion: Waits until all tasks in the queue have been completed.
 ******************************************************************************/
void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(queueMutex);
    everythingDone.wait(lock, [this]() { return activeJobs == 0; });
}

