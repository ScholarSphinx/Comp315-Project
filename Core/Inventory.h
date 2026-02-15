#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>

#include "Product.h"
#include "Order.h"

class Inventory
{
private:
    std::unordered_map<int, std::shared_ptr<Product>> m_products;
    mutable std::mutex m_inventoryMutex;

public:
    // ---- Product management ----
    void addProduct(std::shared_ptr<Product> product);

    const std::unordered_map<int, std::shared_ptr<Product>>&
        getProducts() const;

    // ---- Concurrency-safe order processing ----
    OrderStatus processOrder(int productId,
        int quantity,
        double& revenueOut);
};
