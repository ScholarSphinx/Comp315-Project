#pragma once

#include <string>
#include "../Core/Inventory.h"

class ProductFileManager
{
public:
    static bool addProductToFile(const std::string& filename,
		const std::shared_ptr<Product>& product); //Adding a product to the file by appending its details in CSV format, allowing us to persist product information in a file for later retrieval and management, ensuring that we can maintain a record of products in the inventory and easily add new products as needed by appending their details to the file, allowing for effective management of products in the inventory through file-based persistence.

    static bool removeProductFromFile(const std::string& filename,
		int productId); //Removing a product from the file by reading the existing product data, filtering out the line corresponding to the specified product ID, and writing the remaining product data back to the file, allowing us to manage the inventory by removing products as needed while maintaining the integrity of the remaining product data in the file, ensuring that we can maintain an accurate and up-to-date record of products in the inventory after removing a product.

    static bool reloadInventory(const std::string& filename,
		Inventory& inventory); //Reloading the inventory from the file by reading the product data and updating the inventory accordingly, allowing us to synchronize the in-memory inventory with the persisted product data in the file, ensuring that we can maintain an accurate and up-to-date representation of the inventory based on the contents of the file, allowing for effective management of products in the inventory through file-based persistence.
    static void updateProductActiveStatus(const std::string& fileName, int targetId, int status);
};