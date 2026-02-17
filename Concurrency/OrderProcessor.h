#pragma once

#include "../Core/Inventory.h"
#include "../Core/Order.h"
#include "../IO/Logger.h"

#include <vector>
#include <thread>
#include <atomic>
#include <random>

struct Metrics
{
	std::atomic<int> total{ 0 }; //Using atomic integers to track the total number of orders processed, successful orders, and failed orders allows for thread-safe updates to these metrics from multiple threads concurrently processing orders, ensuring that the metrics remain accurate and consistent even in a multithreaded environment
    std::atomic<int> success{ 0 };
    std::atomic<int> failed{ 0 };
};

class OrderProcessor
{
private:
	Inventory& m_inventory; //Reference to the inventory, allowing the order processor to access the inventory data and process orders based on the products available in the inventory, ensuring that the order processing logic can interact with the inventory to manage stock levels and calculate revenues for orders
	Metrics m_metrics; //Metrics to track the total number of orders processed, successful orders, and failed orders, allowing for monitoring the performance and outcomes of the order processing logic, providing insights into the success rate and efficiency of the order processing system

public:
	explicit OrderProcessor(Inventory& inventory); //Constructor for the OrderProcessor class, taking a reference to the inventory as a parameter, allowing the order processor to access and interact with the inventory data for processing orders, ensuring that the order processing logic can manage stock levels and calculate revenues based on the products available in the inventory

	void simulate(); //Simulating the processing of orders in a concurrent environment, allowing for testing and demonstration of the order processing logic under conditions where multiple orders are being processed simultaneously, ensuring that the order processor can handle concurrent order processing while maintaining accurate metrics and proper inventory management

    const Metrics& getMetrics() const; //getters
};
