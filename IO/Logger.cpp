#include "Logger.h"
#include <fstream>
#include <mutex>
#include <thread> 
#include <unordered_map>
#include "../Config.h"


std::unordered_map<std::thread::id, int> Logger::threadMap;
int Logger::threadCounter = 1;
static std::mutex logMutex;


void Logger::logOrder(int orderNumber,
    const std::string& productName,
    int quantity,
    bool success,
    std::thread::id threadId) //Logging a message to the console with a timestamp, allowing for tracking of events and actions in the application, ensuring that important information is recorded for debugging and monitoring purposes
{
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream file("orders_log.txt", std::ios::app);
    if (!file) return;

    file << "Order #" << orderNumber
        << " | Product: " << productName
        << " | Qty: " << quantity
        << " | Result: " << (success ? "SUCCESS" : "FAILED");

    
    if (THREAD_DISPLAY_MODE == ThreadDisplayMode::Raw)
    {
        file << " | Thread: " << threadId;
    }
    else if (THREAD_DISPLAY_MODE == ThreadDisplayMode::Mapped)
    {
        if (threadMap.find(threadId) == threadMap.end())
        {
            threadMap[threadId] = threadCounter++;
        }

        int threadLabel = threadMap[threadId];
        file << " | Thread: T" << threadLabel;
    }

    file << "\n";
}
void Logger::logError(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream file("error_log.txt", std::ios::app);
    if (!file) return;

    file << "[ERROR]: " << message << "\n";

   
}