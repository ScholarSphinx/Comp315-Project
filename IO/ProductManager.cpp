#include "ProductManager.h"
#include <fstream>
#include <sstream>
#include <vector>

bool ProductFileManager::addProductToFile(
    const std::string& filename,
    const std::shared_ptr<Product>& product)
{
    std::ofstream file(filename, std::ios::app);
    if (!file)
        return false;

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
    std::ifstream file(filename);
    if (!file)
        return false;

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.find(std::to_string(productId) + ",") != 0)
            lines.push_back(line);
    }

    file.close();

    std::ofstream out(filename, std::ios::trunc);
    for (const auto& l : lines)
        out << l << "\n";

    return true;
}