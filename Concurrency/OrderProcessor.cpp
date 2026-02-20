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
	const auto& productMap = m_inventory.getProducts(); //Retrieving the products from the inventory, allowing us to access the available products and their details for simulating order processing, ensuring that we can generate orders based on the products currently in the inventory and accurately simulate the order processing logic with the available product data.
    if (productMap.empty())
        return;

	if (!CUMULATIVE_ORDERS) //If cumulative orders is disabled, we reset the metrics before starting the simulation to ensure that we are tracking the metrics for the current simulation run only, allowing us to evaluate the performance and outcomes of each individual simulation without carrying over metrics from previous runs, providing a clear and accurate assessment of the order processing logic for each simulation.
    {
        m_metrics.total.store(0);
        m_metrics.success.store(0);
        m_metrics.failed.store(0);
    }

	std::vector<std::shared_ptr<Product>> products; //Creating a vector to store shared pointers to products, allowing us to easily access and manage the products for simulating order processing, ensuring that we can efficiently generate orders based on the available products in the inventory and maintain proper memory management through the use of shared pointers.
    products.reserve(productMap.size());

	for (const auto& [id, product] : productMap) //Iterating through the products in the inventory and adding them to the vector of products, allowing us to create a convenient collection of products for simulating order processing, ensuring that we can easily access the products for generating orders and processing them in the simulation, providing a structured way to manage the product data for the order processing logic.
        products.push_back(product);

    int actualThreads =
		std::min(THREAD_COUNT, MAX_CONCURRENT_THREADS); //Determining the actual number of threads to use for the simulation by taking the minimum of the configured THREAD_COUNT and the MAX_CONCURRENT_THREADS defined in the configuration, ensuring that we do not exceed the maximum allowed number of concurrent threads while still utilizing as many threads as possible for efficient order processing, providing a balance between performance and resource management in the simulation.

    int ordersPerThread =
		TOTAL_ORDERS / actualThreads; //Calculating the number of orders each thread will process by dividing the total number of orders by the actual number of threads, ensuring that the workload is evenly distributed among the threads for efficient order processing, allowing us to simulate concurrent order processing with a balanced distribution of orders across the threads, providing a realistic simulation of how the order processing system would perform under concurrent conditions.

    //Worker Lambda
	auto worker = [this, &products, ordersPerThread]() //Defining a worker lambda function that will be executed by each thread in the simulation, allowing us to encapsulate the logic for processing orders in a concurrent environment. This lambda function will generate random orders for products and process them using the inventory's processOrder method, while also updating the metrics for total, successful, and failed orders in a thread-safe manner, ensuring that we can simulate realistic order processing with concurrent threads while maintaining accurate tracking of the outcomes of each order.
        {
            std::random_device rd;
			std::mt19937 gen(rd()); //Using a random number generator to create random orders for the simulation, allowing us to simulate realistic order processing with varying products and quantities, ensuring that we can test the order processing logic under diverse conditions and evaluate its performance and outcomes with a range of different orders, providing a comprehensive assessment of the order processing system's behavior in a concurrent environment.

			std::uniform_int_distribution<> productDist( //Creating a uniform integer distribution to randomly select products from the vector of products, allowing us to generate random orders for different products in the simulation, ensuring that we can test the order processing logic with a variety of products and evaluate its performance and outcomes across different product scenarios, providing a more comprehensive assessment of the order processing system's behavior in a concurrent environment.
                0, products.size() - 1);

			std::uniform_int_distribution<> qtyDist( //Creating a uniform integer distribution to randomly generate order quantities within the defined minimum and maximum order quantity range, allowing us to simulate realistic order processing with varying quantities for each order, ensuring that we can test the order processing logic under diverse conditions and evaluate its performance and outcomes with a range of different order quantities, providing a comprehensive assessment of the order processing system's behavior in a concurrent environment.
                MIN_ORDER_QUANTITY,
                MAX_ORDER_QUANTITY);

			for (int i = 0; i < ordersPerThread; ++i) //Iterating through the number of orders each thread will process, allowing us to generate and process the specified number of orders for each thread in the simulation, ensuring that we can simulate concurrent order processing with a balanced distribution of orders across the threads, providing a realistic simulation of how the order processing system would perform under concurrent conditions.
            {
                m_metrics.total.fetch_add(1);

				auto product = products[productDist(gen)]; //Randomly selecting a product from the vector of products using the product distribution, allowing us to generate random orders for different products in the simulation, ensuring that we can test the order processing logic with a variety of products and evaluate its performance and outcomes across different product scenarios, providing a more comprehensive assessment of the order processing system's behavior in a concurrent environment.

                double revenue = 0.0;

				OrderStatus status = //Processing the order for the selected product with a random quantity using the inventory's processOrder method, allowing us to simulate realistic order processing with varying products and quantities, ensuring that we can test the order processing logic under diverse conditions and evaluate its performance and outcomes with a range of different orders, providing a comprehensive assessment of the order processing system's behavior in a concurrent environment. The status of the order processing will be used to update the metrics for successful and failed orders accordingly.
                    m_inventory.processOrder(
                        product->getId(),
                        qtyDist(gen),
                        revenue
                    );

				if (status == OrderStatus::Success) //Updating the metrics for successful and failed orders based on the status of the order processing, allowing us to track the outcomes of each order in a thread-safe manner using atomic operations, ensuring that we can accurately monitor the performance and success rate of the order processing system under concurrent conditions, providing insights into the effectiveness of the order processing logic and its ability to handle concurrent orders while maintaining accurate metrics.
                    m_metrics.success.fetch_add(1);
                else
                    m_metrics.failed.fetch_add(1);
            }
        };

	if (THREAD_POOL_MODE) //If thread pool mode is enabled, we create a thread pool and enqueue the worker lambda function for each thread, allowing us to manage the execution of the worker threads efficiently and ensure that all tasks are completed before proceeding, providing a structured way to handle concurrent order processing with a thread pool while maintaining accurate tracking of metrics and proper synchronization of threads in the simulation.
    {
        ThreadPool pool(actualThreads);

		for (int i = 0; i < actualThreads; ++i) //Enqueuing the worker lambda function for each thread in the thread pool, allowing us to manage the execution of the worker threads efficiently and ensure that all tasks are completed before proceeding, providing a structured way to handle concurrent order processing with a thread pool while maintaining accurate tracking of metrics and proper synchronization of threads in the simulation.
            pool.enqueue(worker);

		pool.wait(); //Waiting for all tasks in the thread pool to complete, allowing us to ensure that all worker threads have finished processing their assigned orders before proceeding with further actions in the simulation, providing a way to manage the execution flow of the application based on the completion of tasks in the thread pool and ensuring that we can accurately track the outcomes of all orders processed by the worker threads in a concurrent environment.
    }
    else
    {
		std::vector<std::thread> threads; //Creating a vector to store the worker threads, allowing us to manage and keep track of the threads in the simulation, ensuring that we can properly join them after they have completed their tasks, providing a structured way to handle concurrent order processing with individual threads while maintaining accurate tracking of metrics and proper synchronization of threads in the simulation.
        threads.reserve(actualThreads);

		for (int i = 0; i < actualThreads; ++i) //Creating and launching a new thread for each worker lambda function, allowing us to simulate concurrent order processing with individual threads while maintaining accurate tracking of metrics and proper synchronization of threads in the simulation. Each thread will execute the worker lambda function, which generates and processes orders for the assigned number of orders per thread, providing a realistic simulation of how the order processing system would perform under concurrent conditions with individual threads.
            threads.emplace_back(worker);

		for (auto& t : threads) //Joining each thread to ensure that they have completed their tasks before proceeding, allowing us to manage the execution flow of the application based on the completion of threads and ensuring that we can accurately track the outcomes of all orders processed by the worker threads in a concurrent environment, providing a structured way to handle concurrent order processing with individual threads while maintaining accurate tracking of metrics and proper synchronization of threads in the simulation.
			if (t.joinable()) //Checking if the thread is joinable before attempting to join it, ensuring that we only join threads that have been successfully created and are still running, preventing potential issues with joining threads that may have already completed or were not properly initialized, providing a safe and robust way to manage thread synchronization in the simulation.
				t.join(); //Joining the thread to ensure that it has completed its tasks before proceeding, allowing us to manage the execution flow of the application based on the completion of threads and ensuring that we can accurately track the outcomes of all orders processed by the worker threads in a concurrent environment, providing a structured way to handle concurrent order processing with individual threads while maintaining accurate tracking of metrics and proper synchronization of threads in the simulation.
    }
}



const Metrics& OrderProcessor::getMetrics() const
{
	return m_metrics; //Returning a reference to the metrics object, allowing users to access the total number of orders processed, successful orders, and failed orders in a thread-safe manner using atomic operations, ensuring that we can provide accurate and up-to-date metrics for monitoring the performance and outcomes of the order processing system under concurrent conditions, providing insights into the effectiveness of the order processing logic and its ability to handle concurrent orders while maintaining accurate metrics.
}
