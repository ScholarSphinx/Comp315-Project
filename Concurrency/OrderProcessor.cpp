#include "OrderProcessor.h"
#include <algorithm>
#include "../Config.h"
#include "../Core/ThreadPool.h"


OrderProcessor::OrderProcessor(Inventory& inventory)
    : m_inventory(inventory)
{
}

void OrderProcessor::simulate()
{
    const auto& productMap = m_inventory.getProducts();
    if (productMap.empty())
        return;

    if (!CUMULATIVE_ORDERS)
    {
        m_metrics.total.store(0);
        m_metrics.success.store(0);
        m_metrics.failed.store(0);
    }

    std::vector<std::shared_ptr<Product>> activeProducts;
    activeProducts.reserve(productMap.size());

    for (const auto& [id, product] : productMap)
    {
        // Only add to simulation list if status is 1 (Active)
        if (product->isActive())
        {
            activeProducts.push_back(product);
        }
    }

    // If all products are deactivated (status 0), exit simulation early
    if (activeProducts.empty())
        return;


    int actualThreads = std::min(THREAD_COUNT, MAX_CONCURRENT_THREADS);
    int ordersPerThread = TOTAL_ORDERS / actualThreads;

    //Worker Lambda
    auto worker = [this, activeProducts, ordersPerThread]()
        {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<> productDist(
                0, static_cast<int>(activeProducts.size()) - 1);

            std::uniform_int_distribution<> qtyDist(
                MIN_ORDER_QUANTITY,
                MAX_ORDER_QUANTITY);

            for (int i = 0; i < ordersPerThread; ++i)
            {
                // Increment total orders
                m_metrics.total.fetch_add(1);

                // Generate Order Data
                auto product = activeProducts[productDist(gen)];

                int quantity = qtyDist(gen);

                int orderId = m_orderCounter.fetch_add(1) + 1;

                double revenue = 0.0;

                // Process Order (Thread Safe)

                OrderStatus status =
                    m_inventory.processOrder(
                        product->getId(),
                        quantity,
                        revenue
                    );

                bool success = (status == OrderStatus::Success);

                // Update Metrics (Atomic)
                if (success)
                    m_metrics.success.fetch_add(1);
                else
                    m_metrics.failed.fetch_add(1);
                //LOG TO FILE

                Logger::logOrder(
                    orderId,
                    product->getName(),
                    quantity,
                    success
                );

                //SEND TO UI (Order History)

                if (onOrderProcessed)
                {
                    std::string logText =
                        "Order #" + std::to_string(orderId) +
                        " | " + product->getName() +
                        " | Qty: " + std::to_string(quantity) +
                        " | " + (success ? "SUCCESS" : "FAILED");

                    onOrderProcessed(logText);
                }
            }
        };


    ThreadPool pool(actualThreads);

	for (int i = 0; i < actualThreads; ++i) //Enqueuing the worker lambda function for each thread in the thread pool, allowing us to manage the execution of the worker threads efficiently and ensure that all tasks are completed before proceeding, providing a structured way to handle concurrent order processing with a thread pool while maintaining accurate tracking of metrics and proper synchronization of threads in the simulation.
        pool.enqueue(worker);

	pool.wait(); //Waiting for all tasks in the thread pool to complete, allowing us to ensure that all worker threads have finished processing their assigned orders before proceeding with further actions in the simulation, providing a way to manage the execution flow of the application based on the completion of tasks in the thread pool and ensuring that we can accurately track the outcomes of all orders processed by the worker threads in a concurrent environment.

    
}



const Metrics& OrderProcessor::getMetrics() const
{
	return m_metrics; //Returning a reference to the metrics object, allowing users to access the total number of orders processed, successful orders, and failed orders in a thread-safe manner using atomic operations, ensuring that we can provide accurate and up-to-date metrics for monitoring the performance and outcomes of the order processing system under concurrent conditions, providing insights into the effectiveness of the order processing logic and its ability to handle concurrent orders while maintaining accurate metrics.
}
