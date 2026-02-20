#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
public:
	ThreadPool(size_t threadCount); //Creating a thread pool with a specified number of worker threads, allowing for concurrent execution of tasks without blocking the main thread, ensuring that we can efficiently manage and execute multiple tasks in parallel while maintaining responsiveness in the application
	~ThreadPool(); //Destroying the thread pool and ensuring that all worker threads are properly joined and resources are cleaned up, allowing for a graceful shutdown of the thread pool and preventing any potential issues with dangling threads or resource leaks when the thread pool is no longer needed

	void enqueue(std::function<void()> task); //Enqueuing a new task to be executed by the thread pool, allowing users to submit tasks for concurrent execution without blocking the main thread, ensuring that we can efficiently manage and execute multiple tasks in parallel while maintaining responsiveness in the application. The task is represented as a std::function, allowing for flexibility in the types of tasks that can be submitted to the thread pool.
	void wait(); //Waiting for all tasks in the thread pool to complete, allowing users to synchronize with the completion of tasks before proceeding with further actions, ensuring that we can manage the execution flow of the application based on the completion of tasks in the thread pool and providing a way to ensure that all tasks have finished before moving on to subsequent operations.

private:
	std::vector<std::thread> workers; //Using a vector to store worker threads allows us to manage and keep track of the threads in the thread pool, enabling us to join them properly during shutdown and ensuring that we can efficiently manage the lifecycle of the worker threads in the thread pool.
	std::queue<std::function<void()>> tasks; //Using a queue to store tasks allows us to manage the order of task execution in the thread pool, ensuring that tasks are executed in the order they were enqueued and providing a simple way to manage and distribute tasks among the worker threads in the thread pool.

	std::mutex queueMutex; //Using a mutex to protect access to the task queue ensures thread safety when multiple threads are enqueuing tasks or worker threads are retrieving tasks for execution, preventing data corruption and ensuring that the task queue is accessed in a synchronized manner.
	std::condition_variable condition; //Using a condition variable allows worker threads to wait for tasks to be enqueued without busy-waiting, providing an efficient way to manage the execution flow of worker threads and ensuring that they are notified when new tasks are available for execution in the thread pool.

	std::atomic<bool> stop{ false }; //Using an atomic boolean to signal when the thread pool is stopping allows for safe communication between threads without the need for additional locking mechanisms, ensuring that worker threads can check the stop condition in a thread-safe manner and allowing for a graceful shutdown of the thread pool when requested.
	std::atomic<int> activeTasks{ 0 }; //Using an atomic integer to track the number of active tasks allows for thread-safe tracking of how many tasks are currently being executed by the worker threads, providing a way to determine when all tasks have completed and allowing the wait() function to block until all tasks have finished executing before proceeding with further actions in the application.
};