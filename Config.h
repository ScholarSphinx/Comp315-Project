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

// --- Restock Amount Default ---
constexpr int RESTOCK_AMOUNT = 5;  //Default restock amount
constexpr int RESTOCK_THRESHOLD = 20; //When stock is lower than 20 restock amount, otherwise restock a smaller amount
constexpr int MEDIUM_STOCK_THRESHOLD = 20; 
constexpr int NO_STOCK_THRESHOLD = 0;
constexpr int LOW_STOCK_THRESHOLD = 10;

// Set Values
constexpr int TAX_RATE = 0.15; //Tax rate applied to products, allowing for the calculation of the final price of products by adding a specified percentage to the base price, ensuring that we can account for taxes in the pricing strategy and provide accurate pricing information to users when they view product details or process orders, enhancing the realism and functionality of the inventory management system by incorporating tax calculations into the product pricing logic.
constexpr int DISC_RATE = 0.10; //Discount rate applied to products, allowing for the calculation of the final price of products by subtracting a specified percentage from the base price, ensuring that we can offer discounts on products and provide accurate pricing information to users when they view product details or process orders, enhancing the realism and functionality of the inventory management system by incorporating discount calculations into the product pricing logic, providing flexibility in how we manage pricing and promotions for products in the inventory.

// --- Thread Display Mode ---
enum class ThreadDisplayMode
{
    Raw,    // Show actual std::thread::id
    Mapped  // Show T1, T2, T3...
};
constexpr ThreadDisplayMode THREAD_DISPLAY_MODE = ThreadDisplayMode::Mapped;

// CSV Path
constexpr const char* INVENTORY_FILE = "Assets/inventory.csv"; //Used as a base
constexpr const char* EDITED_INVENTORY_FILE = "Assets/inventory_editable.csv"; // Used for saving changes to the inventory, such as deactivating products or restocking, allowing us to maintain a persistent record of the inventory state and ensure that changes made during the application's operation are saved and can be loaded in future sessions, providing a way to manage and update the inventory data effectively while ensuring data integrity and consistency across application runs.
constexpr bool EDIT_MODE = true; // Set to true to enable saving changes to the inventory, allowing us to persist modifications such as deactivating products or restocking across application sessions, ensuring that we can maintain an up-to-date and accurate inventory state while providing flexibility in how we manage and update the inventory data based on user interactions and application needs.