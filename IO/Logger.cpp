#include "Logger.h"
#include <fstream>
#include <mutex>

static std::mutex logMutex;

void Logger::log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream file("orders_log.txt", std::ios::app);
    file << message << std::endl;
}
