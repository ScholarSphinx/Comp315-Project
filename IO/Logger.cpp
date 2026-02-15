#include "Logger.h"
#include <fstream>
#include <iostream>

std::mutex Logger::m_mutex;

void Logger::log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    std::ofstream file("orders_log.txt", std::ios::app);

    if (!file.is_open())
    {
        std::cerr << "ERROR: Cannot open log file.\n";
        return;
    }

    file << message << std::endl;
}
