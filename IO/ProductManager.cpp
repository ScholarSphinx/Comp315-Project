#include "ProductManager.h"
#include <fstream>
#include <sstream>
#include <vector>

bool ProductFileManager::addProductToFile( //Adding a product to the file by appending its details in CSV format, allowing us to persist product information in a file for later retrieval and management, ensuring that we can maintain a record of products in the inventory and easily add new products as needed.
    const std::string& filename,
    const std::shared_ptr<Product>& product)
{
    std::ofstream file(filename, std::ios::app);
    if (!file)
        return false;
	// Writing the product details in CSV format (ID, Name, Price, Quantity) to the file, ensuring that the product information is stored in a structured and easily parsable format for later retrieval and management. This allows us to maintain a record of products in the inventory and easily add new products as needed by appending their details to the file.
    file << product->getId() << "," 
        << product->getName() << ","
        << product->getPrice() << ","
        << product->getQuantity() << "\n";

    return true;
}

bool ProductFileManager::removeProductFromFile(
    const std::string& filename,
    int productId)
{
	std::ifstream file(filename); //Opening the file for reading to load the existing product data, allowing us to read the current contents of the file and identify the product that needs to be removed based on its unique ID, ensuring that we can manage the inventory by removing products as needed while maintaining the integrity of the remaining product data in the file.
    if (!file)
        return false;

    std::vector<std::string> lines;
    std::string line;

	while (std::getline(file, line)) //Reading each line of the file to process the product data, allowing us to identify and filter out the line corresponding to the product that needs to be removed based on its unique ID, ensuring that we can manage the inventory by removing products as needed while maintaining the integrity of the remaining product data in the file.
    {
		if (line.find(std::to_string(productId) + ",") != 0) //Checking if the line does not start with the product ID followed by a comma, which indicates that this line does not correspond to the product we want to remove. If the line does not match the product ID, we keep it in the list of lines to be written back to the file, ensuring that we only remove the specific product line while preserving all other product data in the file, allowing us to maintain an accurate and up-to-date record of products in the inventory after removing a product.
			lines.push_back(line); //If the line does not start with the product ID followed by a comma, we keep it in the list of lines to be written back to the file, ensuring that we only remove the specific product line while preserving all other product data in the file, allowing us to maintain an accurate and up-to-date record of products in the inventory after removing a product.
    }

    file.close();

	std::ofstream out(filename, std::ios::trunc); //Opening the file for writing with truncation to overwrite the existing contents with the updated list of products, allowing us to effectively remove the specified product from the file while preserving the remaining product data, ensuring that we maintain an accurate and up-to-date record of products in the inventory after removing a product.
    for (const auto& l : lines)
		out << l << "\n"; //Writing the remaining lines back to the file, ensuring that we preserve all product data except for the removed product, allowing us to maintain an accurate and up-to-date record of products in the inventory after removing a product.

    return true;
}