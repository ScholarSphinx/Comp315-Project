#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workers.emplace_back([this]()
            {
                while (true)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this]()
                            {
                                return stop || !tasks.empty();
                            });

                        if (stop && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                        activeTasks++;
                    }

                    task();

                    activeTasks--;
                    condition.notify_all();
                }
            });
    }
}

ThreadPool::~ThreadPool()
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

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
    }

    condition.notify_one();
}

void ThreadPool::wait()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    condition.wait(lock, [this]()
        {
            return tasks.empty() && activeTasks == 0;
        });
}