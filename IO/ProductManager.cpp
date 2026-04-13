#include "ProductManager.h"
#include <fstream>
#include <sstream>
#include <vector>

int ProductFileManager::calculateNextId(const std::string& filename) {
    std::ifstream file(filename);
    int count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) count++;
    }
    return count + 1; // ID is line count + 1
}

bool ProductFileManager::addProductToFile(const std::string& filename, const std::shared_ptr<Product>& product, const std::string& type, double rate) {
    std::ofstream file(filename, std::ios::app);
    if (!file) return false;

    // Format: ID, Name, Price, Quantity, Type, Rate, Active(1)
    file << product->getId() << ","
        << product->getName() << ","
        << product->getPrice() << ","
        << product->getQuantity() << ","
        << type << ","
        << rate << ","
        << "1\n"; // New products are always active

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

// ProductFileManager.cpp
void ProductFileManager::updateProductActiveStatus(const std::string& fileName, int targetId, int status) {
    std::ifstream inFile(fileName);
    std::vector<std::string> lines;
    std::string line;

    if (!inFile.is_open()) return;

    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::vector<std::string> parts;
        std::string part;

        while (std::getline(ss, part, ',')) parts.push_back(part);

        // If ID matches, update the 7th column (index 6) to the new status (0 or 1)
        if (!parts.empty() && std::stoi(parts[0]) == targetId) {
            if (parts.size() >= 7) parts[6] = std::to_string(status);
            else parts.push_back(std::to_string(status));

            line = "";
            for (size_t i = 0; i < parts.size(); ++i) {
                line += parts[i] + (i == parts.size() - 1 ? "" : ",");
            }
        }
        lines.push_back(line);
    }
    inFile.close();

    std::ofstream outFile(fileName, std::ios::trunc);
    for (const auto& l : lines) outFile << l << "\n";
}