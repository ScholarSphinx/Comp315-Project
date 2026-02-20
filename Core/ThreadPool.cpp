#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount) //Creating a thread pool with a specified number of worker threads, allowing for concurrent execution of tasks without blocking the main thread, ensuring that we can efficiently manage and execute multiple tasks in parallel while maintaining responsiveness in the application
{
	for (size_t i = 0; i < threadCount; ++i) //Creating the specified number of worker threads and adding them to the thread pool, allowing for concurrent execution of tasks by multiple threads, ensuring that we can efficiently manage and execute multiple tasks in parallel while maintaining responsiveness in the application. Each worker thread will continuously wait for tasks to be enqueued and execute them as they become available, providing a scalable and efficient way to handle concurrent workloads in the application.
    {
		workers.emplace_back([this]() //Lambda function that defines the behavior of each worker thread, allowing them to continuously wait for tasks to be enqueued and execute them as they become available. This function will run in an infinite loop, where each worker thread will wait for a task to be enqueued, execute it, and then check for the next task until the thread pool is stopped. This design ensures that worker threads can efficiently manage and execute tasks in parallel while maintaining responsiveness in the application.
            {
                while (true)
                {
					std::function<void()> task; //Defining a variable to hold the task that will be executed

                    {
						std::unique_lock<std::mutex> lock(queueMutex); //Locking the queue mutex to ensure thread safety while accessing the task queue, allowing worker threads to safely retrieve tasks for execution without risking data corruption
                        condition.wait(lock, [this]()
                            {
                                return stop || !tasks.empty();
                            });

						if (stop && tasks.empty()) ///Checking if the thread pool is stopping and there are no more tasks to execute, if both conditions are true, we break out of the loop and allow the worker thread to exit, ensuring a graceful shutdown of the thread pool when requested and preventing any potential issues with dangling threads or resource leaks when the thread pool is no longer needed.
                            return;

						task = std::move(tasks.front()); //Retrieving the next task from the front of the task queue, allowing the worker thread to execute it. We use std::move to efficiently transfer ownership of the task from the queue to the local variable, ensuring that we can execute the task without unnecessary copying and allowing for efficient management of tasks in the thread pool.
                        tasks.pop();
						activeTasks++; //Incrementing the count of active tasks to indicate that a task is currently being executed by a worker thread, allowing the wait() function to track how many tasks are still in progress and block until all tasks have completed before proceeding with further actions in the application, ensuring that we can manage the execution flow of the application based on the completion of tasks in the thread pool and providing a way to ensure that all tasks have finished before moving on to subsequent operations.
                    }

                    task();

                    activeTasks--;
					condition.notify_all(); //Notifying all waiting threads that a task has completed, allowing the wait() function to check if all tasks have finished and unblock if there are no more active tasks, ensuring that we can manage the execution flow of the application based on the completion of tasks in the thread pool and providing a way to ensure that all tasks have finished before moving on to subsequent operations in the application.
                }
            });
    }
}

ThreadPool::~ThreadPool() //Destroying the thread pool and ensuring that all worker threads are properly joined and resources are cleaned up, allowing for a graceful shutdown of the thread pool and preventing any potential issues with dangling threads or resource leaks when the thread pool is no longer needed.
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (auto& worker : workers)
        if (worker.joinable())
            worker.join();
}

void ThreadPool::enqueue(std::function<void()> task) //Enqueuing a new task to be executed by the thread pool, allowing users to submit tasks for concurrent execution without blocking the main thread, ensuring that we can efficiently manage and execute multiple tasks in parallel while maintaining responsiveness in the application. The task is represented as a std::function, allowing for flexibility in the types of tasks that can be submitted to the thread pool.
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
		tasks.push(std::move(task)); //Adding the new task to the task queue, allowing it to be executed by one of the worker threads in the thread pool. We use std::move to efficiently transfer ownership of the task into the queue, ensuring that we can manage tasks in the thread pool without unnecessary copying and allowing for efficient execution of tasks in parallel while maintaining responsiveness in the application.
    }

    condition.notify_one();
}

void ThreadPool::wait() //Waiting for all tasks in the thread pool to complete, allowing users to synchronize with the completion of tasks before proceeding with further actions, ensuring that we can manage the execution flow of the application based on the completion of tasks in the thread pool and providing a way to ensure that all tasks have finished before moving on to subsequent operations.
{
    std::unique_lock<std::mutex> lock(queueMutex);
    condition.wait(lock, [this]()
        {
			return tasks.empty() && activeTasks == 0; //Waiting until the task queue is empty and there are no active tasks being executed by worker threads, ensuring that we can manage the execution flow of the application based on the completion of tasks in the thread pool and providing a way to ensure that all tasks have finished before moving on to subsequent operations in the application. This allows users to synchronize with the completion of tasks and ensures that we can safely proceed with further actions once all tasks have been processed.
        });
}