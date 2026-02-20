#pragma once

#include <string>
#include "../Core/Inventory.h"

class ProductFileManager
{
public:
    static bool addProductToFile(const std::string& filename,
        const std::shared_ptr<Product>& product);

    static bool removeProductFromFile(const std::string& filename,
        int productId);

    static bool reloadInventory(const std::string& filename,
        Inventory& inventory);
};