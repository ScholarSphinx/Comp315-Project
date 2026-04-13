#pragma once

// --- Core Simulation Settings ---
constexpr int THREAD_COUNT = 5; //Number of threads to simulate concurrent order processing, allowing for testing the performance and behavior of the order processing system under conditions where multiple orders are being processed simultaneously, ensuring that the system can handle concurrent operations effectively
constexpr int TOTAL_ORDERS = 50; //Total number of orders to simulate, defining the workload for the order processing simulation, allowing for testing the system's performance and behavior under a defined number of orders, ensuring that we can evaluate the efficiency and success rate of the order processing logic with a realistic workload

// --- Behaviour Settings ---
constexpr bool CUMULATIVE_ORDERS = true; //set to false for each simulation to reset metrics

// --- Order Quantity Range ---
constexpr int MIN_ORDER_QUANTITY = 1;
constexpr int MAX_ORDER_QUANTITY = 5;

// --- Concurrency Limiter ---
constexpr int MAX_CONCURRENT_THREADS = 8;


// CSV Path
constexpr const char* INVENTORY_FILE = "Assets/inventory.csv";