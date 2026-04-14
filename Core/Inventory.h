#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>

#include "Product.h"
#include "Order.h"

enum class SortCriteria {
    PriceAsc,
    PriceDesc,
    QtyAsc,
    QtyDesc
};

class Inventory
{
private:
	std::unordered_map<int, std::shared_ptr<Product>> m_products; //Using an unordered_map to store products by their unique ID allows for efficient lookup, addition, and management of products in the inventory, enabling quick access to product information and stock levels based on the product ID
	mutable std::mutex m_inventoryMutex; //Using a mutex to protect access to the inventory data ensures thread safety when multiple threads are processing orders concurrently

public:
    // ---- Sort Products ----
    std::vector<std::shared_ptr<Product>> getSortedProducts(SortCriteria criteria) const;
    // ---- Product management ----
	void addProduct(std::shared_ptr<Product> product); //Adding a new product to the inventory, using shared_ptr to manage the memory of the product and allow for polymorphism, ensuring that the product is properly stored and accessible in the inventory
	void saveToFile(const std::string& filePath); //Saving the current state of the inventory to a file, allowing for persistence of inventory data across application sessions, ensuring that we can maintain an up-to-date and accurate inventory state by writing the current state of the inventory to a file that can be loaded in future runs of the application, providing a way to manage and update the inventory data effectively while ensuring data integrity and consistency across application runs
    const std::unordered_map<int, std::shared_ptr<Product>>&  //Providing read-only access to the products in the inventory, allowing users to retrieve information about the products without modifying the inventory data, ensuring that the integrity of the inventory is maintained while still allowing access to product details
        getProducts() const;

    // ---- Concurrency-safe order processing ----
	OrderStatus processOrder(int productId, //Processing an order for a product by its ID, ensuring thread safety and proper stock management, returning the status of the order processing (success, insufficient stock, product not found, or invalid quantity)
        int quantity,
        double& revenueOut);

	void deactivateProduct(int productId); //Removing a product from the inventory by its unique ID, ensuring that the product is properly removed and the inventory data is updated accordingly, allowing for effective management of products in the inventory and maintaining accurate records of available products
	void restockAll(int mediumThreshold, int defaultAmount, int lowStockAmount); //Increasing the stock of a product by a specified amount, ensuring that the inventory levels are updated correctly when new inventory is added or when correcting stock levels, allowing for proper inventory management and accurate tracking of available products
};
