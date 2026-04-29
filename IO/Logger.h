#pragma once
#include <string>
#include <unordered_map>
#include <thread>


class Logger
{
public:
    static void logOrder(int orderNumber,
        const std::string& productName,
        int quantity,
        bool success,
        std::thread::id threadId);   

    static void logError(const std::string& message);
    static std::string getThreadLabel(std::thread::id threadId);

private:
    static std::unordered_map<std::thread::id, int> threadMap;
    static int threadCounter;
};