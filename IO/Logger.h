#pragma once
#include <string>

class Logger
{
public:
	static void log(const std::string& message); //Logging a message to the console with a timestamp, allowing for tracking of events and actions in the application, ensuring that important information is recorded for debugging and monitoring purposes
};
