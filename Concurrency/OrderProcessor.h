#pragma once

#include "../Core/Inventory.h"
#include "../Core/Order.h"
#include "../IO/Logger.h"

#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <functional>

struct Metrics
{
	std::atomic<int> total{ 0 }; //Using atomic integers to track the total number of orders processed, successful orders, and failed orders allows for thread-safe updates to these metrics from multiple threads concurrently processing orders, ensuring that the metrics remain accurate and consistent even in a multithreaded environment
    std::atomic<int> success{ 0 };
    std::atomic<int> failed{ 0 };
	std::function<void(const std::string&)> onOrderProcessed; //Using a std::function to allow for a callback mechanism when an order is processed, enabling the order processor to notify other parts of the application (such as the GUI) about the outcome of each order processing event, allowing for real-time updates and interactions based on the results of order processing, enhancing the responsiveness and interactivity of the application by providing a way to react to order processing events as they occur.
};

class OrderProcessor
{
private:
	Inventory& m_inventory; //Reference to the inventory, allowing the order processor to access the inventory data and process orders based on the products available in the inventory, ensuring that the order processing logic can interact with the inventory to manage stock levels and calculate revenues for orders
	Metrics m_metrics; //Metrics to track the total number of orders processed, successful orders, and failed orders, allowing for monitoring the performance and outcomes of the order processing logic, providing insights into the success rate and efficiency of the order processing system
	std::atomic<int> m_orderCounter{ 0 };

public:
	explicit OrderProcessor(Inventory& inventory); //Constructor for the OrderProcessor class, taking a reference to the inventory as a parameter, allowing the order processor to access and interact with the inventory data for processing orders, ensuring that the order processing logic can manage stock levels and calculate revenues based on the products available in the inventory

	void simulate(); //Simulating the processing of orders in a concurrent environment, allowing for testing and demonstration of the order processing logic under conditions where multiple orders are being processed simultaneously, ensuring that the order processor can handle concurrent order processing while maintaining accurate metrics and proper inventory management
	void saveChangesToEditableFile(); //Saving changes to an editable inventory file, allowing for persistence of modifications made to the inventory during the simulation, such as deactivating products or restocking, ensuring that we can maintain an up-to-date and accurate inventory state across application sessions by writing the current state of the inventory back to a file that can be loaded in future runs of the application, providing a way to manage and update the inventory data effectively while ensuring data integrity and consistency across application runs.
    const Metrics& getMetrics() const; //getters
	std::function<void(const std::string&)> onOrderProcessed; //Using a std::function to allow for a callback mechanism when an order is processed, enabling the order processor to notify other parts of the application (such as the GUI) about the outcome of each order processing event, allowing for real-time updates and interactions based on the results of order processing, enhancing the responsiveness and interactivity of the application by providing a way to react to order processing events as they occur.
};
