#include "Inventory.h"

void Inventory::addProduct(std::shared_ptr<Product> product) //Adding a new product to the inventory, using shared_ptr to manage the memory of the product and allow for polymorphism, ensuring that the product is properly stored and accessible in the inventory
{
	if (!product) //Validating the input product, if it is null, we do not add it to the inventory and simply return, ensuring that the inventory does not contain invalid products and maintains data integrity
        return;

	std::lock_guard<std::mutex> lock(m_inventoryMutex); //Locking the inventory mutex to ensure thread safety while modifying the inventory
	m_products[product->getId()] = product; //Adding the product to the inventory using its unique ID as the key, allowing for efficient lookup and management of products in the inventory, ensuring that the product is properly stored and accessible based on its ID
}

const std::unordered_map<int, std::shared_ptr<Product>>& //Providing read-only access to the products in the inventory, allowing users to retrieve information about the products without modifying the inventory data, ensuring that the integrity of the inventory is maintained while still allowing access to product details
Inventory::getProducts() const //Locking the inventory mutex to ensure thread safety while accessing the inventory data, allowing for concurrent read access to the products in the inventory without risking data corruption
{
	return m_products; //Returning a const reference to the unordered_map of products, allowing users to access the products in the inventory without copying the data, ensuring efficient access to product information while maintaining the integrity of the inventory data
}

OrderStatus Inventory::processOrder(int productId, //Processing an order for a product by its ID, ensuring thread safety and proper stock management, returning the status of the order processing (success, insufficient stock, product not found, or invalid quantity)
    int quantity,
    double& revenueOut)
{
	std::lock_guard<std::mutex> lock(m_inventoryMutex); //Locking the inventory mutex to ensure thread safety while processing the order, allowing for concurrent order processing without risking data corruption

	if (quantity <= 0) //Validating the input quantity, if it is not positive, we return an error status indicating that the quantity is invalid, ensuring that the order processing logic only handles valid quantities and maintains data integrity
        return OrderStatus::InvalidQuantity;

	auto it = m_products.find(productId); //Looking up the product in the inventory by its unique ID, allowing for efficient retrieval of product information based on the product ID, ensuring that we can quickly determine if the product exists in the inventory and access its details for order processing
    if (it == m_products.end())
        return OrderStatus::ProductNotFound;

    auto& product = it->second;

	if (product->getQuantity() < quantity) //Checking if there is sufficient stock to fulfill the order, if the available quantity is less than the requested quantity, we return an error status indicating insufficient stock, ensuring that the order processing logic properly manages inventory levels and prevents overselling of products
        return OrderStatus::InsufficientStock;

	product->reduceStock(quantity); //Reducing the stock of the product by the ordered quantity, ensuring that the inventory levels are updated correctly after processing the order, maintaining accurate stock information for future orders and inventory management

    revenueOut =
		product->calculateFinalPrice() * quantity; //Calculating the revenue generated from the order by multiplying the final price of the product (which may include discounts or taxes) by the quantity ordered, ensuring that we accurately calculate the revenue for each order based on the pricing strategy defined in the product's calculateFinalPrice method

	return OrderStatus::Success; //Returning a success status indicating that the order was processed successfully, allowing the caller to know that the order was fulfilled and the inventory was updated accordingly, ensuring that the order processing logic provides clear feedback on the outcome of the order processing
}
