#include "Logger.h"
#include <fstream>
#include <mutex>

static std::mutex logMutex;

void Logger::logOrder(int orderNumber,
    const std::string& productName,
    int quantity,
    bool success) //Logging a message to the console with a timestamp, allowing for tracking of events and actions in the application, ensuring that important information is recorded for debugging and monitoring purposes
{
	std::lock_guard<std::mutex> lock(logMutex); //Locking the log mutex to ensure thread safety while logging messages, allowing for concurrent logging from multiple threads without risking data corruption or interleaving of log messages, ensuring that the log output remains coherent and accurate even in a multithreaded environment

    std::ofstream file("orders_log.txt", std::ios::app); //change to json - for order history
    if (!file) return;

    file << "Order #" << orderNumber
        << " | Product: " << productName
        << " | Qty: " << quantity
        << " | Result: " << (success ? "SUCCESS" : "FAILED")
        << "\n";
}
