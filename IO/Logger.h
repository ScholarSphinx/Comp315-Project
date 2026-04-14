#pragma once
#include <string>

class Logger
{
public:
    static void logOrder(int orderNumber,
        const std::string& productName,
        int quantity,
        bool success);

    static void logError(const std::string& message);
};